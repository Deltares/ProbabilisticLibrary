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
#include "HohenbichlerFormCombiner.h"
#include "combineElements.h"
#include "alphaBeta.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> HohenbichlerFormCombiner::combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress)
        {
            elements elm;
            const std::vector<std::shared_ptr<Statistics::Stochast>> stochasts = getUniqueStochasts(designPoints);
            const auto nStochasts = stochasts.size();

            for (const auto& designPoint : designPoints)
            {
                const auto reorderedDesignPoint = designPoint->getSampleForStochasts(stochasts);
                auto alpha = vector1D(nStochasts);
                for (size_t i = 0; i < nStochasts; i++)
                {
                    alpha(i) = -(reorderedDesignPoint->Values[i] / designPoint->Beta);
                }
                auto dp = alphaBeta(designPoint->Beta, alpha);
                elm.push_back(dp);
            }

            auto cmb = combineElements();
            auto rho = vector1D(nStochasts);
            for (size_t i = 0; i < nStochasts; i++)
            {
                rho(i) = selfCorrelationMatrix->getSelfCorrelation(stochasts[i]);
            }

            auto result = cmb.combineMultipleElements(elm, rho, combineMethodType);
            nonConvergedForm += result.n;

            auto dp = std::make_shared<DesignPoint>();
            dp->Beta = result.ab.getBeta();
            for (size_t i = 0; i < nStochasts; i++)
            {
                auto alpha = std::make_shared<StochastPointAlpha>();
                alpha->Stochast = stochasts[i];
                alpha->Alpha = result.ab.getAlphaI(i);
                dp->Alphas.push_back(alpha);
            }
            return dp;
        }

    };
}

