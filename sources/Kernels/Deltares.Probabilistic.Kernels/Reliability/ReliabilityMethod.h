#pragma once
#include "DesignPoint.h"
#include "../Model/ZModelRunner.h"

class ReliabilityMethod
{
public:
	virtual DesignPoint* getDesignPoint(ZModelRunner modelRunner) { return nullptr; };
};

