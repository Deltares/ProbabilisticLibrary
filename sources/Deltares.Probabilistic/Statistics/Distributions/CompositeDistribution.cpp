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
                if (!contributingStochast->Stochast->isValid())
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




