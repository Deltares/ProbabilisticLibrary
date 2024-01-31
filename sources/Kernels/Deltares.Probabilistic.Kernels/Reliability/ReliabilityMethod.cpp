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

		std::vector<double> ReliabilityMethod::getAlphas(std::shared_ptr<Sample> sample, int nstochasts, double z0Fac)
		{
			auto alpha = std::vector<double>(nstochasts);

			if (sample == nullptr)
			{
				// alpha values, if no failure was found
				double defaultAlpha = -sqrt(1.0 / nstochasts);

				for (int k = 0; k < nstochasts; k++)
				{
					alpha[k] = defaultAlpha;
				}
			}
			else
			{
				double beta = sample->getBeta();

				if (beta == 0)
				{
					const double defaultAlpha = -sqrt(1.0 / nstochasts);
					for (int k = 0; k < nstochasts; k++)
					{
						alpha[k] = defaultAlpha;
					}
				}
				else
				{
					for (int k = 0; k < sample->getSize(); k++)
					{
						alpha[k] = -sample->Values[k] / beta * z0Fac;
					}
				}
			}

			return alpha;
		}

		std::shared_ptr<DesignPoint> ReliabilityMethod::getDesignPointFromSampleAndBeta(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<Sample> u, double beta, std::shared_ptr<ConvergenceReport> convergenceReport)
		{
			if (u != nullptr)
			{
				double z0Fac = getZFactor(beta);

				auto alphas = getAlphas(u, u->getSize(), z0Fac);

				return modelRunner->getDesignPoint(beta, alphas, convergenceReport, u->ScenarioIndex);
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


