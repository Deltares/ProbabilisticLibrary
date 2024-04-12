#include "ImportanceSamplingCluster.h"
#include "../Statistics/StandardNormal.h"

namespace Deltares
{
	namespace Reliability
	{
		double ImportanceSamplingCluster::getBeta() const
		{
			return Statistics::StandardNormal::getUFromQ(ProbFailure);
		}

		double ImportanceSamplingCluster::getFailFraction() const
		{
			return Numeric::NumericSupport::Divide(FailCount, TotalCount);
		}

		void ImportanceSamplingCluster::initialize(int nStochasts, double z0Fac, bool z0Ignore)
		{
			this->z0Fac = z0Fac;
			this->z0Ignore = z0Ignore;

			MinSample = std::make_shared<Models::Sample>(nStochasts);
			MinSample = MinSample->getSampleAtBeta(z0Fac * Statistics::StandardNormal::BetaMax);

			ZSample = std::make_shared<Models::Sample>(nStochasts);
			ZSample = ZSample->getSampleAtBeta(z0Fac * Statistics::StandardNormal::BetaMax);
			ZSample->Z = z0Fac * std::numeric_limits<double>::max();

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
				if (sample->getBeta() < MinBeta)
				{
					MinBeta = sample->getBeta();
					MinSample = sample;
				}

				DesignPointBuilder->addSample(sample);
			}

			if (ZSample == nullptr || sample->Z < ZSample->Z)
			{
				ZSample = sample;
			}

			// calculate the probability per cluster
			this->ProbFailure = getProbabilityOfFailure(!z0Ignore);
		}

		bool ImportanceSamplingCluster::HasValidCount()
		{
			return FailCount > 0 && TotalCount - FailCount > 0;
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
