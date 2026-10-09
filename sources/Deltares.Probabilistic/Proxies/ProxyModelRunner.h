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
#pragma once
#include <string>

#include <vector>

#include "../Model/UConverter.h"
#include "../Model/Sample.h"
#include "../Model/ZModel.h"
#include "ProxySettings.h"
#include "ProxyModel.h"
#include "../Model/ModelRunner.h"
#include "../Model/ModelSample.h"
#include "../Model/ProgressIndicator.h"


namespace Deltares::Proxies
{
    typedef std::function<bool(bool finalCall)> ShouldExitLambda;
    typedef std::function<void(int iterationIndex)> RemoveTaskLambda;
    typedef std::function<bool(int stochastIndex)> ShouldInvertLambda;

    class ProxyModelRunner : public Models::ModelRunner
    {
    public:
        ProxyModelRunner(std::shared_ptr<Proxies::ProxyModel> proxyModel, std::shared_ptr<Models::UConverter>uConverter, std::shared_ptr<Models::ProgressIndicator> progressIndicator = nullptr) : ModelRunner(proxyModel, uConverter, progressIndicator)
        {
            this->uConverter = uConverter;
            this->proxyModel = proxyModel;
        }

        ~ProxyModelRunner() override
        {
        }

        void initializeForRun() override;
        void setAllowProxy(bool allowProxy);

    protected:
        bool isProxyAllowed() const override { return allowProxy; }

    private:
        std::shared_ptr<Models::UConverter> uConverter;
        std::shared_ptr<Proxies::ProxyModel> proxyModel;
        bool allowProxy = true;
    };
}

