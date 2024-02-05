#pragma once

#include <set>
#include <vector>

#include "Sample.h"
#include "StochastPoint.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/StochastSettingsSet.h"

class UConverter
{
private:
	bool hasQualitiveStochasts = false;
	bool hasVariableStochasts = false;

	std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts;
	std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> varyingStochasts;
	std::vector<int> varyingStochastIndex;
	std::vector<int> variableStochastIndex;

	std::shared_ptr<CorrelationMatrix> correlationMatrix;
	std::shared_ptr<CorrelationMatrix> varyingCorrelationMatrix;

	std::vector<double> getExpandedValues(const std::vector<double> & values);
	std::vector<double> getExpandedValues(const std::vector<double> & values, double defaultValue);

	bool checkFullyCorrelated(const int i);
	void updateDependedParameter(std::vector<double> & uValues, const int i);
	std::shared_ptr<Sample> getQualitativeExcludedSample(std::shared_ptr<Sample> sample);
	void assignVariableStochasts(std::shared_ptr<StochastPoint> realization, std::vector<double>& uCorrelated);

public:

	UConverter(std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts, std::shared_ptr<CorrelationMatrix> correlationMatrix);
	void initializeForRun();
	std::vector<double> getXValues(std::shared_ptr<Sample> sample);
	int getStochastCount();
	int getVaryingStochastCount();
	std::shared_ptr<StochastPoint> GetStochastPoint(std::shared_ptr<Sample> sample, double beta);
	void updateStochastSettings(std::shared_ptr<Deltares::Reliability::StochastSettingsSet> settings);
};

