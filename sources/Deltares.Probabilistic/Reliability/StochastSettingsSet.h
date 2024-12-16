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
#include "StochastSettings.h"
#include "../Model/Sample.h"
#include "../Model/StochastPoint.h"

#include <memory>

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Models;

        class StochastSettingsSet
        {
        private:
            void loadStochastPoint(std::shared_ptr<StochastPoint> stochastPoint);
        public:
            StochastSettingsSet() {}
            StochastSettingsSet(std::shared_ptr<StochastPoint> stochastPoint)
            {
                loadStochastPoint(stochastPoint);
            }

            std::vector<std::shared_ptr<StochastSettings>> stochastSettings;
            int getStochastCount()
            {
                return static_cast<int>(this->stochastSettings.size());
            }

            std::vector<std::shared_ptr<Deltares::Reliability::StochastSettings>> VaryingStochastSettings;
            int getVaryingStochastCount()
            {
                return static_cast<int>(this->VaryingStochastSettings.size());
            }

            std::shared_ptr<Sample> getStartPoint();
            void setStartPoint(std::shared_ptr<Sample> startPoint);

            bool AreStartValuesCorrelated = true;
        };
    }
}

