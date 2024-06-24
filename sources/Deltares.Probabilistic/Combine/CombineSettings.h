#pragma once
#include "DesignPointCombiner.h"

namespace Deltares
{
    namespace Reliability
    {
        class CombineSettings
        {
        public:
            CombinerType combinerMethod = CombinerType::Hohenbichler;
            combineAndOr combineType = combineAndOr::combOr;
        };
    }
}

