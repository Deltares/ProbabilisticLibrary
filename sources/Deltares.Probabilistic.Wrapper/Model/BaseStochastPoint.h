#pragma once

#include "../../Deltares.Probabilistic/Reliability/DesignPoint.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            public ref class BaseStochastPoint
            {
            public:
                virtual std::shared_ptr<Reliability::DesignPoint> getDesignPoint() 
                {
                    return nullptr;
                }
            };
        }
    }
}

