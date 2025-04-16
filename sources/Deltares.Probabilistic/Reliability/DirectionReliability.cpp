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
#include "DirectionReliability.h"
#include "DirectionReliabilitySettings.h"
#include "DirectionCalculation.h"
#include "../Model/ModelRunner.h"
#include "../Math/NumericSupport.h"
#include "DirectionSectionsCalculation.h"
#include <memory>

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Numeric;

        std::shared_ptr<DesignPoint> DirectionReliability::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            std::shared_ptr<Sample> zeroSample = std::make_shared<Sample>(modelRunner->getVaryingStochastCount());
            double z = modelRunner->getZValue(zeroSample);
            double z0 = getZFactor(z);

            std::shared_ptr<Sample> directionSample = this->Settings->StochastSet->getStartPoint();

            double beta = getBeta(*modelRunner, *directionSample, z0);

            std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(directionSample, beta);

            return designPoint;
        }

        double DirectionReliability::getBeta(Models::ModelRunner& modelRunner, Sample& directionSample, double z0)
        {
            auto zValues = PrecomputeValues();
            return getBeta(modelRunner, directionSample, z0, zValues);
        }

        double DirectionReliability::getBeta(Models::ModelRunner& modelRunner, Sample& directionSample,
            double z0, const PrecomputeValues& zValues)
        {
            auto normalizedSample = directionSample.getNormalizedSample();

            auto task = BetaValueTask(normalizedSample, z0 < 0.0);

            double beta = getDirectionBeta(modelRunner, task, zValues);
            beta *= z0;

            directionSample.AllowProxy = task.UValues->AllowProxy;

            return beta;
        }

        double DirectionReliability::getDirectionBeta(Models::ModelRunner& modelRunner,
            const BetaValueTask& directionTask, const PrecomputeValues& zValues)
        {
            if (modelRunner.canCalculateBeta())
            {
                return modelRunner.getBeta(directionTask.UValues);
            }
            else
            {
                auto sectionsCalc = DirectionSectionsCalculation();
                auto sections = sectionsCalc.getDirectionSections(modelRunner, directionTask, zValues);

                double beta = getBetaFromSections(sections);

                directionTask.UValues->AllowProxy = directionTask.UValues->AllowProxy;

                return beta;
            }
        }

        double DirectionReliability::getBetaFromSections(const std::vector<DirectionSection>& sections) const
        {
            // sum the probabilities
            double failingProbability = 0.0;
            double nonFailingProbability = 0.5; // start counting at u = 0

            for (int i = sections.size() - 1; i >= 0; i--)
            {
                switch (sections[i].Type)
                {
                case DoubleType::Positive:
                    nonFailingProbability += sections[i].getProbability();
                    break;
                case DoubleType::Negative:
                    failingProbability += sections[i].getProbability();
                    break;
                default:
                    //nothing to do
                    break;
                }
            }

            if (failingProbability == 0 && nonFailingProbability == 0.5)
            {
                return nan("");
            }
            else if (nonFailingProbability == 0.5 && this->Settings->FindMinimalValue)
            {
                double zmin = 1e99;
                double rmin = 0.0;
                for (const auto& section : sections)
                {
                    if (section.ZHigh < zmin && section.ZHigh != 0.0)
                    {
                        rmin = section.UHigh;
                        zmin = section.ZHigh;
                    }
                }
                return rmin;
            }
            else
            {
                double probFailure = failingProbability / (failingProbability + nonFailingProbability);
                return Statistics::StandardNormal::getUFromQ(probFailure);
            }
        }

    }
}


