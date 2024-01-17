#pragma once

#include <vector>

#include "Sample.h"
#include "StochastPointAlpha.h"

class StochastPoint
{
public:
	double Beta;
	std::vector<StochastPointAlpha*> Alphas;

	Sample* getSample();
};

