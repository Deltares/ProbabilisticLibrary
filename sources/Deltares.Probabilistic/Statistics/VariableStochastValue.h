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
#pragma once

#include "StochastProperties.h"

namespace Deltares
{
    namespace Statistics
    {
        enum VariableStochastType { Properties, MeanAndDeviation };

        class VariableStochastValue
        {
        public:
            double X = 0;

            VariableStochastType variableStochastType = VariableStochastType::Properties;

            double mean = 0;
            double deviation = 0;

            std::shared_ptr<StochastProperties> Stochast = std::make_shared<StochastProperties>();

            std::shared_ptr<VariableStochastValue> clone()
            {
                std::shared_ptr<VariableStochastValue> value = std::make_shared<VariableStochastValue>();

                value->X = this->X;
                value->mean = this->mean;
                value->deviation = this->deviation;
                value->variableStochastType = this->variableStochastType;

                value->Stochast = this->Stochast->clone();

                return value;
            }

            static VariableStochastType getVariableStochastType(std::string variableStochastType);
            static std::string getVariableStochastTypeString(Statistics::VariableStochastType variableStochastType);
        };
    }
}

