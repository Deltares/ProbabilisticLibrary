#include <math.h>
#include "../utils/basic_math.h"
#include "../probFuncs/conversions.h"
#include "../probFuncs/listDistribs.h"
#include "../distributions/normalDist.h"
#include "../probMethods/progress.h"
#include "Hohenbichler.h"

namespace Deltares {
    namespace ProbLibCore {

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

        Hohenbichler::Hohenbichler(const progress& pg) : p(pg)
        {
            auto FormFlags = FormSettings();
            FormFlags.relaxationFactor = 0.4;
            FormFlags.du = 0.1;
            FormFlags.epsilonBeta = 0.01;
            FormFlags.epsilonZvalue = 0.01;
            Form = new formWithTrialLoops(FormFlags, p);
        }

        Hohenbichler::~Hohenbichler()
        {
            delete Form;
        }

        std::pair<double, ConvergenceStatus> Hohenbichler::PerformHohenbichler(const double betaV, const double pfU, const double rhoInput)
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
                return { conversions::QfromBeta(betaV), ConvergenceStatus::success };
            }
            //
            //   Limit the correlation coefficient away from 1. and -1.
            //   For rho > rhoLimit no calculation is performed, because the result is always equal 1.0.
            //   Values of rho less than -1.0 are not valid and set equal to -1.0.
            //
            rho = max(min(rho, rhoLimitHohenbichler), -1.0);

            //
            //   Initialise the probabilistic data module
            //
            auto w = HohenbichlerZ(betaV, pfU, rho);

            if (params.size() == 0)
            {
                auto u1 = new uSpace();
                auto u2 = new uSpace();
                params.addDistrib(u1);
                params.addDistrib(u2);
            }
            auto s = stochSettings(id, params);
            auto calcResult = Form->calc(w, s);

            //
            //   Compute the failure probability
            //
            double pfVpfU = calcResult.result.getQ();

            if (rhoInput > rhoLimitHohenbichler)
            {
                pfVpfU += (rhoInput - rhoLimitHohenbichler) / (1.0 - rhoLimitHohenbichler) * (1.0 - pfVpfU);
            }
            return { pfVpfU, calcResult.convergence };
        }

        HohenbichlerZ::HohenbichlerZ(const double betaV, const double pfU, const double r) :
            beta2(betaV), pf1(pfU), rho(r), squaredRoot(sqrt(1.0 - r * r)) {}

        //> ZHohenbichler: Z-function for Hohenbichler with input parameters x and output parameter z
        double HohenbichlerZ::zfunc(const probDraw& drw) const
        {
            //
            // u     : u-value (standard normal distributed)
            // w     : w-value (standard normal distributed)
            // beta2 : Reliability index for stochastic variable 2 (smallest reliability index)
            // pf1   : Probability of failure for stochastic variable 1 (smallest probability of failure)
            // rho   : Correlation coefficient between Z_1 and Z_2
            //
            const double u = drw.getx(0);
            const double w = drw.getx(1);
            double phiU = conversions::PfromBeta(u);
            double uAccent = conversions::betaFromQ(pf1 * phiU);
            return beta2 - rho * uAccent - squaredRoot * w;
        }
    }
}
