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
#include "Direction.h"
#include <cmath>
#include <algorithm>

#include "../Math/NumericSupport.h"
#include "../Model/Sample.h"
#include "../Model/GradientCalculator.h"

using namespace Deltares::Models;
using namespace Deltares::Numeric;

namespace Deltares::Uncertainty
{
    void Direction::AddResult(double distance, double z)
    {
        distances.push_back(distance);
        zValues.push_back(z);

        lastDifference = distance - lastDistance;
        lastDistance = distance;
        lastWeight = Statistics::StandardNormal::getQFromU(std::abs(distance));

        // Create a list of Result objects
        std::vector<Result> results;
        for (size_t i = 0; i < distances.size(); i++)
        {
            Result tempVar = Result();
            tempVar.Distance = distances[i];
            tempVar.ZValue = zValues[i];
            results.push_back(tempVar);
        }

        // Sort the results based on the Distance property
        std::sort(results.begin(), results.end(),
            [](const Result& p, const Result& q) {return p.Distance < q.Distance; });

        // Clear the original lists and repopulate them with the sorted values
        distances.clear();
        zValues.clear();

        for (const auto& result : results)
        {
            distances.push_back(result.Distance);
            zValues.push_back(result.ZValue);
        }
    }

    double Direction::GetDistanceAtZ(double z) const
    {
        if (Valid)
        {
            return NumericSupport::interpolate(z, zValues, distances, true);
        }
        else
        {
            return 0;
        }
    }

    std::shared_ptr<Sample> Direction::CreateNewSampleAt(double z, double maxBeta)
    {
        double distance = NumericSupport::interpolate(z, zValues, distances, true); //interpolates the distance (d) for the given z value in the direction

        if (!Valid || distance < 0.0 || distance > maxBeta)
        {
            if (Valid)
            {
                Valid = false;
            }
            return sample;
        }
        else
        {
            std::shared_ptr<Sample> newSample = sample->getSampleAtBeta(distance);
            newSample->IterationIndex = this->index;
            return newSample;
        }
    }

    std::vector<double> Direction::selectLastDifferences(const std::vector<std::shared_ptr<Direction>>& directions)
    {
        std::vector<double> result(directions.size());
        for (size_t i = 0; i < directions.size(); i++)
        {
            result[i] = directions[i]->lastDifference;
        }
        return result;
    }

    std::vector<double> Direction::selectLastWeight(const std::vector<std::shared_ptr<Direction>>& directions)
    {
        std::vector<double> result(directions.size());
        for (size_t i = 0; i < directions.size(); i++)
        {
            result[i] = directions[i]->lastWeight;
        }
        return result;
    }

    std::vector<std::shared_ptr<Direction>> Direction::getValidDirections(const std::vector<std::shared_ptr<Direction>>& directions)
    {
        std::vector< std::shared_ptr<Direction>> results;
        for (const std::shared_ptr<Direction>& direction : directions)
        {
            if (direction->Valid)
            {
                results.push_back(direction);
            }
        }
        return results;
    }
}


