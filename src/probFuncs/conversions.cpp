#include <math.h>
#include "../utils/basic_math.h"
#include "../extern/asa241.hpp"
#include "../extern/asa239.hpp"
#include "conversions.h"

using namespace ASA::conversions;

namespace Deltares {
    namespace ProbLibCore {

        double conversions::betaFromQ(const double q)
        {
            return betaFromQ_cdf_inverse(q);
        }

        double conversions::betaFromP(const double p)
        {
            return -betaFromQ_cdf_inverse(p);
        }

        //
        // wrapper around the function call to r8_normal_01_cdf_inverse
        //
        // Input value q is expected to be 0 < q < 1.
        // Function r8_normal_01_cdf_inverse returns huge(q) for values q <=0 or q >=1.
        // To allow for small numerical errors around 0 and 1, this subroutine modifies
        // the value of q, if necessary, before invoking r8_normal_cdf_inverse.
        //

        double conversions::betaFromQ_cdf_inverse(const double q)
        {
            const double margin = 1e-35;
            double beta;
            if (fabs(q) < margin)
            {
                // q close to zero (machine precision), p close to 1, return positive beta value
                double qsr = max(q, qMin);
                beta = -r8_normal_01_cdf_inverse(qsr);
            }
            else if (fabs(q - 1.0) < margin)
            {
                // q close to 1 (machine precision, p close to 0, return negative beta value
                double qsr = max(qMin, fabs(q - 1.0));
                beta = r8_normal_01_cdf_inverse(qsr);
            }
            else if (q > 0.5)
            {
                //
                // The standard normal distribution is symmetric around the value zero.
                // So it is sufficient to consider the probability of exceedance between
                // 0 and 0.5. Probabilities of exceedance above 0.5 are transformed to values
                // between 0 and 0.5 by q = 1 - q
                //
                double qsr = 1.0 - q;
                beta = r8_normal_01_cdf_inverse(qsr);
            }
            else
            {
                // 0 < q < 0.5
                beta = -r8_normal_01_cdf_inverse(q);
            }
            return beta;
        }

        void conversions::PQfromBeta(const double u, double& p, double& q)
        {
            double pdf;
            normp(u, p, q, pdf);
        }

        double conversions::PfromBeta(const double u)
        {
            double q;
            double p;
            PQfromBeta(u, p, q);
            return p;
        }

        double conversions::QfromBeta(const double u)
        {
            double q;
            double p;
            PQfromBeta(u, p, q);
            return q;
        }

        double conversions::Dgammq(const double x, const double p)
        {
            return ASA::conversions::gammad(x, p);
        }

        void conversions::normp(const double z, double& p, double& q, double& pdf)

            //****************************************************************************80
            //
            //  Purpose:
            //
            //    NORMP computes the cumulative density of the standard normal distribution.
            //
            //  Discussion:
            //
            //    This is algorithm 5666 from Hart, et al.
            //
            //  Licensing:
            //
            //    This code is distributed under the GNU LGPL license. 
            //
            //  Modified:
            //
            //    16 January 2008
            //
            //  Author:
            //
            //    Original FORTRAN77 version by Alan Miller.
            //    C++ version by John Burkardt.
            //
            //  Reference:
            //
            //    John Hart, Ward Cheney, Charles Lawson, Hans Maehly, 
            //    Charles Mesztenyi, John Rice, Henry Thacher, 
            //    Christoph Witzgall,
            //    Computer Approximations,
            //    Wiley, 1968,
            //    LC: QA297.C64.
            //
            //  Parameters:
            //
            //    Input, double Z, divides the real line into two 
            //    semi-infinite intervals, over each of which the standard normal 
            //    distribution is to be integrated.
            //
            //    Output, double *P, *Q, the integrals of the standard normal
            //    distribution over the intervals ( - Infinity, Z] and 
            //    [Z, + Infinity ), respectively.
            //
            //    Output, double *PDF, the value of the standard normal distribution
            //    at Z.
            //
        {
            double cutoff = 7.071;
            double expntl;
            double p0 = 220.2068679123761;
            double p1 = 221.2135961699311;
            double p2 = 112.0792914978709;
            double p3 = 33.91286607838300;
            double p4 = 6.373962203531650;
            double p5 = 0.7003830644436881;
            double p6 = 0.03526249659989109;
            double q0 = 440.4137358247522;
            double q1 = 793.8265125199484;
            double q2 = 637.3336333788311;
            double q3 = 296.5642487796737;
            double q4 = 86.78073220294608;
            double q5 = 16.06417757920695;
            double q6 = 1.755667163182642;
            double q7 = 0.08838834764831844;
            double root2pi = 2.506628274631001;
            double zabs;

            zabs = fabs(z);
            //
            //  37 < |Z|.
            //
            if (37.0 < zabs)
            {
                pdf = 0.0;
                p = 0.0;
            }
            //
            //  |Z| <= 37.
            //
            else
            {
                expntl = exp(-0.5 * zabs * zabs);
                pdf = expntl / root2pi;
                //
                //  |Z| < CUTOFF = 10 / sqrt(2).
                //
                if (zabs < cutoff)
                {
                    p = expntl * ((((((
                        p6 * zabs
                        + p5) * zabs
                        + p4) * zabs
                        + p3) * zabs
                        + p2) * zabs
                        + p1) * zabs
                        + p0) / (((((((
                            q7 * zabs
                            + q6) * zabs
                            + q5) * zabs
                            + q4) * zabs
                            + q3) * zabs
                            + q2) * zabs
                            + q1) * zabs
                            + q0);
                }
                //
                //  CUTOFF <= |Z|.
                //
                else
                {
                    p = pdf / (
                        zabs + 1.0 / (
                            zabs + 2.0 / (
                                zabs + 3.0 / (
                                    zabs + 4.0 / (
                                        zabs + 0.65)))));
                }
            }

            if (z < 0.0)
            {
                q = 1.0 - p;
            }
            else
            {
                q = p;
                p = 1.0 - q;
            }
        }

        double conversions::ReturnTimeFromBeta(const double beta)
        {
            return 1.0 / FreqFromBeta(beta);
        }

        double conversions::FreqFromBeta(const double beta)
        {
            double p; double q;
            PQfromBeta(beta, p, q);
            if (beta > uLimit)
            {
                q = max(q, qMin);
                return q;
            }
            else
            {
                p = max(p, qMin);
                return -log(p);
            }
        }

        double conversions::LogQFromBeta(const double beta)
        {
            return -log(FreqFromBeta(beta));
        }
    }
}
