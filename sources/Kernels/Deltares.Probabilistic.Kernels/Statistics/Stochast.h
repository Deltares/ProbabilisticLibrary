#pragma once
#include "StochastProperties.h"
#include "Distribution.h"
#include "DeterministicDistribution.h"

enum DistributionType { Deterministic, Normal, LogNormal, Uniform };

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

	void setDistributionType(DistributionType distributionType);
	DistributionType getDistributionType();

	bool isInverted();
	void setInverted(bool inverted);

	bool isTruncated();
	void setTruncated(bool truncated);

	bool isVarying();

	double getMean();
	void setMean(double mean);

	double getDeviation();
	void setDeviation(double deviation);
};

