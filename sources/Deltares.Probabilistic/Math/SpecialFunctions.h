#pragma once
#include <vector>

namespace Deltares
{
	namespace Numeric
	{
		class SpecialFunctions
		{
		public:
			static double getGamma(const double x);
			static double getGammaUpperRegularized(const double a, const double x);
		private:
			static double getGammaSeries(const double a, const double x);
			static double getGammaContinuedFraction(const double a, const double x);
			static double getLogGamma(const double x);

			static inline std::vector<double> cof =
			{ 76.18009172947146, -86.50532032941677, 24.01409824083091,
			 -1.231739572450155,  .1208650973866179e-2,  -.5395239384953e-5 };
		};
	}
}

