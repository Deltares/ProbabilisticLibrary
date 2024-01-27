#include "StartPointCalculator.h"
#include "../Utils/probLibException.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
	namespace Reliability
	{
		Sample* StartPointCalculator::getStartPoint(Models::ZModelRunner* modelRunner)
		{
			switch (this->Settings->StartMethod)
			{
			case StartMethodType::None:
				return this->Settings->StochastSet->getSample();
			case StartMethodType::RaySearch:
				throw probLibException("Not implemented yet");
			case StartMethodType::SensitivitySearch:
				throw probLibException("Not implemented yet");
			case StartMethodType::SphereSearch:
				throw probLibException("Not implemented yet");
			default:
				throw probLibException("Not supported");
			}
		}
	}
}


