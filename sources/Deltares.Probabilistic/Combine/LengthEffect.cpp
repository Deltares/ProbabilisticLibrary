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
#include "LengthEffect.h"
#include "upscaling.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Reliability
    {
        DesignPoint LengthEffect::UpscaleLength(std::shared_ptr<DesignPoint> crossSection,
            const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix,
            const std::vector<double>& correlationLengths,
            const double length)
        {
            const size_t nStochasts = crossSection->Alphas.size();
            auto selfCorrelation = std::vector<double>();

            for (size_t i = 0; i < nStochasts; i++)
            {
                auto stochast = crossSection->Alphas[i]->Stochast;
                selfCorrelation.push_back(selfCorrelationMatrix->getSelfCorrelation(stochast));
            }

            return UpscaleLength(crossSection, selfCorrelation, correlationLengths, length);
        };

        DesignPoint LengthEffect::UpscaleLength(std::shared_ptr<DesignPoint> crossSection,
            const std::vector<double>& selfCorrelations,
            const std::vector<double>& correlationLengths,
            const double length)
        {
            const size_t nStochasts = crossSection->Alphas.size();

            auto up = upscaling();
            auto alpha = vector1D(nStochasts);
            for (size_t i = 0; i < nStochasts; i++)
            {
                alpha(i) = crossSection->Alphas[i]->Alpha;
            }
            auto dp = alphaBeta(crossSection->Beta, alpha);
            vector1D rho1(nStochasts);
            vector1D rho2(nStochasts);
            for (size_t i = 0; i < nStochasts; i++)
            {
                rho1(i) = selfCorrelations[i];
                rho2(i) = correlationLengths[i];
            }

            auto message = std::make_shared<Message>();
            message->Type = MessageType::Debug;
            auto dpLength = up.upscaleLength(dp, rho1, rho2, length, message->Text);

            auto dpL = DesignPoint();
            dpL.Identifier = "Length Effect";
            dpL.Beta = dpLength.first.getBeta();
            dpL.Messages.push_back(message);
            for (size_t i = 0; i < nStochasts; i++)
            {
                auto alphaValue = dpLength.first.getAlphaI(i);
                auto alpha = std::make_shared<StochastPointAlpha>();
                alpha->Alpha = alphaValue;
                alpha->Stochast = crossSection->Alphas[i]->Stochast;
                alpha->U = -dpL.Beta * alphaValue;
                alpha->X = alpha->Stochast->getXFromU(alpha->U);
                dpL.Alphas.push_back(alpha);
            }
            dpL.ContributingDesignPoints.push_back(crossSection);

            return dpL;
        };

    }
}

