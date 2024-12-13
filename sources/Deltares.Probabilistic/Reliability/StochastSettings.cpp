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
#include "StochastSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        bool StochastSettings::isMinMaxDefault()
        {
            return this->MinValue <= -Statistics::StandardNormal::UMax && this->MaxValue >= Statistics::StandardNormal::UMax;
        }

        void StochastSettings::initializeForRun()
        {
            this->XMinValue = Statistics::StandardNormal::getPFromU(this->MinValue);
            this->XMaxValue = Statistics::StandardNormal::getPFromU(this->MaxValue);
        }

        double StochastSettings::getRepresentativeU(double u)
        {
            return this->stochast->getRepresentativeU(u);
        }

        std::shared_ptr<StochastSettings> StochastSettings::clone()
        {
            std::shared_ptr<StochastSettings> clone = std::make_shared<StochastSettings>();

            clone->IsInitializationAllowed = this->IsInitializationAllowed;
            clone->IsQualitative = this->IsQualitative;
            clone->IsVarianceAllowed = this->IsVarianceAllowed;
            clone->Intervals = this->Intervals;
            clone->MaxValue = this->MaxValue;
            clone->MinValue = this->MinValue;
            clone->XMaxValue = this->XMaxValue;
            clone->XMinValue = this->XMinValue;
            clone->StartValue = this->StartValue;
            clone->UncorrelatedStartValue = this->UncorrelatedStartValue;
            clone->VarianceFactor = this->VarianceFactor;
            clone->StochastIndex = this->StochastIndex;
            clone->stochast = this->stochast;

            return clone;

        }

    }
}

