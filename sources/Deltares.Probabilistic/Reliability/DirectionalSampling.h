#pragma once

#include "DirectionalSamplingSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		class DirectionalSampling : public ReliabilityMethod
		{
		private:
			double getConvergence(const double pf, const double sumPfSamp, const double sumPfSamp2, const double dTimes);
			std::vector<double> getDirectionBetas(std::shared_ptr<Models::ModelRunner> modelRunner, std::vector<std::shared_ptr<Sample>> samples, double z0, int step);
		public:
			std::shared_ptr<DirectionalSamplingSettings> Settings = std::make_shared<DirectionalSamplingSettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
		};
	}
}

