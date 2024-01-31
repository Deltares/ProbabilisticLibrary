#pragma once
#include "CrudeMonteCarloSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		class CrudeMonteCarlo : public ReliabilityMethod
		{
		private:
			std::shared_ptr<DesignPoint> getReducedDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner, double qFail, double qRange);
			bool checkConvergence(std::shared_ptr<Models::ZModelRunner> modelRunner, double pf, int samples, int nmaal);
			double getConvergence(double pf, int samples);
			void applyLimits(std::shared_ptr<Sample> sample);
		public:
			std::shared_ptr<CrudeMonteCarloSettings> Settings = std::make_shared<CrudeMonteCarloSettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner) override;
		};
	}
}

