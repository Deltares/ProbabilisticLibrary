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
#pragma once

#include "FORMThenDirectionalSamplingSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/DirectionalSamplingThenFORM.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class DirectionalSamplingThenFORM : public ReliabilityMethod
            {
            private:
                SharedPointerProvider<Reliability::DirectionalSamplingThenFORM>* shared = new SharedPointerProvider(new Reliability::DirectionalSamplingThenFORM());
            public:
                DirectionalSamplingThenFORM() {}
                ~DirectionalSamplingThenFORM() { this->!DirectionalSamplingThenFORM(); }
                !DirectionalSamplingThenFORM() { delete shared; }

                std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
                {
                    shared->object->formSettings = Settings->GetFormSettings();
                    shared->object->DsSettings = Settings->GetDsSettings();
                    return shared->object;
                }

                FORMThenDirectionalSamplingSettings^ Settings = gcnew FORMThenDirectionalSamplingSettings();

                System::Object^ GetSettings() override { return Settings; }

                bool IsValid() override { return Settings->IsValid(); }

                void Stop()    override { shared->object->Stop(); }

                bool IsStopped() override { return shared->object->isStopped(); }
            };
        }
    }
}



