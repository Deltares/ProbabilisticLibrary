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

		double* ReliabilityMethod::getAlphas(Sample* sample, int nstochasts, double z0Fac)
		{
			double* alpha = new double[nstochasts];

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
					double defaultAlpha = -sqrt(1.0 / nstochasts);
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

		DesignPoint* ReliabilityMethod::getDesignPointFromSample(Deltares::Models::ZModelRunner* modelRunner, double pf, Sample* u, double z0, ConvergenceReport* convergenceReport)
		{
			if (u != nullptr)
			{
				double z0Fac = getZFactor(z0);

				double beta = StandardNormal::getUFromQ(pf);
				double* alfa = getAlphas(u, u->getSize(), z0Fac);

				return modelRunner->getDesignPoint(beta, alfa, convergenceReport, u->ScenarioIndex);
			}
			else
			{
				return nullptr;
			}
		}
	}
}


