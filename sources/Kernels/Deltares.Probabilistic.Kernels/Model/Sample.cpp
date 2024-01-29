#include "Sample.h"

int Sample::getSize()
{
	return size;
}

double Sample::getBeta()
{
	return NumericSupport::GetLength(Values);
}

void Sample::setBeta(double beta)
{
	double value = sqrt(beta * beta / size) * NumericSupport::GetSign(beta);

	for (int i = 0; i < size; i++)
	{
		Values[i] = value;
	}
}

Sample* Sample::clone()
{
	Sample* clonedSample = new Sample(this->Values);

	clonedSample->AllowProxy = this->AllowProxy;
	clonedSample->IterationIndex = this->IterationIndex;
	clonedSample->ScenarioIndex = this->ScenarioIndex;
	clonedSample->Weight = this->Weight;

	return clonedSample;
}

Sample* Sample::normalize(double newBeta)
{
	double actualBeta = NumericSupport::GetLength(this->Values);

	Sample* normalizedSample = this->clone();

	for (int k = 0; k < this->size; k++)
	{
		normalizedSample->Values[k] = newBeta * this->Values[k] / actualBeta;
	}

	return normalizedSample;
}

Sample* Sample::multiply(double factor)
{
	Sample* multipliedSample = this->clone();

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


