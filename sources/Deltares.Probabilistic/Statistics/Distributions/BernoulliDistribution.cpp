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

namespace Deltares
{
    namespace Statistics
    {
        void BernoulliDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            stochast->Location = values[0];
        }

        bool BernoulliDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Location > 0.0 && stochast->Location < 1.0;
        }

        bool BernoulliDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Location >= 0.0 && stochast->Location <= 1.0;
        }

        double BernoulliDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Location;
        }

        double BernoulliDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return sqrt(stochast->Location * (1 - stochast->Location));
        }

        void BernoulliDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            stochast->Location = mean;
        }

        double BernoulliDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double q = StandardNormal::getQFromU(u);

            if (q > stochast->Location)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }

        double BernoulliDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double cdf = getCDF(stochast, x);
            return StandardNormal::getUFromP(cdf);
        }

        double BernoulliDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (Numeric::NumericSupport::areEqual(x, 1.0, delta))
            {
                return stochast->Location;
            }
            else if (Numeric::NumericSupport::areEqual(x, 0.0, delta))
            {
                return 1.0 - stochast->Location;
            }
            else
            {
                return 0.0;
            }
        }

        double BernoulliDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x < 0)
            {
                return 0;
            }
            else if (x >= 1.0)
            {
                return 1.0;
            }
            else
            {
                return 1.0 - stochast->Location;
            }
        }

        void BernoulliDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            stochast->Location = Numeric::NumericSupport::getMean(values);
        }

        void BernoulliDistribution::fitPrior(std::shared_ptr<StochastProperties> stochast, std::shared_ptr<StochastProperties> prior, std::vector<double>& values)
        {
            int n = static_cast<int>(values.size());

            int n_zero = 0;
            for (double x : values)
            {
                if (x == 0.0) { n_zero++; }
            }

            double mu = getMean(prior);
            double sigma = getDeviation(prior);

            double c = (mu * (1 - mu) / sigma) - 1;
            double a = mu * c;
            double b = (1 - mu) * c;

            double a_post = a + n_zero;
            double b_post = b + n - n_zero;

            double p = a_post / (a_post + b_post);

            double p_corrected = std::max(0.0, std::min(1.0, p));

            stochast->Location = p_corrected;
        }

        std::vector<double> BernoulliDistribution::getDiscontinuityPoints(const StochastProperties& stochast)
        {
            return { 0.0, 1.0 };
        }

        std::vector<double> BernoulliDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
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




