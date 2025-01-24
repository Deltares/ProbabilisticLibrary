// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "HohenbichlerNumInt.h"

#include <unordered_set>

#include "../Math/NumericSupport.h"
#include "../Statistics/StandardNormal.h"

using namespace Deltares::Statistics;
using namespace Deltares::Numeric;

namespace Deltares {
    namespace Reliability {

        using namespace Deltares::Models;

        std::shared_ptr<DesignPoint> HohenbichlerNumInt::AlphaHohenbichler(const std::shared_ptr<DesignPoint>& designPoint1, const std::shared_ptr<DesignPoint>& designPoint2,
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

            std::vector<std::shared_ptr<StochastPointAlpha>> parameters1 = designPoint1->getAlphas(stochasts);
            std::vector<std::shared_ptr<StochastPointAlpha>> parameters2 = designPoint2->getAlphas(stochasts);

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

        double HohenbichlerNumInt::BetaHohenbichler(double dp1, double dp2, const double rho, const combineAndOr system)
        {
            if (dp1 > dp2)
            {
                std::swap(dp1, dp2);
            }
            return Hohenbichler(dp1, dp2, rho, system);
        }

        double HohenbichlerNumInt::Hohenbichler(const double dp1, const double dp2, const double rho, const combineAndOr system)
        {
            constexpr double maxDiffRho = 1e-10;

            if (NumericSupport::areEqual(rho, 1.0, maxDiffRho))
            {
                if (system == combineAndOr::combOr)
                {
                    return std::min(dp1, dp2);
                }
                else
                {
                    return std::max(dp1, dp2);
                }
            }
            else if (NumericSupport::areEqual(rho, -1.0, maxDiffRho))
            {
                if (system == combineAndOr::combOr)
                {
                    double pf = std::min(1.0, StandardNormal::getPFromU(dp1) + StandardNormal::getPFromU(dp2));
                    return StandardNormal::getUFromQ(pf);
                }
                else
                {
                    double pf = std::fabs(StandardNormal::getPFromU(dp1) - StandardNormal::getPFromU(dp2));
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

                auto u = NumericSupport::LinearSpaced(ngrid, -StandardNormal::UMax, -dp2); // grid end points

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

            if (system == combineAndOr::combAnd)
            {
                double betaAND = StandardNormal::getUFromP(1.0 - PfAND);
                return betaAND;
            }
            else
            {
                // compute P(Z1<0 OR PZ2<0)
                double PfOR = StandardNormal::getPFromU(-dp1) + StandardNormal::getQFromU(dp2) - PfAND;
                double betaOR = StandardNormal::getUFromP(1.0 - PfOR);
                return betaOR;
            }
        }

        DesignPoint HohenbichlerNumInt::GetRealization(const double beta, const std::vector<std::shared_ptr<StochastPointAlpha>>& alpha)
        {
            DesignPoint dp;
            dp.Beta = beta;
            dp.Alphas = alpha;
            return dp;
        }

        DesignPoint HohenbichlerNumInt::GetRealization(const double beta, const std::vector<std::shared_ptr<StochastPointAlpha>>& alpha, const std::vector<double>& values)
        {
            DesignPoint dp;
            dp.Beta = beta;
            dp.Alphas = alpha;
            for (size_t i = 0; i < values.size(); i++)
            {
                dp.Alphas[i]->Alpha = values[i];
                dp.Alphas[i]->U = -values[i] * beta;
            }
            return dp;
        }

    }
}
