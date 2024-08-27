#include "KSCalculator.h"
#include <cmath>
#include <algorithm>
#include <memory>
#include <vector>

#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        double KSCalculator::getGoodnessOfFit(std::vector<double>& xValues, std::shared_ptr<Distribution> distribution, std::shared_ptr<StochastProperties> stochast)
        {
            if (xValues.empty())
            {
                return std::nan("");
            }
            else
            {
                std::sort(xValues.begin(), xValues.end());

                double size = xValues.size();

                double ks = 0;

                for (int i = 0; i < xValues.size(); i++)
                {
                    double cdf = distribution->getCDF(stochast, xValues[i]);

                    double diff1 = std::fabs(cdf - i / size);
                    double diff2 = std::fabs(cdf - (i + 1) / size);

                    double diff = std::max(diff1, diff2);
                    if (diff > ks)
                    {
                        ks = diff;
                    }
                }

                return ks;
            }
        }
    }
}
