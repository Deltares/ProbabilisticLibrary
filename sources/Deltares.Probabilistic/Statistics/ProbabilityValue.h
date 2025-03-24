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

#include "StandardNormal.h"

namespace Deltares
{
    namespace Statistics
    {
        /**
         * \brief Defines a probability Point in a fragility curve or empirical distribution
         */
        class ProbabilityValue 
        {
        public:

            /**
             * \brief Default constructor, has Reliability = 0 and quantile 0.5
             */
            ProbabilityValue() = default;

            /**
             * \brief Constructor accepting quantile
             * \param p Quantile, same as non-exceeding probability
             */
            ProbabilityValue(double p)
            {
                this->setProbabilityOfNonFailure(p);
            }

            /**
             * \brief Reliability of the fragility point
             */
            double Reliability;

            double getProbabilityOfFailure()
            {
                return StandardNormal::getQFromU(this->Reliability);
            }

            void setProbabilityOfFailure(double q)
            {
                this->Reliability = StandardNormal::getUFromQ(q);
            }

            double getProbabilityOfNonFailure()
            {
                return StandardNormal::getPFromU(this->Reliability);
            }

            void setProbabilityOfNonFailure(double p)
            {
                this->Reliability = StandardNormal::getUFromP(p);
            }

            double getReturnPeriod()
            {
                return StandardNormal::getTFromU(this->Reliability);
            }

            void setReturnPeriod(double r)
            {
                this->Reliability = StandardNormal::getUFromT(r);
            }
        };
    }
}

