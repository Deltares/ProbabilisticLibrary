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
#include <cmath>
#include <vector>

#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Optimization
    {
        /**
         * \brief Indicates which values have to be used from the search parameter settings
         */
        enum UseValuesType {AllValues, MinValue, MaxValue};

        /**
         * \brief Settings for a parameter in the grid search algorithm
         */
        class SearchParameterSettings
        {
        public:
            /**
             * \brief Minimum value which can be assigned to the parameter
             */
            double MinValue = 0;

            /**
             * \brief Maximum value which can be assigned to the parameter
             */
            double MaxValue = 0;

            /**
             * \brief Number of different values which can be assigned to the parameter
             */
            int NumberOfValues = 1;

            /**
             * \brief Start value for the parameter
             * \remark Not used in the grid search algorithm, but in future algorithms
             */
            double StartValue = nan("");

            /**
             * \brief Indicates whether the grid can be repositioned for this parameter
             * \remark Only useful when UseValues is AllValues
             */
            bool Move = false;

            /**
             * \brief The number of refinements to be performed
             */
            int NumberOfRefinements = 0;

            /**
             * \brief Indicates which values have to be used in the grid search algorithm
             */
            UseValuesType UseValues = UseValuesType::AllValues;

            /**
             * \brief Gets the values to be queried in the grid search algorithm 
             * \return Values
             * \remark Based on MinValue, MaxValue, UseValueType and NumberOfValues
             */
            std::vector<double> getValues() const
            {
                std::vector<double> values(this->UseValues == UseValuesType::AllValues ? this->NumberOfValues : 1);

                if (this->UseValues == UseValuesType::AllValues) 
                {
                    double interval = getInterval();

                    for (int i = 0; i < values.size(); i++)
                    {
                        values[i] = MinValue + i * interval;
                    }
                }
                else if (this->UseValues == UseValuesType::MinValue)
                {
                    values[0] = MinValue;
                }
                else if (this->UseValues == UseValuesType::MaxValue)
                {
                    values[0] = MaxValue;
                }
                else
                {
                    throw Reliability::probLibException("Use values type not supported");
                }

                return values;
            }

            /**
             * \brief Gets the interval between values
             * \return Interval
             */
            double getInterval() const
            {
                if (NumberOfValues > 1)
                {
                    return (MaxValue - MinValue) / (NumberOfValues - 1);
                }
                else
                {
                    return 0;
                }
            }
        };
    }
}

