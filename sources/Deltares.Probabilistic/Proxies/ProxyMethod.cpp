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
#include "../Model/ModelSample.h"
#include "../Math/NumericSupport.h"

namespace Deltares::Proxies
{
    void ProxyMethod::invoke(std::shared_ptr<Models::ModelSample>& sample, ProxyCoefficients& proxyCoefficients)
    {
        size_t outputSize = proxyCoefficients.coefficients.size();
        sample->OutputValues = std::vector<double>(outputSize);

        for (size_t index = 0; index < sample->OutputValues.size(); index++)
        {
            if (proxyCoefficients.coefficients[index].valid)
            {
                sample->OutputValues[index] = invokeValue(sample->Values, proxyCoefficients.coefficients[index]);
            }
            else
            {
                sample->OutputValues[index] = std::nan("");
            }
        }

        if (proxyCoefficients.zCoefficients.valid)
        {
            sample->Z = invokeValue(sample->Values, proxyCoefficients.zCoefficients);
        }
        else
        {
            sample->Z = std::nan("");
        }
    }

    ProxyCoefficients ProxyMethod::train(std::vector<std::shared_ptr<Models::ModelSample>>& trainingSamples)
    {
        ProxyCoefficients proxyCoefficients;

        for (size_t index = 0; index < trainingSamples[0]->OutputValues.size(); index++)
        {
            std::vector<double> proxyValues = Models::ModelSample::select(
                trainingSamples, [index](std::shared_ptr<Models::ModelSample> p) { return p->OutputValues[index]; });

            if (!Numeric::NumericSupport::any(proxyValues, [](double x) {return !Numeric::NumericSupport::isValidValue(x); }))
            {
                proxyCoefficients.coefficients.push_back(trainValue(trainingSamples, proxyValues));
            }
            else
            {
                proxyCoefficients.coefficients.push_back(ProxyCoefficient());
            }
        }

        std::vector<double> zValues = Models::ModelSample::select(
            trainingSamples, [](std::shared_ptr<Models::ModelSample> p) { return p->Z; });

        if (!Numeric::NumericSupport::any(zValues, [](double x) {return !Numeric::NumericSupport::isValidValue(x); }))
        {
            proxyCoefficients.zCoefficients = trainValue(trainingSamples, zValues);
        }
        else
        {
            proxyCoefficients.zCoefficients= ProxyCoefficient();
        }

        return proxyCoefficients;
    }
}



