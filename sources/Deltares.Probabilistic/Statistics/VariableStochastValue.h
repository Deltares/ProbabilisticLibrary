#pragma once

#include "StochastProperties.h"

namespace Deltares
{
    namespace Statistics
    {
        class VariableStochastValue
        {
        public:
            double X;
            std::shared_ptr<StochastProperties> Stochast = std::make_shared<StochastProperties>();

            std::shared_ptr<VariableStochastValue> clone()
            {
                std::shared_ptr<VariableStochastValue> value = std::make_shared<VariableStochastValue>();

                value->X = this->X;
                value->Stochast = this->Stochast->clone();

                return value;
            }
        };
    }
}

