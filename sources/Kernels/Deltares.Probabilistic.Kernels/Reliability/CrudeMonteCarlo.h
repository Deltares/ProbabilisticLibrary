#pragma once
#include "CrudeMonteCarloSettings.h"
#include "ReliabilityMethod.h"

class CrudeMonteCarlo : public ReliabilityMethod
{
private:
	DesignPoint* GetReducedDesignPoint(Deltares::Models::ZModelRunner* modelRunner, double qFail, double qRange);
	bool CheckConvergence(Deltares::Models::ZModelRunner* modelRunner, double pf, int samples, int nmaal);
	double GetConvergence(double pf, int samples);
public:
	Deltares::Reliability::CrudeMonteCarloSettings* Settings = new Deltares::Reliability::CrudeMonteCarloSettings();
	DesignPoint* getDesignPoint(Deltares::Models::ZModelRunner* modelRunner) override;
};

