#pragma once

#include <vector>

#include "Sample.h"
#include "StochastPointAlpha.h"

class StochastPoint
{
public:
	double Beta;
	std::vector<std::shared_ptr<StochastPointAlpha>> Alphas;

	std::shared_ptr<Sample> getSample();

	virtual ~StochastPoint() { ; }
};

