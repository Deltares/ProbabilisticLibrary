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
#include <vector>
#include <cmath>
#include <memory>

#include "ImportanceSamplingSupport.h"
#include "../Math/NumericSupport.h"
#include "../Model/Sample.h"

using namespace Deltares::Models;

namespace Deltares::Reliability
{
    double ImportanceSamplingSupport::getWeight(const std::shared_ptr<Sample>& modifiedSample, const std::shared_ptr<Sample>& sample, double dimensionality)
    {
        double sumSquared = Numeric::NumericSupport::GetSquaredSum(modifiedSample->Values);
        double sumSquaredOrg = Numeric::NumericSupport::GetSquaredSum(sample->Values);
        double ratioPdf = std::exp(0.5 * (sumSquaredOrg - sumSquared));
        return dimensionality * ratioPdf;
    }

    double ImportanceSamplingSupport::getDimensionality(const std::vector<double>& factors)
    {
        double dimensionality = 1; // correction for the dimensionality effect

        for (size_t k = 0; k < factors.size(); k++)
        {
            dimensionality *= factors[k];
        }

        return dimensionality;
    }

    std::shared_ptr<Sample> ImportanceSamplingSupport::getOriginalSample(const std::shared_ptr<Sample>& sample, const std::shared_ptr<Sample>& center, const std::vector<double>& factors)
    {
        std::shared_ptr<Sample> original = std::make_shared<Sample>(sample->Values.size());

        for (size_t k = 0; k < sample->Values.size(); k++)
        {
            original->Values[k] = (sample->Values[k] - center->Values[k]) / factors[k];
        }

        return original;
    }

    double ImportanceSamplingSupport::getSampleWeight(const std::shared_ptr<Sample>& sample, const std::shared_ptr<Sample>& center, double dimensionality, const std::vector<double>& factors)
    {
        std::shared_ptr<Sample> originalSample = getOriginalSample(sample, center, factors);
        return getWeight(sample, originalSample, dimensionality);
    }

    double ImportanceSamplingSupport::getConvergence(double pf, double minWeight, int samples)
    {
        if (pf > 0 && pf < 1)
        {
            if (pf > 0.5)
            {
                pf = 1 - pf;
            }

            double varPf = sqrt(std::max(0.0, (minWeight - pf) / (samples * pf)));
            return varPf;
        }
        else
        {
            return nan("");
        }
    }
}



