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
#include "FOSM.h"
#include <vector>
#include <cmath>
#include <memory>

#include "../Math/NumericSupport.h"
#include "../Model/Sample.h"
#include "../Model/GradientCalculator.h"
#include "../Statistics/DistributionType.h"

using namespace Deltares::Models;

namespace Deltares
{
    namespace Sensitivity
    {
        Sensitivity::SensitivityResult FOSM::getSensitivityStochast(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            int nStochasts = modelRunner->getVaryingStochastCount();

            std::shared_ptr<Models::GradientCalculator> gradientCalculator = std::make_shared<GradientCalculator>();
            gradientCalculator->Settings = std::make_shared<GradientSettings>();
            gradientCalculator->Settings->StepSize = Settings->StepSize;
            gradientCalculator->Settings->gradientType = GradientType::OneDirection;

            std::shared_ptr<Sample> zeroSample = std::make_shared<Sample>(nStochasts);

            std::vector<double> gradient = gradientCalculator->getGradient(modelRunner, zeroSample);
            double length = Numeric::NumericSupport::GetLength(gradient);
            double z0 = zeroSample->Z;

            // disturb slightly if no gradient found
            if (length < 1E-6)
            {
                std::shared_ptr<Sample>  disturbedSample = std::make_shared<Sample>(Numeric::NumericSupport::select(zeroSample->Values, [](double p) { return 0.001; }));
                gradient = gradientCalculator->getGradient(modelRunner, disturbedSample);
            }

            std::shared_ptr<Sample> nextSample = std::make_shared<Sample>(gradient);
            nextSample = nextSample->getNormalizedSample();
            double z1 = modelRunner->getZValue(nextSample);

            if (this->correlationMatrixBuilder->isEmpty() && this->Settings->CalculateCorrelations && this->Settings->CalculateInputCorrelations)
            {
                modelRunner->registerSample(this->correlationMatrixBuilder, nextSample);
            }

            std::shared_ptr<Statistics::Stochast> stochast = std::make_shared<Statistics::Stochast>();

            double deviation = std::abs(z1 - z0);
            Statistics::DistributionType distributionType = deviation > 0.0
                ? Statistics::DistributionType::Normal
                : Statistics::DistributionType::Deterministic;

            stochast->setDistributionType(distributionType);
            stochast->setMeanAndDeviation(z0, deviation);

            auto result = modelRunner->getSensitivityResult(stochast);

            for (std::shared_ptr<Statistics::ProbabilityValue> quantile : this->Settings->RequestedQuantiles)
            {
                std::shared_ptr<Sample> quantileSample = nextSample->getSampleAtBeta(quantile->Reliability);
                std::shared_ptr<Models::Evaluation> evaluation = std::make_shared<Models::Evaluation>(modelRunner->getEvaluation(quantileSample));
                result.quantileEvaluations.push_back(evaluation);
            }

            if (this->Settings->CalculateCorrelations)
            {
                this->correlationMatrixBuilder->registerSamples(stochast, std::vector<double> {z1});
            }

            return result;
        }
    }
}



