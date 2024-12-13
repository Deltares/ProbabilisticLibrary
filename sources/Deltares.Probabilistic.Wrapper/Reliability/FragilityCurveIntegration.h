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

#include "FragilityCurveIntegrationSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/FragilityCurveIntegration.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class FragilityCurveIntegration : public ReliabilityMethod
            {
            private:
                SharedPointerProvider<Reliability::FragilityCurveIntegration>* shared = new SharedPointerProvider(new Reliability::FragilityCurveIntegration());
                System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ GetReferencedStochasts(Statistics::Wrappers::Stochast^ stochast);
            public:
                FragilityCurveIntegration() {}
                ~FragilityCurveIntegration() { this->!FragilityCurveIntegration(); }
                !FragilityCurveIntegration() { delete shared; }

                std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
                {
                    shared->object->Settings = Settings->GetSettings();
                    return shared->object;
                }

                FragilityCurveIntegrationSettings^ Settings = gcnew FragilityCurveIntegrationSettings();

                DesignPoint^ GetDesignPoint(Stochast^ stochast, Stochast^ fragilityCurve, Stochast^ normalizingFragilityCurve);

                System::Object^ GetSettings() override { return Settings; }

                bool IsValid() override { return Settings->IsValid(); }

                void Stop()    override { shared->object->Stop(); }

                bool IsStopped() override { return shared->object->isStopped(); }
            };
        }
    }
}



