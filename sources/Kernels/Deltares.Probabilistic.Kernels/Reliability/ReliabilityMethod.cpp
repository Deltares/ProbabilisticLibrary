#include "ReliabilityMethod.h"
#include "../Statistics/StandardNormal.h"
#include "../Model/DesignPoint.h"
#include "../Model/ConvergenceReport.h"

#include <cmath>

namespace Deltares
{
	namespace Reliability
	{
		int ReliabilityMethod::getZFactor(double z)
		{
			return std::isnan(z) || z > 0.0 ? 1 : -1;
		}

		bool ReliabilityMethod::isStopped()
		{
			return this->stopped;
		}

		void ReliabilityMethod::Stop()
		{
			this->stopped = true;
		}

		std::shared_ptr<DesignPoint> ReliabilityMethod::getDesignPointFromSampleAndBeta(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<Sample> u, double beta, std::shared_ptr<ConvergenceReport> convergenceReport)
		{
			if (u != nullptr)
			{
				//double z0Fac = getZFactor(beta);
				//auto alphas = getAlphas(u, u->getSize(), z0Fac);

				return modelRunner->getDesignPoint(u, beta, convergenceReport);
			}
			else
			{
				return nullptr;
			}
		}

		std::shared_ptr<DesignPoint> ReliabilityMethod::getDesignPointFromSample(std::shared_ptr<Models::ZModelRunner> modelRunner, double pf, std::shared_ptr<Sample> u, double z0, std::shared_ptr<ConvergenceReport> convergenceReport)
		{
			return getDesignPointFromSampleAndBeta(modelRunner, u, Statistics::StandardNormal::getUFromQ(pf), convergenceReport);
		}

		std::shared_ptr<DesignPoint> ReliabilityMethod::getDesignPointFromSample(std::shared_ptr<Sample> sample, std::shared_ptr<Models::ZModelRunner> modelRunner, double z0, std::shared_ptr<ConvergenceReport> convergenceReport)
		{
			double beta = sample != nullptr ? sample->getBeta() : nan("");

			return getDesignPointFromSampleAndBeta(modelRunner, sample, beta, convergenceReport);
		}
	}
}


