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
#include "StartPointCalculatorSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        std::string StartPointCalculatorSettings::getStartPointMethodString(StartMethodType method)
        {
            switch (method)
            {
            case StartMethodType::FixedValue: return "fixed_value";
            case StartMethodType::RaySearch: return "ray_search";
            case StartMethodType::SphereSearch: return "sphere_search";
            case StartMethodType::SensitivitySearch: return "sensitivity_search";
            default: throw probLibException("Start point method");
            }
        }

        StartMethodType StartPointCalculatorSettings::getStartPointMethod(std::string method)
        {
            if (method == "fixed_value") return StartMethodType::FixedValue;
            else if (method == "ray_search") return StartMethodType::RaySearch;
            else if (method == "sphere_search") return StartMethodType::SphereSearch;
            else if (method == "sensitivity_search") return StartMethodType::SensitivitySearch;
            else throw probLibException("Start point method");
        }
    }
}

