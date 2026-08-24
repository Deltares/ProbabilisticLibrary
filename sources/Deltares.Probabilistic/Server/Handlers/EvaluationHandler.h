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
#pragma once

#include <string>
#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Model/Evaluation.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class Evaluation
     */
    class EvaluationHandler : public StoredObjectHandler<Models::Evaluation>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::Evaluation;
        }

        double GetValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Models::Evaluation> evaluation = GetObject(id);

            if (property_ == "z") return evaluation->Z;
            else if (property_ == "quantile") return evaluation->Quantile;
            else if (property_ == "beta") return evaluation->Beta;
            else if (property_ == "weight") return evaluation->Weight;
            else return ObjectHandler::GetValue(id, property_);
        }

        void SetValue(int id, const std::string& property_, double value) override
        {
            std::shared_ptr<Models::Evaluation> evaluation = GetObject(id);

            if (property_ == "z") evaluation->Z = value;
            else if (property_ == "quantile") evaluation->Quantile = value;
            else if (property_ == "beta") evaluation->Beta = value;
            else if (property_ == "weight") evaluation->Weight = value;
            else ObjectHandler::SetValue(id, property_, value);
        }

        double GetIndexedValue(int id, const std::string& property_, int index) override
        {
            std::shared_ptr<Models::Evaluation> evaluation = GetObject(id);

            if (property_ == "input_values") return evaluation->InputValues[index];
            else if (property_ == "output_values") return evaluation->OutputValues[index];
            else return StoredObjectHandler::GetIndexedValue(id, property_, index);
        }

        int GetIntValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Models::Evaluation> evaluation = GetObject(id);

            if (property_ == "iteration") return evaluation->Iteration;
            else if (property_ == "tag") return evaluation->Tag;
            else if (property_ == "input_values_count") return static_cast<int>(evaluation->InputValues.size());
            else if (property_ == "output_values_count") return static_cast<int>(evaluation->OutputValues.size());
            else return ObjectHandler::GetIntValue(id, property_);
        }

        void SetIntValue(int id, const std::string& property_, int value) override
        {
            std::shared_ptr<Models::Evaluation> evaluation = GetObject(id);

            if (property_ == "iteration") evaluation->Iteration = value;
            else if (property_ == "tag") evaluation->Tag = value;
            else ObjectHandler::SetValue(id, property_, value);
        }
    };
}

