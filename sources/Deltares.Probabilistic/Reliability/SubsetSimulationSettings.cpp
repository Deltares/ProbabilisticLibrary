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
#include "SubsetSimulationSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        std::string SubsetSimulationSettings::getSampleMethodString(SampleMethodType method)
        {
            switch (method)
            {
            case SampleMethodType::MarkovChain: return "markov_chain";
            case SampleMethodType::AdaptiveConditional: return "adaptive_conditional";
            default: throw probLibException("Sample method");
            }
        }

        SampleMethodType SubsetSimulationSettings::getSampleMethod(std::string method)
        {
            if (method == "markov_chain") return SampleMethodType::MarkovChain;
            else if (method == "adaptive_conditional") return SampleMethodType::AdaptiveConditional;
            else throw probLibException("Sample method");
        }
    }
}
