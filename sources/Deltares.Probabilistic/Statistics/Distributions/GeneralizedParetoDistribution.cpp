#include "GeneralizedParetoDistribution.h"
#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"

#include <cmath>
#include <algorithm>

namespace Deltares
{
    namespace Statistics
    {
        bool GeneralizedParetoDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Shape > 0 && stochast->Scale >= 0;
        }

        bool GeneralizedParetoDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale > 0;
        }

        double GeneralizedParetoDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return getMeanByIteration(stochast);
        }

        double GeneralizedParetoDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return getDeviationByIteration(stochast);
        }

        double GeneralizedParetoDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double k = stochast->Shape;
            double s = stochast->Scale;
            double m = stochast->Shift;

            double epsilon = 1e-4;

            if (x >= m && std::abs(k) <= epsilon && s > 0)
            {
                return std::exp(-(x - m) / s) / s;
            }
            else if (x >= m && (k * x < s + k * m) && k > epsilon)
            {
                return std::pow(1 - k * (x - m) / s, 1 / k - 1) / s;
            }
            else if (x >= m && (k < epsilon) && s > 0)
            {
                return std::pow(1 - k * (x - m) / s, 1 / k - 1) / s;
            }
            else
            {
                return 0;
            }
        }

        double GeneralizedParetoDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double k = stochast->Shape;
            double s = stochast->Scale;
            double m = stochast->Shift;

            double epsilon = 1e-4;

            if (x >= m && std::fabs(k) <= epsilon && s > 0)
            {
                return std::exp(-(x - m) / s);
            }
            else if (x >= m && (k * x < s + k * m) && std::abs(k) > epsilon)
            {
                return 1 - std::pow(1 - k * (x - m) / s, 1 / k);
            }
            else if ((k * x >= s + k * m) && k > epsilon)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        double GeneralizedParetoDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double p = StandardNormal::getPFromU(u);

            double k = stochast->Shape;
            double s = stochast->Scale;
            double m = stochast->Shift;

            double epsilon = 1e-4;
            double q = 1 - p;

            if (std::abs(k) <= epsilon && s > 0)
            {
                return m - s * std::log(q);
            }
            else if (std::abs(k) > epsilon && s > 0)
            {
                return m + s * (1 - std::pow(q, k)) / k;
            }
            else
            {
                return std::nan("");
            }
        }

        double GeneralizedParetoDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
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

        void GeneralizedParetoDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            setXAtUByIteration(stochast, x, u, constantType);
        }

        std::vector<double> GeneralizedParetoDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints;

            specialPoints.push_back(stochast->Scale);

            return specialPoints;
        }
    }
}





