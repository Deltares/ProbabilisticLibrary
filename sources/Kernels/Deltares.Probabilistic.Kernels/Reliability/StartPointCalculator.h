#pragma once
#include "StartPointCalculatorSettings.h"
#include "../Model/Sample.h"
#include "../Model/ZModelRunner.h"

namespace Deltares
{
	namespace Reliability {

		class StartPointCalculator
		{
		public:
			StartPointCalculatorSettings* Settings = new StartPointCalculatorSettings();
			Sample* getStartPoint(Models::ZModelRunner* modelRunner);
		};
	}
}

