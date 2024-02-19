#include <math.h>
#include <float.h>
#include <omp.h>
#include "../utils/basic_math.h"
#include "../probFuncs/conversions.h"
#include "../utils/vector1D.h"
#include "../utils/probLibException.h"
#include "../extern/dgammaDs.h"
#include "ray_search.h"
#include "directionalSampling.h"

namespace Deltares {
    namespace ProbLibCore {

        directionalSampling::directionalSampling(const DSsettings& s, randomForSampling& r, dsRootFinder& f, const progress& p)
            : settings(s), rnd(r), pg(p), rootFinder(f) {}

        directionalSampling::directionalSampling(const DSsettings& s, randomForSampling& r, dsRootFinder& f, const size_t nrThreads,
            const progress& p) : settings(s), rnd(r), numcores(nrThreads), pg(p), rootFinder(f) {}

        bool directionalSampling::checkConvergence(const double dTimes, const double Pf, const double sumPfSamp,
            const double sumPfSamp2, double& convIndicator)
        {
            //
            // Determine the accuracy
            //
            double CoVar = sumPfSamp2 - 2.0 * Pf * sumPfSamp + dTimes * pow(Pf, 2);
            CoVar = CoVar / dTimes / (dTimes - 1.0);
            convIndicator = CoVar / pow(Pf, 2);
            //
            // Check for convergence
            //
            return (CoVar <= varCoeffF2 * pow(Pf, 2) &&
                CoVar <= varCoeffNF2 * pow(1.0 - Pf, 2));
        }

        lsfResult directionalSampling::calc(const zmodel& m, const stochSettings& s)
        {
            nrStochasts = s.getNrStochasts();
            lsfResult lsf = lsfResult(nrStochasts, settings.designPointOption);
            lsf.method = "DS";
            lsf.samplesNeeded = settings.maxSamples;

            auto u = probDraw(nrStochasts, 0.0, s);

            double z0 = m.zfunc(u);
            if (fabs(z0) < almostZero)
            {
                lsf.result.setBeta(0.0);
                lsf.result.assign(1.0 / sqrt(double(nrStochasts)));
                lsf.samplesNeeded = 0;
                return lsf;
            }
            setVarCoeff(z0, settings.varCoeffFailure, settings.varCoeffNoFailure);

            if (numcores > 1)
            {
                sampling_loop_par(m, s, lsf, z0);
            }
            else
            {
                sampling_loop_seq(m, s, lsf, z0);
            }

            lsf.calcXinDesignPoint(m, s);

            return lsf;
        }

        // sequential implementation:
        void directionalSampling::sampling_loop_seq(const zmodel& m, const stochSettings& s, lsfResult& lsf, const double z0)
        {
            double qTotal = 0.0;
            double Pf = -999.0;
            double rMin = DBL_MAX;
            double sumPfSamp2 = 0.0;
            double sumPfSamp = 0.0;

            auto t = vector1D(nrStochasts);

            const double p = 0.5 * nrStochasts;
            //auto gmmd = dgammaDs(p);
            auto func = rs_func_wrapper(t, m, s, 0);
            rootFinder.init(z0);
            for (int nTimes = 1; nTimes <= settings.maxSamples; nTimes++)
            {
                double PfSamp;
                rnd.getNormalizedRandomVector(t);
                func.updateDir(t);
                auto pr = rootFinder.FindRoot(func, 0.0, maximumLengthU);
                double r = pr.second;
                if (r < maximumLengthU)
                {
                    auto gmmd = dgammaDs(p);
                    PfSamp = gmmd.Dgammq(p, 0.5 * pow(r, 2));
                }
                else
                {
                    PfSamp = 0.0;
                }

                if (r < rMin)
                {
                    rMin = r;
                    lsf.u = t;
                }

                qTotal += PfSamp;
                double dTimes = (double)nTimes;
                Pf = qTotal / dTimes;
                sumPfSamp += PfSamp;
                sumPfSamp2 += pow(PfSamp, 2);
                double convIndicator = -999.0;
                if (qTotal > 0.0 && nTimes >= settings.minSamples)
                {
                    if (checkConvergence(dTimes, Pf, sumPfSamp, sumPfSamp2, convIndicator))
                    {
                        lsf.convergence = ConvergenceStatus::success;
                        lsf.samplesNeeded = nTimes;
                    }
                    else lsf.convergence = ConvergenceStatus::noConvergence;
                }

                if (pg.printProgressAndCancel(nTimes, Pf, convIndicator))
                {
                    lsf.samplesNeeded = nTimes;
                    lsf.convergence = ConvergenceStatus::abortedByUser;
                    break;
                }

                if (lsf.convergence == ConvergenceStatus::success) break;
            }

            final_result(lsf, z0, Pf, rMin);
        }

