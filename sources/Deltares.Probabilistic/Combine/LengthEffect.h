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
#include <memory>
#include "combiner.h"
#include "../Reliability/DesignPoint.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Class for length effect calculation
         * (wrapper for upscaleLength in upscaling)
         */
        class LengthEffect
        {
        public:
            // \brief This method upscales from a cross section to a given section length
            // \param crossSection : Reliability index and alpha cross section
            // \param selfCorrelationMatrix : Correlation variables
            // \param correlationLengths : Correlation length variables
            // \param length : Section length
            // \return Design point for section
            static DesignPoint UpscaleLength(std::shared_ptr<DesignPoint> crossSection,
                const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix,
                const std::vector<double>& correlationLengths,
                const double length);

            // \brief This method upscales from a cross section to a given section length
            // \param crossSection : Reliability index and alpha cross section
            // \param selfCorrelations : Correlation variables
            // \param correlationLengths : Correlation length variables
            // \param length : Section length
            // \return Design point for section
            static DesignPoint UpscaleLength(std::shared_ptr<DesignPoint> crossSection,
                const std::vector<double>& selfCorrelations,
                const std::vector<double>& correlationLengths,
                const double length);
        };
    }
}

