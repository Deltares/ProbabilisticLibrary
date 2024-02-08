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
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> startSample, double relaxationFactor);
			bool areAllResultsValid(std::vector<double> values);
			bool isConverged(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> sample, std::shared_ptr<ConvergenceReport> convergenceReport, double beta, double zGradientLength);
			std::shared_ptr<ReliabilityReport> getReport(int iteration, double reliability);
		public:
			std::shared_ptr<FORMSettings> Settings = std::make_shared<FORMSettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
			void getReport(int iteration, std::shared_ptr<ReliabilityReport>& reportInvalid);
		};
	}
}

