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
#include "LengthEffect.h"
#include "upscaling.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Reliability
    {
        DesignPoint LengthEffect::UpscaleLength(DesignPoint& section,
            const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix,
            const std::vector<double>& correlationLengths,
            const double length, const double breachLength)
        {
            const size_t nStochasts = section.Alphas.size();
            auto selfCorrelation = std::vector<double>();

            for (size_t i = 0; i < nStochasts; i++)
            {
                auto stochast = section.Alphas[i]->Stochast;
                selfCorrelation.push_back(selfCorrelationMatrix->getSelfCorrelation(stochast));
            }

            return UpscaleLength(section, selfCorrelation, correlationLengths, length, breachLength);
        };

        DesignPoint LengthEffect::UpscaleLength(DesignPoint& section,
            const std::vector<double>& selfCorrelations,
            const std::vector<double>& correlationLengths,
            const double length, const double breachLength)
        {
            const size_t nStochasts = section.Alphas.size();

            auto up = upscaling();
            auto alpha = vector1D(nStochasts);
            for (size_t i = 0; i < nStochasts; i++)
            {
                alpha(i) = section.Alphas[i]->Alpha;
            }
            auto dp = alphaBeta(section.Beta, alpha);
            vector1D rho1(nStochasts);
            vector1D rho2(nStochasts);
            for (size_t i = 0; i < nStochasts; i++)
            {
                rho1(i) = selfCorrelations[i];
                rho2(i) = correlationLengths[i];
            }

            auto dpLength = up.upscaleLength(dp, rho1, rho2, length, breachLength);

            auto dpL = DesignPoint();
            dpL.Beta = dpLength.first.getBeta();
            for (size_t i = 0; i < nStochasts; i++)
            {
                auto alphaValue = dpLength.first.getAlphaI(i);
                auto alpha = std::make_shared<StochastPointAlpha>();
                alpha->Alpha = alphaValue;
                alpha->Stochast = section.Alphas[i]->Stochast;
                dpL.Alphas.push_back(alpha);
            }

            return dpL;
        };

    }
}

