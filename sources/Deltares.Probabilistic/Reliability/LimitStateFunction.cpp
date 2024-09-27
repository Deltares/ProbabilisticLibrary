#include "LimitStateFunction.h"

#include "../Model/ModelInputParameter.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Reliability
    {
        void LimitStateFunction::initialize(std::vector<std::shared_ptr<Models::ModelInputParameter>>& inputParameters, std::vector<std::shared_ptr<Models::ModelInputParameter>>& outputParameters)
        {
            // if no parameter is specified, always use the first output parameter
            if (this->criticalParameter == "")
            {
                this->criticalParameterIndex = 0;
                this->criticalParameterIndexFromInput = false;
            }
            else
            {
                this->criticalParameterIndex = -1;

                for (int i = 0; i < inputParameters.size(); i++)
                {
                    if (inputParameters[i]->name == this->criticalParameter)
                    {
                        this->criticalParameterIndex = inputParameters[i]->index;
                        this->criticalParameterIndexFromInput = true;
                    }
                }

                for (int i = 0; i < outputParameters.size(); i++)
                {
                    if (outputParameters[i]->name == this->criticalParameter)
                    {
                        this->criticalParameterIndex = outputParameters[i]->index;
                        this->criticalParameterIndexFromInput = false;
                    }
                }
            }

            this->compareParameterIndex = -1;

            if (this->useCompareParameter)
            {
                for (int i = 0; i < inputParameters.size(); i++)
                {
                    if (inputParameters[i]->name == this->compareParameter)
                    {
                        this->compareParameterIndex = inputParameters[i]->index;;
                        this->compareParameterIndexFromInput = true;
                    }
                }

                for (int i = 0; i < outputParameters.size(); i++)
                {
                    if (outputParameters[i]->name == this->compareParameter)
                    {
                        this->compareParameterIndex = outputParameters[i]->index;
                        this->compareParameterIndexFromInput = false;
                    }
                }
            }
        }

        void LimitStateFunction::updateZValue(std::shared_ptr<Models::ModelSample> sample)
        {
            if (this->isActive)
            {
                double criticalResultValue = this->criticalParameterIndexFromInput ? sample->Values[this->criticalParameterIndex] : sample->OutputValues[this->criticalParameterIndex];
                double criticalCompareValue = this->criticalValue;
                if (this->useCompareParameter)
                {
                    criticalCompareValue = this->compareParameterIndexFromInput ? sample->Values[this->compareParameterIndex] : sample->OutputValues[this->compareParameterIndex];
                }

                if (this->compareType == CompareType::LessThan)
                {
                    sample->Z = criticalResultValue - criticalCompareValue;
                }
                else
                {
                    sample->Z = criticalCompareValue - criticalResultValue;
                }
            }
        }

        CompareType LimitStateFunction::GetCompareType(std::string compare)
        {
            if (compare == "less_than") return CompareType::LessThan;
            else if (compare == "greater_than") return CompareType::GreaterThan;
            else throw probLibException("compare type");
        }

        std::string LimitStateFunction::GetCompareTypeString(CompareType compareType)
        {
            if (compareType == CompareType::LessThan) return "less_than";
            else if (compareType == CompareType::GreaterThan) return "greater_than";
            else throw probLibException("compare type");
        }
    }
}

