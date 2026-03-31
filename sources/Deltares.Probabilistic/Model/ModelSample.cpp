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
#include "ModelSample.h"

namespace Deltares::Models
{
    void ModelSample::clear()
    {
        IterationIndex = -1;
        threadId = 0;
        Weight = std::nan("");
        AllowProxy = true;
        UsedProxy = false;
        IsRestartRequired = false;
        Z = std::nan("");
        Beta = 0;
        Tag = 0;
    }

    /**
     * \brief Copies the results from another sample
     */
    void ModelSample::copyFrom(const std::shared_ptr<ModelSample>& source)
    {
        this->Z = source->Z;
        this->OutputValues = std::vector<double>(source->OutputValues.size());
        for (size_t i = 0; i < this->OutputValues.size(); i++)
        {
            this->OutputValues[i] = source->OutputValues[i];
        }
    }

    void ModelSample::fillModelSampleStruct(ModelSampleStruct* sampleStruct) const
    {
        sampleStruct->Values = Values.data();
        sampleStruct->ValuesCount = static_cast<int>(Values.size());

        sampleStruct->OutputValues = OutputValues.data();
        sampleStruct->OutputValuesCount = static_cast<int>(OutputValues.size());

        sampleStruct->IterationIndex = IterationIndex;
        sampleStruct->threadId = threadId;
        sampleStruct->Weight = Weight;
        sampleStruct->AllowProxy = AllowProxy;
        sampleStruct->UsedProxy = UsedProxy;
        sampleStruct->IsRestartRequired = IsRestartRequired;
        sampleStruct->Beta = Beta;
        sampleStruct->Z = Z;
        sampleStruct->ExtendedLogging = ExtendedLogging;
        sampleStruct->LoggingCounter = LoggingCounter;
        sampleStruct->Tag = Tag;
    }

    void ModelSample::setModelSampleStruct(ModelSampleStruct* sampleStruct)
    {
        Values.resize(sampleStruct->ValuesCount);
        for (int i = 0; i < sampleStruct->ValuesCount; i++)
        {
            Values[i] = sampleStruct->Values[i];
        }

        OutputValues.resize(sampleStruct->OutputValuesCount);
        for (int i = 0; i < sampleStruct->OutputValuesCount; i++)
        {
            OutputValues[i] = sampleStruct->OutputValues[i];
        }

        IterationIndex = sampleStruct->IterationIndex;
        threadId = sampleStruct->threadId;
        Weight = sampleStruct->Weight;
        AllowProxy = sampleStruct->AllowProxy;
        UsedProxy = sampleStruct->UsedProxy;
        IsRestartRequired = sampleStruct->IsRestartRequired;
        Beta = sampleStruct->Beta;
        Z = sampleStruct->Z;
        ExtendedLogging = sampleStruct->ExtendedLogging;
        LoggingCounter = sampleStruct->LoggingCounter;
        Tag = sampleStruct->Tag;
    }

    bool ModelSample::hasSameValues(std::shared_ptr<ModelSample> other)
    {
        if (this->Values.size() != other->Values.size())
        {
            return false;
        }

        for (int i = 0; i < this->Values.size(); i++)
        {
            if (this->Values[i] != other->Values[i])
            {
                return false;
            }
        }

        return true;
    }
}


