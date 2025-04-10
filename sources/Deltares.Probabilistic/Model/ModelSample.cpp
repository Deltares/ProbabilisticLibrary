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
    /**
     * \brief Resets all contents of the sample to its default values
     * \remark Values are not cleared, since they are provided in the constructor
     */
    void ModelSample::clear()
    {
        IterationIndex = -1;
        threadId = 0;
        Weight = std::nan("");
        AllowProxy = true;
        IsRestartRequired = false;
        Z = std::nan("");
        Beta = 0;
        Tag = 0;
    }

    /**
     * \brief Copies the results from another sample
     * \param source sample to copy from
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

    /**
     * \brief Checks that two samples are equal except for Z
     * \param other sample to compare with
     * \return true: samples are the same
     */
    bool ModelSample::hasSameValues(const ModelSample& other) const
    {
        if (Values.size() != other.Values.size())
        {
            return false;
        }

        for (size_t i = 0; i < Values.size(); i++)
        {
            if (Values[i] != other.Values[i])  // NOLINT(clang-diagnostic-float-equal) must be exactly equal
            {
                return false;
            }
        }

        // both flags for proxy models must be equal ; check with xor:
        if (IsRestartRequired ^ other.IsRestartRequired) return false;
        if (AllowProxy ^ other.AllowProxy) return false;

        return true;
    }
}
