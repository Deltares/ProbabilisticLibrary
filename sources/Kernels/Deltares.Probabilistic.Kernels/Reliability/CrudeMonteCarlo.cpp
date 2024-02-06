#include "CrudeMonteCarlo.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

#include "../Statistics/StandardNormal.h"
#include "../Model/Sample.h"
#include "../Model/RandomSampleGenerator.h"
#include "ConvergenceReport.h"
#include "ReliabilityReport.h"
#include "DesignPoint.h"
#include "CrudeMonteCarloSettings.h"
#include "DesignPointBuilder.h"
#include "StochastSettings.h"

using namespace Deltares::Models;

namespace Deltares
{
	namespace Reliability
	{
		std::shared_ptr<DesignPoint> CrudeMonteCarlo::getDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner)
		{
			modelRunner->updateStochastSettings(this->Settings->StochastSet);

			double qRange = 1;
			double zRemainder = 1;

			std::shared_ptr<Sample> remainderSample = std::make_shared<Sample>(this->Settings->StochastSet->getVaryingStochastCount());

			for (int i = 0; i < this->Settings->StochastSet->getVaryingStochastCount(); i++)
			{
				if (!this->Settings->StochastSet->VaryingStochastSettings[i]->isMinMaxDefault())
				{
					double probLow = Statistics::StandardNormal::getPFromU(this->Settings->StochastSet->VaryingStochastSettings[i]->MinValue);
					double probHigh = Statistics::StandardNormal::getQFromU(this->Settings->StochastSet->VaryingStochastSettings[i]->MaxValue);

					double prob = 1 - probLow - probHigh;

					qRange *= prob;

					if (probLow > probHigh)
					{
						remainderSample->Values[i] = this->Settings->StochastSet->VaryingStochastSettings[i]->MinValue - 0.1;
					}
					else
					{
						remainderSample->Values[i] = this->Settings->StochastSet->VaryingStochastSettings[i]->MaxValue + 0.1;
					}
				}
			}

			if (qRange < 1)
			{
				// perform one run to identify whether the remainder is failing
				zRemainder = modelRunner->getZValue(remainderSample);
			}

			return getReducedDesignPoint(modelRunner, zRemainder, qRange);
		}

