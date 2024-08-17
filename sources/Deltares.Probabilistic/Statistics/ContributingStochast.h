#pragma once

#include <memory>

#include "BaseStochast.h"

namespace Deltares
{
    namespace Statistics
    {
        class ContributingStochast
        {
        public:
            double Probability = 0;
            std::shared_ptr<BaseStochast> Stochast = nullptr;

            bool isValid()
            {
                return Probability >= 0.0 && Probability <= 1.0 && Stochast != nullptr;
            }
        };
    }
}

