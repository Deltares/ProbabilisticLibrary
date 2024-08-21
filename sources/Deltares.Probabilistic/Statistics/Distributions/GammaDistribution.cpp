#include "GammaDistribution.h"
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
        void GammaDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            this->setMeanAndDeviation(stochast, values[0], values[1]);
        }

        bool GammaDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale >= 0 && stochast->Shape >= 0;
        }

        bool GammaDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale > 0;
        }

        double GammaDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale * stochast->Shape;
        }

        double GammaDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale * sqrt(stochast->Shape);
        }

        void GammaDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            stochast->Scale = deviation * deviation / mean;
            stochast->Shape = mean / stochast->Scale;
        }

        double GammaDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (!isValid(stochast))
            {
                return x == 0 ? 1 : 0;
            }
            else if (stochast->Shape <= 0 || stochast->Scale <= 0)
            {
                return x == 0 ? 1 : 0;
            }
            else
            {
                try
                {
                    return std::pow(x, stochast->Shape - 1) * std::exp(-x / stochast->Scale) / (std::pow(stochast->Scale, stochast->Shape) * Numeric::SpecialFunctions::getGamma(stochast->Shape));
                }
                catch (std::exception)
                {
                    return std::nan("");
                }
            }
        }

        double GammaDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (!isValid(stochast))
            {
                return x <= 0 ? 0 : 1;
            }
            else if (stochast->Shape <= 0 || stochast->Scale <= 0)
            {
                return x <= 0 ? 0 : 1;
            }
            else if (x <= 0)
            {
                return 0;
            }
            else
            {
                try
                {
                    return Numeric::SpecialFunctions::getGammaLowerRegularized(stochast->Shape, x / stochast->Scale);
                }
                catch (std::exception)
                {
                    return std::nan("");
                }
            }
        }

        double GammaDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
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

        double GammaDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double cdf = this->getCDF(stochast, x);
            return StandardNormal::getUFromP(cdf);
        }

        void GammaDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            setXAtUByIteration(stochast, x, u, constantType);
        }

        void GammaDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            double mean = Numeric::NumericSupport::getMean(values);

            std::vector<double> logValues = Numeric::NumericSupport::select(values, [](double p) { return std::log(p); });
            double meanLog = Numeric::NumericSupport::getMean(logValues);

            double s = std::log(mean) - meanLog;

            stochast->Shape = (3 - s + std::sqrt((s - 3) * (s - 3) + 24 * s)) / (12 * s);
            stochast->Scale = mean / stochast->Shape;
        }
    }
}





