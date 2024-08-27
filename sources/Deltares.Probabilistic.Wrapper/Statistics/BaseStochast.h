#pragma once
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            public ref class BaseStochast
            {
            public:
                virtual std::shared_ptr<Statistics::Stochast> GetStochast()
                {
                    return nullptr;
                }
            };
        }
    }
}

