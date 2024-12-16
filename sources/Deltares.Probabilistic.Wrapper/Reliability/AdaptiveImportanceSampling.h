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
#include "ImportanceSamplingSettings.h"
#include "AdaptiveImportanceSamplingSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/AdaptiveImportanceSampling.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class AdaptiveImportanceSampling : public ReliabilityMethod
            {
            private:
                SharedPointerProvider<Reliability::AdaptiveImportanceSampling>* shared = new SharedPointerProvider(new Reliability::AdaptiveImportanceSampling());
            public:
                AdaptiveImportanceSampling() { }
                ~AdaptiveImportanceSampling() { this->!AdaptiveImportanceSampling(); }
                !AdaptiveImportanceSampling() { delete shared; }

                AdaptiveImportanceSamplingSettings^ Settings = gcnew AdaptiveImportanceSamplingSettings();

                System::Object^ GetSettings() override { return Settings; }

                bool IsValid() override { return Settings->IsValid(); }

                std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
                {
                    shared->object->Settings = Settings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}



