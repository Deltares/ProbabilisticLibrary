#pragma once
namespace Deltares
{
    namespace Statistics
    {
        /**
         * \brief Defines what to do when the distribution type of a stochast changes
         */
        enum DistributionChangeType { Nothing, MaintainMeanAndDeviation, FitFromHistogramValues };
    }
}
