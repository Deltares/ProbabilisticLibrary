#pragma once
#include "StochastProperties.h"
#include "Distributions/DeterministicDistribution.h"
#include "Distributions/Distribution.h"

namespace Deltares
{
	namespace Statistics
	{
		enum DistributionType { Deterministic, Normal, LogNormal, Uniform, Gumbel };

		class Stochast : public StochastProperties
		{
		private:
			DistributionType distributionType = DistributionType::Deterministic;

			Distribution* distribution = new DeterministicDistribution();

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
		};
	}
}

