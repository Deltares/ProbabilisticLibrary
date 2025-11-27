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
#include "ProxyModel.h"

#include "LinearProxyMethod.h"
#include "ProxyTrainer.h"
#include "SingleProxyTrainer.h"
#include "../Model/ModelSample.h"
#include "../Utils/probLibException.h"

namespace Deltares::Proxies
{
    void ProxyModel::initializeForRun()
    {
        this->proxyMethod = getProxyMethod();

        if (this->trainingSamples.empty())
        {
            ProxyTrainer proxyTrainer = getProxyTrainer();
            proxyTrainer.SetConverter(this->uConverter);

            std::vector<std::shared_ptr<Models::ModelSample>> initialSamples = proxyTrainer.getTrainingSet();

            for (std::shared_ptr<Models::ModelSample> newSample : initialSamples)
            {
                this->trainingSamples.push_back(newSample);
            }
        }

        coefficients = this->proxyMethod.train(this->trainingSamples);
    }

    ProxyMethod ProxyModel::getProxyMethod()
    {
        switch (settings->MethodType)
        {
        case ProxyMethodType::FirstOrder: return LinearProxyMethod();
        default: throw Reliability::probLibException("Proxy method is not implemented.");
        }
    }

    ProxyTrainer ProxyModel::getProxyTrainer()
    {
        switch (settings->InitializationType)
        {
        case ProxyInitializationType::Single: return SingleProxyTrainer();
        default: throw Reliability::probLibException("Proxy trainer is not implemented.");
        }
    }

    void ProxyModel::invoke(std::shared_ptr<Models::ModelSample> sample)
    {
        proxyMethod.invoke(sample, coefficients);
    }

    void ProxyModel::invoke(std::vector<std::shared_ptr<Models::ModelSample>> samples)
    {
        for (const auto& sample : samples)
        {
            invoke(sample);
        }
    }

    void ProxyModel::validate(Logging::ValidationReport& report) const
    {
        settings->validate(report);
    }
}

