// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//

#include "FragilityCurve.h"
#include "DesignPoint.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Reliability
    {
        void DesignPoint::expandContributions()
        {
            this->expandContributingFragilityCurves();
            this->expandFragilityCurves();
            this->updateInfluenceFactors();
        }

        void DesignPoint::expandFragilityCurves()
        {
            std::vector<std::shared_ptr<StochastPointAlpha>> stochastRealizations(Alphas);

            // replace fragility curve by stochasts in fragility points
            for (auto stochastRealization : stochastRealizations)
            {
                std::shared_ptr<FragilityCurve> fragilityCurve = std::dynamic_pointer_cast<FragilityCurve>(stochastRealization->Stochast);
                if (fragilityCurve != nullptr)
                {
                    std::shared_ptr<StochastPoint> fragilityCurveAlpha = fragilityCurve->getDesignPoint(stochastRealization->X);

                    if (fragilityCurveAlpha != nullptr && !fragilityCurveAlpha->Alphas.empty() && fragilityCurveAlpha->Alphas[0]->Stochast != fragilityCurve)
                    {
                        std::erase(Alphas, stochastRealization);

                        double factor = (fragilityCurve->isGloballyDescending() ^ fragilityCurve->inverted) ? -1 : 1;
                        if (std::dynamic_pointer_cast<FragilityCurve>(stochastRealization->Stochast) != nullptr)
                        {
                            factor = -1;
                        }

                        for (std::shared_ptr<StochastPointAlpha> alphaRealization : fragilityCurveAlpha->Alphas)
                        {
                            // merge alpha values for same stochast from different fragility curves,
                            // therefore find an already existing alpha value for the stochast

                            std::shared_ptr<StochastPointAlpha> subAlpha = nullptr;
                            for (std::shared_ptr<StochastPointAlpha> existingAlpha : this->Alphas)
                            {
                                if (existingAlpha->Stochast == alphaRealization->Stochast)
                                {
                                    subAlpha = existingAlpha;
                                    break;
                                }
                            }

                            if (subAlpha == nullptr)
                            {
                                subAlpha = std::make_shared<StochastPointAlpha>();
                                subAlpha->Stochast = alphaRealization->Stochast;
                                this->Alphas.push_back(subAlpha);
                            }

                            double alpha = factor * stochastRealization->Alpha * alphaRealization->Alpha;
                            int prevailingSign = std::fabs(subAlpha->Alpha) > std::fabs(alpha) ? Numeric::NumericSupport::GetSign(subAlpha->Alpha) : Numeric::NumericSupport::GetSign(alpha);

                            subAlpha->Alpha = prevailingSign * std::sqrt(subAlpha->Alpha * subAlpha->Alpha + alpha * alpha);
                            subAlpha->AlphaCorrelated = subAlpha->Alpha;
                            subAlpha->U = - this->Beta * subAlpha->Alpha;
                            subAlpha->X = subAlpha->Stochast->getXFromU(subAlpha->U);
                        }
                    }
                }
            }
        }

        void DesignPoint::expandContributingFragilityCurves()
        {
            std::vector<std::shared_ptr<StochastPointAlpha>> stochastRealizations(Alphas);

            // replace fragility curve by fragility curves by which it is built up
            for (auto stochastRealization : stochastRealizations)
            {
                std::shared_ptr<FragilityCurve> fragilityCurve = std::dynamic_pointer_cast<FragilityCurve>(stochastRealization->Stochast);
                if (fragilityCurve != nullptr)
                {
                    double uCurve = stochastRealization->U;

                    for (std::shared_ptr<FragilityCurve> subCurve : fragilityCurve->contributingFragilityCurves)
                    {
                        if (subCurve != fragilityCurve)
                        {
                            std::erase(this->Alphas, stochastRealization);

                            double xValue = subCurve->getXFromU(uCurve);

                            std::shared_ptr<StochastPointAlpha> subAlpha = std::make_shared<StochastPointAlpha>();
                            subAlpha->Stochast = subCurve;
                            subAlpha->Alpha = stochastRealization->Alpha;
                            subAlpha->AlphaCorrelated = stochastRealization->AlphaCorrelated;
                            subAlpha->U = uCurve;
                            subAlpha->X = xValue;
                            this->Alphas.push_back(subAlpha);
                        }
                    }
                }
            }
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
    }
}

