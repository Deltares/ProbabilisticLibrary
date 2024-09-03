// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "StandardNormal.h"
#include <cmath>
#include "../Math/ASA/asa241.hpp"

using namespace ASA::conversions;

namespace Deltares
{
    namespace Statistics
    {
        void StandardNormal::normp(const double z, double& p, double& q, double& pdf)

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

        double StandardNormal::getPFromU(double u)
        {
            double p = 0; double q = 0; double pdf = 0;

            normp(u, p, q, pdf);

            return p;
        }

        void StandardNormal::getPQfromU(const double u, double& p, double& q)
        {
            double pdf;
            normp(u, p, q, pdf);
        }

        PQ StandardNormal::getPQFromU(const double u)
        {
            double pdf;
            PQ pq = PQ();
            normp(u, pq.p, pq.q, pdf);

            return pq;
        }

        double StandardNormal::getQFromU(double u)
        {
            double p = 0; double q = 0; double pdf = 0;

            normp(u, p, q, pdf);

            return q;
        }

        double StandardNormal::getUFromPQ(const PQ* pq)
        {
            return getUFromQ(pq->q);
        }

        double StandardNormal::getUFromP(const double p)
        {
            return -getUFromQ(p);
        }

        double StandardNormal::getUFromQ(const double q)
        {
            const double margin = 1e-35;
            double beta;
            if (std::isnan(q))
            {
                return nan("");
            }
            else if (q == 0)
            {
                return BetaMax;
            }
            else if (q == 1)
            {
                return -BetaMax;
            }
            else if (fabs(q) < margin)
            {
                // q close to zero (machine precision), p close to 1, return positive beta value
                double qsr = (q > qMin ? q : qMin);
                beta = -r8_normal_01_cdf_inverse(qsr);
            }
            else if (fabs(q - 1.0) < margin)
            {
                // q close to 1 (machine precision, p close to 0, return negative beta value
                double p = fabs(q - 1.0);
                double qsr = (qMin > p ? qMin : p);
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

        /// <summary>
        /// Gets the return period from the probability of failure
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        double StandardNormal::getRFromP(double p)
        {
            return - 1 / log(1 - p);
        }

        /// <summary>
        /// Gets the probability of failure from the return period
        /// </summary>
        /// <param name="r">return period</param>
        /// <returns></returns>
        double StandardNormal::getPFromR(double r)
        {
            return 1 - exp(-1 / abs(r));
        }

        /// <summary>
        /// Gets the return period from the reliability index
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        double StandardNormal::getRFromU(double u)
        {
            double q = getQFromU(u);
            return -1 / log(q);
        }

        /// <summary>
        /// Gets the reliability index from the return period
        /// </summary>
        /// <param name="r">return period</param>
        /// <returns></returns>
        double StandardNormal::getUFromR(double r)
        {
            double q = exp(-1 / abs(r));
            return getUFromQ(q);
        }

        double StandardNormal::ReturnTimeFromBeta(const double beta)
        {
            return 1.0 / FreqFromBeta(beta);
        }

        double StandardNormal::FreqFromBeta(const double beta)
        {
            const double uLimit = 5.6;
            double p; double q;
            if (beta > uLimit)
            {
                q = getQFromU(beta);
                q = std::max(q, qMin);
                return q;
            }
            else
            {
                p = getPFromU(beta);
                p = std::max(p, qMin);
                return -log(p);
            }
        }

        double StandardNormal::LogQFromBeta(const double beta)
        {
            return -log(FreqFromBeta(beta));
        }

    }
}





