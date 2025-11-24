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

#include "../Model/ModelSample.h"
#include "../Model/ZModel.h"

namespace Deltares::Proxies
{
    /**
     * \brief Combines a proxy model and settings
     */
    class ProxyMethod
    {
    public:

        /**
         * \brief Calculates the result of a sample using the proxy method
         * \param sample Sample for which the model results will be calculated
         */
        virtual void invoke(std::shared_ptr<Models::ModelSample> sample) {}

        /**
         * \brief Trains the proxy method
         * \param trainingSamples Samples which are used for training
         */
        virtual void train(std::vector<std::shared_ptr<Models::ModelSample>>& trainingSamples) {}
    };
}


