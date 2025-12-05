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
#include "ProxyCoefficients.h"
#include "../Model/ModelSample.h"
#include "../Model/ZModel.h"

namespace Deltares::Proxies
{
    /**
     * \brief Combines a proxy model and settings
     */
    class LinearProxyMethod : public ProxyMethod
    {
    protected:
        /**
         * \brief Trains the proxy method for one particular output value
         * \param trainingSamples Samples which are used for training
         * \param proxyValues Values to be proxied
         */
        ProxyCoefficient trainValue(std::vector<std::shared_ptr<Models::ModelSample>>& trainingSamples,
                                    std::vector<double> proxyValues) override;

        /**
         * \brief Gets the output value for one particular output value calculated by the proxy
         * \param inputValues Inout values of the sample
         * \param proxyCoefficient Coefficients for the output value
         */
        double invokeValue(std::vector<double> inputValues, ProxyCoefficient proxyCoefficient) override;

    };
}


