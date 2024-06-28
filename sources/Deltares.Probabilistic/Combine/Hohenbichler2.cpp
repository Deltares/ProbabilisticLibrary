#include "Hohenbichler2.h"
#include "../Math/NumericSupport.h"
#include "../Statistics/StandardNormal.h"
#include "../Utils/probLibException.h"

using namespace Deltares::Statistics;
using namespace Deltares::Numeric;

namespace Deltares {
    namespace Reliability {

        DesignPoint GetRealization(const double beta, const std::vector<std::shared_ptr<StochastPointAlpha>>& alpha)
        {
            DesignPoint dp;
            dp.Beta = beta;
            dp.Alphas = alpha;
            return dp;
        }

        DesignPoint GetRealization(const double beta, const std::vector<std::shared_ptr<StochastPointAlpha>>& alpha, const std::vector<double>& values)
        {
            DesignPoint dp;
            dp.Beta = beta;
            dp.Alphas = alpha;
            for (size_t i = 0; i < values.size(); i++) dp.Alphas[i]->Alpha = values[i];
            return dp;
        }

        void copyParameter(const std::shared_ptr<Stochast>& stochast, const std::shared_ptr<DesignPoint>&  designPoint, std::vector<std::shared_ptr<StochastPointAlpha>> & parameters)
        {
            for (const auto& Alpha : designPoint->Alphas)
            {
                if (Alpha->Stochast == stochast)
                {
                    parameters.push_back(Alpha);
                    return;
                }
            }
            auto empty = std::make_shared<StochastPointAlpha>();
            parameters.push_back(empty);
        }


        std::shared_ptr<DesignPoint> Hohenbichler2::AlphaHohenbichler(const std::shared_ptr<DesignPoint>& designPoint1, const std::shared_ptr<DesignPoint>& designPoint2,
            const std::vector<std::shared_ptr<Stochast>>& stochasts,
            const std::shared_ptr<SelfCorrelationMatrix>& selfCorrelation, const combineAndOr system)
        {
            auto nVar = stochasts.size();

            const auto reorderedDesignPoint1 = designPoint1->getSampleForStochasts(stochasts);
            const auto reorderedDesignPoint2 = designPoint2->getSampleForStochasts(stochasts);

            double rho = 0.0;
            for (size_t i = 0; i < nVar; i++)
            {
                reorderedDesignPoint1->Values[i] /= -designPoint1->Beta;
                reorderedDesignPoint2->Values[i] /= -designPoint2->Beta;
                auto corr = selfCorrelation->getSelfCorrelation(stochasts[i], designPoint1, designPoint2);
                rho += reorderedDesignPoint1->Values[i] * reorderedDesignPoint2->Values[i] * corr;
            }
            const double betaNew = BetaHohenbichler(designPoint1->Beta, designPoint2->Beta, rho, system);

            auto alpha1 = std::vector<double>();
            auto alpha2 = std::vector<double>();

            auto parameters1 = std::vector<std::shared_ptr<StochastPointAlpha>>();
            auto parameters2 = std::vector<std::shared_ptr<StochastPointAlpha>>();
            for (size_t i = 0; i < nVar; i++)
            {
                copyParameter(stochasts[i], designPoint1, parameters1);
                copyParameter(stochasts[i], designPoint2, parameters2);
            }

            for (size_t i = 0; i < nVar; i++)
            {
                constexpr double epsilon = 0.01;
                // perturbation of ui and its effect on beta.
                // [1] determine alpha-value for correlated part

                double rhoSCi = selfCorrelation->getSelfCorrelation(stochasts[i]);
                double rhoCompl = sqrt(1.0 - rhoSCi * rhoSCi);

                // [1a] perturbation of beta-values
                auto dpj1 = GetRealization(designPoint1->Beta + reorderedDesignPoint1->Values[i] * epsilon, parameters1); // copy alphas
                auto dpj2 = GetRealization(designPoint2->Beta + reorderedDesignPoint2->Values[i] * epsilon * rhoSCi, parameters2);

                // [1b] Hohenbichler computation with perturbed beta-values
                double betaj1 = BetaHohenbichler(dpj1.Beta, dpj2.Beta, rho, system);

                // [1c] compute equivalent alpha-value for correlated part
                alpha1.push_back((betaj1 - betaNew) / epsilon);

                // [2] determine alpha - value for uncorrelated part
                // [2a] perturbation of beta - values

                auto dpx1 = GetRealization(designPoint1->Beta, parameters1); // copy alphas
                auto dpx2 = GetRealization(designPoint2->Beta + reorderedDesignPoint2->Values[i] * epsilon * rhoCompl, parameters2);

                // [2b] Hohenbichler computation with perturbed beta - values
                double betax1 = BetaHohenbichler(dpx1.Beta, dpx2.Beta, rho, system);

                // [2c] compute equivalent alpha - value for correlated part
                alpha2.push_back((betax1 - betaNew) / epsilon);
            }

            // combine alpha - values of correlated and uncorrelated part
            // make sure the sign is the same as the original alphas!!!!!!
            auto alphaNew = std::vector<double>();
            for (size_t j = 0; j < nVar; j++)
            {
                double alphaMax = NumericSupport::getMaxAbs(alpha1[j], alpha2[j]);
                double sign = NumericSupport::GetSign(alphaMax);
                alphaNew.push_back(sign * hypot(alpha1[j], alpha2[j] ));
            }

            // normalization
            double length = NumericSupport::GetLength(alphaNew);

            if (length == 0.0)
            {
                length = sqrt(nVar);
                for (auto &x : alphaNew) x = 1.0 / length;
            }
            else
            {
                for (auto &x : alphaNew) x /= length;
            }

            auto returnedDp = std::make_shared<DesignPoint>();
            *returnedDp = GetRealization(betaNew, parameters1, alphaNew);

            return returnedDp;
        }

