#include "dgammaDs.h"
#include <math.h>
#include "../utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

        dgammaDs::dgammaDs(const double a)
        {
            prepareLog = DGamLn(a);
            prepareA = a;
        }

        // Function for the incomplete gamma function
        // See for background: Numerical recipes par. 6.2
        double dgammaDs::Dgammq(const double a, const double x)
        {
            // a  : input parameter a of incomplete gamma function
            // x  : input x-value of incomplete gamma function

            if (x < 0.0)
            {
                throw probLibException("Error in Dgammq: x < 0 violated");
            }
            else if (a <= 0.0)
            {
                throw probLibException("Error in Dgammq: a <= 0 violated; a = ");
            }
            else
            {
                if (x < a + 1.0)
                {
                    double gamser;
                    DGser(gamser, a, x);
                    return 1.0 - gamser;
                }
                else
                {
                    double gammcf;
                    DGcf(gammcf, a, x);
                    return gammcf;
                }
            }
        }

        // Function for the incomplete gamma function
        // using a series representation
        // See for background: Numerical recipes par. 6.2
        void dgammaDs::DGser(double& gamser, const double a, const double x)
        {
            // Declarations
            const int    itmax = 100;
            const double eps = 3.e-7;

            // Gamma function of a
            double gln;
            if (a == prepareA)
            {
                gln = prepareLog;
            }
            else
            {
                gln = DGamLn(a);
            }

            // Iterations for incomplete gamma function
            if (x == 0.0)
            {
                gamser = 0.0;
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
                    gamser = sum * exp(-x + a * log(x) - gln);
                }
                else
                {
                    throw probLibException("Error in DGser: no convergence");
                }
            }
        }

        // Method for the computation of the incomplete gamma function
        // using continued fraction representation
        // See for background: Numerical recipes par. 6.2
        void dgammaDs::DGcf(double& gammcf, const double a, const double x)
        {
            const int    itMax = 100;
            const double eps = 3.e-7;
            const double fpMin = 1.e-30;

            // Gamma function of a
            double gln;
            if (a == prepareA)
            {
                gln = prepareLog;
            }
            else
            {
                gln = DGamLn(a);
            }

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
                gammcf = exp(-x + a * log(x) - gln) * h;
            }
            else
            {
                throw probLibException("Error in DGcf: no convergence");
            }
        }

        // Logarithm of the Gamma function
        // For background: Numerical recipes par. 6.1
        double dgammaDs::DGamLn(const double x)
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
