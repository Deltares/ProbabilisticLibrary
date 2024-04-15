#pragma once
#include <memory>

#include "DesignPointBuilder.h"
#include "../Model/Sample.h"

namespace Deltares
{
	namespace Reliability
	{
		/// <summary>
		/// Holds administration and results per <see cref="Cluster"/>
		/// </summary>
		class ImportanceSamplingCluster
		{
		public:
			std::shared_ptr<Models::Sample> Center = nullptr;
			std::shared_ptr<Reliability::DesignPointBuilder> DesignPointBuilder = nullptr;

			double ProbFailure = 0.0;
			double getBeta() const;
			int TotalCount = 0;
			double TotalWeight = 0.0;
			int FailCount = 0;
			double FailWeight = 0.0;
			double FailFraction = 0.0;
			double getFailFraction() const;

			double MaxFailWeight = 0.0;

			double MinBeta = std::numeric_limits<double>::max();

			/// <summary>
			/// The sample with the lowest beta which fails
			/// </summary>
			std::shared_ptr<Deltares::Models::Sample> MinSample;

			/// <summary>
			/// The sample with the lowest z value
			/// </summary>
			std::shared_ptr<Deltares::Models::Sample> ZSample;

			void initialize(int nStochasts, double z0Fac, bool z0Ignore);

			void addSample(std::shared_ptr<Deltares::Models::Sample> sample);
			bool HasValidCount();
		private:
			double z0Fac = 0;
			bool z0Ignore = false;

			double getProbabilityOfFailure(bool useCount);
			double getProbability(double z0Fac, double failWeight, double totalWeight);
		};
	}
}
