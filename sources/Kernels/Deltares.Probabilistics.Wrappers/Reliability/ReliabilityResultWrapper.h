#pragma once

#include "../../Deltares.Probabilistic.Kernels/Model/ReliabilityResult.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class ReliabilityResultWrapper
			{
			public:
				ReliabilityResultWrapper() {}
				ReliabilityResultWrapper(std::shared_ptr<ReliabilityResult> result)
				{
					this->ProxyIteration = result->ProxyIteration;
					this->Run = result->Run;
					this->Index = result->Index;
					this->Reliability = result->Reliability;
					this->LowReliability = result->LowReliability;
					this->HighReliability = result->HighReliability;
					this->Samples = result->Samples;
					this->ConvBeta = result->ConvBeta;
					this->ProxyConvergence = result->ProxyConvergence;
					this->Variation = result->Variation;
					this->Difference = result->Difference;
					this->Contribution = result->Contribution;
				}

				property int ProxyIteration;
				property int Run;
				property int Index;
				property double Reliability;
				property double LowReliability;
				property double HighReliability;
				property double Samples;
				property double ConvBeta;
				property double ProxyConvergence;
				property double Variation;
				property double Difference;
				property double Contribution;
			};
		}
	}
}


