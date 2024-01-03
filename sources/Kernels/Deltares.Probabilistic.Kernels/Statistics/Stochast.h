#pragma once
#include "StochastProperties.h"
#include "Distribution.h"
#include "DeterministicDistribution.h"

enum DistributionType { Deterministic, Normal, LogNormal, Uniform };

class Stochast : StochastProperties
{
private:
	DistributionType distributionType = DistributionType::Deterministic;

	Distribution* distribution = new DeterministicDistribution();

	bool inverted = false;

	bool truncated = false;

	void updateDistribution();

public:

	Stochast(DistributionType distributionType, double* values)
	{
		setDistributionType(distributionType);
		distribution->initialize(this, values);
	}

	double getXFromU(double u)
	{
		return distribution->getXFromU(this, u);
	}

	void setDistributionType(DistributionType distributionType)
	{
		this->distributionType = distributionType;
		updateDistribution();
	}

	DistributionType getDistributionType()
	{
		return this->distributionType;
	}

	bool isInverted()
	{
		return inverted;
	}

	void setInverted(bool inverted)
	{
		this->inverted = inverted;
		updateDistribution();
	}

	bool isTruncated()
	{
		return truncated;
	}

	void setTruncated(bool truncated)
	{
		this->truncated = truncated;
		updateDistribution();
	}

	bool isVarying()
	{
		return this->distribution->isVarying(this);
	}

	double getMean()
	{
		return distribution->getMean(this);
	}

	void setMean(double mean)
	{
		double deviation = distribution->getDeviation(this);
		distribution->setMeanAndDeviation(this, mean, deviation);
	}

	double getDeviation()
	{
		return distribution->getDeviation(this);
	}

	void setDeviation(double deviation)
	{
		double mean = distribution->getMean(this);
		distribution->setMeanAndDeviation(this, mean, deviation);
	}
};

