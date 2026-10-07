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

#include <iostream>

#include "../Model/SampleStorage.h"
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

        DesignPointBuilder uMean = DesignPointBuilder(nStochasts, Settings->designPointMethod, this->Settings->StochastSet);

        CobylaOptimization optimizer;
        optimizer.Settings.EpsilonBeta = Settings->EpsilonBeta;
        optimizer.Settings.MaxIterations = Settings->MaximumIterations;

        auto searchArea = optimizer.Settings.SearchArea;
        searchArea->setDimensions(nStochasts);
        Sample startPoint = Settings->StochastSet->getStartPoint();
        for( int i = 0; i < nStochasts; i++)
        {
            searchArea->Dimensions[i] = std::make_shared<SearchParameterSettings>();
            searchArea->Dimensions[i]->MinValue = Settings->StochastSet->VaryingStochastSettings[i]->MinValue;
            searchArea->Dimensions[i]->MaxValue = Settings->StochastSet->VaryingStochastSettings[i]->MaxValue;
            searchArea->Dimensions[i]->StartValue = startPoint.Values[i];
        }

        int counter = 0;
        int* pCounter = &counter;

        ZModel zModel = getZModelForModelRunner(*modelRunner, uMean, Settings->MaximumIterations, z0Fac, pCounter);

        auto result = optimizer.getOptimizedSample(zModel);
        double beta = z0Fac * result.minimumValue;

        auto uMin = uMean.getSample();
        std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();
        convergenceReport->IsConverged = result.succeeded;
        std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(uMin, beta, convergenceReport, "Cobyla Reliability");

        return designPoint;
    };

    ZModel CobylaReliability::getZModelForModelRunner(ModelRunner& modelRunner, DesignPointBuilder& uMean, int maxIterations, double z0Fac, int* counter)
    {
        const ZLambda zLambda = [](ModelSample& modelSample)
        {
            Sample sample = Sample(modelSample.Values);
            modelSample.Z = sample.getBeta();
        };

        ZModel model = ZModel(zLambda);

        const ZBetaLambda zConstraint = [&modelRunner, &uMean, maxIterations, z0Fac, &counter](ModelSample& modelSample)
        {
            Sample sample = Sample(modelSample.Values);
            double z = modelRunner.getZValue(sample);

            modelRunner.reportProgress(++(*counter), maxIterations, z0Fac * sample.getBeta());

            if (z * z0Fac < 0.0)
            {
                uMean.addSample(sample);
            }

            modelSample.Z = z;

            // should be z instead of std::abs(z) for better results
            return std::abs(z);
        };

        model.setConstraint(zConstraint);

        return model;
    }
}

