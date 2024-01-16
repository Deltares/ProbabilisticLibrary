#include "Stochast.h"

#include "DeterministicDistribution.h"
#include "NormalDistribution.h"
#include "LogNormalDistribution.h"
#include "UniformDistribution.h"


void Stochast::updateDistribution()
{
	switch (this->distributionType)
	{
		case Deterministic: this->distribution = new DeterministicDistribution(); break;
		case Normal: this->distribution = new NormalDistribution(); break;
		case LogNormal: this->distribution = new LogNormalDistribution(); break;
		case Uniform: this->distribution = new UniformDistribution(); break;
	}
}

Stochast::Stochast() {};

Stochast::Stochast(DistributionType distributionType, double* values)
{
	setDistributionType(distributionType);
	distribution->initialize(this, values);
}

double Stochast::getXFromU(double u)
{
	return this->distribution->getXFromU(this, u);
}

void Stochast::setDistributionType(DistributionType distributionType)
{
	this->distributionType = distributionType;
	Stochast::updateDistribution();
}

DistributionType Stochast::getDistributionType()
{
	return this->distributionType;
}

bool Stochast::isInverted()
{
	return inverted;
}

void Stochast::setInverted(bool inverted)
{
	this->inverted = inverted;
	updateDistribution();
}

bool Stochast::isTruncated()
{
	return truncated;
}

void Stochast::setTruncated(bool truncated)
{
	this->truncated = truncated;
	updateDistribution();
}

bool Stochast::isVarying()
{
	return this->distribution->isVarying(this);
}

bool Stochast::isQualitative()
{
	return this->distribution->isQualitative();
}

double Stochast::getRepresentativeU(double u)
{
	return this->distribution->getRepresentativeU(u);
}

double Stochast::getMean()
{
	return distribution->getMean(this);
}

void Stochast::setMean(double mean)
{
	double deviation = distribution->getDeviation(this);
	distribution->setMeanAndDeviation(this, mean, deviation);
}

double Stochast::getDeviation()
{
	return distribution->getDeviation(this);
}

void Stochast::setDeviation(double deviation)
{
	double mean = distribution->getMean(this);
	distribution->setMeanAndDeviation(this, mean, deviation);
}



