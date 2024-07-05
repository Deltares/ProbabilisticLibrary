#pragma once

#include "CrudeMonteCarloSettings.h"
#include "ReliabilityMethod.h"
#include "../../Deltares.Probabilistic/Reliability/CrudeMonteCarlo.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class CrudeMonteCarlo : public ReliabilityMethod
            {
            private:
                SharedPointerProvider<Reliability::CrudeMonteCarlo>* shared = new SharedPointerProvider(new Reliability::CrudeMonteCarlo());
            public:
                CrudeMonteCarlo() { }
                ~CrudeMonteCarlo() { this->!CrudeMonteCarlo(); }
                !CrudeMonteCarlo() { delete shared; }

                CrudeMonteCarloSettings^ Settings = gcnew CrudeMonteCarloSettings();

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



