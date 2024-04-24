#include "Sample.h"

namespace Deltares
{
	namespace Models
	{
		int Sample::getSize()
		{
			return size;
		}

		double Sample::getBeta()
		{
			return Numeric::NumericSupport::GetLength(Values);
		}

		double Sample::getDistance(std::shared_ptr<Sample> other)
		{
			return Numeric::NumericSupport::getDistance(this->Values, other->Values);
		}

		double Sample::getDistance2(std::shared_ptr<Sample> other)
		{
			return Numeric::NumericSupport::getDistance2(this->Values, other->Values);
		}

		void Sample::setInitialValues(double beta)
		{
			double value = sqrt(beta * beta / size) * Numeric::NumericSupport::GetSign(beta);

			for (int i = 0; i < size; i++)
			{
				Values[i] = value;
			}
		}

		std::shared_ptr<Sample> Sample::getSampleAtBeta(double newBeta)
		{
			const double actualBeta = this->getBeta();

			std::shared_ptr<Sample> normalizedSample = this->clone();

			if (actualBeta == 0)
			{
				const double defaultValue = newBeta * sqrt(1.0 / this->size);
				for (int k = 0; k < this->size; k++)
				{
					normalizedSample->Values[k] = defaultValue;
				}
			}
			else
			{
				const double factor = newBeta / actualBeta;

				for (int k = 0; k < this->size; k++)
				{
					normalizedSample->Values[k] = factor * this->Values[k];
				}
			}

			return normalizedSample;
		}

		std::shared_ptr<Sample> Sample::getMultipliedSample(double factor)
		{
			std::shared_ptr<Sample> multipliedSample = this->clone();

			for (int i = 0; i < this->size; i++)
			{
				multipliedSample->Values[i] = factor * this->Values[i];
			}

			return multipliedSample;
		}

		void Sample::correctSmallValues(double tolerance)
		{
			for (int k = 0; k < this->getSize(); k++)
			{
				if (std::abs(this->Values[k]) < tolerance)
				{
					this->Values[k] = 0;
				}
			}
		}

		bool Sample::areValuesEqual(std::shared_ptr<Sample> other)
		{
			if (this->Values.size() != other->Values.size())
			{
				return false;
			}

			for (size_t i = 0; i < this->Values.size(); i++)
			{
				if (this->Values[i] != other->Values[i])
				{
					return false;
				}
			}

			return true;
		}

		std::shared_ptr<Sample> Sample::clone()
		{
			std::shared_ptr<Sample> clonedSample = std::make_shared<Sample>(this->Values);

			clonedSample->AllowProxy = this->AllowProxy;
			clonedSample->IterationIndex = this->IterationIndex;
			clonedSample->Weight = this->Weight;

			return clonedSample;
		}
	}
}


