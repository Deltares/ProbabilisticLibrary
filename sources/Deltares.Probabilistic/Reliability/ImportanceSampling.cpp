#include "ImportanceSampling.h"
#include <vector>
#include <cmath>
#if __has_include(<format>)
#include <format>
#else
#include "../Utils/probLibString.h"
#endif
#include <memory>
#include <numbers>

#include "../Statistics/StandardNormal.h"
#include "../Model/Sample.h"
#include "../Model/RandomSampleGenerator.h"
#include "ConvergenceReport.h"
#include "ReliabilityReport.h"
#include "DesignPoint.h"
#include "ImportanceSamplingSettings.h"
#include "ImportanceSamplingCluster.h"
#include "DesignPointBuilder.h"
#include "StartPointCalculator.h"
#include "StochastSettings.h"

using namespace Deltares::Models;

namespace Deltares
{
	namespace Reliability
	{
		void ImportanceSampling::setSampleLambda(RegisterSampleLambda newSampleFunction)
		{
			this->sampleFunction = newSampleFunction;
		}

		void ImportanceSampling::setBreakLoopLambda(BreakLoopLambda newBreakFunction)
		{
			this->breakFunction = newBreakFunction;
		}

		std::shared_ptr<DesignPoint> ImportanceSampling::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
		{
			modelRunner->updateStochastSettings(this->Settings->StochastSet);

            std::shared_ptr<SampleProvider> sampleProvider = std::make_shared<SampleProvider>(this->Settings->StochastSet, true);
            modelRunner->setSampleProvider(sampleProvider);

			std::shared_ptr<RandomSampleGenerator> sampleCreator = std::make_shared<RandomSampleGenerator>();
			sampleCreator->Settings = this->Settings->randomSettings;
			sampleCreator->Settings->StochastSet = this->Settings->StochastSet;
            sampleCreator->sampleProvider = sampleProvider;
			sampleCreator->initialize();

            int nStochasts = modelRunner->getVaryingStochastCount();

            double z0Fac = 0; //< +1 or -1
            auto z0Ignore = false;
            bool initial = true;
			bool reported = false;

			std::vector<std::shared_ptr<Sample>> samples; // copy of u for all parallel threads as double
			std::vector<double> zValues; // copy of z for all parallel threads as double

			// list of all clusters
			std::shared_ptr<DesignPoint> startDesignPoint = nullptr;
			std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusterResults = getClusters(modelRunner, startDesignPoint);

			// holds the clusters corresponding to the samples
			int clusterIndex = -1;
			std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusters;

			std::shared_ptr<ImportanceSamplingCluster> combinedCluster = std::make_shared<ImportanceSamplingCluster>();
			combinedCluster->Center = this->Settings->StochastSet->getStartPoint();

			std::vector<double> factors = this->getFactors(Settings->StochastSet);

			double dimensionality = getDimensionality(factors);

			std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

			auto zIndex = 0;

			//auto smallEnough = false;
			auto breakLoop = false;

			// loop over the samples
			for (auto sampleIndex = 0; sampleIndex < Settings->MaximumSamples + 1 && !isStopped() && !breakLoop; sampleIndex++)
			{
				breakLoop = false;

				zIndex++;

				if (initial || zIndex >= zValues.size())
				{
					samples.clear();
					clusters.clear();

					int chunkSize = modelRunner->Settings->MaxChunkSize;

					int runs = std::min(chunkSize, Settings->MaximumSamples + 1 - sampleIndex);

                    sampleProvider->reset();

					if (initial)
					{
						auto initialSample = sampleProvider->getSample();
						samples.push_back(initialSample);
						clusters.push_back(nullptr);
						runs = runs - 1;
					}

					// prepare realizations
					for (int i = 0; i < runs; i++)
					{
						// get values for stochastic parameters
						clusterIndex++;
						if (clusterIndex >= clusterResults.size())
						{
							clusterIndex = 0;
						}

						std::shared_ptr<ImportanceSamplingCluster> cluster = clusterResults[clusterIndex];

						const std::shared_ptr<Sample> sample = sampleCreator->getRandomSample();

						std::shared_ptr<Sample> modifiedSample = sample->clone();

						for (int k = 0; k < nStochasts; k++)
						{
							modifiedSample->Values[k] = factors[k] * sample->Values[k] + cluster->Center->Values[k];
						}

						modifiedSample->Weight = getWeight(modifiedSample, sample, dimensionality);

						samples.push_back(modifiedSample);
						clusters.push_back(cluster);
					}

					// calculate realizations
					zValues = modelRunner->getZValues(samples);

					if (initial)
					{
						z0Fac = this->getZFactor(zValues[0]);
						z0Ignore = std::isnan(zValues[0]);

						combinedCluster->initialize(nStochasts, z0Fac, z0Ignore, this->Settings->designPointMethod, this->Settings->StochastSet);

						for (std::shared_ptr<ImportanceSamplingCluster> cluster : clusterResults)
						{
							cluster->initialize(nStochasts, z0Fac, z0Ignore, this->Settings->designPointMethod, this->Settings->StochastSet);
						}
					}

					if (modelRunner->shouldExitPrematurely(samples))
					{
						// return the result so far
						std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(combinedCluster->DesignPointBuilder->getSample(), Statistics::StandardNormal::getUFromQ(combinedCluster->ProbFailure), convergenceReport);
						if (startDesignPoint != nullptr)
						{
							designPoint->ContributingDesignPoints.push_back(startDesignPoint);
						}

						return designPoint;
					}

					zIndex = 0;
				}

				// value of the z function (z<0 : failure, z>0 : no failure, z = NaN : ignore)
				double z = zValues[zIndex];
				std::shared_ptr<ImportanceSamplingCluster> sampleCluster = clusters[zIndex];

				// determine multiplication factor for z (z<0), if u = 0
				if (initial)
				{
					z0Fac = this->getZFactor(z);
					z0Ignore = std::isnan(z);

					combinedCluster->initialize(nStochasts, z0Fac, z0Ignore, this->Settings->designPointMethod, this->Settings->StochastSet);

					for (std::shared_ptr<ImportanceSamplingCluster> cluster : clusterResults)
					{
						cluster->initialize(nStochasts, z0Fac, z0Ignore, this->Settings->designPointMethod, this->Settings->StochastSet);
					}

					initial = false;
					continue;
				}

				if (std::isnan(z))
				{
					if (prematureExit(Settings, sampleCluster->TotalCount, sampleIndex))
					{
						this->report(modelRunner, sampleIndex);

						// assume no failure can be calculated
						breakLoop = true;
					}

					continue;
				}

				// from now on, z < 0 means failing
				z *= z0Fac;

				std::shared_ptr<Sample> sample = samples[zIndex];
				sample->Z = z;
				sample->Z = getCorrectionForOverlappingClusters(sample, sampleCluster, clusterResults);

				combinedCluster->addSample(sample);
				sampleCluster->addSample(sample);

				// register minimum value of r and corresponding alpha
				if (this->sampleFunction != nullptr && sample->Z < 0)
				{
					this->sampleFunction(sample);
				}

				convergenceReport->FailedSamples = combinedCluster->FailCount;
				convergenceReport->FailFraction = combinedCluster->FailFraction;

				// check if convergence is reached (or stop criterion)
				bool enoughSamples = sampleIndex >= Settings->MinimumSamples;
				double probFailure = getProbabilityOfFailure(clusterResults);

				// if there is at least one failure and at least one non-failure observed
				if (probFailure > 0)
				{
					std::shared_ptr<Sample> designPoint = combinedCluster->DesignPointBuilder->getSample();

					std::shared_ptr<ImportanceSamplingCluster> mostContributingCLuster = findMostContributingCluster(clusterResults);
					double designPointWeight = getSampleWeight(designPoint, mostContributingCLuster->Center, dimensionality, factors);

					convergenceReport->IsConverged = checkConvergence(modelRunner, probFailure, designPointWeight, combinedCluster->TotalCount, sampleIndex);
					convergenceReport->FailWeight = combinedCluster->FailWeight;
					convergenceReport->MaxWeight = combinedCluster->MaxFailWeight;

					breakLoop = breakLoopWithFailureObs(Settings, enoughSamples, convergenceReport->IsConverged, combinedCluster);
				}
				else
				{
					// TODO: 
					breakLoop = breakLoopWithNoFailureObs(modelRunner, Settings, sampleIndex, reported);
				}
			}

			// build up design point

			double probFailure = getProbabilityOfFailure(clusterResults);
			double beta = Statistics::StandardNormal::getUFromQ(probFailure);

			std::shared_ptr<Sample> minSample = combinedCluster->DesignPointBuilder->getSample();

			std::shared_ptr<ImportanceSamplingCluster> mostContributingCluster = findMostContributingCluster(clusterResults);
			double designPointWeight = this->getSampleWeight(minSample, mostContributingCluster->Center, dimensionality, factors);
			convergenceReport->Convergence = getConvergence(probFailure, designPointWeight, combinedCluster->TotalCount);
			convergenceReport->NearestSample = combinedCluster->NearestSample;

			std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(minSample, beta, convergenceReport);

			if (startDesignPoint != nullptr)
			{
				designPoint->ContributingDesignPoints.push_back(startDesignPoint);
			}

			if (clusterResults.size() > 1)
			{
				for (size_t i = 0; i < clusterResults.size(); i++)
				{
#ifdef __cpp_lib_format
					std::string clusterIdentifier = std::format("Cluster {0:}", i + 1);
#else
					std::string clusterIdentifier = "Cluster " + std::to_string(i+1);
#endif

					double clusterBeta = Statistics::StandardNormal::getUFromQ(clusterResults[i]->ProbFailure);
					std::shared_ptr<Sample> clusterSample = clusterResults[i]->DesignPointBuilder->getSample();
					std::shared_ptr<DesignPoint> clusterDesignPoint = modelRunner->getDesignPoint(clusterSample, clusterBeta, convergenceReport, clusterIdentifier);

					designPoint->ContributingDesignPoints.push_back(clusterDesignPoint);
				}
			}

			return designPoint;
		}

