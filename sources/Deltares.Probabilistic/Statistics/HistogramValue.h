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
#include "../Utils/DirtySupport.h"

namespace Deltares
{
    namespace Statistics
    {
        class HistogramValue
        {
        public:
            HistogramValue() {}

            HistogramValue(double lowerBound, double upperBound)
            {
                this->LowerBound = lowerBound;
                this->UpperBound = upperBound;
            }

            double LowerBound = 0;
            double UpperBound = 0;
            double Amount = 0;

            double NormalizedAmount = 0;
            double CumulativeNormalizedAmount = 0;

            /// <summary>
            /// Average of lower and upper bound
            /// </summary>
            double getCenter()
            {
                return (LowerBound + UpperBound) / 2;
            }

            /// <summary>
            /// Difference between lower and upper bound
            /// </summary>
            double getSize()
            {
                return UpperBound - LowerBound;
            }

            /// <summary>
            /// Tells whether a value is part of this range
            /// </summary>
            /// <param name="x"></param>
            /// <returns></returns>
            bool contains(double x) const 
            {
                if (LowerBound == UpperBound)
                {
                    return x == LowerBound;
                }
                else
                {
                    return x >= LowerBound && x < UpperBound;
                }
            }

            bool compareTo(std::shared_ptr<HistogramValue> other)
            {
                if (this->LowerBound == other->LowerBound)
                {
                    return this->getSize() > other->getSize();
                }
                else
                {
                    return this->LowerBound > other->LowerBound;
                }
            }

            /**
             * \brief Indicates whether the properties of the histogram value are valid
             */
            bool isValid() const 
            {
                return UpperBound >= LowerBound;
            }

            void setDirtyFunction(Utils::SetDirtyLambda setDirtyLambda)
            {
                this->setDirtyLambda = setDirtyLambda;
            }

            void setDirty()
            {
                if (setDirtyLambda != nullptr)
                {
                    setDirtyLambda();
                }
            }

            std::shared_ptr<HistogramValue> clone()
            {
                std::shared_ptr<HistogramValue> clone = std::make_shared<HistogramValue>();

                clone->LowerBound = this->LowerBound;
                clone->UpperBound = this->UpperBound;
                clone->Amount = this->Amount;

                return clone;
            }

        private:
            Utils::SetDirtyLambda setDirtyLambda = nullptr;
        };
    }
}

