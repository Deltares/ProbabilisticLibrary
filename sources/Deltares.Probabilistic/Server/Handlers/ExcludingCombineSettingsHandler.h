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
#include "../../Combine/ExcludingCombineSettings.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ExcludingCombineSettings
     */
    class ExcludingCombineSettingsHandler : public StoredObjectHandler<Reliability::ExcludingCombineSettings>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::ExcludingCombineSettings;
        }

        std::string GetStringValue(const std::shared_ptr<Reliability::ExcludingCombineSettings>& settings, const std::string& property_) override
        {
            if (property_ == "combiner_method") return Reliability::DesignPointCombiner::getExcludingCombinerMethodString(settings->combinerMethod);
            else return StoredObjectHandler::GetStringValue(settings, property_);
        }

        void SetStringValue(const std::shared_ptr<Reliability::ExcludingCombineSettings>& settings, const std::string& property_, const std::string& value) override
        {
            if (property_ == "combiner_method") settings->combinerMethod = Reliability::DesignPointCombiner::getExcludingCombinerMethod(value);
            else StoredObjectHandler::SetStringValue(settings, property_, value);
        }
    };
}

