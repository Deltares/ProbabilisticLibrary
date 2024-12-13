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

            /**
             * \brief Gets a correction factor for alpha values
             * \return Correction factor
             */
            double getAlphaFactor();

            bool inverted = false;

            bool fixed = false;

            double fixedValue = 0;

            double getUFromX(double x) override
            {
                if (fixed)
                {
                    return fixedValue;
                }
                else
                {
                    int factor = inverted ? -1 : 1;
                    return factor * Stochast::getUFromX(x);
                }
            }

            /**
             * \brief Indicates that this stochast is a fragility curve
             * \return Indication
             */
            bool isFragilityCurve() override { return true; }

            /**
             * \brief Indicates whether the first fragility value has a higher reliability than the last fragility value
             * \return Indication
             */
            bool isGloballyDescending();

            /**
             * \brief Gets the design point at a given x-value
             * \param x X-value
             * \return Design point
             */
            std::shared_ptr<Models::StochastPoint> getDesignPoint(double x);

        private:
            const double margin = 1E-8;

            std::shared_ptr<Models::StochastPoint> getRealizationBetweenPoints(double x, std::shared_ptr<Statistics::FragilityValue> lowerFragilityValue, std::shared_ptr<Statistics::FragilityValue> upperFragilityValue);
            double interpolateAlphas(double lowerFraction, double lowerAlpha, double upperAlpha);
        };
    }
}

