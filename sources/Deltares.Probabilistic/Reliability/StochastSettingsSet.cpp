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
#include "StochastSettingsSet.h"
#include "StochastSettings.h"

#include <memory>

namespace Deltares
{
    namespace Reliability
    {
        void StochastSettingsSet::loadStochastPoint(std::shared_ptr<StochastPoint> stochastPoint)
        {
            this->stochastSettings.clear();

            for (size_t i = 0; i < stochastPoint->Alphas.size(); i++)
            {
                std::shared_ptr<Deltares::Reliability::StochastSettings> settings = std::make_shared<Deltares::Reliability::StochastSettings>();
                settings->StartValue = stochastPoint->Alphas[i]->U;
                settings->stochast = stochastPoint->Alphas[i]->Stochast;

                this->stochastSettings.push_back(settings);
            }

            this->AreStartValuesCorrelated = false;
        }

        std::shared_ptr<Sample> StochastSettingsSet::getStartPoint()
        {
            std::shared_ptr<Sample> sample = std::make_shared<Sample>(this->getVaryingStochastCount());

            for (int i = 0; i < this->getVaryingStochastCount(); i++)
            {
                sample->Values[i] = this->VaryingStochastSettings[i]->UncorrelatedStartValue;
            }

            return sample;
        }

        void StochastSettingsSet::setStartPoint(const std::shared_ptr<Sample> startPoint)
        {
            for (int i = 0; i < this->getVaryingStochastCount(); i++)
            {
                this->VaryingStochastSettings[i]->StartValue = startPoint->Values[i];
                this->VaryingStochastSettings[i]->UncorrelatedStartValue = startPoint->Values[i];

                this->AreStartValuesCorrelated = false;
            }
        }
    }
}




