#include <cmath>
#include "Hohenbichler.h"

#include "CombineType.h"
#include "HohenbichlerZ.h"
#include "../Math/NumericSupport.h"
#include "../Statistics/StandardNormal.h"
#include "../Reliability/DesignPoint.h"
#include "../Model/ModelRunner.h"
#include "../Reliability/FORM.h"

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
        // Note: the probability of failure can be a probability of exceedance or a probability of non-exceedance
        //
        // This method has two failure probabilities as input,
        // \f$ P\left( {Z_2  < 0} \right)\ \f$ and \f$ P\left( {Z_1  < 0} \right)\ \f$,
        // but for the largest one the reliability index (beta-value) is the input; for the smallest failure probability the
        // failure probability itself is the input. The reason why the reliability index is the input for the largest
        // failure probability is because it is more accurate. For the smallest failure probability the failure
        // probability itself is the input because, outside this routine this failure probability is necessary.
        // So this failure probability hasn't to be computed twice.
        //
        // Determining which of the two probabilities of failure is greatest, is also needed outside this routine.
        // Therefore, this is not done within the routine but only outside the routine.
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
            else if (useNumInt)
            {
                return { BetaHohenbichler(betaV, StandardNormal::getUFromP(pfU), rho, combAnd), 0 };
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

            auto stochast = std::vector<std::shared_ptr<Stochast>>();
            const size_t nStoch = 2;
            for (size_t i = 0; i < nStoch; i++)
            {
                auto dist = DistributionType::Normal;
                std::vector<double> params{ 0.0, 1.0 };
                std::shared_ptr<Stochast> s(new Stochast(dist, params));
                stochast.push_back(s);
            }
            std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
            std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
            uConverter->initializeForRun();
            std::shared_ptr<ZModel> zModel(new ZModel([&w](std::shared_ptr<ModelSample> v) { return w.zfunc(v); }));
            std::shared_ptr<ModelRunner> modelRunner(new ModelRunner(zModel, uConverter));
            std::shared_ptr<FORM> relMethod(new FORM);
            relMethod->Settings->RelaxationFactor = 0.4;
            relMethod->Settings->RelaxationLoops = maxTrialLoops;
            relMethod->Settings->EpsilonBeta = 0.01;
            relMethod->Settings->GradientSettings->StepSize = 0.1;
            relMethod->Settings->GradientSettings->gradientType = GradientType::TwoDirections;
            relMethod->Settings->MaxIterationsGrowthFactor  = 2;
            std::shared_ptr<DesignPoint> newResult(relMethod->getDesignPoint(modelRunner));
            auto converged = (newResult->convergenceReport->IsConverged ? 0 : 1);

            //
            //   Compute the failure probability
            //
            double pfVpfU = StandardNormal::getQFromU(newResult->Beta);

            if (rhoInput > rhoLimitHohenbichler)
            {
                pfVpfU += (rhoInput - rhoLimitHohenbichler) / (1.0 - rhoLimitHohenbichler) * (1.0 - pfVpfU);
            }
            return { pfVpfU, converged };
        }

        double Hohenbichler::BetaHohenbichler(double dp1, double dp2, double rho, combineAndOr system)
        {
            if (dp1 > dp2)
            {
                return HohenbichlerNumInt(dp2, dp1, rho, system);
            }
            else
            {
                return HohenbichlerNumInt(dp1, dp2, rho, system);
            }
        }

        double Hohenbichler::HohenbichlerNumInt(double dp1, double dp2, double rho, combineAndOr system)
        {
            const double maxDiffRho = 1e-10;

            if (Numeric::NumericSupport::areEqual(rho, 1.0, maxDiffRho))
            {
                if (system == combOr)
                {
                    return std::min(dp1, dp2);
                }
                else
                {
                    return std::max(dp1, dp2);
                }
            }
            else if (Numeric::NumericSupport::areEqual(rho, -1.0, maxDiffRho))
            {
                if (system)
                {
                    double pf = std::min(1.0, StandardNormal::getPFromU(dp1) + StandardNormal::getPFromU(dp2));
                    return StandardNormal::getUFromQ(pf);
                }
                else
                {
                    double pf = abs(StandardNormal::getPFromU(dp1) - StandardNormal::getPFromU(dp2));
                    return StandardNormal::getUFromQ(pf);
                }
            }

            // otherwise: start Hohenbichler procedure

            double pCond = 0;
            double rhoCompl = sqrt(1.0 - rho * rho);

            if (-dp2 > -StandardNormal::UMax)
            {
                const int ngridPerBeta = 1000;
                int ngrid = (int)round((-dp2 + StandardNormal::UMax) * ngridPerBeta) + 1; // number of grids for numerical integration

                auto u = LinearSpaced(ngrid, -StandardNormal::UMax, -dp2); // grid end points

                auto uCentered = std::vector<double>(); // grid centers
                auto uDiff = std::vector<double>();

                for (size_t i = 0; i < u.size()-1; i++)
                {
                    uCentered.push_back((u[i] + u[i + 1]) / 2);
                    uDiff.push_back(u[i + 1] - u[i]);
                }

                double pTotal = 0;

                for (size_t i = 0; i < uCentered.size(); i++)
                {
                    double prob = StandardNormal::getPFromU(-(dp1 + rho * uCentered[i]) / rhoCompl);
                    double pRange = StandardNormal::getPFromU(u[i + 1]) - StandardNormal::getPFromU(u[i]);

                    pTotal += pRange * prob;
                }

                pCond = pTotal / StandardNormal::getPFromU(-dp2);

                pCond = std::min(1.0, pCond);
            }

            double PfAND = pCond * StandardNormal::getPFromU(dp2);
            double betaAND = StandardNormal::getUFromP(1.0 - PfAND);

            // compute P(Z1<0 OR PZ2<0)
            double PfOR = StandardNormal::getPFromU(-dp1) + StandardNormal::getPFromU(dp2) - PfAND;
            double betaOR = StandardNormal::getUFromP(1.0 - PfOR);

            if (system == combAnd)
            {
                return betaAND;
            }
            else
            {
                return betaOR;
            }
        }

        std::vector<double> Hohenbichler::LinearSpaced(int length, double start, double stop)
        {
            if (length < 0)
            {
                throw probLibException("length in LinearSpaced < 0");
            }

            switch (length)
            {
            case 0:
                return std::vector<double> {};
            case 1:
                return std::vector<double> { stop };
            default:
            {
                double num = (stop - start) / (double)(length - 1);
                auto array = std::vector<double>();
                for (int i = 0; i < length; i++)
                {
                    array.push_back(start + (double)i * num);
                }

                array[length - 1] = stop;
                return array;
            }
            }
        }



    }
}
