// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "SpecialFunctions.h"

#include <cmath>

#include "../Utils/probLibException.h"

namespace Deltares::Numeric
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
            throw Reliability::probLibException("Error in Dgammq: x < 0 violated; x = ", x);
        }
        else if (a <= 0.0)
        {
            throw Reliability::probLibException("Error in Dgammq: a <= 0 violated; a = ", a);
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

    double SpecialFunctions::getGammaLowerRegularized(const double a, const double x)
    {
        const double gamma = getGamma(x);
        const double upperGammaIncomplete = gamma * getGammaUpperRegularized(a, x);
        const double lowerGammaIncomplete = gamma - upperGammaIncomplete;
        const double lowerGammaRegularized = lowerGammaIncomplete / gamma;

        return lowerGammaRegularized;
    }

    // Function for the incomplete gamma function
    // using a series representation
    // See for background: Numerical recipes par. 6.2
    double SpecialFunctions::getGammaSeries(const double a, const double x)
    {
        // Declarations
        constexpr int    max_iterations = 100;
        constexpr double eps = 3.e-7;

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
            for (int n = 1; n <= max_iterations; n++)
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
                throw Reliability::probLibException("Error in DGser: no convergence");
            }
        }
    }

    // Method for the computation of the incomplete gamma function
    // using continued fraction representation
    // See for background: Numerical recipes par. 6.2
    double SpecialFunctions::getGammaContinuedFraction(const double a, const double x)
    {
        constexpr int    max_iterations = 100;
        constexpr double eps = 3.e-7;
        constexpr double fpMin = 1.e-30;

        // Gamma function of a
        double gln = getLogGamma(a);

        // Iterations for incomplete gamma function

        double b = x + 1.0 - a;
        double c = 1.0 / fpMin;
        double d = 1.0 / b;
        double h = d;
        bool converged = false;

        for (int i = 1; i <= max_iterations; i++)
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
            throw Reliability::probLibException("Error in DGcf: no convergence");
        }
    }

    // Logarithm of the Gamma function
    // For background: Numerical recipes par. 6.1
    double SpecialFunctions::getLogGamma(const double x)
    {
        constexpr double stp = 2.5066282746310005;

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

    double SpecialFunctions::getBetaRegularized(double a, double b, double x)
    {
        const double beta_incomplete = getBetaIncomplete(a, b, x);
        const double beta = getBeta(a, b);
        return beta_incomplete / beta;
    }

    double SpecialFunctions::getBeta(double a, double b)
    {
        return std::beta(a, b);
    }

    // Implementation taken from https://github.com/codeplea/incbeta
    double SpecialFunctions::getBetaIncomplete(double a, double b, double x)
    {
        if (x < 0.0 || x > 1.0)
        {
            return std::nan("");
        }

        bool convergenceExpected = x >= (a + 1.0) / (a + b + 2.0);

        if (!convergenceExpected)
        {
            // If no convergence is expected, use the fact that beta is symmetrical, swap a and b and call this function again
            return (1.0 - getBetaIncomplete(b, a, 1.0 - x)); 
        }
        else
        {
            constexpr double stop_value = 1.0e-8;
            constexpr double tiny_value = 1.0e-30;

            //Find the first part before the continued fraction.
            const double lbeta_ab = lgamma(a) + lgamma(b) - lgamma(a + b);
            const double front = exp(log(x) * a + log(1.0 - x) * b - lbeta_ab) / a;

            //Use Lentz's algorithm to evaluate the continued fraction
            double f = 1.0;
            double c = 1.0;
            double d = 0.0;

            for (int i = 0; i <= 200; ++i)
            {
                int m = i / 2;

                double numerator;
                if (i == 0)
                {
                    numerator = 1.0; /*First numerator is 1.0.*/
                }
                else if (i % 2 == 0)
                {
                    numerator = (m * (b - m) * x) / ((a + 2.0 * m - 1.0) * (a + 2.0 * m)); /*Even term.*/
                }
                else
                {
                    numerator = -((a + m) * (a + b + m) * x) / ((a + 2.0 * m) * (a + 2.0 * m + 1)); /*Odd term.*/
                }

                // Do an iteration of Lentz's algorithm
                d = 1.0 + numerator * d;
                if (fabs(d) < tiny_value)
                {
                    d = tiny_value;
                }
                d = 1.0 / d;

                c = 1.0 + numerator / c;
                if (fabs(c) < tiny_value) c = tiny_value;

                const double cd = c * d;
                f *= cd;

                // Check for stop
                if (fabs(1.0 - cd) < stop_value)
                {
                    return front * (f - 1.0);
                }
            }

            // Needed more loops, did not converge
            return std::nan(""); 
        }
    }
}


