#pragma once

#include <functional>

#include "RootFinder.h"

namespace Deltares
{
    namespace Numeric
    {
        class XValue;

        class LinearRootFinder : public RootFinder
        {
        public:
            /// <summary>
            /// Calculates the input value, which would lead to a result value when invoked for a given function. Extrapolation is allowed.
            /// </summary>
            /// <param name="xLow">Minimum start value</param>
            /// <param name="xHigh">Maximum start value</param>
            /// <param name="target">The result value</param>
            /// <param name="tolerance">The tolerance in the function result</param>
            /// <param name="maxIterations">Maximum number of iterations</param>
            /// <param name="function">The function</param>
            /// <param name="knownLowValue">Optional already calculated result for minStart</param>
            /// <param name="knownHighValue">Optional already calculated result for maxStart</param>
            /// <returns>The value which would lead to the result value when invoked for the function</returns>
            double CalculateValue(double xLow, double xHigh, double target, double tolerance, int maxIterations, RootFinderMethod function, double knownLowValue = nan(""), double knownHighValue = nan("")) override;

        private:
            XValue interpolate(const XValue& low, const XValue& high, double target, RootFinderMethod function);
            XValue bisection(const XValue& low, const XValue& high, RootFinderMethod function);
        };
    }
}