		std::shared_ptr<DesignPoint> CrudeMonteCarlo::getReducedDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner, double zRemainder, double qRange)
		{
			int nParameters = modelRunner->getVaryingStochastCount();
			double* zValues = new double[0]; // copy of z for all parallel threads as double
			DesignPointBuilder* uMean = new DesignPointBuilder(nParameters, Settings->designPointMethod, this->Settings->StochastSet);

			RandomSampleGenerator* randomSampleGenerator = new RandomSampleGenerator();
			randomSampleGenerator->Settings = this->Settings->randomSettings;
			randomSampleGenerator->Settings->StochastSet = this->Settings->StochastSet;
			randomSampleGenerator->initialize();

			std::shared_ptr<Sample> uMin = std::make_shared<Sample>(nParameters);
			double rmin = std::numeric_limits<double>::infinity();
			double pf = 0.0;
			bool initial = true;
			double z0Fac = 0;
			int nFailed = 0;
			int nSamples = 0;
			std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();
			std::vector<std::shared_ptr<Sample>> samples;
			int zIndex = 0;

			double qFail = 0;

			for (int sampleIndex = 0; sampleIndex < Settings->MaximumSamples + 1 && !isStopped(); sampleIndex++)
			{
				zIndex++;

				if (initial || zIndex >= samples.size())
				{
					samples.clear();

					int chunkSize = modelRunner->Settings->MaxChunkSize;
					int runs = std::min(chunkSize, Settings->MaximumSamples + 1 - sampleIndex);

					if (initial)
					{
						samples.push_back(std::make_shared<Sample>(nParameters));
						runs = runs - 1;
					}

					for (int i = 0; i < runs; i++)
					{
						std::shared_ptr<Sample> sample = randomSampleGenerator->getRandomSample();
						if (qRange < 1)
						{
							applyLimits(sample);
						}

						samples.push_back(sample);
					}

					delete[] zValues;
					zValues = modelRunner->getZValues(samples);

					if (initial)
					{
						z0Fac = getZFactor(zValues[0]);
						uMin->setBeta(z0Fac * Statistics::StandardNormal::BetaMax);
						uMean->initialize(z0Fac * Statistics::StandardNormal::BetaMax);
					}

					if (modelRunner->shouldExitPrematurely(zValues, z0Fac, samples, rmin))
					{
						// return the result so far
						return modelRunner->getDesignPoint(uMin, Statistics::StandardNormal::getUFromQ(pf), convergenceReport);
					}

					zIndex = 0;
				}

				if (initial)
				{
					z0Fac = getZFactor(zValues[zIndex]);
					double zRemainderFactor = getZFactor(zRemainder);

					if (z0Fac != zRemainderFactor)
					{
						qFail = 1 - qRange;
					}
					else
					{
						qFail = 0;
					}

					initial = false;
					continue;
				}

				double z = zValues[zIndex];
				std::shared_ptr<Sample> u = samples[zIndex];

				if (std::isnan(z))
				{
					continue;
				}

				nSamples++;
				if (z < 0)
				{
					nFailed = nFailed + 1;
				}

				convergenceReport->FailedSamples = nFailed;
				convergenceReport->FailFraction = NumericSupport::Divide(nFailed, nSamples);

				if (z * z0Fac < 0)
				{
					uMean->addSample(u);
					double rbeta = u->getBeta();
					if (rbeta < rmin)
					{
						rmin = rbeta;
						uMin = u;
					}
				}
				pf = NumericSupport::Divide(nFailed, nSamples);
				pf = qFail + qRange * pf;

				convergenceReport->IsConverged = checkConvergence(modelRunner, pf, nSamples, sampleIndex);

				if (convergenceReport->IsConverged)
				{
					break;
				}
			}

			double beta = Statistics::StandardNormal::getUFromQ(pf);
			uMin = uMean->getSample();

			convergenceReport->Convergence = getConvergence(pf, nSamples);

			std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(uMin, beta, convergenceReport);

			samples.clear();

			return designPoint;
		}

		void CrudeMonteCarlo::applyLimits(std::shared_ptr<Sample> sample)
		{
			for (int i = 0; i < sample->getSize(); i++)
			{
				std::shared_ptr<StochastSettings> settings = this->Settings->StochastSet->VaryingStochastSettings[i];
				if (!settings->isMinMaxDefault())
				{
					double q = Statistics::StandardNormal::getPFromU(sample->Values[i]);
					q = settings->XMinValue + q * (settings->XMaxValue - settings->XMinValue);
					sample->Values[i] = Statistics::StandardNormal::getUFromP(q);
				}
			}
		}

		bool CrudeMonteCarlo::checkConvergence(std::shared_ptr<Models::ZModelRunner> modelRunner, double pf, int samples, int nmaal)
		{
			std::shared_ptr<ReliabilityReport> report(new ReliabilityReport());
			report->Step = nmaal;
			report->MaxSteps = Settings->MaximumSamples;

			if (pf > 0 && pf < 1)
			{
				double convergence = getConvergence(pf, samples);
				report->Reliability = Statistics::StandardNormal::getUFromQ(pf);
				report->Variation = convergence;
				modelRunner->reportResult(report);
				bool enoughSamples = nmaal >= Settings->MinimumSamples;
				return enoughSamples && convergence < Settings->VariationCoefficient;
			}
			else
			{
				modelRunner->reportResult(report);
				return false;
			}
		}

		double CrudeMonteCarlo::getConvergence(double pf, int samples)
		{
			if (pf > 0 && pf < 1)
			{
				if (pf > 0.5)
				{
					pf = 1 - pf;
				}
				double varPf = sqrt((1 - pf) / (samples * pf));
				return varPf;
			}
			else
			{
				return nan("");
			}
		}
	}
}



