#pragma once
#include "CrudeMonteCarloSettings.h"
#include "ReliabilityMethod.h"

class CrudeMonteCarlo : public ReliabilityMethod
{
private:
	DesignPoint* GetReducedDesignPoint(ZModelRunner* modelRunner, double qFail, double qRange);
	bool CheckConvergence(ZModelRunner* modelRunner, double pf, int samples, int nmaal);
	double GetConvergence(double pf, int samples);
public:
	CrudeMonteCarloSettings* Settings = new CrudeMonteCarloSettings();
	DesignPoint* getDesignPoint(ZModelRunner* modelRunner) override;
};

