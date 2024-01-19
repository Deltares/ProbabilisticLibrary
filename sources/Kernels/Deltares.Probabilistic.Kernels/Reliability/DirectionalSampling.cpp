#include "DirectionalSampling.h"
#include "DirectionReliability.h"
#include "../Model/RandomSampleGenerator.h"
#include "../Math/ASA/dgammaDs.h"
#include <omp.h>

namespace Deltares
{
	namespace Reliability
	{
		DesignPoint* Deltares::Reliability::DirectionalSampling::getDesignPoint(Deltares::Models::ZModelRunner* modelRunner)
		{
			int nstochasts = modelRunner->getVaryingStochastCount();
			double pf = 0;
			double qtot = 0;
			double rmin = 200; // initialise convergence indicator and loops
			double uThreshold = std::numeric_limits<double>::infinity();
			DesignPointBuilder* uMean = new DesignPointBuilder(nstochasts, this->Settings->DesignPointMethod, this->Settings->StochastSet);
			int parSamples = 0;

			omp_set_num_threads(modelRunner->Settings->MaxParallelProcesses);

			double* betaValues = new double[0];
			std::vector<double> weights;
			std::vector<Sample*> samples;

			modelRunner->updateStochastSettings(this->Settings->StochastSet);

			ConvergenceReport* convergenceReport = new ConvergenceReport();

			RandomSampleGenerator* randomSampleGenerator = new RandomSampleGenerator();
			randomSampleGenerator->Settings = this->Settings->RandomSettings;
			randomSampleGenerator->Settings->StochastSet = this->Settings->StochastSet;
			randomSampleGenerator->initialize();

			Sample* zeroSample = new Sample(nstochasts);
			double z0 = modelRunner->getZValue(zeroSample);
			double z0Fac = this->getZFactor(z0);

			delete zeroSample;

			Sample* uMin = new Sample(nstochasts);
			int chunkSize = modelRunner->Settings->MaxChunkSize;

			// loop for number of samples
			for (int nmaal = 0; nmaal < Settings->MaximumSamples && !isStopped(); nmaal++)
			{
				if (nmaal % chunkSize == 0)
				{
					samples.clear();

					int runs = std::min(chunkSize, Settings->MaximumSamples - parSamples * chunkSize);

					// run max par samples times zrfunc in parallel
					for (int i = 0; i < runs; i++)
					{
						samples.push_back(randomSampleGenerator->getRandomSample());
					}

					betaValues = getDirectionBetas(modelRunner, samples, z0Fac, nmaal);

					// check whether restart is needed
					if (modelRunner->shouldExitPrematurely(false))
					{
						// return the result so far
						return GetRealizationFromP(modelRunner, pf, uMin, z0Fac, nullptr);
					}

					parSamples = parSamples + 1;
				}

				double betaDirection = betaValues[nmaal % chunkSize];
				if (betaDirection < 0 || std::isnan(betaDirection))
				{
					continue;
				}

				Sample* u = samples[nmaal % chunkSize];

				// get the sample at the limit state
				Sample* uSurface = u->normalize(betaDirection);

				// calculate failure probability
				if (betaDirection >= 0 && betaDirection < StandardNormal::BetaMax * u->getSize())
				{
					const double p = 0.5 * nstochasts;
					auto gmmd = ProbLibCore::dgammaDs(p);
					uSurface->Weight = gmmd.Dgammq(0.5 * nstochasts, 0.5 * betaDirection * betaDirection);

					qtot += uSurface->Weight;

					uMean->addSample(uSurface);
				}
				else
				{
					uSurface->Weight = 0;
				}

				weights.push_back(uSurface->Weight);

				pf = qtot / weights.size();

				// min waarde r en alfa vastleggen
				if (betaDirection < rmin)
				{
					rmin = betaDirection;
					uThreshold = rmin;

					uMin = uSurface;
				}

				// controleren of afbreekcriterium is bereikt
				bool enoughSamples = nmaal >= Settings->MinimumSamples;

				if (qtot > 0)
				{
					// nauwkeurigheid bepalen

					double convergence = getConvergence(pf, weights);

					double beta = z0Fac * StandardNormal::getUFromQ(pf);

					ReliabilityReport* report = new ReliabilityReport();
					report->ReportMatchesEvaluation = false;
					report->Contribution = uSurface->Weight;
					report->Reliability = beta;
					report->Variation = convergence;

					modelRunner->reportResult(report);

					if (enoughSamples && convergence <= Settings->VariationCoefficient)
					{
						break;
					}
				}
				else
				{
					ReliabilityReport* report = new ReliabilityReport();
					report->ReportMatchesEvaluation = false;
					report->Variation = uSurface->Weight;

					modelRunner->reportResult(report);
				}
			}

			// beta bij kans bepalen
			if (z0 < 0)
			{
				pf = 1 - pf;
			}

			Sample* uDesign = uMean->getSample();

			//getRealization(beta, alpha, convergenceReport, uMin->ScenarioIndex);
			convergenceReport->Convergence = getConvergence(pf, weights);

			return GetRealizationFromP(modelRunner, pf, uDesign, z0Fac, convergenceReport);
		}

		double DirectionalSampling::getConvergence(double pf, std::vector<double> weights)
		{
			double covar = 0;
			for (int k = 0; k < weights.size(); k++)
			{
				covar += (weights[k] - pf) * (weights[k] - pf);
			}
			covar = std::sqrt(covar / weights.size() / (weights.size() - 1));
			double covarpf = covar / pf;
			double covarnpf = covar / (1 - pf);

			double convergence = std::max(covarpf, covarnpf);

			return convergence;
		}

		double* DirectionalSampling::getDirectionBetas(Models::ZModelRunner* modelRunner, std::vector<Sample*> samples, double z0, int step)
		{
			double* betaValues = new double[samples.size()];

			DirectionReliability* directionReliability = new DirectionReliability();
			directionReliability->Settings = this->Settings->DirectionSettings;

			#pragma omp parallel for
			for (int i = 0; i < samples.size(); i++)
			{
				samples[i]->IterationIndex = step + i;
				betaValues[i] = directionReliability->getBeta(modelRunner, samples[i], z0);
			}

			return betaValues;
		}
	}
}



