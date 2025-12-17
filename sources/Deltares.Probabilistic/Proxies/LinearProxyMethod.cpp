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
#include "LinearProxyMethod.h"

#include <vector>

#include "../Math/vector1D.h"
#include "../Math/matrix.h"

namespace Deltares::Proxies
{
    double LinearProxyMethod::invokeValue(std::vector<double> inputValues, ProxyCoefficient proxyCoefficient)
    {
        double z = proxyCoefficient.coefficients(0);
        for (size_t j = 0; j < inputValues.size(); j++)
        {
            z += inputValues[j] * proxyCoefficient.coefficients(j + 1);
        }

        return z;
    }

    ProxyCoefficient LinearProxyMethod::trainValue(std::vector<std::shared_ptr<Models::ModelSample>>& trainingSamples, std::vector<double> proxyValues)
    {
        Numeric::vector1D results(trainingSamples.size());
        Numeric::vector1D weights(trainingSamples.size());

        for (size_t i = 0; i < trainingSamples.size(); i++)
        {
            weights(i) = trainingSamples[i]->Weight;
            results(i) = proxyValues[i] * trainingSamples[i]->Weight;
        }

        size_t n_samples = trainingSamples.size(); // samples
        size_t n_variables = trainingSamples[0]->Values.size() + 1; // variables

        Numeric::Matrix xValues = Numeric::Matrix(n_samples, n_variables);
        for (size_t i = 0; i < n_samples; i++) // samples
        {
            xValues(i, 0) = 1.0 * weights(i);
            for (size_t j = 1; j < n_variables; j++) // variables
            {
                xValues(i, j) = trainingSamples[i]->Values[j - 1] * weights(i);
            }
        }

        // Do QR decomposition
        Numeric::QRMatrix qr = xValues.qr_decompose();

        // Solve using QR decomposition
        Numeric::vector1D solution = qr.solve(results);

        ProxyCoefficient proxyCoefficient;

        proxyCoefficient.valid = true;
        proxyCoefficient.coefficients = solution;

        return proxyCoefficient;
    }
}

