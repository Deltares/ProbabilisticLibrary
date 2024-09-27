#include "ParameterSelector.h"

namespace Deltares
{
    namespace Sensitivity
    {
        void ParameterSelector::initialize(std::vector<std::shared_ptr<Models::ModelInputParameter>>& inputParameters, std::vector<std::shared_ptr<Models::ModelInputParameter>>& outputParameters)
        {
            // if no parameter is specified, always use the first output parameter
            if (this->parameter == "")
            {
                this->parameterIndex = 0;
                this->parameterIndexFromInput = false;
            }
            else
            {
                this->parameterIndex = -1;

                for (int i = 0; i < inputParameters.size(); i++)
                {
                    if (inputParameters[i]->name == this->parameter)
                    {
                        this->parameterIndex = inputParameters[i]->index;
                        this->parameterIndexFromInput = true;
                    }
                }

                for (int i = 0; i < outputParameters.size(); i++)
                {
                    if (outputParameters[i]->name == this->parameter)
                    {
                        this->parameterIndex = outputParameters[i]->index;
                        this->parameterIndexFromInput = false;
                    }
                }
            }
        }

        void ParameterSelector::updateZValue(std::shared_ptr<Models::ModelSample> sample)
        {
            sample->Z = this->parameterIndexFromInput ? sample->Values[this->parameterIndex] : sample->OutputValues[this->parameterIndex];
        }
    }
}
