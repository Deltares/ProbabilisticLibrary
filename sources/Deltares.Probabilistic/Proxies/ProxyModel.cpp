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

#include <any>

#include "LinearProxyMethod.h"
#include "ProxyTrainer.h"
#include "SingleProxyTrainer.h"
#include "../Model/ModelSample.h"
#include "../Model/ModelSampleStorage.h"
#include "../Utils/ProbabilisticLibraryException.h"

namespace Deltares::Proxies
{
    void ProxyModel::initializeForRun()
    {
        this->proxyMethod = getProxyMethod();

        if (this->trainingSamples.empty())
        {
            std::unique_ptr<ProxyTrainer> proxyTrainer = getProxyTrainer();
            proxyTrainer->uConverter = this->uConverter;

            std::vector<Models::ModelSample> initialSamples = proxyTrainer->getTrainingSet();

            for (auto newSample : initialSamples)
            {
                this->trainingSamples.push_back(newSample);
            }
        }

        Models::ModelSampleStorage storage = Models::ModelSampleStorage(trainingSamples.size());

        std::vector<Models::ModelSample*> samplesToCalculate;
        std::vector<Models::ModelSample*> proxyTrainingSamples;

        for (Models::ModelSample& trainingSample : trainingSamples)
        {
            if (trainingSample.OutputValues.empty())
            {
                trainingSample.AllowProxy = false;
                samplesToCalculate.push_back(storage.keep(trainingSample));
                proxyTrainingSamples.push_back(samplesToCalculate.back());
            }
            else
            {
                proxyTrainingSamples.push_back(storage.keep(trainingSample));
            }
        }

        invoke(samplesToCalculate);

        proxyCoefficients = proxyMethod->train(proxyTrainingSamples);
    }

    std::unique_ptr<ProxyMethod> ProxyModel::getProxyMethod()
    {
        switch (settings->MethodType)
        {
        case ProxyMethodType::FirstOrder: return std::make_unique<LinearProxyMethod>();
        default: throw Reliability::ProbabilisticLibraryException("Proxy method is not implemented.");
        }
    }

    std::unique_ptr<ProxyTrainer> ProxyModel::getProxyTrainer() const
    {
        switch (settings->InitializationType)
        {
        case ProxyInitializationType::Single: return std::make_unique<SingleProxyTrainer>();
        default: throw Reliability::ProbabilisticLibraryException("Proxy trainer is not implemented.");
        }
    }

    void ProxyModel::invoke(Models::ModelSample& sample)
    {
        if (sample.AllowProxy)
        {
            proxyMethod->invoke(sample, proxyCoefficients);
            this->model->zValueConverter->updateZValue(sample);
            sample.UsedProxy = true;
        }
        else
        {
            model->invoke(sample);
        }
    }

    void ProxyModel::invoke(const std::vector<Models::ModelSample*>& samples)
    {
        if (std::ranges::any_of(samples, [](const auto& sample) { return sample->AllowProxy; }))
        {
            for (const auto sample : samples)
            {
                invoke(*sample);
            }
        }
        else
        {
            model->invoke(samples);
        }
    }

    void ProxyModel::validate(Logging::ValidationReport& report, const std::string& subject) const
    {
        settings->validate(report);
    }
}

