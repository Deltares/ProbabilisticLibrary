// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "VariableStochastValue.h"

namespace Deltares
{
    namespace Statistics
    {
        Statistics::VariableStochastType VariableStochastValue::getVariableStochastType(std::string variableStochastType)
        {
            if (variableStochastType == "properties") return Statistics::VariableStochastType::Properties;
            else if (variableStochastType == "mean_and_deviation") return Statistics::VariableStochastType::MeanAndDeviation;
            else throw Reliability::probLibException("variable stochast type");
        }

        std::string VariableStochastValue::getVariableStochastTypeString(Statistics::VariableStochastType variableStochastType)
        {
            switch (variableStochastType)
            {
            case Statistics::VariableStochastType::Properties: return "properties";
            case Statistics::VariableStochastType::MeanAndDeviation: return "mean_and_deviation";
            default:  throw Reliability::probLibException("constant parameter type");
            }
        }
    }
}
