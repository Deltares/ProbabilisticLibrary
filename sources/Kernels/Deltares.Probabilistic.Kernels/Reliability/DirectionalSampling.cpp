#include "DirectionalSampling.h"
#include "DirectionReliability.h"
#include "../Model/RandomSampleGenerator.h"
#include "../Math/SpecialFunctions.h"
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

			std::vector<double> betaValues;
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

			double sumPfSamp = 0.0; double sumPfSamp2 = 0.0;
			int validSamples = 0;
			// loop for number of samples
			for (int nmaal = 0; nmaal < Settings->MaximumSamples && !isStopped(); nmaal++)
			{
				if (nmaal % chunkSize == 0)
				{
					clearSamples(samples);

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
						return getDesignPointFromSample(modelRunner, pf, uMin, z0Fac, nullptr);
					}

					parSamples = parSamples + 1;
				}

				if (betaValues[nmaal % chunkSize] * z0 < 0 || std::isnan(betaValues[nmaal % chunkSize]))
				{
					continue;
				}

				Sample* u = samples[nmaal % chunkSize];

				double betaDirection = abs(betaValues[nmaal % chunkSize]);

				// get the sample at the limit state
				Sample* uSurface = u->normalize(betaDirection);

				// calculate failure probability
				if (betaDirection >= 0 && betaDirection < Statistics::StandardNormal::BetaMax)
				{
					uSurface->Weight = Deltares::Numeric::SpecialFunctions::getGammaUpperRegularized(0.5 * nstochasts, 0.5 * betaDirection * betaDirection);

					qtot += uSurface->Weight;

					uMean->addSample(uSurface);
				}
				else
				{
					uSurface->Weight = 0;
				}

				validSamples++;

				pf = qtot / (double)validSamples;

				// store minimum value of r and alpha
				if (betaDirection < rmin)
				{
					rmin = betaDirection;
					uThreshold = rmin;

					uMin = uSurface;
				}

				// check on convergence criterium
				bool enoughSamples = nmaal >= Settings->MinimumSamples;
				convergenceReport->TotalDirections = nmaal+1;

				sumPfSamp += uSurface->Weight;
				sumPfSamp2 += pow(uSurface->Weight, 2);

				if (qtot > 0)
				{
					// determine accuracy

					double convergence = getConvergence(pf, sumPfSamp, sumPfSamp2, (double)validSamples);

					double beta = z0Fac * Statistics::StandardNormal::getUFromQ(pf);

					ReliabilityReport* report = new ReliabilityReport();
					report->ReportMatchesEvaluation = false;
					report->Contribution = uSurface->Weight;
					report->Reliability = beta;
					report->Variation = convergence;

					modelRunner->reportResult(report);
					delete report;

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
					delete report;
				}
				delete uSurface;
			}

			Sample* uDesign = uMean->getSample();

			//getRealization(beta, alpha, convergenceReport, uMin->ScenarioIndex);
			convergenceReport->Convergence = getConvergence(pf, sumPfSamp, sumPfSamp2, (double)validSamples);

			if (z0 < 0)
			{
				pf = 1 - pf;
			}

			DesignPoint* designPoint = getDesignPointFromSample(modelRunner, pf, uDesign, z0Fac, convergenceReport);

			delete uDesign;
			delete randomSampleGenerator;
			clearSamples(samples);

			return designPoint;
		}

		double DirectionalSampling::getConvergence(const double pf, const double sumPfSamp, const double sumPfSamp2, const double dTimes)
		{
			double covar = sumPfSamp2 - 2.0 * pf * sumPfSamp + dTimes * pow(pf, 2);
			covar = std::sqrt(covar / dTimes / (dTimes - 1.0));
			double covarpf = covar / pf;
			double covarnpf = covar / (1 - pf);

			double convergence = std::max(covarpf, covarnpf);

			return convergence;
		}

		std::vector<double> DirectionalSampling::getDirectionBetas(Models::ZModelRunner* modelRunner, std::vector<Sample*> samples, double z0, int step)
		{
			auto betaValues = std::vector<double>(samples.size());

			DirectionReliability* directionReliability = new DirectionReliability();
			directionReliability->Settings = this->Settings->DirectionSettings;

			#pragma omp parallel for
			for (int i = 0; i < samples.size(); i++)
			{
				samples[i]->IterationIndex = step + i;
				betaValues[i] = directionReliability->getBeta(modelRunner, samples[i], z0);
			}

			delete directionReliability;

			return betaValues;
		}
	}
}



