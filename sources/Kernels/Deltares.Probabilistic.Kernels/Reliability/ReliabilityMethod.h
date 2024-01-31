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

			std::vector<double> getAlphas(std::shared_ptr<Sample> sample, int nstochasts, double z0Fac);

			std::shared_ptr<DesignPoint> getDesignPointFromSample(std::shared_ptr<Sample> sample, std::shared_ptr<Models::ZModelRunner> modelRunner, double z0, std::shared_ptr<ConvergenceReport> convergenceReport = nullptr);
			std::shared_ptr<DesignPoint> getDesignPointFromSample(std::shared_ptr<Models::ZModelRunner> modelRunner, double pf, std::shared_ptr<Sample> u, double z0, std::shared_ptr<ConvergenceReport> convergenceReport = nullptr);
			std::shared_ptr<DesignPoint> getDesignPointFromSampleAndBeta(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<Sample> u, double beta, std::shared_ptr<ConvergenceReport> convergenceReport = nullptr);

		public:
			virtual std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner) { return nullptr; };
			virtual ~ReliabilityMethod() { ; }

			bool isStopped();
			void Stop();
		};
	}
}

