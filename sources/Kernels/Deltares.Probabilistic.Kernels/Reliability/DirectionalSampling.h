#pragma once

#include "DirectionalSamplingSettings.h"
#include "DirectionReliability.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		class DirectionalSampling : public ReliabilityMethod
		{
		private:
			double getConvergence(double pf, const std::vector<double> & weights);
			std::vector<double> getDirectionBetas(Models::ZModelRunner* modelRunner, std::vector<Sample*> samples, double z0, int step);
		public:
			Deltares::Reliability::DirectionalSamplingSettings* Settings = new Deltares::Reliability::DirectionalSamplingSettings();
			DesignPoint* getDesignPoint(Deltares::Models::ZModelRunner* modelRunner) override;
		};
	}
}

