#include "Sample.h"

int Sample::getSize()
{
	return size;
}

double Sample::getBeta()
{
	return NumericSupport::GetLength(Values, size);
}

void Sample::setBeta(double beta)
{
	double value = sqrt(beta * beta / size) * NumericSupport::GetSign(beta);

	for (int i = 0; i < size; i++)
	{
		Values[i] = value;
	}
}

