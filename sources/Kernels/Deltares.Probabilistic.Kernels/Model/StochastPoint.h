#pragma once

#include <vector>

#include "StochastPointAlpha.h"

class StochastPoint
{
public:
	double Beta;
	std::vector<StochastPointAlpha*> Alphas;
};

