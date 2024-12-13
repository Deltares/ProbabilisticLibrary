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
#include "DesignPointCombiner.h"

namespace Deltares
{
    namespace Reliability
    {
        std::string DesignPointCombiner::getCombineTypeString(combineAndOr type)
        {
            switch (type)
            {
            case combineAndOr::combOr: return "series";
            case combineAndOr::combAnd: return "parallel";
            default: throw probLibException("Combine type");
            }
        }

        combineAndOr DesignPointCombiner::getCombineType(std::string method)
        {
            if (method == "series") return combineAndOr::combOr;
            else if (method == "parallel") return combineAndOr::combAnd;
            else throw probLibException("Combine type");
        }

        std::string DesignPointCombiner::getCombinerMethodString(CombinerType type)
        {
            switch (type)
            {
            case CombinerType::Hohenbichler: return "hohenbichler";
            case CombinerType::ImportanceSampling: return "importance_sampling";
            case CombinerType::DirectionalSampling: return "directional_sampling";
            default: throw probLibException("Combiner method");
            }
        }

        CombinerType DesignPointCombiner::getCombinerMethod(std::string method)
        {
            if (method == "hohenbichler") return CombinerType::Hohenbichler;
            else if (method == "importance_sampling") return CombinerType::ImportanceSampling;
            else if (method == "directional_sampling") return CombinerType::DirectionalSampling;
            else throw probLibException("Combiner method type");

        }

    }
}

