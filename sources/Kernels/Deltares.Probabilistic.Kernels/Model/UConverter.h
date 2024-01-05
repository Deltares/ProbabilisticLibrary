#pragma once

#include <vector>

#include "Sample.h"
#include "StochastPoint.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"


class UConverter
{
private:
	std::vector<Stochast*> stochasts;
	std::vector<Stochast*> varyingStochasts;
	std::vector<int> varyingStochastIndex;

	CorrelationMatrix* correlationMatrix;
	CorrelationMatrix* varyingCorrelationMatrix;

	double* getExpandedUValues(double* values);

public:

	UConverter(std::vector<Stochast*> stochasts, CorrelationMatrix* correlationMatrix);
	void initializeForRun();
	double* getXValues(Sample* sample);
	int getVaryingStochastCount();
	StochastPoint* GetStochastPoint(double beta, double* alphas, int count);
};

