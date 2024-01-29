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
			DesignPoint* getReducedDesignPoint(Deltares::Models::ZModelRunner* modelRunner, double qFail, double qRange);
			bool checkConvergence(Deltares::Models::ZModelRunner* modelRunner, double pf, int samples, int nmaal);
			double getConvergence(double pf, int samples);
			void applyLimits(Sample* sample);
		public:
			Deltares::Reliability::CrudeMonteCarloSettings* Settings = new Deltares::Reliability::CrudeMonteCarloSettings();
			DesignPoint* getDesignPoint(Deltares::Models::ZModelRunner* modelRunner) override;
			~CrudeMonteCarlo()
			{
				//delete Settings;
			}
		};
	}
}

