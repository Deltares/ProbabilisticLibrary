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

#include <vector>

namespace Deltares::Optimization
{
    /**
     * \brief Indicates which values have to be used from the search parameter settings
     */
    enum class UseValuesType {AllValues, MinValue, MaxValue};

    /**
     * \brief Settings for a parameter in the grid search algorithm
     */
    class SearchParameterSettings
    {
    public:
        /**
         * \brief Minimum value which can be assigned to the parameter
         */
        double MinValue = -1.0E30;

        /**
         * \brief Maximum value which can be assigned to the parameter
         */
        double MaxValue = 1.0E30;

        /**
         * \brief Number of different values which can be assigned to the parameter
         */
        int NumberOfValues = 1;

        /**
         * \brief Start value for the parameter
         * \remark Not used in the grid search algorithm, but in future algorithms
         */
        double StartValue = 0.0;

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
        std::vector<double> getValues() const;

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

        /// <summary>
        /// Get the absolute value based on a ratio and the low and high boundaries
        /// </summary>
        /// <param name="ratio"></param>
        /// <returns> the absolute value </returns>
        double GetAbsoluteValue(double ratio) const;

        /// <summary>
        /// Get the relative value based on an absolute value and the low and high boundaries
        /// </summary>
        /// <param name="value"></param>
        /// <returns> the relative value </returns>
        double GetRelativeValue(double value) const;
    };
}

