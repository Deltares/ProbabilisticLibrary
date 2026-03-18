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
    std::string RunSettings::getHandleInvalidTypeString(Deltares::Models::HandleInvalidType type)
    {
        switch (type)
        {
        case HandleInvalidType::Ignore: return "ignore";
        case HandleInvalidType::Fail: return "fail";
        case HandleInvalidType::NoFail: return "no_fail";
        default: throw Reliability::probLibException("handle invalid type");
        }
    }

    Deltares::Models::HandleInvalidType RunSettings::getHandleInvalidType(const std::string& type)
    {
        if (type == "ignore")  return  HandleInvalidType::Ignore;
        else if (type == "fail") return  HandleInvalidType::Fail;
        else if (type == "no_fail") return  HandleInvalidType::NoFail;
        else throw Reliability::probLibException("handle invalid type " + type + " not a known type");
    }
}

