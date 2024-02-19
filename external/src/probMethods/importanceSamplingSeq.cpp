#include <math.h>
#include <float.h>
#include "../utils/basic_math.h"
#include "../utils/probLibException.h"
#include "importanceSamplingSeq.h"

namespace Deltares {
    namespace ProbLibCore {

        ImportanceSamplingSeq::ImportanceSamplingSeq(const IMsettings& s, randomForSampling& r, const progress& pg)
            : settings(s), rnd(r), p(pg)
        {
            if (settings.varianceFactor < 1.0)
            {
                throw probLibException("variance factor in IM must be > 1.0");
            }
        }

        lsfResult ImportanceSamplingSeq::calc(const zmodel& m, const stochSettings& s)
        {
            //
            // Determine multiplication factor for Z (Z<0), if u = 0
            //
            nrStochasts = s.getNrStochasts();
            lsfResult lsf = lsfResult(nrStochasts);
            lsf.samplesNeeded = settings.maximumSamples;
            lsf.method = "Importance sampling";
            lsf.samplesFailure = 0;

            auto offsets = settings.offsets;
            auto varianceFactors = settings.varianceFactors;
            if (varianceFactors.size() == 0)
            {
                offsets = vector1D(nrStochasts);
                varianceFactors = vector1D(nrStochasts);
                varianceFactors.assign(settings.varianceFactor);
            }

            auto drw = probDraw(nrStochasts, 0.0, s);
            double z = m.zfunc(drw);
            const double Z0fac = (z > 0.0 ? 1.0 : -1.0);
            setVarCoeff(Z0fac, settings.varCoeffFailure, settings.varCoeffNoFailure);
            //
            // Loop over the samples
            //
            double rMin = DBL_MAX;
            double Pf = -999.0;
            double varianceFactorPowN = varianceFactors(0);
            for (size_t i = 1; i < nrStochasts; i++)
            {
                varianceFactorPowN *= varianceFactors(i);
            }
            double PfTotal = 0.0;
            double fdivh = -999.0;
            auto drw1 = vector1D(nrStochasts);
            auto drw2 = vector1D(nrStochasts);
            for (int nTimes = 1; nTimes <= settings.maximumSamples; nTimes++)
            {
                //
                // Get values for stochastic parameters
                //
                rnd.getRandomVector(drw1);
                for (size_t n = 0; n < nrStochasts; n++)
                {
                    drw2(n) = offsets(n) + drw1(n) * varianceFactors(n);
                }
                //
                // Value of z-function
                //
                drw.setu(drw2);
                z = m.zfunc(drw);

                double fTotal = 0.0; double dPf = 1.0;
                if (z < 0.0 || z * Z0fac < 0.0)
                {
                    double hTotal = drw1.sumOfSquares();
                    fTotal = drw2.sumOfSquares();
                    dPf = varianceFactorPowN * exp(-0.5 * (fTotal - hTotal));
                }
                //
                // Determine total probability of failure
                //
                if (z < 0.0)
                {
                    PfTotal += dPf;
                    lsf.samplesFailure++;
                }
                if (z * Z0fac < 0.0)
                {
                    //
                    // Register minimum value of r and alpha
                    //
                    double rBeta2 = fTotal;
                    if (rBeta2 < rMin)
                    {
                        rMin = rBeta2;
                        fdivh = dPf;
                        lsf.result.setAlpha(drw2 * (-Z0fac / sqrt(rBeta2)));
                        lsf.u = drw2;
                    }
                }

                //
                // Check if stop criterium is reached
                //
                if (nTimes >= settings.minimumSamples)
                {
                    double dTimes = (double)nTimes;
                    Pf = PfTotal / dTimes;

                    if (Pf > 0.0 && Pf < 1.0)
                    {
                        double varPf = (fdivh / Pf - 1.0) / dTimes;
                        double varNP = (fdivh / (1.0 - Pf) - 1.0) / dTimes;
                        if (varPf < varCoeffF2 && varNP < varCoeffNF2)
                        {
                            lsf.convergence = ConvergenceStatus::success;
                            lsf.samplesNeeded = nTimes;
                            break;
                        }
                        else lsf.convergence = ConvergenceStatus::noConvergence;
                    }
                }

                if (p.printProgressAndCancel(nTimes, PfTotal / (double)nTimes, -999.0)) break;
            }

            lsf.result.setBeta(Pf, 'q');

            lsf.calcXinDesignPoint(m, s);

            return lsf;
        }

        void ImportanceSamplingSeq::updateMaxSamples(const int maxSamples)
        {
            settings.maximumSamples = maxSamples;
        }

        void ImportanceSamplingSeq::updateVariance(const double additionalVariance)
        {
            for (size_t i = 0; i < settings.varianceFactors.size(); i++)
            {
                settings.varianceFactors(i) += additionalVariance;
            }
        }

        void ImportanceSamplingSeq::updateOffset(const vector1D& newOffset)
        {
            settings.offsets = newOffset;
        }
    }
}
