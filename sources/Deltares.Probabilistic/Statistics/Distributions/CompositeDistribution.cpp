#include <cmath>
#include "CompositeDistribution.h"
#include "../../Math/NumericSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        bool CompositeDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            for (std::shared_ptr<ContributingStochast> contributingStochast : stochast->ContributingStochasts)
            {
                if (contributingStochast->Probability > 0.0 && contributingStochast->Stochast->isVarying())
                {
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
                sumWeights += contributingStochast->Probability;
                sum += contributingStochast->Probability * contributingStochast->Stochast->getMean();
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
                sumWeights += contributingStochast->Probability;
                sum += contributingStochast->Probability * contributingStochast->Stochast->getPDF(x);
            }

            return sum / sumWeights;
        }

        double CompositeDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double sum = 0;
            double sumWeights = 0;

            for (std::shared_ptr<ContributingStochast> contributingStochast : stochast->ContributingStochasts)
            {
                sumWeights += contributingStochast->Probability;
                sum += contributingStochast->Probability * contributingStochast->Stochast->getCDF(x);
            }

            return sum / sumWeights;
        }

        std::vector<double> CompositeDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
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




