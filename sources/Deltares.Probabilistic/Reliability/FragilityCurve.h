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
#include "DesignPoint.h"
#include "../Model/StochastPoint.h"
#include "../Statistics/Stochast.h"

namespace Deltares
{
    namespace Reliability
    {
        class FragilityCurve : public Statistics::Stochast
        {
        public:

            FragilityCurve() : Stochast()
            {
                Stochast::setDistributionType(Statistics::CDFCurve);
            }

            void setDistributionType(Statistics::DistributionType distributionType) override
            {
                // do not change distribution type
            }

            bool inverted = false;

            std::vector<std::shared_ptr<FragilityCurve>> contributingFragilityCurves;

            double getUFromX(double x) override
            {
                int factor = inverted ? -1 : 1;
                return factor * Stochast::getUFromX(x);
            }

            bool isGloballyDescending();
            std::shared_ptr<StochastPoint> getDesignPoint(double x);

        private:
            const double margin = 1E-8;

            std::shared_ptr<StochastPoint> getRealizationBetweenPoints(double x, std::shared_ptr<Statistics::FragilityValue> lowerFragilityValue, std::shared_ptr<Statistics::FragilityValue> upperFragilityValue);
            double interpolateAlphas(double lowerFraction, double lowerAlpha, double upperAlpha);
        };
    }
}

