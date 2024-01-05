#pragma once

#include "../Statistics/Stochast.h"

class StochastPointAlpha
{
public:
    Stochast* Stochast = nullptr;
    double Alpha = 0;
    double AlphaCorrelated = 0;
    double U = 0;
    double X = 0;
    double InfluenceFactor = 0;
};

