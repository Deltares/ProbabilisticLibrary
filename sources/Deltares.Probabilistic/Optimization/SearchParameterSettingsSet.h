#pragma once
#include <vector>
#include <memory>

#include "SearchParameterSettings.h"

namespace Deltares
{
    namespace Optimization
    {
        class SearchParameterSettingsSet
        {
        public:
            std::vector<std::shared_ptr<SearchParameterSettings>> Dimensions;
        };
    }
}