        /// <summary>
        /// return the weight for the sample.
        /// This is given by: dimensionality * pdf / pdfOriginal, with pdf = normalFactor * std::exp(-SquaredSum/2).
        /// Rewritten to avoid a possible division by zero.
        /// </summary>
        /// <param name="modifiedSample"> the scaled sample </param>
        /// <param name="sample"> the original sample </param>
        /// <param name="dimensionality"> constant dependent on the number of stochasts </param>
        /// <returns> the weight </returns>
        double ImportanceSampling::getWeight(std::shared_ptr<Sample> modifiedSample, std::shared_ptr<Sample> sample, double dimensionality)
        {
            double sumSquared = Numeric::NumericSupport::GetSquaredSum(modifiedSample->Values);
            double sumSquaredOrg = Numeric::NumericSupport::GetSquaredSum(sample->Values);
            double ratioPdf = std::exp( 0.5 * (sumSquaredOrg - sumSquared) );
            return dimensionality * ratioPdf;
        }

		double ImportanceSampling::getDimensionality(std::vector<double> factors)
		{
			double dimensionality = 1; // correction for the dimensionality effect

			for (int k = 0; k < factors.size(); k++)
			{
				dimensionality *= factors[k];
			}

			return dimensionality;
		}

		std::vector<double> ImportanceSampling::getFactors(std::shared_ptr<StochastSettingsSet> stochastSettings)
		{
			std::vector<double> factors(stochastSettings->getVaryingStochastCount());

			for (int k = 0; k < factors.size(); k++)
			{
				factors[k] = stochastSettings->VaryingStochastSettings[k]->VarianceFactor;
			}

			return factors;
		}


