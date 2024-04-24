#pragma once
namespace Deltares
{
	namespace Optimization
	{
		/**
		 * \brief Types of initialization of a cluster
		 * \remark Forgy, Random are other initialization methods, but not implemented yet
		 */
		enum ClusterInitializationMethod { PlusPlus	};

		/**
		 * \brief Settings for clustering algorithm
		 */
		class ClusterSettings
		{
		public:
			/**
			 * \brief Sample Weighting should be accounted for in clustering
			 */
			bool SampleHasWeighting = false;

			/**
			 * \brief Requested number of clusters
			 */
			int NumberClusters = 1;

			/**
			 * \brief Maximum iterations within the clustering algorithm
			 */
			int MaxIterations = 100;

			/**
			 * \brief Number of trials. Each trial leads to another initial clustering set
			 */
			int Trials = 10;

			/**
			 * \brief Indicates whether number of clusters is fixed (false) or determined by the <see cref="IClusterMethod"/> algorithm
			 */
			bool OptimizeNumberOfClusters = false;

			/**
			 * \brief Initialization method
			 */
			ClusterInitializationMethod clusterInitializationMethod = ClusterInitializationMethod::PlusPlus;

			/**
			 * \brief Max number of clusters when <see cref="optimizeNumberOfClusters"/> is true
			 */
			int MaxClusters = 100;

			/**
			 * \brief Minimum required relative improvement to continue increasing the number of clusters when <see cref="optimizeNumberOfClusters"/> is true
			 */
			double MinImprovement = 0.1;

			/**
			 * \brief The maximum number of samples used to generate clusters
			 */
			int MaxSamples = 500;

			/**
			 * \brief Indicates whether these settings are valid
			 * \return Indication
			 */
			bool isValid()
			{
				return true;
			}
		};
	}
}


