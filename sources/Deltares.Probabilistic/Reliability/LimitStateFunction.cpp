#include "LimitStateFunction.h"

#include "../Model/ModelInputParameter.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Reliability
    {
        void LimitStateFunction::initialize(std::vector<std::shared_ptr<Models::ModelInputParameter>>& inputParameters, std::vector<std::shared_ptr<Models::ModelInputParameter>>& outputParameters)
        {
            this->criticalParameterIndex = -1;
            this->compareParameterIndex = -1;

            for (int i = 0; i < inputParameters.size(); i++)
            {
                if (inputParameters[i]->name == this->criticalParameter)
                {
                    this->criticalParameterIndex = i;
                    this->criticalParameterIndexFromInput = true;
                }
                if (this->useCompareParameter && inputParameters[i]->name == this->compareParameter)
                {
                    this->compareParameterIndex = i;
                    this->compareParameterIndexFromInput = true;
                }
            }

            for (int i = 0; i < outputParameters.size(); i++)
            {
                if (outputParameters[i]->name == this->criticalParameter)
                {
                    this->criticalParameterIndex = i;
                    this->criticalParameterIndexFromInput = false;
                }
                if (this->useCompareParameter && outputParameters[i]->name == this->compareParameter)
                {
                    this->compareParameterIndex = i;
                    this->compareParameterIndexFromInput = false;
                }
            }
        }

        void LimitStateFunction::updateZValue(std::shared_ptr<Models::ModelSample> sample)
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

