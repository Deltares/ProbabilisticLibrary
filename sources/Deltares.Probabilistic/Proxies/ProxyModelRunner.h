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

#include "ProxyModel.h"
#include "../Model/UConverter.h"
#include "../Model/ModelRunner.h"
#include "../Model/ProgressIndicator.h"

namespace Deltares::Proxies
{
    /**
     * \brief Facilitates running samples on a proxy model
     */
    class ProxyModelRunner : public Models::ModelRunner
    {
    public:
        ProxyModelRunner(const std::shared_ptr<Proxies::ProxyModel>& newProxyModel,
                         const std::shared_ptr<Models::UConverter>& newConverter,
                         const std::shared_ptr<Models::ProgressIndicator>& newProgressIndicator = nullptr):
            ModelRunner(newProxyModel, newConverter, newProgressIndicator),
            proxyModel(newProxyModel), uConverter(newConverter)
        {
        }

        ~ProxyModelRunner() override = default;

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

