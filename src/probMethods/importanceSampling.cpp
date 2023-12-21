#include <math.h>
#include <float.h>
#include "../utils/basic_math.h"
#include "../utils/probLibException.h"
#include <omp.h>
#include "importanceSampling.h"

namespace Deltares {
    namespace ProbLibCore {

        IMbuffer::IMbuffer(const size_t bufSize, const size_t nrStochasts, const bool circularIn)
        {
            zpar = vector1D(bufSize);
            dPfPar = vector1D(bufSize);
            fTotalPar = vector1D(bufSize);
            drwPar2 = std::vector<vector1D>(bufSize);
            for (size_t i = 0; i < bufSize; i++)
            {
                drwPar2[i] = vector1D(nrStochasts);
            }
            circular = circularIn;
            lastFilled = 0;
        }

        ImportanceSampling::ImportanceSampling(const IMsettings& s, randomForSampling& r, const progress& pg, IMbuffer& bffr)
            : settings(s), rnd(r), p(pg), buf(bffr)
        {
            if (settings.varianceFactor < 1.0)
            {
                throw probLibException("variance factor in IM must be > 1.0");
            }
        }

        lsfResult ImportanceSampling::calc(const zmodel& m, const stochSettings& s)
        {
            //
            // Determine multiplication factor for Z (Z<0), if u = 0
            //
            nrStochasts = s.getNrStochasts();
            lsfResult lsf = lsfResult(nrStochasts);
            lsf.samplesNeeded = settings.maximumSamples;
            lsf.method = "Importance sampling (parallel/buffered)";
            lsf.samplesFailure = 0;
            if (!settings.restart) buf.lastFilled = 0;

            offsets = settings.offsets;
            varianceFactors = settings.varianceFactors;
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
            varianceFactorPowN = varianceFactors(0);
            for (size_t i = 1; i < nrStochasts; i++)
            {
                varianceFactorPowN *= varianceFactors(i);
            }
            double PfTotal = 0.0;
            double fdivh = -999.0;

            omp_set_num_threads((int)m.numcores);
            bufferSize = (m.numcores > 1 ? min(settings.maximumSamples, settings.chunkSize) : 1);

            int offset = 0;
            for (int nTimes = 1; nTimes <= settings.maximumSamples; nTimes++)
            {
                //
                // Get values for stochastic parameters
                //
                int imod = (nTimes - 1) % bufferSize;
                if (imod == 0)
                {
                    offset = (buf.circular ? 0 : nTimes - 1);
                    if (buf.lastFilled < nTimes)
                    {
                        updateBuffer(m, s, nTimes, offset);
                    }
                }
                z = buf.zpar(offset + imod);
                //
                // Determine total probability of failure
                //
                double dPf = buf.dPfPar(offset + imod);
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
                    double rBeta2 = buf.fTotalPar(offset + imod);
                    if (rBeta2 < rMin)
                    {
                        rMin = rBeta2;
                        fdivh = dPf;
                        lsf.result.setAlpha(buf.drwPar2[offset + imod] * (-Z0fac / sqrt(rBeta2)));
                        lsf.u = buf.drwPar2[offset + imod];
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

        void ImportanceSampling::updateBuffer(const zmodel& m, const stochSettings& s, const int nTimes, const int offset)
        {
            const auto curBufSize = min(bufferSize, settings.maximumSamples + 1 - nTimes);
            for (int i = offset; i < offset + curBufSize; i++)
            {
                rnd.getRandomVector(buf.drwPar2[i]);
            }
#pragma omp parallel for
            for (int i = offset; i < offset + curBufSize; i++)
            {
                double hTotal = buf.drwPar2[i].sumOfSquares();
                for (size_t n = 0; n < nrStochasts; n++)
                {
                    buf.drwPar2[i](n) = offsets(n) + buf.drwPar2[i](n) * varianceFactors(n);
                }
                buf.fTotalPar(i) = buf.drwPar2[i].sumOfSquares();
                buf.dPfPar(i) = varianceFactorPowN * exp(-0.5 * (buf.fTotalPar(i) - hTotal));
                //
                // Value of z-function
                //
                buf.zpar(i) = m.zfunc(probDraw(nrStochasts, buf.drwPar2[i], s), omp_get_thread_num());
            }
            buf.lastFilled = offset + curBufSize;
        }

        void ImportanceSampling::updateMaxSamples(const int maxSamples, const bool restart)
        {
            settings.maximumSamples = maxSamples;
            settings.restart = restart;
        }

        void ImportanceSampling::updateVariance(const double additionalVariance)
        {
            for (size_t i = 0; i < settings.varianceFactors.size(); i++)
            {
                settings.varianceFactors(i) += additionalVariance;
            }
        }

        void ImportanceSampling::updateOffset(const vector1D& newOffset)
        {
            settings.offsets = newOffset;
        }
    }
}