		std::shared_ptr<Sample> ImportanceSampling::getOriginalSample(std::shared_ptr<Sample> sample, std::shared_ptr<Sample> center, std::vector<double> factors)
		{
			std::shared_ptr<Sample> original = std::make_shared<Sample>(sample->Values.size());

			for (int k = 0; k < sample->Values.size(); k++)
			{
				original->Values[k] = (sample->Values[k] - center->Values[k]) / factors[k];
			}

			return original;
		}

		double ImportanceSampling::getSampleWeight(std::shared_ptr<Sample> sample, std::shared_ptr<Sample> center, double dimensionality, std::vector<double> factors)
		{
			std::shared_ptr<Sample> originalSample = getOriginalSample(sample, center, factors);
			return this->getWeight(sample, originalSample, dimensionality);
		}

		bool ImportanceSampling::checkConvergence(std::shared_ptr<Models::ModelRunner> modelRunner, double pf, double minWeight, int samples, int nmaal)
		{
			std::shared_ptr<ReliabilityReport> report(new ReliabilityReport());
			report->Step = nmaal;
			report->MaxSteps = Settings->MaximumSamples;

			if (pf > 0 && pf < 1)
			{
				double convergence = getConvergence(pf, minWeight, samples);
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

		double ImportanceSampling::getConvergence(double pf, double minWeight, int samples)
		{
			if (pf > 0 && pf < 1)
			{
				if (pf > 0.5)
				{
					pf = 1 - pf;
				}

				double varPf = sqrt(std::max(0.0, (minWeight - pf) / (samples * pf)));
				return varPf;
			}
			else
			{
				return nan("");
			}
		}

		bool ImportanceSampling::prematureExit(std::shared_ptr<ImportanceSamplingSettings> settings, int samples, int runs)
		{
			return samples == 0 && runs > settings->MaximumSamplesNoResult;
		}

		double ImportanceSampling::getCorrectionForOverlappingClusters(std::shared_ptr<Sample> sample, std::shared_ptr<ImportanceSamplingCluster> clusterResult, std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusterResults)
		{
			std::shared_ptr<ImportanceSamplingCluster> nearestCluster = getNearestCluster(sample, clusterResults);

			if (nearestCluster != clusterResult)
			{
				return 0;
			}
			else
			{
				return sample->Z;
			}
		}

		std::shared_ptr<ImportanceSamplingCluster> ImportanceSampling::getNearestCluster(std::shared_ptr<Sample> sample, std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusters)
		{
			if (clusters.size() == 1)
			{
				return clusters[0];
			}
			else
			{
				std::shared_ptr<ImportanceSamplingCluster> nearestCluster = nullptr;
				double minDistance = std::numeric_limits<double>::max();

				for (auto cluster : clusters)
				{
					double distance = sample->getDistance2(cluster->Center);
					if (nearestCluster == nullptr || distance < minDistance)
					{
						nearestCluster = cluster;
						minDistance = distance;
					}
				}

				return nearestCluster;
			}
		}

		void ImportanceSampling::report(std::shared_ptr<ModelRunner> modelRunner, int sampleIndex)
		{
			std::shared_ptr<ReliabilityReport> report = std::make_shared<ReliabilityReport>();
			report->Index = sampleIndex;
			report->MaxSteps = this->Settings->MaximumSamples;

			modelRunner->reportResult(report);
		}

		bool ImportanceSampling::breakLoopWithNoFailureObs(std::shared_ptr<ModelRunner> modelRunner, std::shared_ptr<ImportanceSamplingSettings> settings, int sampleIndex, bool& reported)
		{
			if (!reported)
			{
				report(modelRunner, sampleIndex);
				reported = true;
			}

			if (sampleIndex > settings->MaximumSamplesNoResult)
			{
				// assume no failure can be calculated
				report(modelRunner, sampleIndex);
				return true;
			}

			return false;
		}

		bool ImportanceSampling::breakLoopWithFailureObs(std::shared_ptr<ImportanceSamplingSettings> settings, bool enoughSamples, bool smallEnough, std::shared_ptr<ImportanceSamplingCluster> results)
		{
			if (enoughSamples && smallEnough)
			{
				return true;
			}

			if (enoughSamples && breakFunction != nullptr && breakFunction(results))
			{
				return true;
			}

			return false;
		}

		std::vector<std::shared_ptr<ImportanceSamplingCluster>> ImportanceSampling::getClusters(std::shared_ptr<ModelRunner> modelRunner, std::shared_ptr<DesignPoint>& startDesignPoint)
		{
			startDesignPoint = nullptr;

			std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusters;

			if (this->Settings->Clusters.size() > 0)
			{
				for (size_t i = 0; i < this->Settings->Clusters.size(); i++)
				{
					std::shared_ptr<ImportanceSamplingCluster> cluster = std::make_shared<ImportanceSamplingCluster>();
					cluster->Center = this->Settings->Clusters[i];
					cluster->DesignPointBuilder = std::make_shared<DesignPointBuilder>(this->Settings->StochastSet->getVaryingStochastCount(), this->Settings->designPointMethod, this->Settings->StochastSet);

					clusters.push_back(cluster);
				}
			}
			else
			{
				const std::shared_ptr<StartPointCalculator> startPointCalculator = std::make_shared<StartPointCalculator>();
				startPointCalculator->Settings = this->Settings->startPointSettings;
				startPointCalculator->Settings->StochastSet = this->Settings->StochastSet;

				std::shared_ptr<Sample> startPoint = startPointCalculator->getStartPoint(modelRunner);

				if (Settings->startPointSettings->StartMethod != StartMethodType::None)
				{
					startDesignPoint = modelRunner->getDesignPoint(startPoint, startPoint->getBeta());
					startDesignPoint->Identifier = "Start point";
				}

				std::shared_ptr<ImportanceSamplingCluster> cluster = std::make_shared<ImportanceSamplingCluster>();
				cluster->Center = startPoint;
				cluster->DesignPointBuilder = std::make_shared<DesignPointBuilder>(this->Settings->StochastSet->getVaryingStochastCount(), this->Settings->designPointMethod, this->Settings->StochastSet);

				clusters.push_back(cluster);
			}

			return clusters;
		}

		double ImportanceSampling::getProbabilityOfFailure(const std::vector<std::shared_ptr<ImportanceSamplingCluster>>& clusters)
		{
			double sumProbabilities = 0;

			for (std::shared_ptr<ImportanceSamplingCluster> cluster : clusters)
			{
				sumProbabilities += cluster->ProbFailure;
			}

			return sumProbabilities;
		}

		std::shared_ptr<ImportanceSamplingCluster> ImportanceSampling::findMostContributingCluster(std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusters)
		{
			if (clusters.empty())
			{
				return nullptr;
			}
			else if (clusters.size() == 1)
			{
				return clusters[0];
			}
			else
			{
				std::shared_ptr<ImportanceSamplingCluster> mostContributingCluster = clusters[0];
				for (int i = 1; i < clusters.size(); i++)
				{
					if (clusters[i]->ProbFailure > mostContributingCluster->ProbFailure)
					{
						mostContributingCluster = clusters[i];
					}
				}

				return mostContributingCluster;
			}
		}
	}
}



