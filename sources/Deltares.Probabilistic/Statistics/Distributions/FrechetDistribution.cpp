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
#include "FrechetDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/SpecialFunctions.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include <cmath>

#include "DistributionFitter.h"
#include "DistributionSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        using enum DistributionPropertyType;

        void FrechetDistribution::initialize(StochastProperties& stochast, const std::vector<double>& values)
        {
            setMeanAndDeviation(stochast, values[0], values[1]);
        }

        bool FrechetDistribution::isVarying(StochastProperties& stochast)
        {
            return stochast.Scale > 0.0;
        }

        double FrechetDistribution::getMean(StochastProperties& stochast)
        {
            const double gamma1km = Numeric::SpecialFunctions::getGamma(1.0 - 1.0 / stochast.Shape);
            return stochast.Shift + stochast.Scale * gamma1km;
        }

        double FrechetDistribution::getDeviation(StochastProperties& stochast)
        {
            const double gamma1k = Numeric::SpecialFunctions::getGamma(1.0 - 1.0 / stochast.Shape);
            const double gamma2k = Numeric::SpecialFunctions::getGamma(1.0 - 2.0 / stochast.Shape);
            return stochast.Scale * std::sqrt(gamma2k - gamma1k * gamma1k);
        }

        double FrechetDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            const double p = StandardNormal::getPFromU(u);

            if (p == 0.0)
            {
                return stochast.Shift;
            }
            else
            {
                double pow = -std::log(p);
                double xScale = std::pow(pow, -1.0 / stochast.Shape);
                double x = xScale * stochast.Scale;

                return x + stochast.Shift;
            }
        }

        double FrechetDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            if (stochast.Scale == 0.0)
            {
                return x < stochast.Shift ? -StandardNormal::UMax : StandardNormal::UMax;
            }
            else
            {
                const double cdf = getCDF(stochast, x);
                return StandardNormal::getUFromP(cdf);
            }
        }

        double FrechetDistribution::getPDF(StochastProperties& stochast, double x)
        {
            const double z = (x - stochast.Shift) / stochast.Scale;

            if (stochast.Scale == 0.0)
            {
                return x == stochast.Shift ? 1.0 : 0.0;
            }
            else if (z <= 0.0)
            {
                return 0.0;
            }
            else
            {
                return (stochast.Shape / stochast.Scale) * std::pow(z, -1.0 - stochast.Shape) * getCDF(stochast, x);
            }
        }

        double FrechetDistribution::getCDF(StochastProperties& stochast, double x)
        {
            double z = (x - stochast.Shift) / stochast.Scale;

            if (stochast.Scale == 0.0)
            {
                return x < stochast.Shift ? 0.0 : 1.0;
            }
            else if (z <= 0.0)
            {
                return 0.0;
            }
            else
            {
                double pow = std::pow(z, -stochast.Shape);
                return std::exp(-pow);
            }
        }

        void FrechetDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            Numeric::RootFinderMethod method = [](double k)
            {
                double gamma1k = Numeric::SpecialFunctions::getGamma(1 - 1 / k);
                double gamma2k = Numeric::SpecialFunctions::getGamma(1 - 2 / k);

                return std::sqrt(gamma2k - gamma1k * gamma1k) / gamma1k;
            };

            // the quotient deviation / mean is only dependent from the shape parameter, so this will be determined first
            double u = deviation / (mean - stochast.Shift);

            constexpr double toleranceBisection = 0.00001;
            auto bisection = Numeric::BisectionRootFinder(toleranceBisection);

            // shape must be > 2
            double minStart = 2.4;
            double maxStart = 2.6;

            stochast.Shape = bisection.CalculateValue(minStart, maxStart, u, method);

            stochast.Scale = (mean - stochast.Shift) / Numeric::SpecialFunctions::getGamma(1.0 - 1.0 / stochast.Shape);
        }

        void FrechetDistribution::setXAtU(StochastProperties& stochast, double x, double u, ConstantParameterType constantType)
        {
            DistributionSupport::setXAtUByIteration(*this, stochast, x, u, constantType);
        }

        void FrechetDistribution::fit(StochastProperties& stochast, const std::vector<double>& values, const double shift)
        {
            int maxLoops = 100;
            double minValue = 0.001;

            double newShift = std::isnan(shift) ? getFittedMinimum(values) : shift;

            std::vector<double> values0 = Numeric::NumericSupport::select(values, [newShift](double p) { return p - newShift; });

            stochast.Shift = 0;

            double alpha = 1;
            double beta = 1;
            const double n = static_cast<double>(values0.size());

            int counter = 0;

            while (true)
            {
                counter++;

                const double t = Numeric::NumericSupport::sum(values0, [alpha] (double p) {    return std::pow(1 / p, alpha); });

                const double betaNew = std::pow(n / t, 1 / alpha);

                const double sumBetaAlpha = Numeric::NumericSupport::sum(values0, [beta, alpha](double p) { return std::pow(beta / p, alpha) * std::log(beta / p); });
                const double sumLog = Numeric::NumericSupport::sum(values0, [](double q) { return std::log(q); });

                const double alphaNew = n / (sumBetaAlpha + sumLog + n * std::log(beta));

                const bool ready = Numeric::NumericSupport::areEqual(betaNew, beta, 0.01) && Numeric::NumericSupport::areEqual(alphaNew, alpha, 1E-6);

                alpha = alphaNew;
                beta = betaNew;

                if (ready || counter == maxLoops)
                {
                    break;
                }
            }

            // perform final log likelihood estimation
            alpha = std::isnan(alpha) || alpha < minValue ? 1 : alpha;
            beta = std::isnan(beta) || beta < minValue ? 1 : beta;

            double mean = Numeric::NumericSupport::getMean(values);

            auto fitter = DistributionFitter();

            if (std::isnan(shift))
            {
                std::vector minValues = { minValue, minValue, 3 * newShift - 2 * mean };
                std::vector maxValues = { 2 * alpha - minValue, 2 * beta - minValue, newShift };
                std::vector properties = { Shape, Scale, Shift };
                std::vector<double> parameters = fitter.fitByLogLikelihood(values, this, stochast, minValues, maxValues, properties);

                stochast.Shape = std::max(minValue, parameters[0]);
                stochast.Scale = std::max(minValue, parameters[1]);
                stochast.Shift = parameters[2];
            }
            else
            {
                stochast.Shift = shift;

                std::vector minValues = { minValue, minValue };
                std::vector maxValues = { 2 * alpha - minValue, 2 * beta - minValue };
                std::vector properties = { Shape, Scale };
                std::vector<double> parameters = fitter.fitByLogLikelihood(values, this, stochast, minValues, maxValues, properties);

                stochast.Shape = std::max(minValue, parameters[0]);
                stochast.Scale = std::max(minValue, parameters[1]);
            }
            stochast.Observations = static_cast<int>(values.size());
        }

        double FrechetDistribution::getMaxShiftValue(std::vector<double>& values)
        {
            return *std::ranges::min_element(values);
        }

        std::vector<double> FrechetDistribution::getSpecialPoints(StochastProperties& stochast)
        {
            std::vector<double> specialPoints{ stochast.Shift };
            return specialPoints;
        }
    }
}



