#pragma once
#include <memory>
#include <vector>

#include "DiscreteValue.h"
#include "StochastProperties.h"
#include "Distributions/DeterministicDistribution.h"
#include "Distributions/Distribution.h"
#include "DistributionType.h"

namespace Deltares
{
	namespace Statistics
	{
		class Stochast : public StochastProperties
		{
		private:
			DistributionType distributionType = DistributionType::Deterministic;
			std::shared_ptr<Distribution> distribution = std::make_shared<DeterministicDistribution>();

			bool inverted = false;
			bool truncated = false;

			void updateDistribution();

		public:

			Stochast();
			Stochast(DistributionType distributionType, double* values);

			double getXFromU(double u);
			double getUFromX(double x);

			void setDistributionType(DistributionType distributionType);
			DistributionType getDistributionType();

			bool isInverted();
			void setInverted(bool inverted);

			bool isTruncated();
			void setTruncated(bool truncated);

			bool isVarying();

			bool isQualitative();
			double getRepresentativeU(double u);

			double getMean();
			void setMean(double mean);

			double getDeviation();
			void setDeviation(double deviation);

			void initializeForRun();
		};
	}
}

