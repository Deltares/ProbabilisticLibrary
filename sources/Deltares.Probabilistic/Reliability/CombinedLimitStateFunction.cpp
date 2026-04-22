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
#include "CombinedLimitStateFunction.h"

namespace Deltares::Reliability
{
    void CombinedLimitStateFunction::updateZValue(std::shared_ptr<Models::ModelSample> sample)
    {
        double result = combineType == combineAndOr::combOr ? std::numeric_limits<double>::max() : -std::numeric_limits<double>::max();

        for (auto limitStateFunction : limitStateFunctions)
        {
            limitStateFunction->updateZValue(sample);
            double z = sample->Z;

            if (limitStateFunction->normalize && z > 0)
            {
                sample->Z = std::nan("");
                return;
            }

            if (std::isnan(z))
            {
                result = std::nan("");
                break;
            }
            else if (combineType == combineAndOr::combOr && z < result)
            {
                result = z;
            }
            else if (combineType == combineAndOr::combAnd && z > result)
            {
                result = z;
            }
        }

        sample->Z = result;
    }

    void CombinedLimitStateFunction::initialize(std::vector<std::shared_ptr<Models::ModelInputParameter>>& inputParameters, std::vector<std::shared_ptr<Models::ModelInputParameter>>& outputParameters)
    {
        for (auto limitStateFunction : limitStateFunctions)
        {
            limitStateFunction->initialize(inputParameters, outputParameters);
        }
    }

}

