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
			protected:
				SharedPointerProvider<Reliability::ReliabilityMethod>* shared = nullptr;

			public:
				ReliabilityMethod() {  }

				virtual std::shared_ptr<Reliability::ReliabilityMethod> GetReliabilityMethod()
				{
					return nullptr;
				};

				DesignPoint^ GetDesignPoint(Wrappers::ModelRunner^ modelRunner);

				void Stop()
				{
					if (shared != nullptr) 
					{
						shared->object->Stop();
					}
				}

				bool IsStopped()
				{
					if (shared != nullptr) 
					{
						return shared->object->isStopped();
					}
					else
					{
						return false;
					}
				}
			};
		}
	}
}

