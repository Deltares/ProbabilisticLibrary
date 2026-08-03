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
#include "ReliabilityMethod.h"

#include <cmath>

namespace Deltares::Reliability
{
    int ReliabilityMethod::getZFactor(double z)
    {
        return getZFactor(z, Models::ModelReturnType::ZValue);
    }

    int ReliabilityMethod::getZFactor(double z, Models::ModelReturnType modelReturnType)
    {
        if (std::isnan(z))
        {
            return 1;
        }

        switch (modelReturnType)
        {
        case Models::ModelReturnType::ZValue: return z > 0.0 ? 1 : -1;
        case Models::ModelReturnType::ProbabilityFailure: return z < 0.5 ? 1 : -1;
        case Models::ModelReturnType::ReliabilityIndex: return z > 0.0 ? 1 : -1;
        default: throw ProbabilisticLibraryException("model return type");
        }
    }

    double ReliabilityMethod::getFailureAddition(double z, Models::ModelReturnType modelReturnType)
    {
        constexpr double max_p = 1.0 + 1E-6;
        constexpr double min_p = - 1E-6;

        switch (modelReturnType)
        {
        case Models::ModelReturnType::ZValue:
            return z < 0 ? 1.0 : 0.0;
        case Models::ModelReturnType::ProbabilityFailure:
            // add a small margin to be tolerant for rounding and arithmetic errors
            if (z < min_p || z > max_p)
            {
                throw ProbabilisticLibraryException("Probability should be between 0 and 1 (inclusive)");
            }
            else
            {
                return std::clamp(z, 0.0, 1.0);
            }
        case Models::ModelReturnType::ReliabilityIndex:
            if (z >= Statistics::StandardNormal::UMax)
            {
                return 0.0;
            }
            else if (z <= -Statistics::StandardNormal::UMax)
            {
                return 1.0;
            }
            else
            {
                return Statistics::StandardNormal::getQFromU(z);
            }
        default:
            throw ProbabilisticLibraryException("model return type");
        }
    }

    bool ReliabilityMethod::isStopped()
    {
        return this->stopped;
    }

    void ReliabilityMethod::setStopped()
    {
        this->stopped = true;
    }

    void ReliabilityMethod::Stop()
    {
        setStopped();
    }
}

