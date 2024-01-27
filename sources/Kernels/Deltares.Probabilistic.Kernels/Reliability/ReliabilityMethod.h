#pragma once
#include "../Model/DesignPoint.h"
#include "../Model/ZModelRunner.h"
#include "../Model/ConvergenceReport.h"

namespace Deltares
{
	namespace Reliability
	{
		class ReliabilityMethod
		{
		private:
			bool stopped = false;

		protected:

			static int getZFactor(double z);

			std::vector<double> getAlphas(Sample* sample, int nstochasts, double z0Fac);
			void clearSamples(std::vector<Sample*>& samples);

			DesignPoint* getDesignPointFromSample(Sample* sample, Deltares::Models::ZModelRunner* modelRunner, double z0, ConvergenceReport* convergenceReport = nullptr);
			DesignPoint* getDesignPointFromSample(Deltares::Models::ZModelRunner* modelRunner, double pf, Sample* u, double z0, ConvergenceReport* convergenceReport = nullptr);
			DesignPoint* getDesignPointFromSampleAndBeta(Deltares::Models::ZModelRunner* modelRunner, Sample* u, double beta, ConvergenceReport* convergenceReport = nullptr);

		public:
			virtual DesignPoint* getDesignPoint(Deltares::Models::ZModelRunner* modelRunner) { return nullptr; };
			virtual ~ReliabilityMethod() { ; }

			bool isStopped();
			void Stop();
		};
	}
}

