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
#include "DirectionalSamplingSettingsS.h"
#include "../Math/RootFinders/BisectionRootFinder.h"
#include "../Math/SpecialFunctions.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Gets the number of runs which is needed to achieve the variation coefficient at the probability for convergence
         */
        int DirectionalSamplingSettingsS::getRequiredSamples(double probability, double variationCoefficient, int nStochasts)
        {
            Numeric::RootFinderMethod function = [probability, variationCoefficient, nStochasts](double nDirections)
            {
                return getVariationCoefficient(probability, static_cast<int>(nDirections), nStochasts);
            };

            Numeric::BisectionRootFinder bisection = Numeric::BisectionRootFinder(variationCoefficient / 100, 0);

            double nDirections = bisection.CalculateValue(1, 1E6, variationCoefficient, function);

            return static_cast<int>(nDirections);
        }

        /**
         * \brief Gets the number of runs which is needed to achieve the variation coefficient at the probability for convergence
         */
        int DirectionalSamplingSettingsS::getRequiredSamples(int nStochasts) const
        {
            double probability = this->getMaxProbability();
            return getRequiredSamples(probability, this->VariationCoefficientFailure, nStochasts);
        }

        /**
         * \brief Modifies the variation coefficient so that the number of required samples matches a given value
         */
        void DirectionalSamplingSettingsS::setRequiredSamples(int nDirections, int nStochasts)
        {
            double probability = this->getMaxProbability();
            this->VariationCoefficientFailure = getVariationCoefficient(probability, nDirections, nStochasts);
        }

        double DirectionalSamplingSettingsS::getVariationCoefficient(double q, int nDirections, int nStochasts)
        {
            if (q > 0 && q < 1)
            {
                q = std::min(q, 1 - q);

                double betaDirection = Statistics::StandardNormal::getUFromQ(q);
                double weight = Deltares::Numeric::SpecialFunctions::getGammaUpperRegularized(0.5 * nStochasts, 0.5 * betaDirection * betaDirection);

                double covariance = (weight * weight - 2.0 * q * weight + q * q) / nDirections;
                covariance = std::sqrt(covariance) / q;
                return covariance;
            }
            else
            {
                return std::nan("");
            }
        }

        double DirectionalSamplingSettingsS::getMaxProbability() const
        {
            double maxBeta = 0;

            for (size_t i = 0; i < this->RequestedQuantiles.size(); i++)
            {
                maxBeta = std::max(maxBeta, std::abs(this->RequestedQuantiles[i]->Reliability));
            }

            return Statistics::StandardNormal::getQFromU(maxBeta);
        }
    }
}
