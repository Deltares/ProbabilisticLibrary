#pragma once
#include <limits>

class Evaluation
{
public:
	double* X;
	int SizeX;

	double* R;
	int SizeR;

	double Z = std::numeric_limits<double>::quiet_NaN();
	double Weight = 1;
	int Iteration = -1;

	intptr_t Tag = 0;
};

