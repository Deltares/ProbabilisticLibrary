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
		private:
			Sample* getRayStartPoint(Models::ZModelRunner* modelRunner);
			Sample* getSensitivityStartPoint(Models::ZModelRunner* modelRunner);
			Sample* getSphereStartPoint(Models::ZModelRunner* modelRunner);

			Sample* getDirectionStartPoint(Models::ZModelRunner* modelRunner, Sample* startPoint);
			void correctDefaultValues(Sample* startPoint);
			Sample* examineSurfaceForFailure(Models::ZModelRunner* modelRunner, int index, double radiusFactor, Sample* uRay, double& zMin);
		};
	}
}

