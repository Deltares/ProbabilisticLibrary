// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "SettingsS.h"
#include "CrudeMonteCarloS.h"

#include <memory>

#include "ImportanceSamplingS.h"

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

            crudeMonteCarlo->Settings->Samples = this->MaximumSamples;
            crudeMonteCarlo->Settings->VariationCoefficient = this->VariationCoefficient;
            crudeMonteCarlo->Settings->ProbabilityForConvergence = this->ProbabilityForConvergence;
            crudeMonteCarlo->Settings->DeriveSamplesFromVariationCoefficient = this->DeriveSamplesFromVariationCoefficient;
            crudeMonteCarlo->Settings->RunSettings = this->RunSettings;
            crudeMonteCarlo->Settings->randomSettings = this->RandomSettings;
            crudeMonteCarlo->Settings->StochastSet = this->StochastSet;

            return crudeMonteCarlo;
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetImportanceSamplingMethod()
        {
            std::shared_ptr<ImportanceSamplingS> importanceSampling = std::make_shared<ImportanceSamplingS>();

            importanceSampling->Settings->MinimumSamples = this->MinimumSamples;
            importanceSampling->Settings->MaximumSamples = this->MaximumSamples;
            importanceSampling->Settings->VariationCoefficient = this->VariationCoefficient;
            importanceSampling->Settings->ProbabilityForConvergence = this->ProbabilityForConvergence;
            importanceSampling->Settings->DeriveSamplesFromVariationCoefficient = this->DeriveSamplesFromVariationCoefficient;
            importanceSampling->Settings->RunSettings = this->RunSettings;
            importanceSampling->Settings->randomSettings = this->RandomSettings;
            importanceSampling->Settings->StochastSet = this->StochastSet;

            return importanceSampling;
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
            case SensitivityMethodType::SensitivityImportanceSampling: return std::dynamic_pointer_cast<ImportanceSamplingS>(this->GetImportanceSamplingMethod())->Settings->isValid();
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


