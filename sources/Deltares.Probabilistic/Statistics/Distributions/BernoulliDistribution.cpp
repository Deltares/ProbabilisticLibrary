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
            double p = StandardNormal::getPFromU(u);

            if (p < 1 - stochast->Location)
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
            if (x <= 0)
            {
                return 0;
            }
            else if (x < 1.0)
            {
                return 1.0 - stochast->Location;
            }
            else
            {
                return 1.0;
            }
        }

        void BernoulliDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            int zeroValues = 0;

            for (double x : values)
            {
                if (Numeric::NumericSupport::areEqual(x, 0.0, delta))
                {
                    zeroValues++;
                }
            }

            stochast->Location = Numeric::NumericSupport::Divide(zeroValues, static_cast<int>(values.size()));
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




