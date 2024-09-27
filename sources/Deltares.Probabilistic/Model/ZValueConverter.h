#pragma once
#include <memory>

#include "ModelSample.h"

namespace Deltares
{
    namespace Models
    {
        class ZValueConverter
        {
        public:
            virtual void updateZValue(std::shared_ptr<ModelSample> sample) {}
        };
    }
}

