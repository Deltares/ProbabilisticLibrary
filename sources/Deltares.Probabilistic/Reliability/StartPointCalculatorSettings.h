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
#include <vector>
#include "StochastSettingsSet.h"

namespace Deltares
{
    namespace Reliability
    {
        enum StartMethodType { None, RaySearch, SensitivitySearch, SphereSearch };

        /**
         * \brief Settings for the start point calculator
         */
        class StartPointCalculatorSettings
        {
        public:
            /**
             * \brief Type of start point calculation
             */
            StartMethodType StartMethod = StartMethodType::None;
            double MaximumLengthStartPoint = 6;
            double GradientStepSize = 4;
            double RadiusSphereSearch = 10;
            double dsdu = 1;
            bool allQuadrants = false;

            std::shared_ptr<StartPointCalculatorSettings> clone()
            {
                std::shared_ptr<StartPointCalculatorSettings> copy = std::make_shared<StartPointCalculatorSettings>();

                copy->GradientStepSize = this->GradientStepSize;
                copy->MaximumLengthStartPoint = this->MaximumLengthStartPoint;
                copy->RadiusSphereSearch = this->RadiusSphereSearch;
                copy->StartMethod = this->StartMethod;
                copy->allQuadrants = this->allQuadrants;

                copy->StochastSet = this->StochastSet;

                return copy;
            }

            bool isValid()
            {
                return StartMethod == StartMethodType::None ||
                    (StartMethod == StartMethodType::RaySearch && MaximumLengthStartPoint >= 1) ||
                    (StartMethod == StartMethodType::SensitivitySearch && MaximumLengthStartPoint >= 1) ||
                    (StartMethod == StartMethodType::SphereSearch && RadiusSphereSearch >= 0.1);
            }

            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();
            std::vector<double> startVector;

            static std::string getStartPointMethodString(StartMethodType method);
            static StartMethodType getStartPointMethod(std::string method);
        };
    }
}
