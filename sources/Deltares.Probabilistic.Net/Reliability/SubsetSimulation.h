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

#include "SubsetSimulationSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/SubsetSimulation.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class SubsetSimulation : public ReliabilityMethod
            {
            private:
                SharedPointerProvider<Reliability::SubsetSimulation>* shared = new SharedPointerProvider(new Reliability::SubsetSimulation());
            public:
                SubsetSimulation() {}
                ~SubsetSimulation() { this->!SubsetSimulation(); }
                !SubsetSimulation() { delete shared; }

                std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod() override
                {
                    shared->object->Settings = Settings->GetSettings();
                    return shared->object;
                }

                SubsetSimulationSettings^ Settings = gcnew SubsetSimulationSettings();

                System::Object^ GetSettings() override { return Settings; }

                void Stop()    override { shared->object->Stop(); }

                bool IsStopped() override { return shared->object->isStopped(); }
            };
        }
    }
}




