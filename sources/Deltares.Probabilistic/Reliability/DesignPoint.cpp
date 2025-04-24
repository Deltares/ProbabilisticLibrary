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

#include "FragilityCurve.h"
#include "DesignPoint.h"

#include <unordered_set>

#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Models;

        void DesignPoint::expandContributions()
        {
            this->expandFragilityCurves();
            this->updateInfluenceFactors();
        }

        void DesignPoint::expandFragilityCurves()
        {
            std::vector<std::shared_ptr<StochastPointAlpha>> stochastRealizations(Alphas);

            // replace fragility curve by stochasts in fragility points
            for (auto stochastRealization : stochastRealizations)
            {
                if (stochastRealization->Stochast->isFragilityCurve())
                {
                    expandStochastRealization(stochastRealization);
                }
            }
        }

        void DesignPoint::expandStochastRealization(std::shared_ptr<StochastPointAlpha> stochastRealization)
        {
            std::shared_ptr<FragilityCurve> fragilityCurve = std::static_pointer_cast<FragilityCurve>(stochastRealization->Stochast);

            std::shared_ptr<StochastPoint> fragilityCurveAlpha = fragilityCurve->getDesignPoint(stochastRealization->X);

            if (fragilityCurveAlpha != nullptr && !fragilityCurveAlpha->Alphas.empty() && fragilityCurveAlpha->Alphas[0]->Stochast != fragilityCurve)
            {
                std::erase(Alphas, stochastRealization);

                double factor = fragilityCurve->getAlphaFactor();
                if (stochastRealization->Stochast->isFragilityCurve())
                {
                    factor = -1;
                }

                for (std::shared_ptr<StochastPointAlpha> alphaRealization : fragilityCurveAlpha->Alphas)
                {
                    // merge alpha values for same stochast from different fragility curves,
                    // therefore find an already existing alpha value for the stochast

                    std::shared_ptr<StochastPointAlpha> subAlpha = getStochastPoint(alphaRealization);

                    double alpha = factor * stochastRealization->Alpha * alphaRealization->Alpha;
                    double prevailingSign = Numeric::NumericSupport::GetPrevailingSign(std::vector{ subAlpha->Alpha, alpha });

                    subAlpha->Alpha = prevailingSign * std::sqrt(subAlpha->Alpha * subAlpha->Alpha + alpha * alpha);
                    subAlpha->AlphaCorrelated = subAlpha->Alpha;
                    subAlpha->U = -this->Beta * subAlpha->Alpha;
                    subAlpha->X = subAlpha->Stochast->getXFromU(subAlpha->U);
                }

                // update variable stochasts
                updateVariableStochasts(fragilityCurveAlpha);
            }
        }

        void DesignPoint::updateVariableStochasts(std::shared_ptr<StochastPoint> fragilityCurveAlpha)
        {
            for (std::shared_ptr<StochastPointAlpha> alpha : fragilityCurveAlpha->Alphas)
            {
                if (alpha->Stochast->isVariable())
                {
                    std::shared_ptr<Statistics::Stochast> alphaSource = alpha->Stochast->getVariableSource();
                    for (std::shared_ptr<StochastPointAlpha> alpha2 : fragilityCurveAlpha->Alphas)
                    {
                        if (alpha2->Stochast == alphaSource)
                        {
                            alpha->X = alpha->Stochast->getXFromUAndSource(alpha2->X, alpha->U);
                        }
                    }
                }
            }
        }

        std::shared_ptr<StochastPointAlpha> DesignPoint::getStochastPoint(std::shared_ptr<StochastPointAlpha> alphaRealization)
        {
            std::shared_ptr<StochastPointAlpha> subAlpha = nullptr;
            for (std::shared_ptr<StochastPointAlpha> existingAlpha : this->Alphas)
            {
                if (existingAlpha->Stochast == alphaRealization->Stochast)
                {
                    return existingAlpha;
                }
            }

            subAlpha = std::make_shared<StochastPointAlpha>();
            subAlpha->Stochast = alphaRealization->Stochast;
            this->Alphas.push_back(subAlpha);

            return subAlpha;
        }

        void DesignPoint::correctFragilityCurves()
        {
            if (this->Beta < 0)
            {
                for (std::shared_ptr<StochastPointAlpha> stochastRealization : this->Alphas)
                {
                    std::shared_ptr<FragilityCurve> fragilityCurve = std::dynamic_pointer_cast<FragilityCurve>(stochastRealization->Stochast);
                    if (fragilityCurve != nullptr && fragilityCurve->inverted)
                    {
                        stochastRealization->invert();
                    }
                }
            }
        }

        std::vector<std::shared_ptr<Statistics::Stochast>> DesignPoint::getUniqueStochasts(const std::vector<std::shared_ptr<DesignPoint>>& designPoints)
        {
            std::vector<std::shared_ptr<Statistics::Stochast>> uniqueStochasts;

            std::unordered_set<std::shared_ptr<Statistics::Stochast>> addedStochasts;

            // when multiple identical stochasts are present in one design point, they should all be added to the unique design points

            for (const std::shared_ptr<DesignPoint>& designPoint : designPoints)
            {
                for (std::shared_ptr<StochastPointAlpha>& alpha : designPoint->Alphas)
                {
                    if (!addedStochasts.contains(alpha->Stochast))
                    {
                        uniqueStochasts.push_back(alpha->Stochast);
                    }
                }

                for (std::shared_ptr<StochastPointAlpha>& alpha : designPoint->Alphas)
                {
                    if (!addedStochasts.contains(alpha->Stochast))
                    {
                        addedStochasts.insert(alpha->Stochast);
                    }
                }
            }

            return uniqueStochasts;
        }

        int DesignPoint::getTotalModelRuns() const
        {
            int totalModelRuns = this->convergenceReport != nullptr ?  this->convergenceReport->TotalModelRuns : 0;
            for (const std::shared_ptr<DesignPoint>& contributingDesignPoint : ContributingDesignPoints)
            {
                totalModelRuns += contributingDesignPoint->getTotalModelRuns();
            }

            return totalModelRuns;
        }
    }
}

