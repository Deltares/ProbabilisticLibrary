#pragma once
#include "RandomValueGenerator.h"

struct gm_state
{
    double  u[97 + 1];
    double  c;
    double  cd;
    double  cm;
    int     i97;
    int     j97;
};


class GeorgeMarsagliaRandomValueGenerator : public RandomValueGenerator
{
private:
    inline static gm_state state;
public:
	void initialize(bool repeatable, int seed, int seedB) override;
	double next() override;
};

