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
#include "ParameterSelector.h"

namespace Deltares::Models
{
    void ParameterSelector::initialize(std::vector<std::shared_ptr<ModelInputParameter>>& inputParameters, std::vector<std::shared_ptr<ModelInputParameter>>& outputParameters)
    {
        // if no parameter is specified, always use the first output parameter
        if (parameter.empty())
        {
            parameterIndex = 0;
            parameterIndexFromInput = false;

            // if no output parameters are available, use the z-value in the sample
            useSampleZValue = outputParameters.empty();
        }
        else
        {
            useSampleZValue = false;
            parameterIndex = -1;

            for (size_t i = 0; i < inputParameters.size(); i++)
            {
                if (inputParameters[i]->name == parameter)
                {
                    int offset = outputParameters[i]->isArray ? arrayIndex : 0;
                    parameterIndex = inputParameters[i]->computationalIndex + offset;
                    parameterIndexFromInput = true;
                }
            }

            for (size_t i = 0; i < outputParameters.size(); i++)
            {
                if (outputParameters[i]->name == parameter)
                {
                    int offset = outputParameters[i]->isArray ? arrayIndex : 0;
                    parameterIndex = outputParameters[i]->computationalIndex + offset;
                    parameterIndexFromInput = false;
                }
            }
        }
    }

    void ParameterSelector::updateZValue(std::shared_ptr<ModelSample> sample)
    {
        if (!useSampleZValue)
        {
            sample->Z = parameterIndexFromInput ? sample->Values[parameterIndex] : sample->OutputValues[parameterIndex];
        }
    }
}
