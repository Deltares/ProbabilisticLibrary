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
			return isnan(z) || z > 0.0 ? 1 : -1;
		}

		bool ReliabilityMethod::isStopped()
		{
			return this->stopped;
		}

		void ReliabilityMethod::Stop()
		{
			this->stopped = true;
		}

		void ReliabilityMethod::clearSamples(std::vector<Sample*>& samples)
		{
			for (size_t i = 0; i < samples.size(); i++)
			{
				delete(samples[i]);
			}
			samples.clear();
		}

		std::vector<double> ReliabilityMethod::getAlphas(Sample* sample, int nstochasts, double z0Fac)
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

		DesignPoint* ReliabilityMethod::getDesignPointFromSampleAndBeta(Deltares::Models::ZModelRunner* modelRunner, Sample* u, double beta, ConvergenceReport* convergenceReport)
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

		DesignPoint* ReliabilityMethod::getDesignPointFromSample(Deltares::Models::ZModelRunner* modelRunner, double pf, Sample* u, double z0, ConvergenceReport* convergenceReport)
		{
			return getDesignPointFromSampleAndBeta(modelRunner, u, Statistics::StandardNormal::getUFromQ(pf), convergenceReport);
		}

		DesignPoint* ReliabilityMethod::getDesignPointFromSample(Sample* sample, Deltares::Models::ZModelRunner* modelRunner, double z0, ConvergenceReport* convergenceReport)
		{
			double beta = sample != nullptr ? sample->getBeta() : nan("");

			return getDesignPointFromSampleAndBeta(modelRunner, sample, beta, convergenceReport);
		}
	}
}


