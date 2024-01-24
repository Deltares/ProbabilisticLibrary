#include "Stochast.h"

#include <exception>

#include "Distributions/DeterministicDistribution.h"
#include "Distributions/GumbelDistribution.h"
#include "Distributions/NormalDistribution.h"
#include "Distributions/LogNormalDistribution.h"
#include "Distributions/InvertedDistribution.h"
#include "Distributions/TruncatedDistribution.h"
#include "Distributions/UniformDistribution.h"

namespace Deltares
{
	namespace Statistics
	{
		void Stochast::updateDistribution()
		{
			switch (this->distributionType)
			{
			case DistributionType::Deterministic: this->distribution = new DeterministicDistribution(); break;
			case DistributionType::Normal: this->distribution = new NormalDistribution(); break;
			case DistributionType::LogNormal: this->distribution = new LogNormalDistribution(); break;
			case DistributionType::Uniform: this->distribution = new UniformDistribution(); break;
			case DistributionType::Gumbel: this->distribution = new GumbelDistribution(); break;
			default:
				throw std::exception("Distribution type not supported");
			}

			if (inverted && distribution->canInvert())
			{
				this->distribution = new InvertedDistribution(this->distribution);
			}

			if (truncated && distribution->canTruncate())
			{
				this->distribution = new TruncatedDistribution(this->distribution);
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

		double Stochast::getUFromX(double x)
		{
			return this->distribution->getUFromX(this, x);
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
	}
}



