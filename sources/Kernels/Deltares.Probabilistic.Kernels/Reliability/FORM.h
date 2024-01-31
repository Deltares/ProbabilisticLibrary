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
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<Sample> startSample, double relaxationFactor);
			void repairResults(std::vector<double> dzdu);
			bool areResultsValid(std::vector<double> dzdu);
			bool checkConvergence(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<Sample> u, std::shared_ptr<ConvergenceReport> convergenceReport, double beta, double zGradientLength);
		public:
			std::shared_ptr<FORMSettings> Settings = std::make_shared<FORMSettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner) override;
		};
	}
}

