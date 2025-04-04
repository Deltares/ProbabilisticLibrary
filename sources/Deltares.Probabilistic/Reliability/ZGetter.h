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

#pragma once

#include "../Model/ModelRunner.h"
#include "DirectionReliabilitySettings.h"

namespace Deltares::Reliability
{
    class ZGetter
    {
    public:
        ZGetter(const std::shared_ptr<Models::ModelRunner>& modelRunner, const std::shared_ptr<DirectionReliabilitySettings>& settings = nullptr) :
            modelRunner(modelRunner), settings(settings) {}

        std::shared_ptr<Sample> GetU(const std::shared_ptr<Sample>& uDirection, double factor, bool allowProxy = true) const
        {
            std::shared_ptr<Sample> u = uDirection->getMultipliedSample(factor);
            u->AllowProxy = allowProxy;

            if (settings != nullptr && settings->UseInitialValues)
            {
                for (size_t i = 0; i < u->Values.size(); i++)
                {
                    if (!settings->StochastSet->VaryingStochastSettings[i]->IsInitializationAllowed)
                    {
                        u->Values[i] = settings->StochastSet->VaryingStochastSettings[i]->StartValue;
                    }
                }
            }

            return u;
        }

        double GetZ(const std::shared_ptr<Sample>& uDirection, double factor, bool inverted, bool allowProxy = true) const
        {
            const auto u = GetU(uDirection, factor, allowProxy);

            return GetZValueCorrected(u, inverted);
        }

    private:
        std::shared_ptr<Models::ModelRunner> modelRunner;
        std::shared_ptr<DirectionReliabilitySettings> settings;

        double GetZValueCorrected(const std::shared_ptr<Sample>& u, bool invertZ) const
        {
            double z = modelRunner->getZValue(u);
            if (invertZ)
            {
                z = -z;
            }
            return z;
        }
    };

}
