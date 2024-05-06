#pragma once
#include "NumericalIntegrationSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		class NumericalIntegration : public ReliabilityMethod
		{
		public:
			std::shared_ptr<NumericalIntegrationSettings> Settings = std::make_shared<NumericalIntegrationSettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
		private:
			double getStochastProbability(std::shared_ptr<ModelRunner> modelRunner, int stochastIndex, std::shared_ptr<Sample> parentSample, std::shared_ptr<DesignPointBuilder> designPointBuilder, double density, double z0Fac, double& totalDensity);
		};
	}
}



