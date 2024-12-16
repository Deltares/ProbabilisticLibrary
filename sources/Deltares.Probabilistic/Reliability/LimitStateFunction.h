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
#include <memory>
#include <vector>

#include "../Model/ZValueConverter.h"

namespace Deltares
{
    namespace Reliability
    {
        enum CompareType { LessThan, GreaterThan };

        class LimitStateFunction : public Models::ZValueConverter
        {
        public:
            std::string criticalParameter = "";
            std::string compareParameter = "";
            CompareType compareType = CompareType::LessThan;
            double criticalValue = 0;
            bool useCompareParameter = false;

            void initialize(std::vector<std::shared_ptr<Models::ModelInputParameter>>& inputParameters, std::vector<std::shared_ptr<Models::ModelInputParameter>>& outputParameters) override;
            void updateZValue(std::shared_ptr<Models::ModelSample> sample) override;

            static CompareType GetCompareType(std::string compare);
            static std::string GetCompareTypeString(CompareType compareType);

        private:
            int criticalParameterIndex = 0;
            int compareParameterIndex = 0;
            bool criticalParameterIndexFromInput = false;
            bool compareParameterIndexFromInput = false;
            bool useSampleZValue = false;
        };
    }
}

