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

                bool IsValid() override { return Settings->IsValid(); }

                void Stop()	override { shared->object->Stop(); }

                bool IsStopped() override { return shared->object->isStopped(); }
            };
        }
    }
}




