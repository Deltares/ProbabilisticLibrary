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

#include "RunProjectSettings.h"

namespace Deltares::Models
{
    Statistics::RunValuesType RunProjectSettings::getRunValuesType(const std::string& value)
    {
        if (value == "mean_values") return Statistics::MeanValues;
        else if (value == "median_values") return Statistics::MedianValues;
        else if (value == "design_values") return Statistics::DesignValues;
        else throw Reliability::probLibException("Run values type not supported");
    }

    std::string RunProjectSettings::getRunValuesTypeString(Statistics::RunValuesType runValuesType)
    {
        switch (runValuesType)
        {
        case Statistics::MeanValues: return "mean_values";
        case Statistics::MedianValues: return "median_values";
        case Statistics::DesignValues: return "design_values";
        default: throw Reliability::probLibException("Run values type not supported");
        }
    }
}

