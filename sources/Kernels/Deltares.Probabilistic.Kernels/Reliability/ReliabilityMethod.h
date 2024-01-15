#pragma once
#include "../Model/DesignPoint.h"
#include "../Model/ZModelRunner.h"
#include "../Model/ConvergenceReport.h"

class ReliabilityMethod
{
private:
	bool stopped = false;

protected:

	static int getZFactor(double z);

	double* GetAlphas(Sample* sample, int nstochasts, double z0Fac);

	DesignPoint* GetRealizationFromP(Deltares::Models::ZModelRunner* modelRunner, double pf, Sample* u, double z0, ConvergenceReport* convergenceReport = nullptr);

public:
	virtual DesignPoint* getDesignPoint(Deltares::Models::ZModelRunner* modelRunner) { return nullptr; };

	bool isStopped();
	void Stop();
};

