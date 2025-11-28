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
            std::shared_ptr<ProxyTrainer> proxyTrainer = getProxyTrainer();
            proxyTrainer->SetConverter(this->uConverter);

            std::vector<std::shared_ptr<Models::ModelSample>> initialSamples = proxyTrainer->getTrainingSet();

            for (std::shared_ptr<Models::ModelSample> newSample : initialSamples)
            {
                newSample->AllowProxy = false;
                this->trainingSamples.push_back(newSample);
            }

            this->invoke(trainingSamples);
        }

        std::vector<std::shared_ptr<Models::ModelSample>> samplesToCalculate;
        for (std::shared_ptr<Models::ModelSample> trainingSample : trainingSamples)
        {
            if (trainingSample->OutputValues.empty())
            {
                samplesToCalculate.push_back(trainingSample);
            }
        }

        invoke(samplesToCalculate);

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

    std::shared_ptr<ProxyTrainer> ProxyModel::getProxyTrainer()
    {
        switch (settings->InitializationType)
        {
        case ProxyInitializationType::Single: return std::make_shared<SingleProxyTrainer>();
        default: throw Reliability::probLibException("Proxy trainer is not implemented.");
        }
    }

    void ProxyModel::invoke(std::shared_ptr<Models::ModelSample> sample)
    {
        if (sample->AllowProxy)
        {
            proxyMethod.invoke(sample, coefficients);
        }
        else
        {
            model->invoke(sample);
        }
    }

    void ProxyModel::invoke(std::vector<std::shared_ptr<Models::ModelSample>> samples)
    {
        bool allowAnyProxy = false;
        for (auto& sample : samples)
        {
            if (sample->AllowProxy)
            {
                allowAnyProxy = true;
                break;
            }
        }

        if (allowAnyProxy)
        {
            for (const auto& sample : samples)
            {
                invoke(sample);
            }
        }
        else
        {
            model->invoke(samples);
        }
    }

    void ProxyModel::validate(Logging::ValidationReport& report) const
    {
        settings->validate(report);
    }
}

