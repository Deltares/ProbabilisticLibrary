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
#include "BernoulliDistribution.h"
#include "../../Math/NumericSupport.h"
#include "../../Logging/ValidationSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        void BernoulliDistribution::initialize(StochastProperties& stochast, const std::vector<double>& values)
        {
            stochast.Location = values[0];
        }

        bool BernoulliDistribution::isVarying(StochastProperties& stochast)
        {
            return stochast.Location > 0.0 && stochast.Location < 1.0;
        }

        void BernoulliDistribution::validate(Logging::ValidationReport& report, StochastProperties& stochast, std::string& subject)
        {
            Logging::ValidationSupport::checkMinimum(report, 0, stochast.Location, "location", subject);
            Logging::ValidationSupport::checkMaximum(report, 1, stochast.Location, "location", subject);
            Distribution::validate(report, stochast, subject);
        }

        double BernoulliDistribution::getMean(StochastProperties& stochast)
        {
            return stochast.Location;
        }

        double BernoulliDistribution::getDeviation(StochastProperties& stochast)
        {
            return sqrt(stochast.Location * (1.0 - stochast.Location));
        }

        void BernoulliDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            stochast.Location = mean;
        }

        double BernoulliDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            double q = StandardNormal::getQFromU(u);

            if (q > stochast.Location)
            {
                return 0.0;
            }
            else
            {
                return 1.0;
            }
        }

        double BernoulliDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            const double cdf = getCDF(stochast, x);
            return StandardNormal::getUFromP(cdf);
        }

        double BernoulliDistribution::getPDF(StochastProperties& stochast, double x)
        {
            if (Numeric::NumericSupport::areEqual(x, 1.0, delta))
            {
                return stochast.Location;
            }
            else if (Numeric::NumericSupport::areEqual(x, 0.0, delta))
            {
                return 1.0 - stochast.Location;
            }
            else
            {
                return 0.0;
            }
        }

        double BernoulliDistribution::getCDF(StochastProperties& stochast, double x)
        {
            if (x < 0.0)
            {
                return 0.0;
            }
            else if (x >= 1.0)
            {
                return 1.0;
            }
            else
            {
                return 1.0 - stochast.Location;
            }
        }

        void BernoulliDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
        {
            stochast->Location = Numeric::NumericSupport::getMean(values);
            stochast->Observations = static_cast<int>(values.size());
        }

        void BernoulliDistribution::fitPrior(const std::shared_ptr<StochastProperties>& stochast, std::vector<double>& values, const std::shared_ptr<StochastProperties>& prior, const double shift)
        {
            int n_data = static_cast<int>(values.size());
            double n_data_success = Numeric::NumericSupport::sum(values);
            double n_data_fail = n_data - n_data_success;

            int n_prior = prior->Observations;
            double n_prior_success = prior->Location * n_prior;
            double n_prior_fail = n_prior - n_prior_success;

            // assume a beta distribution, the shape properties can be set according to binomial experiments
            // see https://statisticsbyjim.com/probability/beta-distribution/
            // see https://rpubs.com/sitaramgautam/145048

            double postAlpha = n_data_success + n_prior_success;
            double postBeta = n_data_fail + n_prior_fail;

            double postMean = postAlpha / (postAlpha + postBeta);

            stochast->Location = postMean;
            stochast->Observations = prior->Observations + n_data;
        }

        std::vector<double> BernoulliDistribution::getDiscontinuityPoints(const StochastProperties& stochast)
        {
            return { 0.0, 1.0 };
        }

        std::vector<double> BernoulliDistribution::getSpecialPoints(StochastProperties& stochast)
        {
            double offset = 10 * delta;

            std::vector<double> specialPoints;

            specialPoints.push_back(-offset);
            specialPoints.push_back(0.0);
            specialPoints.push_back(offset);

            specialPoints.push_back(1.0 - offset);
            specialPoints.push_back(1.0);
            specialPoints.push_back(1.0 + offset);

            return specialPoints;
        }
    }
}




