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

#include "ProxySettings.h"
#include "../Model/ModelSample.h"
#include "../Model/UConverter.h"

namespace Deltares::Proxies
{
    /**
     * \brief Combines a proxy model and settings
     */
    class ProxyTrainer
    {
    public:
        /**
         * \brief Trains the proxy method
         * \returns Collection with training samples
         */
        virtual std::vector<std::shared_ptr<Models::ModelSample>> getTrainingSet()
        {
            return {};
        }

        /**
         * \brief Sets the converter
         * \param converter Converter to be set
         */
        void SetConverter(const std::shared_ptr<Models::UConverter>& converter)
        {
            this->uConverter = converter;
        }

        /**
         * \brief Sets the settings
         * \param newSettings Settings to be set
         */
        void SetSettings(const ProxySettings& newSettings)
        {
            this->settings = newSettings;
        }

    protected:
        ProxySettings settings;
        std::shared_ptr<Models::UConverter> uConverter = nullptr;
    };
}


