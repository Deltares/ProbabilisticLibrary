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
#include <vector>

#include "ProxyMethod.h"
#include "ProxySettings.h"
#include "ProxyTrainer.h"
#include "../Model/ModelSample.h"
#include "../Model/ZModel.h"

namespace Deltares::Models
{
    class UConverter;
}

namespace Deltares::Proxies
{
    /**
     * \brief Combines a proxy model and settings
     */
    class ProxyModel : public Models::ZModel
    {
    public:
        /**
         * \brief Proxy settings
         */
        std::shared_ptr<ProxySettings> settings = std::make_shared<ProxySettings>();

        /**
         * \brief Coefficients for the proxy model
         */
        ProxyCoefficients coefficients;

        /**
         * \brief Sets the converter
         * \param converter Converter to be set
         */
        void setConverter(std::shared_ptr<Models::UConverter> converter)
        {
            this->uConverter = converter;
        }

        /**
         * \brief Initializes the proxy for running
         */
        void initializeForRun() override;

        /**
         * \brief List of training samples
         */
        std::vector<std::shared_ptr<Models::ModelSample>> trainingSamples;

        /**
         * \brief Calculates a sample
         */
        void invoke(std::shared_ptr<Models::ModelSample> sample) override;

        /**
         * \brief Calculates a number of samples
         */
        void invoke(std::vector<std::shared_ptr<Models::ModelSample>> samples) override;

        /**
         * \brief Reports whether these settings have valid values
         * \param report Report in which the validity is reported
         */
        void validate(Logging::ValidationReport& report) const;

    private:

        std::shared_ptr<Models::UConverter> uConverter = nullptr;

        ProxyMethod proxyMethod;

        ProxyMethod getProxyMethod();

        ProxyTrainer getProxyTrainer();
    };
}


