#include <cmath>
#include <math.h>
#include "form.h"
#include "../utils/basic_math.h"
#include "../probFuncs/probDraw.h"

namespace Deltares {
    namespace ProbLibCore {

        form::form(const FormSettings& s, const progress& pg)
            : startVector(0), settings(s), p(pg) {}

        form::form(const FormSettings& s, vector1D start, const progress& pg)
            : startVector(start), settings(s), p(pg) {}

        lsfResult form::calc(const zmodel& m, const stochSettings& s)
        {
            nrStochasts = s.getNrStochasts();
            const auto sigZMin = 1.0e-8;
            lsfResult lsf = lsfResult(nrStochasts, settings.designPointOption);
            lsf.method = "FORM";
            auto drwu = vector1D(nrStochasts);
            if (startVector.size() == nrStochasts)
            {
                drwu = startVector;
            }

            auto dzdu = vector1D(nrStochasts);
            auto last10u = std::vector<vector1D>();
            auto allBetas = std::vector<double>();
            for (int iter = 1; iter <= settings.maxIter; iter++)
            {
                if (iter <= histU)
                {
                    last10u.push_back(drwu);
                }
                else
                {
                    int j = mod(iter - 1, histU);
                    last10u[j] = drwu;
                }

                double z;
                m.getZandDerivatives(drwu, s, z, dzdu, settings.du);

                double meanZ = z - dzdu.sumOfInner(drwu);

                double sigZ = sqrt(dzdu.sumOfSquares());
                if (sigZ < sigZMin)
                {
                    lsf.convergence = ConvergenceStatus::noConvergence;
                    lsf.stepsNeeded = iter;
                    if (iter == 1)
                    {
                        lsf.result.setBeta(betaMax);
                        auto alpha = vector1D(nrStochasts);
                        for (size_t i = 0; i < nrStochasts; i++)
                        {
                            alpha(i) = sqrt(1.0 / nrStochasts);
                        }
                        lsf.result.setAlpha(alpha);
                    }
                    break;
                }
                auto beta = meanZ / sigZ;
                allBetas.push_back(beta);
                lsf.result = alphaBeta(beta, dzdu * (1.0 / sigZ));

                double normSquared = drwu.sumOfSquares();
                if (fabs(z / sigZ) <= settings.epsilonZvalue &&
                    fabs(pow(lsf.result.getBeta(), 2) - normSquared) <= settings.epsilonBeta * normSquared)
                {
                    lsf.convergence = ConvergenceStatus::success;
                    lsf.stepsNeeded = iter;
                }
                else lsf.convergence = ConvergenceStatus::noConvergence;

                if (p.printProgressAndCancel(iter, beta, fabs(z / sigZ)))
                {
                    lsf.convergence = ConvergenceStatus::abortedByUser;
                    lsf.stepsNeeded = iter;
                    break;
                }

                if (lsf.convergence == ConvergenceStatus::success) break;

                drwu *= (1.0 - settings.relaxationFactor);
                drwu += lsf.result.getAlpha() * (-lsf.result.getBeta() * settings.relaxationFactor);

            }

            if (lsf.convergence != ConvergenceStatus::success && allBetas.size() == (size_t)settings.maxIter)
            {
                estimateBetaNonConv(lsf, allBetas, last10u);
            }

            lsf.calcXinDesignPoint(m, s);

            return lsf;
        }

        void form::estimateBetaNonConv(lsfResult& lsf, const std::vector<double>& allBetas, const std::vector<vector1D>& last10u)
        {
            const size_t nStochasts = last10u[0].size();
            const size_t nIter = last10u.size();
            double rNIter = 1.0 / (double)nIter;
            double sumUk = 0.0;
            auto uk = vector1D(nStochasts);
            for (size_t k = 0; k < nStochasts; k++)
            {
                uk(k) = 0.0;
                for (size_t iter = 0; iter < nIter; iter++)
                {
                    uk(k) += last10u[iter](k);
                }
                uk(k) *= rNIter;
                sumUk += pow(uk(k), 2);
            }

            double meanBeta = 0.0;
            size_t maxIterations = allBetas.size();
            for (size_t iter = 0; iter < nIter; iter++)
            {
                meanBeta += allBetas[iter + maxIterations - nIter];
            }
            meanBeta *= rNIter;

            double signBeta = (meanBeta > 0.0 ? 1.0 : -1.0);
            double beta = signBeta * sqrt(sumUk);
            lsf.result.setBeta(beta);

            for (size_t k = 0; k < nStochasts; k++)
            {
                uk(k) /= -beta;
            }
            lsf.result.setAlpha(uk);
        }
    }
}
