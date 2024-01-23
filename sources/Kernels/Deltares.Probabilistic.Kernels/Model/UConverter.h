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

	std::vector<double> getExpandedValues(const std::vector<double> & values);
	std::vector<double> getExpandedValues(const std::vector<double> & values, double defaultValue);

	bool checkFullyCorrelated(const int i);
	void updateDependedParameter(std::vector<double> & uValues, const int i);

public:

	UConverter(std::vector<Stochast*> stochasts, CorrelationMatrix* correlationMatrix);
	void initializeForRun();
	std::vector<double> getXValues(Sample* sample);
	int getStochastCount();
	int getVaryingStochastCount();
	StochastPoint* GetStochastPoint(double beta, std::vector<double> & alphas, int count);
	void updateStochastSettings(Deltares::Reliability::StochastSettingsSet* settings);
};

