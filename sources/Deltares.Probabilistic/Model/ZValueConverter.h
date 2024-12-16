#pragma once
#include <memory>

#include "ModelInputParameter.h"
#include "ModelSample.h"

namespace Deltares
{
    namespace Models
    {
        class ZValueConverter
        {
        public:
            virtual void initialize(std::vector<std::shared_ptr<ModelInputParameter>>& inputParameters, std::vector<std::shared_ptr<ModelInputParameter>>& outputParameters) {}
            virtual void updateZValue(std::shared_ptr<ModelSample> sample) {}
        };
    }
}

