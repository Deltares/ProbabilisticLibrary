#pragma once
#include <memory>

#include "DesignPointBuilder.h"
#include "../Model/Sample.h"

namespace Deltares
{
	namespace Reliability
	{
		/// <summary>
		/// Holds administration and results per importance sampling cluster
		/// </summary>
		class ImportanceSamplingCluster
		{
		public:
			std::shared_ptr<Models::Sample> Center = nullptr;
			std::shared_ptr<Reliability::DesignPointBuilder> DesignPointBuilder = nullptr;

			double ProbFailure = 0.0;
			int TotalCount = 0;
			double TotalWeight = 0.0;
			int FailCount = 0;
			double FailWeight = 0.0;
			double FailFraction = 0.0;
			double MaxFailWeight = 0.0;

			void initialize(int nStochasts, double z0Fac, bool z0Ignore, DesignPointMethod method, std::shared_ptr<StochastSettingsSet> stochastSet);
			void addSample(std::shared_ptr<Deltares::Models::Sample> sample);

			std::shared_ptr<Models::Sample> NearestSample = nullptr; // result sample which is nearest to the limit state
		private:
			double z0Fac = 0;
			bool z0Ignore = false;

			double getProbabilityOfFailure(bool useCount);
			double getProbability(double z0Fac, double failWeight, double totalWeight);
		};
	}
}
