#pragma once
#include <functional>
#include <cmath>

namespace Deltares
{
    namespace Numeric
    {
        typedef std::function<double(double)> RootFinderMethod;
        typedef std::function<bool()> StopMethod;

        class RootFinder
        {

        public:
            virtual double CalculateValue(double xLow, double xHigh, double target, double tolerance, int maxIterations, RootFinderMethod function, double knownLowValue = nan(""), double knownHighValue = nan(""))
            {
                return nan("");
            }
        };
    }
}




