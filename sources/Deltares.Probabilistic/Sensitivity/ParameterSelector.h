#pragma once

#include "../Model/ZValueConverter.h"

namespace Deltares
{
    namespace Sensitivity
    {
        class ParameterSelector : public Models::ZValueConverter
        {
        public:
            std::string parameter = "";
            void initialize(std::vector<std::shared_ptr<Models::ModelInputParameter>>& inputParameters, std::vector<std::shared_ptr<Models::ModelInputParameter>>& outputParameters) override;
            void updateZValue(std::shared_ptr<Models::ModelSample> sample) override;
        private:
            int parameterIndex = 0;
            bool parameterIndexFromInput = false;
        };
    }
}

