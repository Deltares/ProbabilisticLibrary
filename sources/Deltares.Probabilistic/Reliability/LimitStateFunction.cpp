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
#include "LimitStateFunction.h"

#include "../Model/ModelInputParameter.h"
#include "../Utils/probLibException.h"

namespace Deltares::Reliability
{
    void LimitStateFunction::initialize(std::vector<std::shared_ptr<Models::ModelInputParameter>>& inputParameters, std::vector<std::shared_ptr<Models::ModelInputParameter>>& outputParameters)
    {
        // if no parameter is specified, always use the first output parameter
        if (this->criticalParameter == "")
        {
            this->criticalParameterIndex = 0;
            this->criticalParameterIndexFromInput = false;

            // if no output parameters are available, use the z-value in the sample
            this->useSampleZValue = outputParameters.empty();
        }
        else
        {
            this->useSampleZValue = false;
            this->criticalParameterIndex = -1;

            int indexInput = FindParameterIndex(inputParameters, this->criticalParameter);
            if (indexInput >= 0)
            {
                this->criticalParameterIndex = indexInput;
                this->criticalParameterIndexFromInput = true;
            }

            int indexOutput = FindParameterIndex(outputParameters, this->criticalParameter);
            if (indexOutput >= 0)
            {
                this->criticalParameterIndex = indexInput;
                this->criticalParameterIndexFromInput = false;
            }
        }

        this->compareParameterIndex = -1;

        if (this->useCompareParameter)
        {
            int indexInput = FindParameterIndex(inputParameters, this->compareParameter);
            if (indexInput >= 0)
            {
                this->compareParameterIndex = indexInput;
                this->compareParameterIndexFromInput = true;
            }

            int indexOutput = FindParameterIndex(outputParameters, this->compareParameter);
            if (indexOutput >= 0)
            {
                this->compareParameterIndex = indexOutput;
                this->compareParameterIndexFromInput = false;
            }
        }
    }

    int LimitStateFunction::FindParameterIndex(std::vector<std::shared_ptr<Models::ModelInputParameter>>& parameters, const std::string search_parameter)
    {
        int index = 0;

        for (size_t i = 0; i < parameters.size(); i++)
        {
            if (parameters[i]->name == search_parameter)
            {
                return index;
            }

            index += parameters[i]->isArray ? parameters[i]->arraySize : 1;
        }

        return -1;
    }

    void LimitStateFunction::updateZValue(std::shared_ptr<Models::ModelSample> sample)
    {
        if (!this->useSampleZValue)
        {
            double criticalResultValue = this->criticalParameterIndexFromInput ? sample->Values[this->criticalParameterIndex] : sample->OutputValues[this->criticalParameterIndex];
            double criticalCompareValue = this->criticalValue;
            if (this->useCompareParameter)
            {
                criticalCompareValue = this->compareParameterIndexFromInput ? sample->Values[this->compareParameterIndex] : sample->OutputValues[this->compareParameterIndex];
            }

            if (this->compareType == CompareType::LessThan)
            {
                sample->Z = criticalResultValue - criticalCompareValue;
            }
            else
            {
                sample->Z = criticalCompareValue - criticalResultValue;
            }
        }
    }

    CompareType LimitStateFunction::GetCompareType(std::string compare)
    {
        if (compare == "less_than") return CompareType::LessThan;
        else if (compare == "greater_than") return CompareType::GreaterThan;
        else throw probLibException("compare type");
    }

    std::string LimitStateFunction::GetCompareTypeString(CompareType compareType)
    {
        if (compareType == CompareType::LessThan) return "less_than";
        else if (compareType == CompareType::GreaterThan) return "greater_than";
        else throw probLibException("compare type");
    }
}

