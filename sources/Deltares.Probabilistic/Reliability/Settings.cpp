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
#include "Settings.h"
#include "FORM.h"
#include "CrudeMonteCarlo.h"
#include "ImportanceSampling.h"
#include "AdaptiveImportanceSampling.h"
#include "DirectionalSampling.h"
#include "SubsetSimulation.h"
#include "NumericalIntegration.h"
#include "NumericalBisection.h"
#include "LatinHyperCube.h"

#include <memory>

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<ReliabilityMethod> Settings::GetReliabilityMethod()
        {
            switch (this->ReliabilityMethod)
            {
            case ReliabilityMethodType::ReliabilityFORM: return this->GetFORMMethod();
            case ReliabilityMethodType::ReliabilityNumericalIntegration: return this->GetNumericalIntegrationMethod();
            case ReliabilityMethodType::ReliabilityCrudeMonteCarlo: return this->GetCrudeMonteCarloMethod();
            case ReliabilityMethodType::ReliabilityImportanceSampling: return this->GetImportanceSamplingMethod();
            case ReliabilityMethodType::ReliabilityAdaptiveImportanceSampling: return this->GetAdaptiveImportanceSamplingMethod();
            case ReliabilityMethodType::ReliabilityDirectionalSampling: return this->GetDirectionalSamplingMethod();
            case ReliabilityMethodType::ReliabilitySubsetSimulation: return this->GetSubsetSimulationMethod();
            case ReliabilityMethodType::ReliabilityNumericalBisection: return this->GetNumericalBisectionMethod();
            case ReliabilityMethodType::ReliabilityLatinHyperCube: return this->GetLatinHypercubeMethod();
            default: throw probLibException("Reliability method");
            }
        }

        const std::shared_ptr<ReliabilityMethod> Settings::GetFORMMethod()
        {
            std::shared_ptr<FORM> form = std::make_shared<FORM>();

            form->Settings->EpsilonBeta = this->EpsilonBeta;
            form->Settings->FilterAtNonConvergence = this->FilterAtNonConvergence;
            form->Settings->MaxIterationsGrowthFactor = this->MaxIterationsGrowthFactor;
            form->Settings->MaximumIterations = this->MaximumIterations;
            form->Settings->RelaxationFactor = this->RelaxationFactor;
            form->Settings->RelaxationLoops = this->RelaxationLoops;
            form->Settings->GradientSettings = this->GradientSettings;
            form->Settings->RunSettings = this->RunSettings;
            form->Settings->StartPointSettings = this->StartPointSettings;
            form->Settings->StochastSet = this->StochastSet;

            return form;
        }

        const std::shared_ptr<ReliabilityMethod> Settings::GetNumericalIntegrationMethod()
        {
            std::shared_ptr<NumericalIntegration> numericalIntegration = std::make_shared<NumericalIntegration>();

            numericalIntegration->Settings.designPointMethod = this->designPointMethod;
            numericalIntegration->Settings.runSettings = this->RunSettings;
            numericalIntegration->Settings.StochastSet = this->StochastSet;

            return numericalIntegration;
        }

        const std::shared_ptr<ReliabilityMethod> Settings::GetNumericalBisectionMethod()
        {
            std::shared_ptr<NumericalBisection> numericalBisection = std::make_shared<NumericalBisection>();

            numericalBisection->Settings->designPointMethod = this->designPointMethod;
            numericalBisection->Settings->StochastSet = this->StochastSet;

            numericalBisection->Settings->MinimumIterations = this->MinimumIterations;
            numericalBisection->Settings->MaximumIterations = this->MaximumIterations;
            numericalBisection->Settings->EpsilonBeta = this->EpsilonBeta;

            return numericalBisection;
        }

        const std::shared_ptr<ReliabilityMethod> Settings::GetLatinHypercubeMethod()
        {
            std::shared_ptr<LatinHyperCube> latinHypercube = std::make_shared<LatinHyperCube>();

            latinHypercube->Settings->designPointMethod = this->designPointMethod;
            latinHypercube->Settings->StochastSet = this->StochastSet;

            latinHypercube->Settings->MinimumSamples = this->MinimumSamples;

            return latinHypercube;
        }

        const std::shared_ptr<ReliabilityMethod> Settings::GetCrudeMonteCarloMethod()
        {
            std::shared_ptr<CrudeMonteCarlo> crudeMonteCarlo = std::make_shared<CrudeMonteCarlo>();

            crudeMonteCarlo->Settings->MinimumSamples = this->MinimumSamples;
            crudeMonteCarlo->Settings->MaximumSamples = this->MaximumSamples;
            crudeMonteCarlo->Settings->designPointMethod = this->designPointMethod;
            crudeMonteCarlo->Settings->VariationCoefficient = this->VariationCoefficient;
            crudeMonteCarlo->Settings->RunSettings = this->RunSettings;
            crudeMonteCarlo->Settings->randomSettings = this->RandomSettings;
            crudeMonteCarlo->Settings->StochastSet = this->StochastSet;

            return crudeMonteCarlo;
        }

        const std::shared_ptr<ReliabilityMethod> Settings::GetImportanceSamplingMethod()
        {
            std::shared_ptr<ImportanceSampling> importanceSampling = std::make_shared<ImportanceSampling>();

            importanceSampling->Settings->MinimumSamples = this->MinimumSamples;
            importanceSampling->Settings->MaximumSamples = this->MaximumSamples;
            importanceSampling->Settings->designPointMethod = this->designPointMethod;
            importanceSampling->Settings->VariationCoefficient = this->VariationCoefficient;
            importanceSampling->Settings->runSettings = this->RunSettings;
            importanceSampling->Settings->randomSettings = this->RandomSettings;
            importanceSampling->Settings->StochastSet = this->StochastSet;

            return importanceSampling;
        }

        const std::shared_ptr<ReliabilityMethod> Settings::GetAdaptiveImportanceSamplingMethod()
        {
            std::shared_ptr<AdaptiveImportanceSampling> adaptiveImportanceSampling = std::make_shared<AdaptiveImportanceSampling>();

            adaptiveImportanceSampling->Settings->importanceSamplingSettings->MinimumSamples = this->MinimumSamples;
            adaptiveImportanceSampling->Settings->importanceSamplingSettings->MaximumSamples = this->MaximumSamples;
            adaptiveImportanceSampling->Settings->importanceSamplingSettings->designPointMethod = this->designPointMethod;
            adaptiveImportanceSampling->Settings->importanceSamplingSettings->VariationCoefficient = this->VariationCoefficient;
            adaptiveImportanceSampling->Settings->importanceSamplingSettings->runSettings = this->RunSettings;
            adaptiveImportanceSampling->Settings->importanceSamplingSettings->randomSettings = this->RandomSettings;
            adaptiveImportanceSampling->Settings->importanceSamplingSettings->StochastSet = this->StochastSet;

            adaptiveImportanceSampling->Settings->MinVarianceLoops = this->MinimumVarianceLoops;
            adaptiveImportanceSampling->Settings->MaxVarianceLoops = this->MaximumVarianceLoops;
            adaptiveImportanceSampling->Settings->FractionFailed = this->FractionFailed;

            return adaptiveImportanceSampling;
        }

        const std::shared_ptr<ReliabilityMethod> Settings::GetDirectionalSamplingMethod()
        {
            std::shared_ptr<DirectionalSampling> directionalSampling = std::make_shared<DirectionalSampling>();

            directionalSampling->Settings->MinimumDirections = this->MinimumDirections;
            directionalSampling->Settings->MaximumDirections = this->MaximumDirections;
            directionalSampling->Settings->designPointMethod = this->designPointMethod;
            directionalSampling->Settings->VariationCoefficient = this->VariationCoefficient;
            directionalSampling->Settings->runSettings = this->RunSettings;
            directionalSampling->Settings->randomSettings = this->RandomSettings;
            directionalSampling->Settings->StochastSet = this->StochastSet;

            return directionalSampling;
        }

        const std::shared_ptr<ReliabilityMethod> Settings::GetSubsetSimulationMethod()
        {
            std::shared_ptr<SubsetSimulation> subsetSimulation = std::make_shared<SubsetSimulation>();

            subsetSimulation->Settings->MinimumSamples = this->MinimumSamples;
            subsetSimulation->Settings->MaximumSamples = this->MaximumSamples;
            subsetSimulation->Settings->designPointMethod = this->designPointMethod;
            subsetSimulation->Settings->SampleMethod = this->sampleMethod;
            subsetSimulation->Settings->VariationCoefficient = this->VariationCoefficient;
            subsetSimulation->Settings->RunSettings = this->RunSettings;
            subsetSimulation->Settings->randomSettings = this->RandomSettings;
            subsetSimulation->Settings->StochastSet = this->StochastSet;

            return subsetSimulation;
        }

        bool Settings::isValid()
        {
            switch (this->ReliabilityMethod)
            {
            case ReliabilityMethodType::ReliabilityFORM: return std::dynamic_pointer_cast<FORM>(this->GetFORMMethod())->Settings->isValid();
            case ReliabilityMethodType::ReliabilityNumericalIntegration: return std::dynamic_pointer_cast<NumericalIntegration>(this->GetNumericalIntegrationMethod())->Settings.isValid();
            case ReliabilityMethodType::ReliabilityCrudeMonteCarlo: return std::dynamic_pointer_cast<CrudeMonteCarlo>(this->GetCrudeMonteCarloMethod())->Settings->isValid();
            case ReliabilityMethodType::ReliabilityImportanceSampling: return std::dynamic_pointer_cast<ImportanceSampling>(this->GetImportanceSamplingMethod())->Settings->isValid();
            case ReliabilityMethodType::ReliabilityAdaptiveImportanceSampling: return std::dynamic_pointer_cast<AdaptiveImportanceSampling>(this->GetAdaptiveImportanceSamplingMethod())->Settings->isValid();
            case ReliabilityMethodType::ReliabilityDirectionalSampling: return std::dynamic_pointer_cast<DirectionalSampling>(this->GetDirectionalSamplingMethod())->Settings->isValid();
            case ReliabilityMethodType::ReliabilityNumericalBisection: return std::dynamic_pointer_cast<NumericalBisection>(this->GetNumericalBisectionMethod())->Settings->isValid();
            case ReliabilityMethodType::ReliabilitySubsetSimulation: return std::dynamic_pointer_cast<SubsetSimulation>(this->GetSubsetSimulationMethod())->Settings->isValid();
            default: throw probLibException("Reliability method");
            }
        }

        std::string Settings::getReliabilityMethodTypeString(ReliabilityMethodType method)
        {
            switch (method)
            {
            case ReliabilityMethodType::ReliabilityFORM: return "form";
            case ReliabilityMethodType::ReliabilityNumericalIntegration: return "numerical_integration";
            case ReliabilityMethodType::ReliabilityCrudeMonteCarlo: return "crude_monte_carlo";
            case ReliabilityMethodType::ReliabilityImportanceSampling: return "importance_sampling";
            case ReliabilityMethodType::ReliabilityAdaptiveImportanceSampling: return "adaptive_importance_sampling";
            case ReliabilityMethodType::ReliabilityDirectionalSampling: return "directional_sampling";
            case ReliabilityMethodType::ReliabilitySubsetSimulation: return "subset_simulation";
            case ReliabilityMethodType::ReliabilityNumericalBisection: return "numerical_bisection";
            case ReliabilityMethodType::ReliabilityLatinHyperCube: return "latin_hypercube";
            default: throw probLibException("Reliability method");
            }
        }

        ReliabilityMethodType Settings::getReliabilityMethodType(std::string method)
        {
            if (method == "form") return ReliabilityMethodType::ReliabilityFORM;
            else if (method == "numerical_integration") return ReliabilityMethodType::ReliabilityNumericalIntegration;
            else if (method == "crude_monte_carlo") return ReliabilityMethodType::ReliabilityCrudeMonteCarlo;
            else if (method == "importance_sampling") return ReliabilityMethodType::ReliabilityImportanceSampling;
            else if (method == "adaptive_importance_sampling") return ReliabilityMethodType::ReliabilityAdaptiveImportanceSampling;
            else if (method == "directional_sampling") return ReliabilityMethodType::ReliabilityDirectionalSampling;
            else if (method == "subset_simulation") return ReliabilityMethodType::ReliabilitySubsetSimulation;
            else if (method == "numerical_bisection") return ReliabilityMethodType::ReliabilityNumericalBisection;
            else if (method == "latin_hypercube") return ReliabilityMethodType::ReliabilityLatinHyperCube;
            else throw probLibException("Reliability method");
        }

        std::string Settings::getReliabilityResultTypeString(ReliabilityResultType method)
        {
            switch (method)
            {
            case ReliabilityResultType::ResultDesignPoint: return "design_point";
            case ReliabilityResultType::ResultFragilityCurve: return "fragility_curve";
            default: throw probLibException("Reliability result");
            }
        }

        ReliabilityResultType Settings::getReliabilityResultType(std::string method)
        {
            if (method == "design_point") return ReliabilityResultType::ResultDesignPoint;
            else if (method == "fragility_curve") return ReliabilityResultType::ResultFragilityCurve;
            else throw probLibException("Reliability result");
        }
    }
}


