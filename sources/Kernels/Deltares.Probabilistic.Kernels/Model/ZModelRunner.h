#pragma once
#include <string>

#include "Sample.h"
#include <vector>

#include "ReliabilityReport.h"
#include "ReliabilityResult.h"
#include "RunSettings.h"
#include "UConverter.h"
#include "ZModel.h"
#include "DesignPoint.h"

class ZModelRunner
{
private:
	ZModel* zModel;
	UConverter* uConverter;
	RunSettings* RunSettings = new ::RunSettings();
	std::vector<ReliabilityResult*> reliabilityResults;

public:
	ZModelRunner(ZModel* zModel, UConverter* uConverter)
	{
		this->zModel = zModel;
		this->uConverter = uConverter;
	}

	double getZValue(Sample* sample);
	double* getZValues(std::vector<Sample*> samples);
	int getVaryingStochastCount();
	bool shouldExitPrematurely(double* zValues, double z0Fac, std::vector<Sample*> samples, double beta);
	void reportResult(ReliabilityReport* report);
	DesignPoint* getRealization(double beta, double* alpha, ConvergenceReport* convergenceReport = nullptr, int scenarioIndex = -1, std::string identifier = "");
};

