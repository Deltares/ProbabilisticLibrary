// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "SettingsS.h"
#include "CrudeMonteCarloS.h"

#include <memory>

#include "DirectionalSamplingS.h"
#include "FORMS.h"
#include "FOSM.h"
#include "ImportanceSamplingS.h"
#include "NumericalIntegrationS.h"

namespace Deltares
{
    namespace Sensitivity
    {
        std::shared_ptr<SensitivityMethod> SettingsS::GetSensitivityMethod()
        {
            switch (this->SensitivityMethod)
            {
            case SensitivityMethodType::SensitivityFORM: return this->GetFORMMethod();
            case SensitivityMethodType::SensitivityFOSM: return this->GetFOSMMethod();
            case SensitivityMethodType::SensitivityNumericalIntegration: return this->GetNumericalIntegrationMethod();
            case SensitivityMethodType::SensitivityCrudeMonteCarlo: return this->GetCrudeMonteCarloMethod();
            case SensitivityMethodType::SensitivityImportanceSampling: return this->GetImportanceSamplingMethod();
            case SensitivityMethodType::SensitivityDirectionalSampling: return this->GetDirectionalSamplingMethod();
            default: throw Reliability::probLibException("Sensitivity method");
            }
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetFORMMethod()
        {
            std::shared_ptr<FORMS> form = std::make_shared<FORMS>();

            form->Settings->Minimum = this->MinimumU;
            form->Settings->Maximum = this->MaximumU;
            form->Settings->StepSize = this->GlobalStepSize;
            form->Settings->GradientSettings = this->GradientSettings;
            form->Settings->RunSettings = this->RunSettings;
            form->Settings->CalculateCorrelations = this->CalculateCorrelations;
            form->Settings->CalculateInputCorrelations = this->CalculateInputCorrelations;
            form->Settings->RequestedQuantiles = this->RequestedQuantiles;

            return form;
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetFOSMMethod()
        {
            std::shared_ptr<FOSM> fosm = std::make_shared<FOSM>();

            fosm->Settings->StepSize = this->GlobalStepSize;
            fosm->Settings->RunSettings = this->RunSettings;
            fosm->Settings->CalculateCorrelations = this->CalculateCorrelations;
            fosm->Settings->CalculateInputCorrelations = this->CalculateInputCorrelations;
            fosm->Settings->RequestedQuantiles = this->RequestedQuantiles;

            return fosm;
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetNumericalIntegrationMethod()
        {
            std::shared_ptr<NumericalIntegrationS> numericalIntegration = std::make_shared<NumericalIntegrationS>();

            numericalIntegration->Settings->RunSettings = this->RunSettings;
            numericalIntegration->Settings->StochastSet = this->StochastSet;
            numericalIntegration->Settings->CalculateCorrelations = this->CalculateCorrelations;
            numericalIntegration->Settings->CalculateInputCorrelations = this->CalculateInputCorrelations;
            numericalIntegration->Settings->RequestedQuantiles = this->RequestedQuantiles;

            return numericalIntegration;
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetCrudeMonteCarloMethod()
        {
            std::shared_ptr<CrudeMonteCarloS> crudeMonteCarlo = std::make_shared<CrudeMonteCarloS>();

            crudeMonteCarlo->Settings->MaximumSamples = this->MaximumSamples;
            crudeMonteCarlo->Settings->VariationCoefficient = this->VariationCoefficient;
            crudeMonteCarlo->Settings->ProbabilityForConvergence = this->ProbabilityForConvergence;
            crudeMonteCarlo->Settings->DeriveSamplesFromVariationCoefficient = this->DeriveSamplesFromVariationCoefficient;
            crudeMonteCarlo->Settings->RunSettings = this->RunSettings;
            crudeMonteCarlo->Settings->randomSettings = this->RandomSettings;
            crudeMonteCarlo->Settings->RequestedQuantiles = this->RequestedQuantiles;
            crudeMonteCarlo->Settings->StochastSet = this->StochastSet;
            crudeMonteCarlo->Settings->CalculateCorrelations = this->CalculateCorrelations;
            crudeMonteCarlo->Settings->CalculateInputCorrelations = this->CalculateInputCorrelations;

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
            importanceSampling->Settings->RequestedQuantiles = this->RequestedQuantiles;
            importanceSampling->Settings->RunSettings = this->RunSettings;
            importanceSampling->Settings->randomSettings = this->RandomSettings;
            importanceSampling->Settings->StochastSet = this->StochastSet;
            importanceSampling->Settings->CalculateCorrelations = this->CalculateCorrelations;
            importanceSampling->Settings->CalculateInputCorrelations = this->CalculateInputCorrelations;

            return importanceSampling;
        }

        const std::shared_ptr<Sensitivity::SensitivityMethod> SettingsS::GetDirectionalSamplingMethod()
        {
            std::shared_ptr<DirectionalSamplingS> directionalSampling = std::make_shared<DirectionalSamplingS>();

            directionalSampling->Settings->NumberDirections = this->MaximumDirections;
            directionalSampling->Settings->MaximumIterations = this->MaximumIterations;
            directionalSampling->Settings->VariationCoefficientFailure = this->VariationCoefficient;
            directionalSampling->Settings->RequestedQuantiles = this->RequestedQuantiles;
            directionalSampling->Settings->RunSettings = this->RunSettings;
            directionalSampling->Settings->randomSettings = this->RandomSettings;
            directionalSampling->Settings->StochastSet = this->StochastSet;
            directionalSampling->Settings->CalculateCorrelations = this->CalculateCorrelations;
            directionalSampling->Settings->CalculateInputCorrelations = this->CalculateInputCorrelations;

            return directionalSampling;
        }

        bool SettingsS::isValid()
        {
            switch (this->SensitivityMethod)
            {
            case SensitivityMethodType::SensitivityCrudeMonteCarlo: return std::dynamic_pointer_cast<CrudeMonteCarloS>(this->GetCrudeMonteCarloMethod())->Settings->isValid();
            case SensitivityMethodType::SensitivityImportanceSampling: return std::dynamic_pointer_cast<ImportanceSamplingS>(this->GetImportanceSamplingMethod())->Settings->isValid();
            case SensitivityMethodType::SensitivityNumericalIntegration: return std::dynamic_pointer_cast<NumericalIntegrationS>(this->GetNumericalIntegrationMethod())->Settings->isValid();
            case SensitivityMethodType::SensitivityDirectionalSampling: return std::dynamic_pointer_cast<DirectionalSamplingS>(this->GetDirectionalSamplingMethod())->Settings->isValid();
            case SensitivityMethodType::SensitivityFORM: return std::dynamic_pointer_cast<FORMS>(this->GetFORMMethod())->Settings->isValid();
            case SensitivityMethodType::SensitivityFOSM: return std::dynamic_pointer_cast<FOSM>(this->GetFOSMMethod())->Settings->isValid();
            default: throw Reliability::probLibException("Sensitivity method");
            }
        }

        std::string SettingsS::getSensitivityMethodTypeString(SensitivityMethodType method)
        {
            switch (method)
            {
            case SensitivityMethodType::SensitivityFORM: return "form";
            case SensitivityMethodType::SensitivityFOSM: return "fosm";
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
            else if (method == "fosm") return SensitivityMethodType::SensitivityFOSM;
            else if (method == "numerical_integration") return SensitivityMethodType::SensitivityNumericalIntegration;
            else if (method == "crude_monte_carlo") return SensitivityMethodType::SensitivityCrudeMonteCarlo;
            else if (method == "importance_sampling") return SensitivityMethodType::SensitivityImportanceSampling;
            else if (method == "directional_sampling") return SensitivityMethodType::SensitivityDirectionalSampling;
            else throw Reliability::probLibException("Sensitivity method");
        }

    }
}


