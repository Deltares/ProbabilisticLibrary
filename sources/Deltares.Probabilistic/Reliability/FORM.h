#pragma once

#include "FORMSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		class FORM : public ReliabilityMethod
		{
		public:
			std::shared_ptr<FORMSettings> Settings = std::make_shared<FORMSettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
		private:
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> startSample,
				const double relaxationFactor, const int relaxationIndex);
			bool areAllResultsValid(std::vector<double> values);
			bool isConverged(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> sample, std::shared_ptr<ConvergenceReport> convergenceReport, double beta, double zGradientLength);
			std::shared_ptr<ReliabilityReport> getReport(int iteration, double reliability);
			std::pair<double, std::shared_ptr<Sample>> estimateBetaNonConv(const std::vector<double>& allBetas, const std::vector< std::shared_ptr<Sample>>& last10u);
			const int histU = 10; // keep last 10 u values for history / statistics
		};
	}
}

