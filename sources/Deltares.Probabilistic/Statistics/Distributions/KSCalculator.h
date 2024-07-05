#pragma once
#include <memory>
#include <vector>

#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class KSCalculator
        {
        public:
            /**
             * \brief Calculates the Kolmogorov-Smirnov statistic, which is a value which indicates how well input values match a stochastic variable)
             * \param xValues input values 
             * \param distribution Stochastic distribution from which the CDF value is calculated 
             * \param stochast Stochastic variable
             * \return Kolmogorov-Smirnov statistic (0 = perfect match, 1 = worst possible match)) 
             */
            static double getGoodnessOfFit(std::vector<double>& xValues, std::shared_ptr<Distribution> distribution, std::shared_ptr<StochastProperties> stochast);
        };
    }
}

