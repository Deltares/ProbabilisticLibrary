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

namespace Deltares::Uncertainty
{
        std::shared_ptr<UncertaintyMethod> SettingsS::GetUncertaintyMethod() const
        {
            switch (this->UncertaintyMethod)
            {
            case UncertaintyMethodType::UncertaintyFORM: return this->GetFORMMethod();
            case UncertaintyMethodType::UncertaintyFOSM: return this->GetFOSMMethod();
            case UncertaintyMethodType::UncertaintyNumericalIntegration: return this->GetNumericalIntegrationMethod();
            case UncertaintyMethodType::UncertaintyCrudeMonteCarlo: return this->GetCrudeMonteCarloMethod();
            case UncertaintyMethodType::UncertaintyImportanceSampling: return this->GetImportanceSamplingMethod();
            case UncertaintyMethodType::UncertaintyDirectionalSampling: return this->GetDirectionalSamplingMethod();
            default: throw Reliability::probLibException("Uncertainty method");
            }
        }

        std::shared_ptr<FORMS> SettingsS::GetFORMMethod() const
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

        std::shared_ptr<FOSM> SettingsS::GetFOSMMethod() const
        {
            std::shared_ptr<FOSM> fosm = std::make_shared<FOSM>();

            fosm->Settings->StepSize = this->GlobalStepSize;
            fosm->Settings->RunSettings = this->RunSettings;
            fosm->Settings->CalculateCorrelations = this->CalculateCorrelations;
            fosm->Settings->CalculateInputCorrelations = this->CalculateInputCorrelations;
            fosm->Settings->RequestedQuantiles = this->RequestedQuantiles;

            return fosm;
        }

        std::shared_ptr<NumericalIntegrationS> SettingsS::GetNumericalIntegrationMethod() const
        {
            std::shared_ptr<NumericalIntegrationS> numericalIntegration = std::make_shared<NumericalIntegrationS>();

            numericalIntegration->Settings->RunSettings = this->RunSettings;
            numericalIntegration->Settings->StochastSet = this->StochastSet;
            numericalIntegration->Settings->CalculateCorrelations = this->CalculateCorrelations;
            numericalIntegration->Settings->CalculateInputCorrelations = this->CalculateInputCorrelations;
            numericalIntegration->Settings->RequestedQuantiles = this->RequestedQuantiles;

            return numericalIntegration;
        }

        std::shared_ptr<CrudeMonteCarloS> SettingsS::GetCrudeMonteCarloMethod() const
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

        std::shared_ptr<ImportanceSamplingS> SettingsS::GetImportanceSamplingMethod() const
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

        std::shared_ptr<DirectionalSamplingS> SettingsS::GetDirectionalSamplingMethod() const
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

        void SettingsS::validate(Logging::ValidationReport& report) const
        {
            switch (this->UncertaintyMethod)
            {
            case UncertaintyMethodType::UncertaintyCrudeMonteCarlo: GetCrudeMonteCarloMethod()->Settings->validate(report); break;
            case UncertaintyMethodType::UncertaintyImportanceSampling: GetImportanceSamplingMethod()->Settings->validate(report); break;
            case UncertaintyMethodType::UncertaintyNumericalIntegration: GetNumericalIntegrationMethod()->Settings->validate(report); break;
            case UncertaintyMethodType::UncertaintyDirectionalSampling: GetDirectionalSamplingMethod()->Settings->validate(report); break;
            case UncertaintyMethodType::UncertaintyFORM: GetFORMMethod()->Settings->validate(report); break;
            case UncertaintyMethodType::UncertaintyFOSM: GetFOSMMethod()->Settings->validate(report); break;
            default: throw Reliability::probLibException("Uncertainty method");
            }
        }

        std::string SettingsS::getUncertaintyMethodTypeString(UncertaintyMethodType method)
        {
            switch (method)
            {
            case UncertaintyMethodType::UncertaintyFORM: return "form";
            case UncertaintyMethodType::UncertaintyFOSM: return "fosm";
            case UncertaintyMethodType::UncertaintyNumericalIntegration: return "numerical_integration";
            case UncertaintyMethodType::UncertaintyCrudeMonteCarlo: return "crude_monte_carlo";
            case UncertaintyMethodType::UncertaintyImportanceSampling: return "importance_sampling";
            case UncertaintyMethodType::UncertaintyDirectionalSampling: return "directional_sampling";
            default: throw Reliability::probLibException("Uncertainty method");
            }
        }

        UncertaintyMethodType SettingsS::getUncertaintyMethodType(std::string method)
        {
            if (method == "form") return UncertaintyMethodType::UncertaintyFORM;
            else if (method == "fosm") return UncertaintyMethodType::UncertaintyFOSM;
            else if (method == "numerical_integration") return UncertaintyMethodType::UncertaintyNumericalIntegration;
            else if (method == "crude_monte_carlo") return UncertaintyMethodType::UncertaintyCrudeMonteCarlo;
            else if (method == "importance_sampling") return UncertaintyMethodType::UncertaintyImportanceSampling;
            else if (method == "directional_sampling") return UncertaintyMethodType::UncertaintyDirectionalSampling;
            else throw Reliability::probLibException("Uncertainty method");
        }

    }