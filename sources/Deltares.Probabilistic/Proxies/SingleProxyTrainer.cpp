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
#include "SingleProxyTrainer.h"

#include "ProxySettings.h"
#include "../Model/UConverter.h"

namespace Deltares::Proxies
{
    std::vector<std::shared_ptr<Models::ModelSample>> SingleProxyTrainer::getTrainingSet()
    {
        int nStochasts = uConverter->getStochastCount();

        std::vector<std::shared_ptr<Models::ModelSample>> samples;

        for (int i = 0; i < nStochasts + 1; ++i)
        {
            std::vector<double> u(nStochasts);

            for (int j = 0; j < nStochasts; ++j)
            {
                if (i == j)
                {
                    u[j] = settings.GetQualitativeValue(settings.Direction);
                }
                else
                {
                    u[j] = settings.GetQualitativeValue(QualitativeValue::Medium);
                }
            }

            std::shared_ptr<Models::Sample> uSample = std::make_shared<Models::Sample>(u);
            std::vector<double> xValues = uConverter->getXValues(uSample);
            std::shared_ptr<Models::ModelSample> sample = std::make_shared<Models::ModelSample>(xValues);

            samples.push_back(sample);
        }

        if (settings.AddFullFactorial)
        {
            std::vector<double> u(nStochasts);

            for (int j = 0; j < nStochasts; ++j)
            {
                u[j] = settings.GetQualitativeValue(settings.Direction);
            }

            std::shared_ptr<Models::Sample> uSample = std::make_shared<Models::Sample>(u);
            std::vector<double> xValues = uConverter->getXValues(uSample);
            std::shared_ptr<Models::ModelSample> sample = std::make_shared<Models::ModelSample>(xValues);

            samples.push_back(sample);
        }

        return samples;
    }
}