        void directionalSampling::final_result(lsfResult& lsf, const double z0, double Pf, const double rMin)
        {
            double factor = (z0 > 0.0 ? -1.0 : 1.0);
            lsf.result.setAlpha(factor * lsf.u);
            if (z0 < 0.0)
            {
                Pf = 1.0 - Pf;
            }
            lsf.result.setBeta(Pf, 'q');
            lsf.u *= rMin;
        }

        // OpenMP implementation:
        void directionalSampling::sampling_loop_par(const zmodel& m, const stochSettings& s, lsfResult& lsf, const double z0)
        {
            double qTotal = 0.0;
            double Pf = -999.0;
            double rMin = DBL_MAX;
            double sumPfSamp2 = 0.0;
            double sumPfSamp = 0.0;
            double convIndicator = -999.0;

            // preparations for OpenMP loop:
            std::vector<vector1D> ti;
            vector1D rValues, pfsValues, t;
            std::vector<std::string> errMsgs;
            omp_set_num_threads((int)numcores);
            ti = std::vector<vector1D>(settings.chunkSize);
            for (int i = 0; i < settings.chunkSize; i++)
            {
                ti[i] = vector1D(nrStochasts);
            }
            rValues = vector1D(settings.chunkSize);
            pfsValues = vector1D(settings.chunkSize);
            bool success = true;
            errMsgs = std::vector<std::string>(numcores);

            const double p = 0.5 * nrStochasts;
            auto gmmd = dgammaDs(p);
            rootFinder.init(z0);
            for (int nTimes = 1; nTimes <= settings.maxSamples; nTimes++)
            {
                auto iMod = (nTimes - 1) % settings.chunkSize;
                if (iMod == 0)
                {
                    if (pg.printProgressAndCancel(nTimes, Pf, convIndicator))
                    {
                        lsf.samplesNeeded = nTimes;
                        lsf.convergence = ConvergenceStatus::abortedByUser;
                        break;
                    }

                    int curChunkSize = min(settings.chunkSize, settings.maxSamples + 1 - nTimes);
                    for (int i = 0; i < curChunkSize; i++)
                    {
                        for (size_t j = 0; j < nrStochasts; j++)
                        {
                            ti[i](j) = rnd.get_uniform();
                        }
                    }
#pragma omp parallel for reduction(&&:success) schedule(dynamic)
                    for (int i = 0; i < curChunkSize; i++)
                    {
#ifndef __cplusplus_cli
                        try
                        {
#endif // __cplusplus_cli
                            if (success)
                            {
                                for (int j = 0; j < (int)nrStochasts; j++)
                                {
                                    ti[i](j) = conversions::betaFromQ(ti[i](j));
                                }
                                ti[i].normalize();
                                auto func = rs_func_wrapper(ti[i], m, s, omp_get_thread_num());
                                auto pr = rootFinder.FindRoot(func, 0.0, maximumLengthU);
                                rValues(i) = pr.second;

                                if (rValues(i) < maximumLengthU)
                                {
                                    pfsValues(i) = gmmd.Dgammq(p, 0.5 * pow(rValues(i), 2));
                                }
                                else
                                {
                                    pfsValues(i) = 0.0;
                                }
                            }
#ifndef __cplusplus_cli
                    }
                        catch (const std::exception& e)
                        {
                            success = false;
                            errMsgs[omp_get_thread_num()] = e.what();
                        }
#endif // __cplusplus_cli
                    }
                    if (!success)
                    {
                        for (size_t i = 0; i < numcores; i++)
                        {
                            if (errMsgs[i] != "") throw probLibException(errMsgs[i]);
                        }
                    }
                }
                double r = rValues(iMod);
                double PfSamp = pfsValues(iMod);
                t = ti[iMod];

                if (r < rMin)
                {
                    rMin = r;
                    lsf.u = t;
                }

                qTotal += PfSamp;
                double dTimes = (double)nTimes;
                Pf = qTotal / dTimes;
                sumPfSamp += PfSamp;
                sumPfSamp2 += pow(PfSamp, 2);
                if (qTotal > 0.0 && nTimes >= settings.minSamples)
                {
                    if (checkConvergence(dTimes, Pf, sumPfSamp, sumPfSamp2, convIndicator))
                    {
                        lsf.convergence = ConvergenceStatus::success;
                        lsf.samplesNeeded = nTimes;
                    }
                    else lsf.convergence = ConvergenceStatus::noConvergence;
                }

                if (lsf.convergence == ConvergenceStatus::success) break;
            }

            final_result(lsf, z0, Pf, rMin);
        }
    }
}
