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

#include "../Model/Sample.h"

namespace Deltares::Uncertainty
{
    /**
     * \brief Helper struct for class Direction
     */
    class Result
    {
    public:
        double Distance;
        double ZValue;
    };

    /**
     * \brief Helper class for the sensitivity using the Directional Sampling algorithm
     */
    class Direction
    {
    public:
        Direction(const std::shared_ptr<Models::Sample>& sample, int index, bool Valid)
            : Valid(Valid), index(index), sample(sample)
        {
            sample->IterationIndex = index;
        }

        void AddResult(double distance, double z);
        double GetDistanceAtZ(double z) const;
        std::shared_ptr<Models::Sample> CreateNewSampleAt(double z, double maxBeta);
        bool IsValid() const { return Valid; }

        /**
         * \brief Selects the field lastDifference of all valid members in the list
         * \param directions Collection of directions
         * \return Resulting numeric values
         */
        static std::vector<double> selectValidLastDifferences(const std::vector<std::shared_ptr<Direction>>& directions);

        /**
         * \brief Selects the field lastWeight of all valid members in the list
         * \param directions Collection of directions
         * \return Resulting numeric values
         */
        static std::vector<double> selectValidLastWeight(const std::vector<std::shared_ptr<Direction>>& directions);

    private:
        bool Valid = true;
        double lastWeight = 0;
        double lastDifference = 0;
        double lastDistance = 0;
        int index = 0;
        std::shared_ptr<Models::Sample> sample;
        std::vector<double> distances = std::vector<double>();
        std::vector<double> zValues = std::vector<double>();
    };

}

