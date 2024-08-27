#include "BetaDistribution.h"
#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/SpecialFunctions.h"

#include <cmath>
#include <algorithm>

namespace Deltares
{
    namespace Statistics
    {
        void BetaDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            this->setMeanAndDeviation(stochast, values[0], values[1]);
        }

        bool BetaDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Shape >= 0 && stochast->ShapeB >= 0;
        }

        bool BetaDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Shape > 0 && stochast->ShapeB > 0;
        }

        double BetaDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Shape / (stochast->Shape + stochast->ShapeB);
        }

        double BetaDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            double sum = stochast->Shape + stochast->ShapeB;
            double variance = stochast->Shape * stochast->ShapeB / (sum * sum * (sum + 1));
            return sqrt(variance);
        }

        void BetaDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            double sigma = deviation;
            double variance = sigma * sigma;

            stochast->Shape = ((1 - mean) / variance - 1 / mean) * mean * mean;
            stochast->ShapeB = stochast->Shape * (1 / mean - 1);
        }

        double BetaDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (!isValid(stochast))
            {
                return x == 0 ? 1 : 0;
            }
            else if (x < 0 || x > 1)
            {
                return 0;
            }
            else
            {
                return std::pow(x, stochast->Shape - 1) * std::pow(1 - x, stochast->ShapeB - 1) / Numeric::SpecialFunctions::getBeta (stochast->Shape, stochast->ShapeB);
            }
        }

        double BetaDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (!isValid(stochast))
            {
                return x <= 0 ? 0 : 1;
            }
            else if (x <= 0)
            {
                return 0;
            }
            else if (x >= 1)
            {
                return 1;
            }
            else
            {
                try
                {
                    double cdf = Numeric::SpecialFunctions::getBetaIncomplete(stochast->Shape, stochast->ShapeB, x);
                    return cdf;
                }
                catch (std::exception)
                {
                    return x <= 0 ? 0 : 1;
                }
            }
        }

        double BetaDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            if (!isValid(stochast))
            {
                return 0;
            }
            else
            {
                return this->getXFromUByIteration(stochast, u);
            }
        }

        double BetaDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x <= stochast->Scale)
            {
                return -StandardNormal::UMax;
            }
            else
            {
                double cdf = this->getCDF(stochast, x);
                return StandardNormal::getUFromP(cdf);
            }
        }

        void BetaDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            setXAtUByIteration(stochast, x, u, constantType);
        }

        void BetaDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            // https://stats.stackexchange.com/questions/12232/calculating-the-parameters-of-a-beta-distribution-using-the-mean-and-variance

            double mean = Numeric::NumericSupport::getMean(values);
            double sigma = Numeric::NumericSupport::getStandardDeviation(mean, values);
            double variance = sigma * sigma;

            stochast->Shape = ((1 - mean) / variance - 1 / mean) * mean * mean;
            stochast->ShapeB = stochast->Shape * (1 / mean - 1);
        }

        std::vector<double> BetaDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints;

            specialPoints.push_back(0.0);
            specialPoints.push_back(1.0);

            return specialPoints;
        }
    }
}





