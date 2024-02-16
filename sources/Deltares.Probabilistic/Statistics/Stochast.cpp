#include "Stochast.h"

#include "Distributions/DistributionLibrary.h"
#include "Distributions/InvertedDistribution.h"

namespace Deltares
{
	namespace Statistics
	{
		Stochast::Stochast() {};

		Stochast::Stochast(DistributionType distributionType, std::vector<double> values)
		{
			setDistributionType(distributionType);
			distribution->initialize(properties, values);
		}

		Stochast::Stochast(DistributionType distributionType, std::shared_ptr<StochastProperties> properties)
		{
			setDistributionType(distributionType);
			this->properties = properties;
		}

		double Stochast::getPDF(double x)
		{
			return this->distribution->getPDF(properties, x);
		}

		double Stochast::getCDF(double x)
		{
			return this->distribution->getCDF(properties, x);
		}

		double Stochast::getXFromU(double u)
		{
			return this->distribution->getXFromU(properties, u);
		}

		double Stochast::getUFromX(double x)
		{
			return this->distribution->getUFromX(properties, x);
		}

		double Stochast::getXFromUAndSource(double xSource, double u)
		{
			std::shared_ptr<StochastProperties> valueSetProperties = this->ValueSet->getInterpolatedStochast(xSource);

			return this->distribution->getXFromU(valueSetProperties, u);
		}

		void Stochast::setXAtU(double x, double u, ConstantParameterType constantType)
		{
			this->distribution->setXAtU(properties, x, u, constantType);
		}

		void Stochast::setDistributionType(DistributionType distributionType)
		{
			double oldMean = this->getMean();
			double oldDeviation = this->getDistributionType() == Deterministic ? this->getProperties()->Scale : this->getDeviation();

			this->distributionType = distributionType;
			this->distribution = DistributionLibrary::getDistribution(this->distributionType, truncated, inverted);

			if ((oldMean != 0 || oldDeviation != 0) && this->distribution->maintainMeanAndDeviation(this->properties))
			{
				this->setMeanAndDeviation(oldMean, oldDeviation);
			}
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
			this->distribution = DistributionLibrary::getDistribution(this->distributionType, truncated, inverted);
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
			this->distribution = DistributionLibrary::getDistribution(this->distributionType, truncated, inverted);
		}

		bool Stochast::isVarying()
		{
			return this->distribution->isVarying(properties);
		}

		bool Stochast::isValid()
		{
			return this->distribution->isValid(properties);
		}

		bool Stochast::isQualitative()
		{
			return this->distribution->isQualitative();
		}

		double Stochast::getRepresentativeU(double u)
		{
			return this->distribution->getRepresentativeU(properties, u);
		}

		double Stochast::getMean()
		{
			return distribution->getMean(properties);
		}

		void Stochast::setMean(double mean)
		{
			double deviation = this->getDistributionType() == Deterministic ? this->getProperties()->Scale : this->getDeviation();
			distribution->setMeanAndDeviation(properties, mean, deviation);
		}

		double Stochast::getDeviation()
		{
			return distribution->getDeviation(properties);
		}

		void Stochast::setDeviation(double deviation)
		{
			double mean = distribution->getMean(properties);
			distribution->setMeanAndDeviation(properties, mean, deviation);
		}

		void Stochast::setShift(double shift)
		{
			distribution->setShift(properties, shift, false);
		}

		void Stochast::setMeanAndDeviation(double mean, double deviation)
		{
			distribution->setMeanAndDeviation(properties, mean, deviation);
		}

		void Stochast::initializeForRun()
		{
			distribution->initializeForRun(properties);

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
			distribution->fit(properties, values);
		}

		std::vector<double> Stochast::getSpecialXValues()
		{
			return distribution->getSpecialPoints(properties);
		}

	}
}



