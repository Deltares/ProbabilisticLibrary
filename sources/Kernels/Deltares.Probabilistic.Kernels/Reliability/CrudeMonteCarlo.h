#pragma once
#include "CrudeMonteCarloSettings.h"
#include "ReliabilityMethod.h"

class CrudeMonteCarlo : public ReliabilityMethod
{
public:
	CrudeMonteCarloSettings* Settings = new CrudeMonteCarloSettings();
	DesignPoint* getDesignPoint(ZModelRunner modelRunner) override;
};

