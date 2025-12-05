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

#include "ProxyCoefficients.h"
#include "../Model/ModelSample.h"
#include "../Model/ZModel.h"
#include "../Math/vector1D.h"

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
         * \param proxyCoefficients The coefficients needed for the proxy to calculate the output values
         */
        virtual void invoke(const std::shared_ptr<Models::ModelSample>& sample, ProxyCoefficients& proxyCoefficients);

        /**
         * \brief Trains the proxy method
         * \param trainingSamples Samples which are used for training
         */
        virtual ProxyCoefficients train(std::vector<std::shared_ptr<Models::ModelSample>>& trainingSamples);

    protected:
        /**
         * \brief Trains the proxy method for one particular output value
         * \param trainingSamples Samples which are used for training
         * \param proxyValues Values to be proxied
         */
        virtual ProxyCoefficient trainValue(std::vector<std::shared_ptr<Models::ModelSample>>& trainingSamples,
            std::vector<double> proxyValues) = 0;

        /**
         * \brief Gets the output value for one particular output value calculated by the proxy
         * \param inputValues Inout values of the sample
         * \param coefficients Coefficients for the output value
         */
        virtual double invokeValue(std::vector<double> inputValues, ProxyCoefficient coefficients) = 0;
    };
}


