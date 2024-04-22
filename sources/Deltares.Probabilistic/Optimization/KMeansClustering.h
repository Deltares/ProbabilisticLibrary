#pragma once

#include "../Model/Sample.h"
#include "ClusterSettings.h"

namespace Deltares
{
	namespace Optimization
	{
		class KMeansClustering
		{

		public:
			std::shared_ptr<ClusterSettings> Settings = std::make_shared<ClusterSettings>();
			std::vector<std::shared_ptr<Models::Sample>> getClusterCenters(std::vector<std::shared_ptr<Models::Sample>> samples);
		private:
			class Cluster
			{
			public:
				std::shared_ptr<Models::Sample> Center = nullptr;
				std::vector<std::shared_ptr<Models::Sample>> Samples;
				std::vector<std::shared_ptr<Models::Sample>> PreviousSamples;

				double getSumSquared();
				void updateMean();
			};

			std::vector<std::shared_ptr<Cluster>> FixedCluster(std::vector<std::shared_ptr<Models::Sample>>& samples, std::shared_ptr<ClusterSettings> options, double& sumSquared);
			std::vector<std::shared_ptr<Cluster>> DoClustering(std::vector<std::shared_ptr<Models::Sample>>& samples, std::shared_ptr<ClusterSettings> options, int randomSeed);
			std::vector<std::shared_ptr<Cluster>> InitializeClusters(std::vector<std::shared_ptr<Models::Sample>>& samples, std::shared_ptr<ClusterSettings> options, int randomSeed);
			std::vector<std::shared_ptr<Cluster>> InitPlusPlus(int numberClusters, std::vector<std::shared_ptr<Models::Sample>>& samples, int randomSeed, bool sampleHasWeighting);
			int ProporSelect(std::vector<double>& values);
			bool updateClustering(std::vector<std::shared_ptr<Cluster>>& clusters);
			std::shared_ptr<Cluster> getNearestCluster(std::shared_ptr<Models::Sample> sample, std::vector<std::shared_ptr<Cluster>>& clusters);
			double SilhouetteCoefficient(std::vector<std::shared_ptr<Cluster>>& clusters);
			double IntraClusterDistance(std::shared_ptr<Cluster> cluster, std::shared_ptr<Models::Sample> sample);
			double InterClusterDistance(std::shared_ptr<Cluster> cluster, std::shared_ptr<Models::Sample> sample, std::vector<std::shared_ptr<Cluster>>& clusters);
			std::vector<std::shared_ptr<Models::Sample>> getCentersFromClusters(std::vector<std::shared_ptr<KMeansClustering::Cluster>> clusters);
		};
	}
}

