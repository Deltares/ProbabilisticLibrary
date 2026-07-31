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

#include <set>

#include "../Math/NumericSupport.h"

namespace Deltares::Reliability
{
    using namespace Deltares::Models;
    using namespace Deltares::Statistics;

    bool FragilityCurve::isGloballyDescending()
    {
        if (this->getProperties()->FragilityValues.size() > 1)
        {
            this->initializeForRun();

            return this->getProperties()->FragilityValues.front()->Reliability > this->getProperties()->FragilityValues.back()->Reliability;
        }
        else
        {
            return false;
        }
    }

    double FragilityCurve::getAlphaFactor()
    {
        // exclusive or
        return (this->isGloballyDescending() ^ this->inverted) ? -1.0 : 1.0;
    }

    std::shared_ptr<StochastPoint> FragilityCurve::getDesignPoint(double x)
    {
        // check whether there is a fragility value exactly at x
        for (const auto& fragility_value : getProperties()->FragilityValues)
        {
            if (fragility_value->designPoint != nullptr && Numeric::NumericSupport::areEqual(x, fragility_value->X, margin))
            {
                return fragility_value->designPoint;
            }
        }

        std::shared_ptr<FragilityValue> lowerFragilityValue = nullptr;
        std::shared_ptr<FragilityValue> upperFragilityValue = nullptr;

        for (const auto& fragilityValue : getProperties()->FragilityValues)
        {
            if (fragilityValue->designPoint != nullptr)
            {
                if (fragilityValue->X < x) lowerFragilityValue = fragilityValue;
                if (fragilityValue->X > x && upperFragilityValue == nullptr) upperFragilityValue = fragilityValue;
            }
        }

        bool lowerPresent = lowerFragilityValue != nullptr;
        bool upperPresent = upperFragilityValue != nullptr;

        if (!lowerPresent && !upperPresent)
        {
            return nullptr;
        }
        else if (!lowerPresent || !upperPresent)
        {
            std::shared_ptr<FragilityValue> fragilityValue = lowerPresent ? lowerFragilityValue : upperFragilityValue;

            double beta = this->getUFromX(x);
            return fragilityValue->designPoint->getCopy(beta, margin);
        }
        else
        {
            return getRealizationBetweenPoints(x, *lowerFragilityValue, *upperFragilityValue);
        }
    }

    std::shared_ptr<StochastPoint> FragilityCurve::getRealizationBetweenPoints(double x,
        const FragilityValue& lower_fragility_value, const FragilityValue& upper_fragility_value)
    {
        // create a list of all stochasts

        std::vector<std::shared_ptr<Stochast>> contributingStochasts;
        std::set<std::shared_ptr<Stochast>> contributingStochastsMap;

        for (const std::shared_ptr<StochastPointAlpha>& alpha : lower_fragility_value.designPoint->Alphas)
        {
            if (!contributingStochastsMap.contains(alpha->Stochast))
            {
                contributingStochasts.push_back(alpha->Stochast);
                contributingStochastsMap.insert(alpha->Stochast);
            }
        }

        for (const std::shared_ptr<StochastPointAlpha>& alpha : upper_fragility_value.designPoint->Alphas)
        {
            if (!contributingStochastsMap.contains(alpha->Stochast))
            {
                contributingStochasts.push_back(alpha->Stochast);
                contributingStochastsMap.insert(alpha->Stochast);
            }
        }

        std::shared_ptr<StochastPoint> designPoint = std::make_shared<StochastPoint>();
        designPoint->Beta = this->getUFromX(x);

        double upperFraction = (x - lower_fragility_value.X) / (upper_fragility_value.X - lower_fragility_value.X);
        double lowerFraction = 1 - upperFraction;

        double normal = 0;

        for (const auto& stochast : contributingStochasts)
        {
            std::shared_ptr<StochastPointAlpha> alpha = std::make_shared<StochastPointAlpha>();
            alpha->Stochast = stochast;

            std::shared_ptr<StochastPointAlpha> lowerRealization = lower_fragility_value.designPoint->getAlpha(stochast);
            std::shared_ptr<StochastPointAlpha> upperRealization = upper_fragility_value.designPoint->getAlpha(stochast);

            double lowerAlpha = lowerRealization == nullptr ? 0 : lowerRealization->Alpha;
            double upperAlpha = upperRealization == nullptr ? 0 : upperRealization->Alpha;

            alpha->Alpha = interpolateAlphas(lowerFraction, lowerAlpha, upperAlpha);

            double lowerCorrelatedAlpha = lowerRealization == nullptr ? 0 : lowerRealization->AlphaCorrelated;
            double upperCorrelatedAlpha = upperRealization == nullptr ? 0 : upperRealization->AlphaCorrelated;

            alpha->AlphaCorrelated = interpolateAlphas(lowerFraction, lowerCorrelatedAlpha, upperCorrelatedAlpha);

            if (std::fabs(alpha->Alpha) > 0 || std::fabs(alpha->AlphaCorrelated) > 0)
            {
                normal += alpha->Alpha * alpha->Alpha;
                designPoint->Alphas.push_back(alpha);
            }
        }

        normal = std::sqrt(normal);

        for (const std::shared_ptr<StochastPointAlpha>& alpha : designPoint->Alphas)
        {
            alpha->Alpha /= normal;
            alpha->AlphaCorrelated /= normal;
            alpha->U = - designPoint->Beta * alpha->Alpha;
            alpha->X = alpha->Stochast->getXFromU(-designPoint->Beta * alpha->AlphaCorrelated);
        }

        return designPoint;
    }

    double FragilityCurve::interpolateAlphas(double lowerFraction, double lowerAlpha, double upperAlpha)
    {
        double lowerInfluenceFactor = lowerFraction * lowerAlpha * lowerAlpha;
        double upperInfluenceFactor = (1 - lowerFraction) * upperAlpha * upperAlpha;

        double newInfluenceFactor = lowerInfluenceFactor + upperInfluenceFactor;
        double newAlpha = std::sqrt(newInfluenceFactor);

        newAlpha *= lowerInfluenceFactor > upperInfluenceFactor ? Numeric::NumericSupport::GetSign(lowerAlpha) : Numeric::NumericSupport::GetSign(upperAlpha);

        return newAlpha;
    }
}
