#pragma once

#include "FORMSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		class FORM : public ReliabilityMethod
		{
		private:
			DesignPoint* getDesignPoint(Models::ZModelRunner* modelRunner, Sample* startSample, double relaxationFactor);
			void repairResults(std::vector<double> dzdu);
			bool areResultsValid(std::vector<double> dzdu);
		public:
			FORMSettings* Settings = new FORMSettings();
			DesignPoint* getDesignPoint(Deltares::Models::ZModelRunner* modelRunner) override;
		};
	}
}