        double Hohenbichler2::BetaHohenbichler(double dp1, double dp2, double rho, combineAndOr system)
        {
            double beta;
            if (dp1 > dp2)
            {
                beta = HohenbichlerNumInt(dp2, dp1, rho, system);
            }
            else
            {
                beta = HohenbichlerNumInt(dp1, dp2, rho, system);
            }
            return beta;
        }

        double Hohenbichler2::HohenbichlerNumInt(double dp1, double dp2, double rho, combineAndOr system)
        {
            constexpr double maxDiffRho = 1e-10;

            if (Numeric::NumericSupport::areEqual(rho, 1.0, maxDiffRho))
            {
                if (system == combOr)
                {
                    return std::min(dp1, dp2);
                }
                else
                {
                    return std::max(dp1, dp2);
                }
            }
            else if (Numeric::NumericSupport::areEqual(rho, -1.0, maxDiffRho))
            {
                if (system == combOr)
                {
                    double pf = std::min(1.0, StandardNormal::getPFromU(dp1) + StandardNormal::getPFromU(dp2));
                    return StandardNormal::getUFromQ(pf);
                }
                else
                {
                    double pf = abs(StandardNormal::getPFromU(dp1) - StandardNormal::getPFromU(dp2));
                    return StandardNormal::getUFromQ(pf);
                }
            }

            // otherwise: start Hohenbichler procedure

            double pCond = 0;
            double rhoCompl = sqrt(1.0 - rho * rho);

            if (-dp2 > -StandardNormal::UMax)
            {
                const int ngridPerBeta = 1000;
                int ngrid = (int)round((-dp2 + StandardNormal::UMax) * ngridPerBeta) + 1; // number of grids for numerical integration

                auto u = LinearSpaced(ngrid, -StandardNormal::UMax, -dp2); // grid end points

                auto uCentered = std::vector<double>(); // grid centers
                auto uDiff = std::vector<double>();

                for (size_t i = 0; i < u.size() - 1; i++)
                {
                    uCentered.push_back((u[i] + u[i + 1]) / 2);
                    uDiff.push_back(u[i + 1] - u[i]);
                }

                double pTotal = 0;

                for (size_t i = 0; i < uCentered.size(); i++)
                {
                    double prob = StandardNormal::getPFromU(-(dp1 + rho * uCentered[i]) / rhoCompl);
                    double pRange = StandardNormal::getPFromU(u[i + 1]) - StandardNormal::getPFromU(u[i]);
                    pTotal += pRange * prob;
                }

                pCond = pTotal / StandardNormal::getPFromU(-dp2);

                pCond = std::min(1.0, pCond);
            }

            double PfAND = pCond * StandardNormal::getQFromU(dp2);
            double betaAND = StandardNormal::getUFromP(1.0 - PfAND);

            // compute P(Z1<0 OR PZ2<0)
            double PfOR = StandardNormal::getPFromU(-dp1) + StandardNormal::getQFromU(dp2) - PfAND;
            double betaOR = StandardNormal::getUFromP(1.0 - PfOR);

            if (system == combAnd)
            {
                return betaAND;
            }
            else
            {
                return betaOR;
            }
        }

        std::vector<double> Hohenbichler2::LinearSpaced(int length, double start, double stop)
        {
            if (length < 0)
            {
                throw probLibException("length in LinearSpaced < 0");
            }

            switch (length)
            {
            case 0:
                return std::vector<double> {};
            case 1:
                return std::vector<double> { stop };
            default:
            {
                double num = (stop - start) / (double)(length - 1);
                auto array = std::vector<double>();
                for (int i = 0; i < length; i++)
                {
                    array.push_back(start + (double)i * num);
                }

                array[length - 1] = stop;
                return array;
            }
            }
        }
    }
}
