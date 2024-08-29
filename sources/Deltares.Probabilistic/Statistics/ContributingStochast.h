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

            std::shared_ptr<ContributingStochast> clone()
            {
                std::shared_ptr<ContributingStochast> clone = std::make_shared<ContributingStochast>();

                clone->Probability = this->Probability;
                clone->Stochast = this->Stochast;

                return clone;
            }
        };
    }
}

