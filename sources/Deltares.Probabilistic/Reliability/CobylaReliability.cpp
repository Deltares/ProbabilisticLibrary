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
#include "CobylaReliability.h"
#include "../Optimization/CobylaOptimization.h"

using namespace Deltares::Optimization;
using namespace Deltares::Models;

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint>CobylaReliability::getDesignPoint(std::shared_ptr<ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(Settings->StochastSet);

            const int nStochasts = modelRunner->getVaryingStochastCount();

            auto sampleProvider = SampleProvider(Settings->StochastSet, false);
            auto initialSample = sampleProvider.getSample();
            double z0Fac = getZFactor(modelRunner->getZValue(initialSample));

            auto optModel = std::make_shared<wrappedOptimizationModel>(modelRunner);

            auto optimizer = CobylaOptimization();
            optimizer.settings.EpsilonBeta = Settings->EpsilonBeta;
            optimizer.settings.MaxIterations = Settings->MaximumIterations;

            auto searchArea = SearchArea();
            searchArea.Dimensions = std::vector<SearchDimension>(nStochasts);
            std::shared_ptr<Sample> startPoint = Settings->StochastSet->getStartPoint();
            for( int i = 0; i < nStochasts; i++)
            {
                searchArea.Dimensions[i].LowerBound = Settings->StochastSet->VaryingStochastSettings[i]->MinValue;
                searchArea.Dimensions[i].UpperBound = Settings->StochastSet->VaryingStochastSettings[i]->MaxValue;
                searchArea.Dimensions[i].StartValue = startPoint->Values[i];
            }

            auto result = optimizer.GetCalibrationPoint(searchArea, optModel);
            double beta = 0.0;
            for (int i = 0; i < nStochasts; i++)
            {
                beta += pow(result.Input[i], 2);
            }
            beta = z0Fac * std::sqrt(beta);

            auto dp = std::make_shared<DesignPoint>();
            dp->Identifier = "Cobyla Reliability";
            dp->Beta = beta;
            dp->convergenceReport->IsConverged = result.success;
            for (int i = 0; i < nStochasts; i++)
            {
                auto alpha = std::make_shared<StochastPointAlpha>();
                alpha->Stochast = Settings->StochastSet->stochastSettings[i]->stochast;
                alpha->U = result.Input[i];
                alpha->Alpha = -alpha->U / beta;
                alpha->X = alpha->Stochast->getXFromU(alpha->U);
                dp->Alphas.push_back(alpha);
            }

            return dp;
        };

        double wrappedOptimizationModel::GetConstraintValue(const std::shared_ptr<Sample> sample) const
        {
            auto z = modelRunner->getZValue(sample);
            return std::abs(z);
        }

        double wrappedOptimizationModel::GetZValue(std::shared_ptr<Sample> sample) const
        {
            double beta = 0.0;
            for (const auto & val : sample->Values)
            {
                beta += pow(val, 2);
            }
            return std::sqrt(beta);
        }
    }
}


