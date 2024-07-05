#pragma once
#include "DesignPoint.h"
#include "../Model/ModelRunner.h"
#include "../../Deltares.Probabilistic/Reliability/ReliabilityMethod.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class ReliabilityMethod
            {
            public:
                ReliabilityMethod() {  }

                virtual std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod()
                {
                    return nullptr;
                };

                DesignPoint^ GetDesignPoint(Wrappers::ModelRunner^ modelRunner);

                virtual System::Object^ GetSettings() { return nullptr; }
                virtual bool IsValid() { return false; }
                virtual void Stop()    { }
                virtual bool IsStopped() { return false; }
            };
        }
    }
}

