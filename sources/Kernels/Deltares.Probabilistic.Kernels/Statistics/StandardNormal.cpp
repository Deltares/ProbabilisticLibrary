#include <pch.h>
#include "StandardNormal.h"
#include <cmath>
#include <limits>

double StandardNormal::BetaMax = 40;

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

double StandardNormal::getQFromU(double u)
{
    double p = 0; double q = 0; double pdf = 0;

    normp(u, p, q, pdf);

    return q;
}

double StandardNormal::getUFromQ(const double q)
{
    if (isnan(q))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    else if (q <= 0)
    {
        return BetaMax;
    }
    else if (q >= 1)
    {
        return -BetaMax;
    }
    else if (q == 0.5)
    {
        return 0;
    }

    double margin = 0.00000001;

    if (abs(q - 0.5) < margin)
    {
        // The most simple situation is the case where the probability of exceeding
        // is equal to 0.5. Then the reliability index (beta) is equal to zero
        return 0;
    }

    // The standard normal distribution is symmetric around the value zero.
    // So it is sufficient to consider the probability of exceeding between
    // 0 and 0.5. Probabilities of exceeding above 0.5 are transformed to values
    // between 0 and 0.5 by q = 1 - q
    double qsr = q > 0.5 ? 1 - q : q;
    qsr = max(qsr, 1.0E-300);

    // Initial estimate for the reliability index (beta). Two approximations are used
    // One approximation is used for values between 0 and 0.15. The other one is used
    // for values between 0.15 and 0.5
    double beta;
    if (qsr < 0.15)
    {
        beta = sqrt(-2.0 * log(5 * qsr));
    }
    else
    {
        beta = 1.464795 - 2.929590 * qsr;
    }

    // The initial estimate for the reliability index gives a probability of exceeding
    // Compare this value with the input value. This makes clear that the initial
    // estimate has to be adjusted.
    // There are two possibilities: The initial estimate is too low or it is too high
    // Using linear interpolation the correct beta will be computed.
    // For this linear interpolation the initial estimate is used as a lower bound or
    // upper bound
    const double increment = 0.1;
    int branch = 1;
    double beta1 = 0;
    double beta2 = 0;
    double q1 = 0;
    double q2 = 0;

    double prob = getQFromU(beta);
    if (prob > qsr)
    {
        // The initial estimate is the lower bound for the linear interpolation
        branch = 1;
        beta1 = beta;
        q1 = prob;
        beta = beta + increment;
    }
    else
    {
        // The initial estimate is the upper bound for the linear interpolation
        branch = 2;
        beta2 = beta;
        q2 = prob;
        beta = beta - increment;
    }

    // If the initial estimate is the lower bound, compute the upper bound
    // If the initial estimate is the upper bound, compute the lower bound
    // In this procedure it is possible that the lower bound is adjusted despite
    // the initial estimate is the lower bound. Even so it is possible that
    // the upper bound is adjusted despite the initial estimate is the upper bound.
    bool proceed = true;
    while (proceed)
    {
        prob = getQFromU(beta);
        if (prob > qsr)
        {
            // The lower bound for the linear interpolation is initialized or
            // this lower bound is adjusted
            beta1 = beta;
            q1 = prob;
            if (branch == 2)
            {
                proceed = false;
            }
            else
            {
                beta = beta + increment;
            }
        }
        else
        {
            // The upper bound for the linear interpolation is initialized or
            // this upper bound is adjusted
            beta2 = beta;
            q2 = prob;
            if (branch == 1)
            {
                proceed = false;
            }
            else
            {
                beta = beta - increment;
            }
        }
    }

    const int kMax = 25; // maximum for iteration-loop

    // The beta is adjusted using linear interpolation
    for (int k = 1; k <= 2 * kMax; k++)
    {
        // The iteration process can stop if the probability of exceeding of the
        // computed beta is close enough to the probability of exceeding from the input
        if (prob == qsr)
        {
            if (q > 0.5)
            {
                beta = -beta;
            }

            return beta;
        }

        if (k > kMax)
        {
            // The beta is computed in an iteration process.
            // If after several iteration steps the computed beta does not have a probability
            // of exceeding that is "close enough" to the input probability of exceeding the margin is adjusted
            margin = 10 * margin;
            // margin should be less than 1.0E-8; // Maximum for the margin
        }

        // The iteration process can stop if the interpolation interval is marginal
        double dq = q1 - q2;
        if (dq < 0)
        {
            dq = -dq;
        }

        if (dq < margin)
        {
            if (q > 0.5)
            {
                beta = -beta;
            }

            return beta;
        }

        // Compute a new value for beta using linear interpolation
        beta = beta1 + (q1 - qsr) / (q1 - q2) * (beta2 - beta1);
        prob = getQFromU(beta);

        // Adjust one of the interpolation bounds
        if (prob > qsr)
        {
            beta1 = beta;
            q1 = prob;
        }
        else
        {
            beta2 = beta;
            q2 = prob;
        }
    }

    // If the probability of exceeding of the input is more than 0.5,
    // the computed reliability index (beta) is adjusted by beta = -beta
    if (q > 0.5)
    {
        beta = -beta;
    }

    return beta;
}



