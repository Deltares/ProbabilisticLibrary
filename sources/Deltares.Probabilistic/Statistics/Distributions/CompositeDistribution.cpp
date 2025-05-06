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
#include "CompositeDistribution.h"
#include "../../Math/NumericSupport.h"
#include <algorithm>

namespace Deltares
{
    namespace Statistics
    {
        bool CompositeDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<std::shared_ptr<ContributingStochast>> contributingStochasts;
            for (std::shared_ptr<ContributingStochast> contributingStochast : stochast->ContributingStochasts)
            {
                if (contributingStochast->Probability > 0.0)
                {
                    if (contributingStochast->Stochast->isVarying())
                    {
                        // There is at least 1 really contributing (probability > 0) stochast which is varying, so the composite stochast is varying
                        return true;
                    }

                    contributingStochasts.push_back(contributingStochast);
                }
            }

            if (contributingStochasts.size() < 2)
            {
                return false;
            }

            // All non varying stochasts, but if they do not all lead to the same value, the composite stochast is varying
            double firstMean = contributingStochasts.front()->Stochast->getMean();
            for (std::shared_ptr<ContributingStochast> contributingStochast : contributingStochasts)
            {
                if (contributingStochast != contributingStochasts.front() && contributingStochast->Stochast->getMean() != firstMean)
                {
                    // There are at least 2 stochasts which do not lead to the same value, so the composite stochast is varying
                    return true;
                }
            }

            return false;
        }

        bool CompositeDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            double sum = 0;
            for (std::shared_ptr<ContributingStochast> contributingStochast : stochast->ContributingStochasts)
            {
                if (!contributingStochast->isValid())
                {
                    return false;
                }

                sum += contributingStochast->Probability;
            }

            return Numeric::NumericSupport::areEqual(1.0, sum, delta);
        }

        double CompositeDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            double sum = 0;
            double sumWeights = 0;

            for (std::shared_ptr<ContributingStochast> contributingStochast : stochast->ContributingStochasts)
            {
                if (contributingStochast->Probability > 0)
                {
                    sumWeights += contributingStochast->Probability;
                    sum += contributingStochast->Probability * contributingStochast->Stochast->getMean();
                }
            }

            return sum / sumWeights;
        }

        double CompositeDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return this->getDeviationByIteration(stochast);
        }

        double CompositeDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            return this->getXFromUByIteration(stochast, u);
        }

        double CompositeDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double cdf = getCDF(stochast, x);
            return StandardNormal::getUFromP(cdf);
        }

        double CompositeDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double sum = 0;
            double sumWeights = 0;

            for (std::shared_ptr<ContributingStochast> contributingStochast : stochast->ContributingStochasts)
            {
                if (contributingStochast->Probability > 0)
                {
                    sumWeights += contributingStochast->Probability;
                    sum += contributingStochast->Probability * contributingStochast->Stochast->getPDF(x);
                }
            }

            return sum / sumWeights;
        }

        double CompositeDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double sum = 0;
            double sumWeights = 0;

            for (std::shared_ptr<ContributingStochast> contributingStochast : stochast->ContributingStochasts)
            {
                if (contributingStochast->Probability > 0)
                {
                    sumWeights += contributingStochast->Probability;
                    sum += contributingStochast->Probability * contributingStochast->Stochast->getCDF(x);
                }
            }

            return sum / sumWeights;
        }

        std::vector<double> CompositeDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints;

            for (std::shared_ptr<ContributingStochast> contributingStochast : stochast->ContributingStochasts)
            {
                if (contributingStochast->Probability > 0)
                {
                    std::vector<double> contributingSpecialPoints = contributingStochast->Stochast->getSpecialXValues();
                    for (double x : contributingSpecialPoints)
                    {
                        specialPoints.push_back(x);
                    }
                }
            }

            std::sort(specialPoints.begin(), specialPoints.end());

            return specialPoints;
        }
    }
}




