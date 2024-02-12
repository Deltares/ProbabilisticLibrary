#include "Stochast.h"

#include "../Utils/probLibException.h"
#include "Distributions/DeterministicDistribution.h"
#include "Distributions/DiscreteDistribution.h"
#include "Distributions/GumbelDistribution.h"
#include "Distributions/NormalDistribution.h"
#include "Distributions/LogNormalDistribution.h"
#include "Distributions/InvertedDistribution.h"
#include "Distributions/TruncatedDistribution.h"
#include "Distributions/UniformDistribution.h"
#include "Distributions/QualitativeDistribution.h"

namespace Deltares
{
	namespace Statistics
	{
		void Stochast::updateDistribution()
		{
			switch (this->distributionType)
			{
			case DistributionType::Deterministic: this->distribution = std::make_shared<DeterministicDistribution>(); break;
			case DistributionType::Normal: this->distribution = std::make_shared<NormalDistribution>(); break;
			case DistributionType::LogNormal: this->distribution = std::make_shared<LogNormalDistribution>(); break;
			case DistributionType::Uniform: this->distribution = std::make_shared<UniformDistribution>(); break;
			case DistributionType::Gumbel: this->distribution = std::make_shared<GumbelDistribution>(); break;
			case DistributionType::Discrete: this->distribution = std::make_shared<DiscreteDistribution>(); break;
			case DistributionType::Qualitative: this->distribution = std::make_shared<QualitativeDistribution>(); break;
			default:
				throw Deltares::ProbLibCore::probLibException("Distribution type not supported");
			}

			if (truncated && distribution->canTruncate())
			{
				this->distribution = std::make_shared<TruncatedDistribution>(this->distribution);
			}

			if (inverted && distribution->canInvert())
			{
				this->distribution = std::make_shared<InvertedDistribution>(this->distribution);
			}
		}

		Stochast::Stochast() {};

		Stochast::Stochast(DistributionType distributionType, std::vector<double> values)
		{
			setDistributionType(distributionType);
			distribution->initialize(this, values);
		}

		double Stochast::getPDF(double x)
		{
			return this->distribution->getPDF(this, x);
		}

		double Stochast::getCDF(double x)
		{
			return this->distribution->getCDF(this, x);
		}

		double Stochast::getXFromU(double u)
		{
			return this->distribution->getXFromU(this, u);
		}

		double Stochast::getUFromX(double x)
		{
			return this->distribution->getUFromX(this, x);
		}

		double Stochast::getXFromUAndSource(double xSource, double u)
		{
			std::shared_ptr<StochastProperties> properties = this->ValueSet->getInterpolatedStochast(xSource);

			return this->distribution->getXFromU(properties.get(), u);
		}

		void Stochast::setXAtU(double x, double u, ConstantParameterType constantType)
		{
			this->distribution->setXAtU(this, x, u, constantType);
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

		bool Stochast::canInvert()
		{
			return this->distribution->canInvert();
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

		bool Stochast::canTruncate()
		{
			return this->distribution->canTruncate();
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

		bool Stochast::isValid()
		{
			return this->distribution->isValid(this);
		}

		bool Stochast::isQualitative()
		{
			return this->distribution->isQualitative();
		}

		double Stochast::getRepresentativeU(double u)
		{
			return this->distribution->getRepresentativeU(this, u);
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

		void Stochast::setShift(double shift)
		{
			distribution->setShift(this, shift, false);
		}

		void Stochast::setMeanAndDeviation(double mean, double deviation)
		{
			distribution->setMeanAndDeviation(this, mean, deviation);
		}

		void Stochast::initializeForRun()
		{
			distribution->initializeForRun(this);

			if (this->IsVariableStochast)
			{
				this->ValueSet->initializeForRun();
			}
		}

		bool Stochast::canFit()
		{
			return distribution->canFit();
		}

		void Stochast::fit(std::vector<double> values)
		{
			distribution->fit(this, values);
		}

	}
}



