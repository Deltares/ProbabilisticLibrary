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

#include "ZGetter.h"

namespace Deltares::Reliability
{
    using namespace Deltares::Models;

    std::shared_ptr<Sample> ZGetter::GetSample(Sample& uDirection, double factor, bool allowProxy) const
    {
        std::shared_ptr<Sample> u = uDirection.getMultipliedSample(factor);
        u->AllowProxy = allowProxy;

        if (settings.UseInitialValues)
        {
            for (size_t i = 0; i < u->Values.size(); i++)
            {
                if (!settings.StochastSet.VaryingStochastSettings[i]->IsInitializationAllowed)
                {
                    u->Values[i] = settings.StochastSet.VaryingStochastSettings[i]->StartValue;
                }
            }
        }

        return u;
    }

    double ZGetter::GetZ(Sample& uDirection, double factor, bool inverted, bool allowProxy) const
    {
        const auto u = GetSample(uDirection, factor, allowProxy);

        return GetZValueCorrected(u, inverted);
    }

    double ZGetter::GetZValueCorrected(const std::shared_ptr<Sample>& u, bool invertZ) const
    {
        double z = modelRunner.getZValue(u);
        if (invertZ)
        {
            z = -z;
        }
        return z;
    }

}
