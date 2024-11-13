// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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
#pragma once

#include "FragilityCurveIntegration.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            DesignPoint^ FragilityCurveIntegration::GetDesignPoint(Stochast^ stochast, Stochast^ fragilityCurve, Stochast^ normalizingFragilityCurve)
            {
                std::shared_ptr<Statistics::Stochast> nativeStochast = stochast->GetStochast();
                std::shared_ptr<Statistics::Stochast> nativeFragilityCurve = fragilityCurve->GetStochast();
                std::shared_ptr<Statistics::Stochast> nativeNormalizingFragilityCurve = nullptr;
                if (normalizingFragilityCurve != nullptr)
                {
                    nativeNormalizingFragilityCurve = normalizingFragilityCurve->GetStochast();
                }

                const std::shared_ptr<Reliability::DesignPoint> nativeDesignPoint = shared->object->getDesignPoint(nativeStochast, nativeFragilityCurve, nativeNormalizingFragilityCurve);

                System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts = gcnew System::Collections::Generic::List<Statistics::Wrappers::Stochast^>();
                stochasts->Add(stochast);
                stochasts->Add(fragilityCurve);

                Wrappers::DesignPoint^ designPoint = gcnew Wrappers::DesignPoint(nativeDesignPoint, stochasts);

                return designPoint;
            };
        }
    }
}

