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
#include "RunSettings.h"
#include "../Utils/probLibException.h"

namespace Deltares::Models
{
    std::string RunSettings::getHandleInvalidTypeString(HandleInvalidType type)
    {
        switch (type)
        {
        case HandleInvalidType::Ignore: return "ignore";
        case HandleInvalidType::Fail: return "fail";
        case HandleInvalidType::NoFail: return "no_fail";
        default: throw Reliability::probLibException("handle invalid type");
        }
    }

    HandleInvalidType RunSettings::getHandleInvalidType(const std::string& type)
    {
        if (type == "ignore")  return HandleInvalidType::Ignore;
        else if (type == "fail") return HandleInvalidType::Fail;
        else if (type == "no_fail") return HandleInvalidType::NoFail;
        else throw Reliability::probLibException("handle invalid type " + type + " not a known type");
    }

    std::string RunSettings::getModelReturnTypeString(Deltares::Models::ModelReturnType type)
    {
        switch (type)
        {
        case ModelReturnType::ZValue: return "z_value";
        case ModelReturnType::ProbabilityFailure: return "probability_failure";
        case ModelReturnType::ReliabilityIndex: return "reliability_index";
        default: throw Reliability::probLibException("model return type");
        }

    }

    Deltares::Models::ModelReturnType RunSettings::getModelReturnType(const std::string& type)
    {
        if (type == "z_value")  return ModelReturnType::ZValue;
        else if (type == "probability_failure") return ModelReturnType::ProbabilityFailure;
        else if (type == "reliability_index") return ModelReturnType::ReliabilityIndex;
        else throw Reliability::probLibException("model return type " + type + " not a known type");
    }

}

