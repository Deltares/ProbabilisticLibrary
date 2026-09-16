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
#include "CobylaReliability.h"
#include "../Optimization/CobylaOptimization.h"

using namespace Deltares::Optimization;
using namespace Deltares::Models;

namespace Deltares::Reliability
{
    std::shared_ptr<DesignPoint>CobylaReliability::getDesignPoint(std::shared_ptr<ModelRunner> modelRunner)
    {
        modelRunner->updateStochastSettings(Settings->StochastSet);

        const int nStochasts = modelRunner->getVaryingStochastCount();

        auto sampleProvider = SampleProvider(*Settings->StochastSet);
        auto initialSample = sampleProvider.getSample();
        double z0Fac = getZFactor(modelRunner->getZValue(initialSample));

        auto optModel = WrappedOptimizationModel(modelRunner, z0Fac, Settings->MaximumIterations);
        optModel.uMean = DesignPointBuilder(nStochasts, Settings->designPointMethod, this->Settings->StochastSet);

        auto optimizer = CobylaOptimization();
        optimizer.settings.EpsilonBeta = Settings->EpsilonBeta;
        optimizer.settings.MaxIterations = Settings->MaximumIterations;

        auto searchArea = SearchArea();
        searchArea.Dimensions = std::vector<SearchDimension>(nStochasts);
        Sample startPoint = Settings->StochastSet->getStartPoint();
        for( int i = 0; i < nStochasts; i++)
        {
            searchArea.Dimensions[i].LowerBound = Settings->StochastSet->VaryingStochastSettings[i]->MinValue;
            searchArea.Dimensions[i].UpperBound = Settings->StochastSet->VaryingStochastSettings[i]->MaxValue;
            searchArea.Dimensions[i].StartValue = startPoint.Values[i];
        }

        auto result = optimizer.GetCalibrationPoint(searchArea, optModel);

        double beta = z0Fac * result.getLength();

        auto uMin = optModel.uMean.getSample();
        std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();
        convergenceReport->IsConverged = result.success;
        std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(uMin, beta, convergenceReport, "Cobyla Reliability");

        return designPoint;
    };

    double WrappedOptimizationModel::GetConstraintValue(Sample& sample)
    {
        auto z = modelRunner->getZValue(sample);

        modelRunner->reportProgress(++counter, maxIterations, z0Fac * sample.getBeta());

        if (z * z0Fac < 0.0)
        {
            uMean.addSample(sample);
        }
        return std::abs(z);
    }

    double WrappedOptimizationModel::GetZValue(Sample& sample) const
    {
        return sample.getBeta();
    }
}

