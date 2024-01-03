#pragma once
#include "StochastProperties.h"
#include "Distribution.h"
#include "DeterministicDistribution.h"
#include "NormalDistribution.h"

enum DistributionType { Deterministic, Normal };

class Stochast : StochastProperties
{
private:
	DistributionType distributionType = DistributionType::Deterministic;

	Distribution* distribution;

	bool inverted = false;

	bool truncated = false;

	void updateDistribution()
	{
		switch (this->distributionType) {
		case DistributionType::Deterministic: this->distribution = new DeterministicDistribution(); break;
		case DistributionType::Normal: this->distribution = new NormalDistribution(); break;
		}
	}


public:

	Stochast(DistributionType distributionType, double* values)
	{
		setDistributionType(distributionType);
		distribution->initialize(values);
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

