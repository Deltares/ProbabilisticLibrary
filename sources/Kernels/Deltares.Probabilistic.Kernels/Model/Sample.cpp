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
	return new Sample(this->Values, this->getSize());
}

Sample* Sample::normalize(double newBeta)
{
    double actualBeta = NumericSupport::GetLength(this->Values);
    double* un = new double[this->size];

    if (newBeta > 0)
    {
        for (int k = 0; k < this->size; k++)
        {
            un[k] = newBeta * this->Values[k] / actualBeta;
        }
    }

    return new Sample(un, this->size);
}

Sample* Sample::multiply(double factor)
{
    double* u = new double[this->size];
    for (int i = 0; i < this->size; i++)
    {
        u[i] = factor * this->Values[i];
    }

    return new Sample(u, this->size);
}


