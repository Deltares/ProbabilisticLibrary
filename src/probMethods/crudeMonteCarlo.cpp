#include <math.h>
#include <float.h>
#include "crudeMonteCarlo.h"

namespace Deltares {
    namespace ProbLibCore {

        crudeMonteCarlo::crudeMonteCarlo(const CMsettings& s, randomForSampling& r, const progress& pg)
            : settings(s), rnd(r), p(pg) {}

        lsfResult crudeMonteCarlo::calc(const zmodel& m, const stochSettings& s)
        {
            //
            // Determine multiplication factor for Z (Z<0), if u = 0
            //
            nrStochasts = s.getNrStochasts();
            lsfResult lsf = lsfResult(nrStochasts);
            lsf.samplesNeeded = settings.maximumSamples;
            lsf.method = "CM";

            auto drw = probDraw(nrStochasts, 0.0, s);
            double z = m.zfunc(drw);
            double Z0fac = (z > 0.0 ? 1.0 : -1.0);
            setVarCoeff(Z0fac, settings.varCoeffFailure, settings.varCoeffNoFailure);
            //
            // Loop over the samples
            //
            int nFailTotal = 0;
            double rMin = DBL_MAX;
            double Pf = -999.0;
            vector1D drwu = vector1D(nrStochasts);
            for (int nTimes = 1; nTimes <= settings.maximumSamples; nTimes++)
            {
                //
                // Get values for stochastic parameters
                //
                rnd.getRandomVector(drwu);
                drw.setu(drwu);
                //
                // Value of z-function
                //
                z = m.zfunc(drw);
                //
                // Determine total probability of failure
                //
                if (z < 0.0) nFailTotal++;
                //
                // Register minimum value of r and alpha
                //
                if (z * Z0fac < 0.0)
                {
                    double rBeta = sqrt(drwu.sumOfSquares());
                    if (rBeta < rMin)
                    {
                        rMin = rBeta;
                        lsf.result.setAlpha(drwu * (-1.0 / rBeta * Z0fac));
                        lsf.u = drwu;
                    }
                }

                //
                // Check if stop criterium is reached
                //
                if (nTimes >= settings.minimumSamples)
                {
                    double dTimes = (double)nTimes;
                    Pf = (double)nFailTotal / dTimes;
                    if (Pf > 0.0 && Pf < 1.0)
                    {
                        double varPf = 1.0 / dTimes * (1.0 / Pf - 1.0);
                        double varNP = 1.0 / dTimes * (1.0 / (1.0 - Pf) - 1.0);
                        if (varPf < varCoeffF2 && varNP < varCoeffNF2)
                        {
                            lsf.convergence = ConvergenceStatus::success;
                            lsf.samplesNeeded = nTimes;
                            break;
                        }
                        else lsf.convergence = ConvergenceStatus::noConvergence;
                    }
                }

                if (p.printProgressAndCancel(nTimes, (double)nFailTotal / (double)nTimes, -999.0))
                {
                    lsf.convergence = ConvergenceStatus::abortedByUser;
                    lsf.samplesNeeded = nTimes;
                    break;
                }
            }

            lsf.result.setBeta(Pf, 'q');
            lsf.calcXinDesignPoint(m, s);

            return lsf;
        }
    }
}
