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
	std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts;
	std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> varyingStochasts;
	std::vector<int> varyingStochastIndex;

	std::shared_ptr<CorrelationMatrix> correlationMatrix;
	std::shared_ptr<CorrelationMatrix> varyingCorrelationMatrix;

	std::vector<double> getExpandedValues(const std::vector<double> & values);
	std::vector<double> getExpandedValues(const std::vector<double> & values, double defaultValue);

	bool checkFullyCorrelated(const int i);
	void updateDependedParameter(std::vector<double> & uValues, const int i);

public:

	UConverter(std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts, std::shared_ptr<CorrelationMatrix> correlationMatrix);
	void initializeForRun();
	std::vector<double> getXValues(std::shared_ptr<Sample> sample);
	int getStochastCount();
	int getVaryingStochastCount();
	StochastPoint* GetStochastPoint(double beta, std::vector<double> & alphas, int count);
	void updateStochastSettings(std::shared_ptr<Deltares::Reliability::StochastSettingsSet> settings);
};

