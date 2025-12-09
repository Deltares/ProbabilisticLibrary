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

#include <cmath>
#include <vector>

namespace Deltares
{
    namespace Models
    {
        class Evaluation
        {
        public:
            /**
             * \brief Only for reliability calculations: The limit state value which corresponds with this evaluation
             * \remarks Only for reliability calculation
             */
            double Z = std::nan("");

            /**
             * \brief The quantile which corresponds with this evaluation
             * \remarks Only for sensitivity calculations
             */
            double Quantile = std::nan("");

            double Beta = 1;
            double Weight = 1;
            int Iteration = -1;

            /**
             * \brief Indicates whether the sample results were calculated by a proxy or not
             */
            bool usedProxy = false;

            std::vector<double> InputValues;
            std::vector<double> OutputValues;
            int Tag = 0;
        };
    }
}

