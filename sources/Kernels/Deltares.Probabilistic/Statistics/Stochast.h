#pragma once
#include <memory>
#include <vector>

#include "DiscreteValue.h"
#include "StochastProperties.h"
#include "Distributions/DeterministicDistribution.h"
#include "Distributions/Distribution.h"
#include "DistributionType.h"
#include "VariableStochastValueSet.h"

namespace Deltares
{
	namespace Statistics
	{
		class Stochast
		{
		private:
			DistributionType distributionType = DistributionType::Deterministic;
			std::shared_ptr<StochastProperties> properties = std::make_shared<StochastProperties>();

			bool inverted = false;
			bool truncated = false;

		protected:
			std::shared_ptr<Distribution> distribution = std::make_shared<DeterministicDistribution>();

		public:

			Stochast();
			Stochast(DistributionType distributionType, std::vector<double> values);
			Stochast(DistributionType distributionType, std::shared_ptr<StochastProperties> properties);

			std::shared_ptr<StochastProperties> getProperties()	{ return properties; }

			double getPDF(double x);
			double getCDF(double x);

			double getXFromU(double u);
			double getUFromX(double x);
			double getXFromUAndSource(double source, double u);

			void setXAtU(double x, double u, ConstantParameterType constantType);

			void setDistributionType(DistributionType distributionType);
			DistributionType getDistributionType();

			bool canInvert();
			bool isInverted();
			void setInverted(bool inverted);

			bool canTruncate();
			bool isTruncated();
			void setTruncated(bool truncated);

			bool isVarying();

			bool isQualitative();
			double getRepresentativeU(double u);

			double getMean();
			void setMean(double mean);

			double getDeviation();
			void setDeviation(double deviation);

			void setMeanAndDeviation(double mean, double deviation);
			void setShift(double shift);

			void initializeForRun();

			bool canFit();
			void fit(std::vector<double> values);

			bool isValid();

			bool IsVariableStochast = false;
			std::shared_ptr<Stochast> VariableSource = nullptr;
			std::shared_ptr<VariableStochastValuesSet> ValueSet = std::make_shared<VariableStochastValuesSet>();
		};
	}
}

