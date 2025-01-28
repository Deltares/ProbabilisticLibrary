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

#include "ExcludingCombiner.h"
#include "../Statistics/Scenario.h"
#include "../Statistics/Stochast.h"

namespace Deltares
{
    namespace Reliability
    {
        std::unique_ptr<DesignPoint> ExcludingCombiner::combineDesignPoints(std::vector<std::shared_ptr<Statistics::Scenario>>& scenarios, std::vector<std::shared_ptr<Reliability::DesignPoint>>& designPoints)
        {
            if (scenarios.size() != designPoints.size())
            {
                throw probLibException("Scenarios and design points must be of same length");
            }

            std::vector<std::shared_ptr<Statistics::Stochast>> parameters = this->getUniqueStochasts(designPoints);

            std::shared_ptr<DesignPointBuilder> designPointBuilder = std::make_shared<DesignPointBuilder>(parameters.size(), DesignPointMethod::CenterOfGravity);

            double failure = 0;
            for (size_t i = 0; i < designPoints.size(); i++)
            {
                failure += scenarios[i]->probability * designPoints[i]->getFailureProbability();
            }

            double combinedBeta = Statistics::StandardNormal::getUFromQ(failure);
            designPointBuilder->initialize(combinedBeta);

            for (int i = 0; i < designPoints.size(); i++)
            {
                designPoints[i]->Identifier = scenarios[i]->name;

                std::vector<double> u(parameters.size());
                for (int j = 0; j < u.size(); j++)
                {
                    if (parameters[j] == scenarios[i]->parameter)
                    {
                        u[j] = scenarios[i]->parameter->getUFromX(scenarios[i]->parameterValue);
                    }
                    else
                    {
                        std::shared_ptr<StochastPointAlpha> alpha = designPoints[i]->getAlpha(parameters[j]);
                        u[j] = alpha != nullptr ? alpha->U : 0;
                    }
                }

                std::shared_ptr<Sample> sample = std::make_shared<Sample>(u);
                sample->Weight = scenarios[i]->probability * designPoints[i]->getFailureProbability();

                designPointBuilder->addSample(sample);
            }

            std::shared_ptr<Sample> combinedSample = designPointBuilder->getSample();
            combinedSample = combinedSample->getSampleAtBeta(combinedBeta);

            // create final design point
            std::unique_ptr<DesignPoint> combinedDesignPoint = std::make_unique<DesignPoint>();
            combinedDesignPoint->Beta = combinedBeta;

            // create alpha values for final design point

            for (size_t i = 0; i < parameters.size(); i++)
            {
                const double alphaValue = -combinedSample->Values[i] / combinedDesignPoint->Beta;
                std::shared_ptr<StochastPointAlpha> alpha = std::make_shared<StochastPointAlpha>();
                alpha->Stochast = parameters[i];
                alpha->Alpha = alphaValue;
                alpha->AlphaCorrelated = alphaValue;
                alpha->U = -combinedDesignPoint->Beta * alphaValue;
                alpha->X = parameters[i]->getXFromU(alpha->U);
                combinedDesignPoint->Alphas.push_back(alpha);
            }

            for (std::shared_ptr<DesignPoint> designPoint : designPoints)
            {
                combinedDesignPoint->ContributingDesignPoints.push_back(designPoint);
            }

            combinedDesignPoint->Identifier = "System";

            return combinedDesignPoint;
        }
    }
}

