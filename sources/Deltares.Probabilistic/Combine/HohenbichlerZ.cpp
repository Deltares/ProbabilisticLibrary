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
#include "HohenbichlerZ.h"
#include "../Statistics/StandardNormal.h"

using namespace Deltares::Statistics;

namespace Deltares {
    namespace Reliability {

        HohenbichlerZ::HohenbichlerZ(const double betaV, const double pfU, const double r) :
            beta2(betaV), pf1(pfU), rho(r), sqrt_one_minus_rho2(sqrt(1.0 - r * r)) {}

        //> ZHohenbichler: Z-function for Hohenbichler with input parameters x and output parameter z
        void HohenbichlerZ::zfunc(std::shared_ptr<Deltares::Models::ModelSample> s) const
        {
            //
            // u     : u-value (standard normal distributed)
            // w     : w-value (standard normal distributed)
            // beta2 : Reliability index for stochastic variable 2 (smallest reliability index)
            // pf1   : Probability of failure for stochastic variable 1 (smallest probability of failure)
            // rho   : Correlation coefficient between Z_1 and Z_2
            //
            const double u = s->Values[0];
            const double w = s->Values[1];
            double phiU = StandardNormal::getPFromU(u);
            double uAccent = StandardNormal::getUFromQ(pf1 * phiU);
            auto z = beta2 - rho * uAccent - sqrt_one_minus_rho2 * w;
            s->Z = z;
        }
    }
}
