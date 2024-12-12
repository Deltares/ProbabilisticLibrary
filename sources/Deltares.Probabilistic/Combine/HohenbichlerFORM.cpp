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
#include <cmath>
#include "HohenbichlerFORM.h"
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

        std::pair<double, int> HohenbichlerFORM::PerformHohenbichler(const double betaV, const double pfU, const double rhoInput)
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

            auto stochast = std::vector<std::shared_ptr<Stochast>>();
            constexpr size_t nStochasts = 2;
            for (size_t i = 0; i < nStochasts; i++)
            {
                auto dist = DistributionType::Normal;
                std::vector<double> params{ 0.0, 1.0 };
                auto s = std::make_shared<Stochast>(dist, params);
                stochast.push_back(s);
            }
            auto corr = std::make_shared<CorrelationMatrix>();
            auto uConverter = std::make_shared<UConverter>(stochast, corr);
            uConverter->initializeForRun();
            auto zModel = std::make_shared<ZModel>([&w](std::shared_ptr<ModelSample> v) { return w.zfunc(v); });
            auto modelRunner = std::make_shared<ModelRunner>(zModel, uConverter);
            modelRunner->initializeForRun();
            auto relMethod = std::make_shared<FORM>();
            relMethod->Settings->RelaxationFactor = 0.4;
            relMethod->Settings->RelaxationLoops = maxTrialLoops;
            relMethod->Settings->EpsilonBeta = 0.01;
            relMethod->Settings->GradientSettings->StepSize = 0.1;
            relMethod->Settings->GradientSettings->gradientType = GradientType::TwoDirections;
            relMethod->Settings->MaxIterationsGrowthFactor  = 2;
            auto newResult = relMethod->getDesignPoint(modelRunner);
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



    }
}
