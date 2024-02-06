#pragma once

#include "../Statistics/Stochast.h"

namespace Deltares
{
    namespace Models
    {
        class StochastPointAlpha
        {
        public:
            std::shared_ptr<Deltares::Statistics::Stochast> Stochast = nullptr;
            double Alpha = 0;
            double AlphaCorrelated = 0;
            double U = 0;
            double X = 0;
            double InfluenceFactor = 0;
        };
    }
}

