#pragma once
#include "RootFinder.h"
namespace Deltares
{
	namespace Numeric
	{
		class BisectionRootFinder : public RootFinder
		{
		public:
			double CalculateValue(double minStart, double maxStart, double resultValue, double tolerance, RootFinderMethod function, StopMethod isStopped = [] {return false; }, double xTolerance = 1E-6);
		};
	}
}

