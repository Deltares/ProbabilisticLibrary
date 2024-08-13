#include "SettingsS.h"
#include "CrudeMonteCarloS.h"

#include <memory>

namespace Deltares
{
    namespace Sensitivity
    {
        std::shared_ptr<SensitivityMethod> SettingsS::GetSensitivityMethod()
        {
            switch (this->SensitivityMethod)
            {
            case SensitivityMethodType::SensitivityFORM: return this->GetFORMMethod();
            case SensitivityMethodType::SensitivityNumericalIntegration: return this->GetNumericalIntegrationMethod();
            case SensitivityMethodType::SensitivityCrudeMonteCarlo: return this->GetCrudeMonteCarloMethod();
            case SensitivityMethodType::SensitivityImportanceSampling: return this->GetImportanceSamplingMethod();
            case SensitivityMethodType::SensitivityDirectionalSampling: return this->GetDirectionalSamplingMethod();
            default: throw Reliability::probLibException("Sensitivity method");
            }
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetFORMMethod()
        {
            throw Reliability::probLibException("Not implemented yet");
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetNumericalIntegrationMethod()
        {
            throw Reliability::probLibException("Not implemented yet");
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetCrudeMonteCarloMethod()
        {
            std::shared_ptr<CrudeMonteCarloS> crudeMonteCarlo = std::make_shared<CrudeMonteCarloS>();

            crudeMonteCarlo->Settings->MinimumSamples = this->MinimumSamples;
            crudeMonteCarlo->Settings->MaximumSamples = this->MaximumSamples;
            crudeMonteCarlo->Settings->VariationCoefficient = this->VariationCoefficient;
            crudeMonteCarlo->Settings->NumberSamplesProbability = this->NumberSamplesProbability;
            crudeMonteCarlo->Settings->RunSettings = this->RunSettings;
            crudeMonteCarlo->Settings->randomSettings = this->RandomSettings;
            crudeMonteCarlo->Settings->StochastSet = this->StochastSet;

            return crudeMonteCarlo;
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetImportanceSamplingMethod()
        {
            throw Reliability::probLibException("Not implemented yet");
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetDirectionalSamplingMethod()
        {
            throw Reliability::probLibException("Not implemented yet");
        }

        bool SettingsS::isValid()
        {
            switch (this->SensitivityMethod)
            {
            case SensitivityMethodType::SensitivityCrudeMonteCarlo: return std::dynamic_pointer_cast<CrudeMonteCarloS>(this->GetCrudeMonteCarloMethod())->Settings->isValid();
            default: throw Reliability::probLibException("Sensitivity method");
            }
        }

        std::string SettingsS::getSensitivityMethodTypeString(SensitivityMethodType method)
        {
            switch (method)
            {
            case SensitivityMethodType::SensitivityFORM: return "form";
            case SensitivityMethodType::SensitivityNumericalIntegration: return "numerical_integration";
            case SensitivityMethodType::SensitivityCrudeMonteCarlo: return "crude_monte_carlo";
            case SensitivityMethodType::SensitivityImportanceSampling: return "importance_sampling";
            case SensitivityMethodType::SensitivityDirectionalSampling: return "directional_sampling";
            default: throw Reliability::probLibException("Sensitivity method");
            }
        }

        SensitivityMethodType SettingsS::getSensitivityMethodType(std::string method)
        {
            if (method == "form") return SensitivityMethodType::SensitivityFORM;
            else if (method == "numerical_integration") return SensitivityMethodType::SensitivityNumericalIntegration;
            else if (method == "crude_monte_carlo") return SensitivityMethodType::SensitivityCrudeMonteCarlo;
            else if (method == "importance_sampling") return SensitivityMethodType::SensitivityImportanceSampling;
            else if (method == "directional_sampling") return SensitivityMethodType::SensitivityDirectionalSampling;
            else throw Reliability::probLibException("Sensitivity method");
        }

    }
}


