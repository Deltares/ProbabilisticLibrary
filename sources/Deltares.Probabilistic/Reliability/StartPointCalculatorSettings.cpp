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
#include "../Logging/ValidationSupport.h"

namespace Deltares::Reliability
{
    using enum StartMethodType;
    using namespace Deltares::Logging;

    std::string StartPointCalculatorSettings::getStartPointMethodString(StartMethodType method)
    {
        switch (method)
        {
        case FixedValue: return "fixed_value";
        case RaySearch: return "ray_search";
        case SphereSearch: return "sphere_search";
        case SensitivitySearch: return "sensitivity_search";
        default: throw probLibException("Start point method");
        }
    }

    StartMethodType StartPointCalculatorSettings::getStartPointMethod(const std::string& method)
    {
        if (method == "fixed_value") return FixedValue;
        else if (method == "ray_search") return RaySearch;
        else if (method == "sphere_search") return SphereSearch;
        else if (method == "sensitivity_search") return SensitivitySearch;
        else throw probLibException("Start point method");
    }

    std::shared_ptr<StartPointCalculatorSettings> StartPointCalculatorSettings::clone() const
    {
        auto copy = std::make_shared<StartPointCalculatorSettings>();

        copy->GradientStepSize = this->GradientStepSize;
        copy->MaximumLengthStartPoint = this->MaximumLengthStartPoint;
        copy->RadiusSphereSearch = this->RadiusSphereSearch;
        copy->StartMethod = this->StartMethod;
        copy->allQuadrants = this->allQuadrants;
        copy->maxStepsSphereSearch = this->maxStepsSphereSearch;

        copy->StochastSet = this->StochastSet;

        return copy;
    }

    void StartPointCalculatorSettings::validate(ValidationReport& report) const
    {
        if (StartMethod == RaySearch || StartMethod == SensitivitySearch)
        {
            ValidationSupport::checkMinimum(report, 0.01, MaximumLengthStartPoint, "maximum length start point");
        }
        else if (StartMethod == SphereSearch)
        {
            ValidationSupport::checkMinimum(report, 0.01, RadiusSphereSearch, "radius sphere search");
            ValidationSupport::checkMinimumInt(report, 1, maxStepsSphereSearch, "max steps sphere search");
        }
    }

}

