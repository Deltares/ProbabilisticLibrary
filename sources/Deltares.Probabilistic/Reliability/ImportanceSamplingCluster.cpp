#include "ImportanceSamplingCluster.h"
#include "../Statistics/StandardNormal.h"

namespace Deltares
{
	namespace Reliability
	{
		void ImportanceSamplingCluster::initialize(int nStochasts, double z0Fac, bool z0Ignore, DesignPointMethod method, std::shared_ptr<StochastSettingsSet> stochastSet)
		{
			this->z0Fac = z0Fac;
			this->z0Ignore = z0Ignore;

			DesignPointBuilder = std::make_shared<Reliability::DesignPointBuilder>(nStochasts, method, stochastSet);

			DesignPointBuilder->initialize(z0Fac * Statistics::StandardNormal::BetaMax);
		}

		void ImportanceSamplingCluster::addSample(std::shared_ptr<Models::Sample> sample)
		{
			TotalCount++;
			TotalWeight += sample->Weight;

			if (sample->Z < 0)
			{
				FailCount++;
				FailWeight += sample->Weight;
				MaxFailWeight = std::max(MaxFailWeight, sample->Weight);
				DesignPointBuilder->addSample(sample);
			}

			if (this->NearestSample == nullptr || abs(sample->Z) < abs(this->NearestSample->Z))
			{
				this->NearestSample = sample;
			}

			// calculate the probability per cluster
			this->FailFraction = Numeric::NumericSupport::Divide(FailCount, TotalCount);
			this->ProbFailure = getProbabilityOfFailure(!z0Ignore);
		}

		double ImportanceSamplingCluster::getProbabilityOfFailure(bool useCount)
		{
			if (this->FailCount == 0)
			{
				return 0;
			}
			else if (this->FailCount == this->TotalCount)
			{
				return 1;
			}
			else if (useCount)
			{
				return getProbability(this->z0Fac, this->FailWeight, this->TotalCount);
			}
			else
			{
				return getProbability(this->z0Fac, this->FailWeight, this->TotalWeight);
			}
		}

		double ImportanceSamplingCluster::getProbability(double z0Fac, double failWeight, double totalWeight)
		{
			if (z0Fac == -1)
			{
				return 1 - failWeight / totalWeight;
			}
			else
			{
				return failWeight / totalWeight;
			}
		}
	}
}
