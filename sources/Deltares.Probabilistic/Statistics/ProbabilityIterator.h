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

#include "StandardNormal.h"

namespace Deltares
{
    namespace Statistics
    {
        /**
         * \brief Defines a probability which is usable to iterate over various probabilities
         */
        class ProbabilityIterator
        {
        public:
            ProbabilityIterator(double u)
            {
                pq = Statistics::StandardNormal::getPQFromU(u);
                pqPrev = pq;
            }

            /**
             * \brief Assigns a nw value to u and returns the difference in probability with the previously assigned u-value
             * \param u Next value of u
             * \return Difference
             */
            double getDifference(double u)
            {
                pqPrev = pq;
                pq = Statistics::StandardNormal::getPQFromU(u);

                // depending on the value of u(i) use the probabilities of exceeding or the probabilities of non-exceeding
                return pq.p < 0.5 ? pq.p - pqPrev.p : pqPrev.q - pq.q;
            }

        private:
            PQ pq;
            PQ pqPrev;
        };
    }
}

