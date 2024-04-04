#include "SpecialFunctions.h"

#include <math.h>
#include "../Utils/probLibException.h"

namespace Deltares
{
	namespace Numeric
	{
		double SpecialFunctions::getGamma(const double x)
		{
			return exp(getLogGamma(x));
		}

		// Function for the incomplete gamma function
		// See for background: Numerical recipes par. 6.2
		double SpecialFunctions::getGammaUpperRegularized(const double a, const double x)
		{
			// a  : input parameter a of incomplete gamma function
			// x  : input x-value of incomplete gamma function

			if (x < 0.0)
			{
				throw ProbLibCore::probLibException("Error in Dgammq: x < 0 violated");
			}
			else if (a <= 0.0)
			{
				throw ProbLibCore::probLibException("Error in Dgammq: a <= 0 violated; a = ", a);
			}
			else
			{
				if (x < a + 1.0)
				{
					return 1 - getGammaSeries(a, x);
				}
				else
				{
					return getGammaContinuedFraction(a, x);
				}
			}
		}

		// Function for the incomplete gamma function
		// using a series representation
		// See for background: Numerical recipes par. 6.2
		double SpecialFunctions::getGammaSeries(const double a, const double x)
		{
			// Declarations
			const int    itmax = 100;
			const double eps = 3.e-7;

			// Gamma function of a
			double gln = getLogGamma(a);

			// Iterations for incomplete gamma function
			if (x == 0.0)
			{
				return 0;
			}
			else
			{
				double ap = a;
				double sum = 1.0 / a;
				double del = sum;
				bool converged = false;
				for (int n = 1; n <= itmax; n++)
				{
					ap++;
					del *= x / ap;
					sum += del;
					if (fabs(del) < fabs(sum) * eps)
					{
						converged = true;
						break;
					}
				}

				if (converged)
				{
					return sum * exp(-x + a * log(x) - gln);
				}
				else
				{
					throw ProbLibCore::probLibException("Error in DGser: no convergence");
				}
			}
		}

		// Method for the computation of the incomplete gamma function
		// using continued fraction representation
		// See for background: Numerical recipes par. 6.2
		double SpecialFunctions::getGammaContinuedFraction(const double a, const double x)
		{
			const int    itMax = 100;
			const double eps = 3.e-7;
			const double fpMin = 1.e-30;

			// Gamma function of a
			double gln = getLogGamma(a);

			// Iterations for incomplete gamma function

			double b = x + 1.0 - a;
			double c = 1.0 / fpMin;
			double d = 1.0 / b;
			double h = d;
			bool converged = false;

			for (int i = 1; i <= itMax; i++)
			{
				double an = -i * (i - a);
				b += 2.0;
				d = an * d + b;
				if (fabs(d) < fpMin) d = fpMin;
				c = b + an / c;
				if (fabs(c) < fpMin) c = fpMin;
				d = 1.0 / d;
				double del = d * c;
				h *= del;
				if (fabs(del - 1.0) < eps)
				{
					converged = true;
					break;
				}
			}

			if (converged)
			{
				return exp(-x + a * log(x) - gln) * h;
			}
			else
			{
				throw ProbLibCore::probLibException("Error in DGcf: no convergence");
			}
		}

		// Logarithm of the Gamma function
		// For background: Numerical recipes par. 6.1
		double SpecialFunctions::getLogGamma(const double x)
		{
			const double stp = 2.5066282746310005;

			double y = x;
			double tmp = x + 5.5;
			tmp = (x + 0.5) * log(tmp) - tmp;
			double ser = 1.000000000190015;
			for (size_t j = 1; j <= 6; j++)
			{
				y += 1.0;
				ser += cof[j - 1] / y;
			}

			return tmp + log(stp * ser / x);
		}
	}
}

