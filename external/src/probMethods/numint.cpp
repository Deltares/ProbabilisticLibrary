#include <math.h>
#include <float.h>
#include <omp.h>
#include "numint.h"
#include "../probFuncs/conversions.h"
#include "../utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

        numint::numint(const NIsettings& s, const progress& pg) : settings(s), p(pg)
        {
            init_weights();
        }

        numint::numint(const NIsettings& s, const size_t nrProc, const progress& pg)
            : settings(s), numThreads(nrProc), p(pg)
        {
            init_weights();
        }

        void numint::init_weights()
        {
            const int    nrIntervalsU = settings.numExtraInt;
            const double uMin = settings.minimumUvalue;
            const double uMax = settings.maximumUvalue;
            const double rangeU = uMax - uMin;

            double pUmin, qUmin;
            conversions::PQfromBeta(settings.minimumUvalue, pUmin, qUmin);
            double p1 = pUmin;
            double q1 = qUmin;
            diffpq = std::vector<std::pair<double, double>>(nrIntervalsU + 1);

            for (int j = 1; j <= nrIntervalsU; j++)
            {
                double p2 = p1; double q2 = q1;
                conversions::PQfromBeta(uMin + (double)j * rangeU / (double)nrIntervalsU, p1, q1);
                diffpq[j].first = p1 - p2;
                diffpq[j].second = q2 - q1;
            }
        }

        // Module to perform a computation with the calculation method Numerical Integration.
        //
        // Method to perform a Numerical Integration computation.
        //
        // A grid is placed in all directions of the u-space. Only the centres of the cells are considered. \n
        // The whole cell belongs to the area of failure if the centre of the cell belongs to the eara of failure. \n
        // The probability of failure is then enlarged with the joint probability density of this cell. \n
        // So the probability of failure is computed with the computation method Numerical Integration. \n
        // Also the reliability index is computated and the alpha values (the directions of the design point)
        //
        // The limit state function (z-function) is an external argument of this subroutine
        //
        lsfResult numint::calc(const zmodel& mdl, const stochSettings& s)
        {
            //
            // Initialize
            //
            m = &mdl;
            nrStochasts = s.getNrStochasts();
            auto lsf = lsfResult(nrStochasts, settings.designPointOption);
            lsf.method = "NumInt";
            //
            // Examine the position of the origin: If the origin has a negative z-value the design point must be searched
            // with positive values of the z-function. The explanation for searching with positive z-values as the origin
            // has a negative z-value: if searching with negative z-values, the origin is found as the design point, because
            // that is the point who's closest to the origin (distance is zero). For the computation of the probability of
            // failure all elements of negative z-value has te be used.
            //
            auto drw = probDraw(nrStochasts, 0.0, s);
            double z = m->zfunc(drw);
            //
            // Numerical integration isn't possible with a large set of stochastic parameters
            // Warnings and errors are presented
            //
            if (nrStochasts > 11)
                throw probLibException("Numerical integration with more than 11 stochastic parameters. This is practically impossible");
            if (nrStochasts > 4)
                p.printInfo("Numerical integration with more than 4 stochastic parameters. Expect long computation time");
            //
            // Initialize parameters for the recursive part of the Numerical Integration computation
            //
            // Call to the recursive part of the numerical integration computation
            // The result of this computation is the probability of failure and the
            // direction of the design point (Alpha values)
            //
            vector1D drwu = vector1D(nrStochasts);
            NIstate state;
            state.density = 1.0;
            state.probFailure = 0.0;
            state.rBeta = 0.0;
            // rMin is finally the quadratic distance from the design point to the origin.
            // First a upper bound is chosen. This dependents on the upper and lower bounds of u
            //
            state.rMin = DBL_MAX;
            state.z0Fac = (z > 0.0 ? 1.0 : -1.0);
            state.i = 0;
            state.threadId = 0;
            recursiveNumInt(drwu, lsf, s, state);
            //
            // Compute Beta from the probability failure. Beta is because of this no longer per definition the
            // distance from the design point to the origin
            //
            lsf.result.setBeta(state.probFailure, 'q');
            //
            // If the probability of failure is equal of zero, there is no design point
            // To prevent that the alfa values are nonsense, values are assigned to alfa.
            // The sum of squares of the alpha's is equal to 1. This is used to determine the alpha's
            // The minus is because of the adjudication of the loads
            //
            if (fabs(state.probFailure) < 1e-30)
            {
                lsf.result.assign(-sqrt(1.0 / (double)nrStochasts));
                lsf.convergence = ConvergenceStatus::noConvergence;
            }
            else
            {
                lsf.convergence = ConvergenceStatus::success;
                lsf.stepsNeeded = (int)pow(settings.numExtraInt, nrStochasts);
            }
            lsf.calcXinDesignPoint(mdl, s);
            return lsf;
        }

        //
        // Recursive part of the Numerical Integration computation.
        // The recursive method computes in only one dimension
        //
        void numint::recursiveNumInt(vector1D& drwu, lsfResult& lsf, const stochSettings& s, NIstate& state)
        {
            //
            // Initialize parameters for stochastic parameter u
            //
            const int    nrIntervalsU = settings.numExtraInt;
            const double uMin = settings.minimumUvalue;
            const double rangeU = settings.maximumUvalue - settings.minimumUvalue;
            //
            if (state.i < nrStochasts)
            {
                //
                // Call recursive part if the number of stochastic parameters is higher than the actual number
                //
                if (state.i == 0 && numThreads > 1)
                {
                    omp_set_num_threads((int)numThreads);
                    auto drwus = std::vector<vector1D>(numThreads);
                    auto lsfs = std::vector<lsfResult>(numThreads);
                    auto states = std::vector<NIstate>(numThreads);
                    auto success = std::vector<bool>(numThreads);
                    auto errMsgs = std::vector<std::string>(numThreads);
                    for (size_t i = 0; i < numThreads; i++)
                    {
                        drwus[i] = drwu;
                        lsfs[i] = lsf;
                        states[i] = state;
                        states[i].i++;
                        success[i] = true;
                    }

#pragma omp parallel for
                    for (int j = 1; j <= nrIntervalsU; j++)
                    {
                        int ii;
#ifndef __cplusplus_cli
                        try
                        {
#endif
                            ii = omp_get_thread_num();
                            if (success[ii])
                            {
                                drwus[ii](state.i) = uMin + ((double)j - 0.5) * rangeU / (double)nrIntervalsU;
                                //
                                // Depending on the value of u(i) use the probabilities of exceedance or the probabilities of non-exceedance
                                // as here state.i == 0, we know that state.density = 0 and state.rBeta = 1, so we can ignore it.
                                //
                                states[ii].density = (drwus[ii](state.i) < 0 ? diffpq[j].first : diffpq[j].second);
                                states[ii].rBeta = pow(drwus[ii](state.i), 2);
                                states[ii].threadId = omp_get_thread_num();
                                if (!p.printProgressAndCancel(j, -999.0, -999.0)) recursiveNumInt(drwus[ii], lsfs[ii], s, states[ii]);
                            }
#ifndef __cplusplus_cli
                        }
                        catch (const std::exception& e)
                        {
                            success[ii] = false;
                            errMsgs[ii] = e.what();
                        }
#endif
                    }

                    // manual reduce over all threads
                    for (size_t i = 0; i < numThreads; i++)
                    {
                        if (!success[i]) throw probLibException(errMsgs[i]);
                        state.probFailure += states[i].probFailure;
                        if (states[i].rMin < state.rMin)
                        {
                            state.rMin = states[i].rMin;
                            lsf = lsfs[i];
                        }
                    }
                }
                else
                {
                    auto stateNw = state;
                    stateNw.i++;
                    for (int j = 1; j <= nrIntervalsU; j++)
                    {
                        drwu(state.i) = uMin + ((double)j - 0.5) * rangeU / (double)nrIntervalsU;
                        //
                        // Depending on the value of u(i) use the probabilities of exceedance or the probabilities of non-exceedance
                        //
                        const double contributionU = (drwu(state.i) < 0 ? diffpq[j].first : diffpq[j].second);
                        stateNw.density = state.density * contributionU;
                        stateNw.rBeta = state.rBeta + pow(drwu(state.i), 2);
                        recursiveNumInt(drwu, lsf, s, stateNw);
                        if (state.i == 0) p.printProgressAndCancel(j, -999.0, -999.0);
                    }
                    state.probFailure = stateNw.probFailure;
                    state.rMin = stateNw.rMin;
                }
            }
            else
            {
                //
                // Compute the z-value
                //
                auto drw = probDraw(drwu.size(), drwu, s);
                const double z = m->zfunc(drw, state.threadId);
                //
                // If the z-value is negative add the probability density to the probability of failure
                //
                if (z < 0.0) state.probFailure += state.density;
                //
                // If the combination of the z-value and the parameter for the position of the origin is negative
                // it is possible that a new design point is computed. Then also the alfa values are computated
                //
                if (z * state.z0Fac < 0.0)
                {
                    if (state.rBeta < state.rMin)
                    {
                        state.rMin = state.rBeta;
                        lsf.result.setAlpha(drwu * (-1.0 / (sqrt(state.rBeta) * state.z0Fac)));
                        lsf.u = drwu;
                    }
                }
            }
        }
    }
}
