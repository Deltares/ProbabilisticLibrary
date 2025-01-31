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
#include <vector>
#include <memory>

#include "ExcludingCombiner.h"
#include "HohenbichlerExcludingCombiner.h"

#include "combineElements.h"

namespace Deltares
{
    namespace Reliability
    {
        std::unique_ptr<DesignPoint> HohenbichlerExcludingCombiner::combineExcludingDesignPoints(
            std::vector<std::shared_ptr<Statistics::Scenario>>& scenarios,
            std::vector<std::shared_ptr<Reliability::DesignPoint>>& designPoints)
        {
            elements designPointElements;
            const std::vector<std::shared_ptr<Statistics::Stochast>> stochasts = DesignPoint::getUniqueStochasts(designPoints);
            const auto nStochasts = stochasts.size();

            for (const auto& designPoint : designPoints)
            {
                const auto reorderedDesignPoint = designPoint->getSampleForStochasts(stochasts);
                auto alpha = vector1D(nStochasts);
                for (size_t i = 0; i < nStochasts; i++)
                {
                    alpha(i) = -(reorderedDesignPoint->Values[i] / designPoint->Beta);
                }
                auto designPointElement = alphaBeta(designPoint->Beta, alpha);
                designPointElements.push_back(designPointElement);
            }

            combineElements combiner = combineElements();
            std::vector<double> percentages = std::vector<double>(nStochasts);
            for (size_t i = 0; i < scenarios.size(); i++)
            {
                percentages[i] = scenarios[i]->probability * 100.0;
            }

            auto result = combiner.combineMultipleElementsProb(designPointElements, percentages, combineAndOr::combOr);

            std::unique_ptr<DesignPoint> combinedDesignPoint = std::make_unique<DesignPoint>();
            combinedDesignPoint->Beta = result.ab.getBeta();
            for (size_t i = 0; i < nStochasts; i++)
            {
                auto alpha = std::make_shared<StochastPointAlpha>();
                alpha->Stochast = stochasts[i];
                alpha->Alpha = result.ab.getAlphaI(i);
                combinedDesignPoint->Alphas.push_back(alpha);
            }

            for (std::shared_ptr<DesignPoint> designPoint : designPoints)
            {
                combinedDesignPoint->ContributingDesignPoints.push_back(designPoint);
            }

            return combinedDesignPoint;
        }
    }
}

