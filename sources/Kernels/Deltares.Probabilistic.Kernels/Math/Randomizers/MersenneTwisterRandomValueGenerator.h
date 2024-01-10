#pragma once
#include "RandomValueGenerator.h"

class MersenneTwisterRandomValueGenerator : public RandomValueGenerator
{
public:
	void initialize(bool repeatable, int seed, int seedB) override;
	double next() override;
};

