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
#include <cmath>
#include "PoissonDistribution.h"
#include "../../Math/NumericSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        void PoissonDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            stochast->Location = values[0];
        }

        bool PoissonDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Location > 0.0;
        }

        void PoissonDistribution::validate(Logging::ValidationReport& report, std::shared_ptr<StochastProperties> stochast, std::string& subject)
        {
            Logging::ValidationSupport::checkMinimum(report, 0, stochast->Location, "location", subject);
        }

        double PoissonDistribution::getMean(StochastProperties& stochast)
        {
            return stochast.Location;
        }

        double PoissonDistribution::getDeviation(StochastProperties& stochast)
        {
            return sqrt(stochast.Location);
        }

        void PoissonDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            stochast.Location = mean;
        }

        double PoissonDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            const double p = StandardNormal::getPFromU(u);

            const double maxP = StandardNormal::getPFromU(StandardNormal::UMax);
            double pdf = 0;

            for (int k = 0; k < kMax; k++)
            {
                pdf += getPDF(stochast, k);
                if (pdf > p - delta || pdf > maxP)
                {
                    return k;
                }
            }

            return kMax;
        }

        double PoissonDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            const double cdf = getCDF(stochast.clone(), x); // TODO
            return StandardNormal::getUFromP(cdf);
        }

        double PoissonDistribution::getPowerFactorial(double rate, int power)
        {
            double value = 1.0;

            for (int k = 1; k <= power; k++)
            {
                value *= rate / k;
            }

            return value;
        }


        double PoissonDistribution::getPDF(StochastProperties& stochast, double x)
        {
            const int k = static_cast<int>(std::floor(x));

            if (k >= 0 && Numeric::NumericSupport::areEqual(k, x, delta))
            {
                return getPowerFactorial(stochast.Location, k) * std::exp(-stochast.Location);
            }
            else
            {
                return 0.0;
            }
        }

        double PoissonDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            int k = static_cast<int>(std::floor(x));

            double cdf = 0.0;
            for (int i = 0; i <= k; i++)
            {
                cdf += getPDF(*stochast, i);
            }

            return cdf;
        }

        void PoissonDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
        {
            stochast->Location = Numeric::NumericSupport::getMean(values);
            stochast->Observations = static_cast<int>(values.size());
        }

        std::vector<double> PoissonDistribution::getDiscontinuityPoints(const StochastProperties& stochast)
        {
            std::vector<double> discontinuityPoints;

            double maxP = StandardNormal::getPFromU(StandardNormal::UMax);

            int k = 0;
            double cdf = 0;

            std::shared_ptr<StochastProperties> stochastPtr = std::make_shared<StochastProperties>(stochast);

            while (cdf < maxP && k < kMax)
            {
                discontinuityPoints.push_back(k);

                double pdf = getPDF(*stochastPtr, k);
                cdf += pdf;

                k++;
            }

            return discontinuityPoints;
        }

        std::vector<double> PoissonDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            double offset = 10 * delta;
            double maxP = StandardNormal::getPFromU(StandardNormal::UMax);

            std::vector<double> specialPoints;

            int k = 0;
            double cdf = 0;

            while (cdf < maxP && k < kMax)
            {
                specialPoints.push_back(k - offset);
                specialPoints.push_back(k);
                specialPoints.push_back(k + offset);

                double pdf = getPDF(*stochast, k);
                cdf += pdf;

                k++;
            }

            return specialPoints;
        }
    }
}




