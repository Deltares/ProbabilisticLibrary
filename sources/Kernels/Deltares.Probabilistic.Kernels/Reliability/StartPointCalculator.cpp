#include "StartPointCalculator.h"

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
				throw std::exception("Not implemented yet");
			case StartMethodType::SensitivitySearch:
				throw std::exception("Not implemented yet");
			case StartMethodType::SphereSearch:
				throw std::exception("Not implemented yet");
			default:
				throw std::exception("Not supported");
			}
		}
	}
}


