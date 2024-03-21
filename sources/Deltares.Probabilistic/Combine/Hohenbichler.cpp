#include <math.h>
#include "Hohenbichler.h"
#include "../Statistics/StandardNormal.h"

using namespace Deltares::Statistics;

namespace Deltares {
    namespace Reliability {

        //> @file
        // This file contains a class with the computation method of Hohenbichler
        //<
        //
        //>
        // Class with the computation method of Hohenbichler

        //> Hohenbichler computes \f$ P\left( {Z_2  < 0|Z_1  < 0} \right)\ \f$,
        // i. e. the failure probability of element \f$ {Z_2 }\ \f$ given the failure
        // of element \f$ {Z_1 }\f$. For this computation the correlations are required.
        // The computation of \f$ P\left( {Z_2  < 0|Z_1  < 0} \right)\ \f$ has been performed with the method of Hohenbichler.
        // Note: the probability of failure can be a probability of exceedance (most of time parameters of load)
        // or a probability of non-exceedance (mostly parameters of strength)
        //
        // This subroutine has two failure probabilities as input,
        // \f$ P\left( {Z_2  < 0} \right)\ \f$ and \f$ P\left( {Z_1  < 0} \right)\ \f$,
        // but for the largest one the reliability index (beta-value) is the input; for the smallest failure probability the
        // failure probability itself is the input. The reason why the reliability index is the input for the largest
        // failure probability is because it is more accurate. For the smallest failure probability the failure
        // probability itself is the input because, outside this routine this failure probability is necessary.
        // So this failure probability hasn't to be computed twice.
        //
        // Determining which of the two probabilities of failure is greatest, is also needed outside this routine.
        // Therefore this is not done within the routine but only outside the routine.
        //
        // Later on the probability \f$ P\left( {Z_1  < 0\,AND\,Z_2  < 0} \right) \f$ =
        // \f$ P\left( {Z_1  < 0} \right) \cdot P\left( {Z_2  < 0|Z_1  < 0} \right)\f$ is used. This
        // probability could be computed here also, but isn't done.
        // This is because the probability \f$ P\left( {Z_2  < 0|Z_1  < 0} \right)\ \f$
        // is needed for the computation of the alpha's (i.e. direction of the design point).
        // So the output of the subroutine Hohenbichler is \f$ P\left( {Z_2  < 0|Z_1  < 0} \right)\ \f$

        std::pair<double, int> Hohenbichler::PerformHohenbichler(const double betaV, const double pfU, const double rhoInput)
        {
            //
            //   INPUT/OUTPUT VARIABLES
            //
                // betaV       //< Smallest reliability index of two stochastic parameters.
                //             // This one has the largest probability of failure, \f$ P\left( {Z_2  < 0} \right) \f$
                // pfU         //< Smallest probability of failure of two stochastic parameters, \f$ P\left( {Z_1  < 0} \right) \f$
                //             // This one has the largest reliability index
                // rhoInput    //< Correlation coefficient between \f$ {Z_1 } \f$ and \f$ {Z_2 } \f$
                // out:        //< Failure probability \f$ P\left( {Z_2  < 0|Z_1  < 0} \right) \f$
            //
            //   LOCAL VARIABLES
            //
                // rho                              ! Locally used value of the correlation coefficient, possibly adjusted
            //
            //   The FORM computation isn't necessary if the correlation coefficient is marginal
            //
            double rho = rhoInput;
            if (fabs(rho) < 1.0e-8)
            {
                return { StandardNormal::getQFromU(betaV), 0 };
            }
            //
            //   Limit the correlation coefficient away from 1. and -1.
            //   For rho > rhoLimit no calculation is performed, because the result is always equal 1.0.
            //   Values of rho less than -1.0 are not valid and set equal to -1.0.
            //
            rho = std::max(std::min(rho, rhoLimitHohenbichler), -1.0);

            //
            //   Initialise the probabilistic data module
            //
            auto w = HohenbichlerZ(betaV, pfU, rho);

            //
            //   Compute the failure probability
            //
            double pfVpfU = -999.0; // calcResult.result.getQ();

            if (rhoInput > rhoLimitHohenbichler)
            {
                pfVpfU += (rhoInput - rhoLimitHohenbichler) / (1.0 - rhoLimitHohenbichler) * (1.0 - pfVpfU);
            }
            return { pfVpfU, 0 };
        }

        HohenbichlerZ::HohenbichlerZ(const double betaV, const double pfU, const double r) :
            beta2(betaV), pf1(pfU), rho(r), squaredRoot(sqrt(1.0 - r * r)) {}

        //> ZHohenbichler: Z-function for Hohenbichler with input parameters x and output parameter z
        double HohenbichlerZ::zfunc(std::vector<double> & x) const
        {
            //
            // u     : u-value (standard normal distributed)
            // w     : w-value (standard normal distributed)
            // beta2 : Reliability index for stochastic variable 2 (smallest reliability index)
            // pf1   : Probability of failure for stochastic variable 1 (smallest probability of failure)
            // rho   : Correlation coefficient between Z_1 and Z_2
            //
            const double u = x[0];
            const double w = x[1];
            double phiU = StandardNormal::getPFromU(u);
            double uAccent = StandardNormal::getUFromQ(pf1 * phiU);
            return beta2 - rho * uAccent - squaredRoot * w;
        }
    }
}
