#pragma once
namespace Deltares
{
	namespace Optimization
	{
		enum ClusterInitializationMethod { PlusPlus	}; // Forgy, Random are other, not implemented yet, initialization methods

		class ClusterSettings
		{
		public:
			/// <summary>
			/// Sample Weighting should be accounted for in clustering
			/// </summary>
			bool SampleHasWeighting = false;

			/// <summary>
			/// Requested number of clusters
			/// </summary>
			int NumberClusters = 1;

			/// <summary>
			/// Maximum iterations within the clustering algorithm
			/// </summary>
			int MaxIterations = 100;

			/// <summary>
			/// Number of trials. Each trial leads to another initial clustering set
			/// </summary>
			int Trials = 10;

			/// <summary>
			/// Indicates whether number of clusters is fixed (false) or determined by the <see cref="IClusterMethod"/> algorithm
			/// </summary>
			bool OptimizeNumberOfClusters = false;

			ClusterInitializationMethod ClusterInitializationMethod = ClusterInitializationMethod::PlusPlus;

			/// <summary>
			/// Max number of clusters when <see cref="optimizeNumberOfClusters"/> is true
			/// </summary>
			int MaxClusters = 100;

			/// <summary>
			/// Minimum required relative improvement to continue increasing the number of clusters when <see cref="optimizeNumberOfClusters"/> is true
			/// </summary>
			double MinImprovement = 0.1;

			/**
			 * \brief The maximum number of samples used to generate clusters
			 */
			int MaxSamples = 500;

			bool isValid()
			{
				return true;
			}
		};
	}
}


