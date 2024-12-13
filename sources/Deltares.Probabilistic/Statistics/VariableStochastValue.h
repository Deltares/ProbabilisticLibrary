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

#include "StochastProperties.h"
#include "Distributions/Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        /**
          * \brief Defines a conditional value
          */
        class VariableStochastValue
        {
        public:
            VariableStochastValue()
            {
                // assign nan values to all Stochast properties
                Stochast->Location = std::nan("");
                Stochast->Scale = std::nan("");
                Stochast->Shift = std::nan("");
                Stochast->ShiftB = std::nan("");
                Stochast->Shape = std::nan("");
                Stochast->ShapeB = std::nan("");
                Stochast->Minimum = std::nan("");
                Stochast->Maximum = std::nan("");
                Stochast->Observations = -1;
            }

            VariableStochastValue(double x, std::shared_ptr<StochastProperties> stochast) : VariableStochastValue()
            {
                this->X = x;
                this->Stochast = stochast;
            }


            /**
              * \brief X-value for which the conditional value is defined
              */
            double X = 0;

            /**
              * \brief Mean value, if set (value not equal tot nan) it prevails over the stochast properties
              */
            double mean = std::nan("");

            /**
              * \brief Standard deviation value, if set (value not equal tot nan) it prevails over the stochast properties
              */
            double deviation = std::nan("");

            /**
              * \brief Stochast properties at X
              * \remark If a property in this class is not set (value equal tot nan), the value will be ignored
              */
            std::shared_ptr<StochastProperties> Stochast = std::make_shared<StochastProperties>();

            /**
              * \brief Combines a given stochast with the conditional values in this class
              * \param distribution Distribution
              * \param defaultStochast Given stochast
              * \return Combined stochast
              */
            std::shared_ptr<StochastProperties> getMergedStochast(std::shared_ptr<Distribution> distribution, std::shared_ptr<StochastProperties> defaultStochast = nullptr);

            /**
              * \brief Clones this object
              */
            std::shared_ptr<VariableStochastValue> clone();
        private:
            /**
             * \brief Gets the first non nan value
             * \remark If both nan, nan is returned
             */
            double getNonNanValue(double value1, double value2);
        };
    }
}

