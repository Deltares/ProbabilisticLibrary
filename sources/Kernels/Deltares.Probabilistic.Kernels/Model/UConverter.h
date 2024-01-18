#pragma once

#include <vector>

#include "Sample.h"
#include "StochastPoint.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/StochastSettingsSet.h"

class UConverter
{
private:
	std::vector<Stochast*> stochasts;
	std::vector<Stochast*> varyingStochasts;
	std::vector<int> varyingStochastIndex;

	CorrelationMatrix* correlationMatrix;
	CorrelationMatrix* varyingCorrelationMatrix;

	double* getExpandedUValues(double* values);
	bool checkFullyCorrelated(const int i);
	void updateDependedParameter(double* uValues, const int i);

public:

	UConverter(std::vector<Stochast*> stochasts, CorrelationMatrix* correlationMatrix);
	void initializeForRun();
	double* getXValues(Sample* sample);
	int getStochastCount();
	int getVaryingStochastCount();
	StochastPoint* GetStochastPoint(double beta, double* alphas, int count);
	void updateStochastSettings(Deltares::Reliability::StochastSettingsSet* settings);
};

