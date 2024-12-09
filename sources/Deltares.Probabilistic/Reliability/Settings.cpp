// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
            case ReliabilityMethodType::ReliabilityCobyla: return this->GetCobylaReliabilityMethod();
            case ReliabilityMethodType::ReliabilityFORMthenDirectionalSampling: return this->GetFormThenDsReliabilityMethod();
            case ReliabilityMethodType::ReliabilityDirectionalSamplingThenFORM: return this->GetDsThenFormReliabilityMethod();

            default: throw probLibException("Reliability method");
            }
        }

        std::shared_ptr<FORM> Settings::GetFORMMethod() const
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

        std::shared_ptr<NumericalIntegration> Settings::GetNumericalIntegrationMethod() const
        {
            std::shared_ptr<NumericalIntegration> numericalIntegration = std::make_shared<NumericalIntegration>();

            numericalIntegration->Settings.designPointMethod = this->designPointMethod;
            numericalIntegration->Settings.runSettings = this->RunSettings;
            numericalIntegration->Settings.StochastSet = this->StochastSet;

            return numericalIntegration;
        }

        std::shared_ptr<NumericalBisection> Settings::GetNumericalBisectionMethod() const
        {
            std::shared_ptr<NumericalBisection> numericalBisection = std::make_shared<NumericalBisection>();

            numericalBisection->Settings->designPointMethod = this->designPointMethod;
            numericalBisection->Settings->StochastSet = this->StochastSet;

            numericalBisection->Settings->MinimumIterations = this->MinimumIterations;
            numericalBisection->Settings->MaximumIterations = this->MaximumIterations;
            numericalBisection->Settings->EpsilonBeta = this->EpsilonBeta;

            return numericalBisection;
        }

        std::shared_ptr<LatinHyperCube> Settings::GetLatinHypercubeMethod() const
        {
            std::shared_ptr<LatinHyperCube> latinHypercube = std::make_shared<LatinHyperCube>();

            latinHypercube->Settings->designPointMethod = this->designPointMethod;
            latinHypercube->Settings->StochastSet = this->StochastSet;

            latinHypercube->Settings->MinimumSamples = this->MinimumSamples;

            return latinHypercube;
        }

        std::shared_ptr<CobylaReliability> Settings::GetCobylaReliabilityMethod() const
        {
            auto cobyla_reliability = std::make_shared<CobylaReliability>();

            cobyla_reliability->Settings->designPointMethod = this->designPointMethod;
            cobyla_reliability->Settings->StochastSet = this->StochastSet;
            cobyla_reliability->Settings->EpsilonBeta = this->EpsilonBeta;
            cobyla_reliability->Settings->MaximumIterations = this->MaximumIterations;

            return cobyla_reliability;
        }

        std::shared_ptr<FORMThenDirectionalSampling> Settings::GetFormThenDsReliabilityMethod() const
        {
            auto fdir = std::make_shared<FORMThenDirectionalSampling>();

            fdir->DsSettings->designPointMethod = this->designPointMethod;
            fdir->formSettings->StochastSet = this->StochastSet;
            fdir->formSettings->EpsilonBeta = this->EpsilonBeta;
            fdir->formSettings->MaximumIterations = this->MaximumIterations;
            fdir->formSettings->FilterAtNonConvergence = this->FilterAtNonConvergence;
            fdir->formSettings->MaxIterationsGrowthFactor = this->MaxIterationsGrowthFactor;
            fdir->formSettings->MaximumIterations = this->MaximumIterations;
            fdir->formSettings->RelaxationFactor = this->RelaxationFactor;
            fdir->formSettings->RelaxationLoops = this->RelaxationLoops;
            fdir->formSettings->GradientSettings = this->GradientSettings;
            fdir->formSettings->RunSettings = this->RunSettings;
            fdir->formSettings->StartPointSettings = this->StartPointSettings;
            fdir->DsSettings->MinimumDirections = this->MinimumDirections;
            fdir->DsSettings->MaximumDirections = this->MaximumDirections;
            fdir->DsSettings->designPointMethod = this->designPointMethod;
            fdir->DsSettings->VariationCoefficient = this->VariationCoefficient;
            fdir->DsSettings->runSettings = this->RunSettings;
            fdir->DsSettings->randomSettings = this->RandomSettings;
            fdir->DsSettings->StochastSet = this->StochastSet;

            return fdir;
        }

        std::shared_ptr<DirectionalSamplingThenFORM> Settings::GetDsThenFormReliabilityMethod() const
        {
            auto dsfi = std::make_shared<DirectionalSamplingThenFORM>();

            dsfi->DsSettings->designPointMethod = this->designPointMethod;
            dsfi->formSettings->StochastSet = this->StochastSet;
            dsfi->formSettings->EpsilonBeta = this->EpsilonBeta;
            dsfi->formSettings->MaximumIterations = this->MaximumIterations;
            dsfi->formSettings->FilterAtNonConvergence = this->FilterAtNonConvergence;
            dsfi->formSettings->MaxIterationsGrowthFactor = this->MaxIterationsGrowthFactor;
            dsfi->formSettings->MaximumIterations = this->MaximumIterations;
            dsfi->formSettings->RelaxationFactor = this->RelaxationFactor;
            dsfi->formSettings->RelaxationLoops = this->RelaxationLoops;
            dsfi->formSettings->GradientSettings = this->GradientSettings;
            dsfi->formSettings->RunSettings = this->RunSettings;
            dsfi->formSettings->StartPointSettings = this->StartPointSettings;
            dsfi->DsSettings->MinimumDirections = this->MinimumDirections;
            dsfi->DsSettings->MaximumDirections = this->MaximumDirections;
            dsfi->DsSettings->designPointMethod = this->designPointMethod;
            dsfi->DsSettings->VariationCoefficient = this->VariationCoefficient;
            dsfi->DsSettings->runSettings = this->RunSettings;
            dsfi->DsSettings->randomSettings = this->RandomSettings;
            dsfi->DsSettings->StochastSet = this->StochastSet;

            return dsfi;
        }

        std::shared_ptr<CrudeMonteCarlo> Settings::GetCrudeMonteCarloMethod() const
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

        std::shared_ptr<ImportanceSampling> Settings::GetImportanceSamplingMethod() const
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

        std::shared_ptr<AdaptiveImportanceSampling> Settings::GetAdaptiveImportanceSamplingMethod() const
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

        std::shared_ptr<DirectionalSampling> Settings::GetDirectionalSamplingMethod() const
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

        std::shared_ptr<SubsetSimulation> Settings::GetSubsetSimulationMethod() const
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

        bool Settings::isValid() const
        {
            switch (this->ReliabilityMethod)
            {
            case ReliabilityMethodType::ReliabilityFORM: return GetFORMMethod()->Settings->isValid();
            case ReliabilityMethodType::ReliabilityNumericalIntegration: return GetNumericalIntegrationMethod()->Settings.isValid();
            case ReliabilityMethodType::ReliabilityCrudeMonteCarlo: return GetCrudeMonteCarloMethod()->Settings->isValid();
            case ReliabilityMethodType::ReliabilityImportanceSampling: return GetImportanceSamplingMethod()->Settings->isValid();
            case ReliabilityMethodType::ReliabilityAdaptiveImportanceSampling: return GetAdaptiveImportanceSamplingMethod()->Settings->isValid();
            case ReliabilityMethodType::ReliabilityDirectionalSampling: return GetDirectionalSamplingMethod()->Settings->isValid();
            case ReliabilityMethodType::ReliabilityNumericalBisection: return GetNumericalBisectionMethod()->Settings->isValid();
            case ReliabilityMethodType::ReliabilityLatinHyperCube: return GetLatinHypercubeMethod()->Settings->isValid();
            case ReliabilityMethodType::ReliabilitySubsetSimulation: return GetSubsetSimulationMethod()->Settings->isValid();
            case ReliabilityMethodType::ReliabilityFORMthenDirectionalSampling:
            {
                auto fdir = GetFormThenDsReliabilityMethod();
                return fdir->formSettings->isValid() && fdir->DsSettings->isValid();
            }
            case ReliabilityMethodType::ReliabilityDirectionalSamplingThenFORM:
            {
                auto dsfi = GetDsThenFormReliabilityMethod();
                return dsfi->formSettings->isValid() && dsfi->DsSettings->isValid();
            }
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
            case ReliabilityMethodType::ReliabilityCobyla: return "cobyla_reliability";
            case ReliabilityMethodType::ReliabilityFORMthenDirectionalSampling: return "form_then_directional_sampling";
            case ReliabilityMethodType::ReliabilityDirectionalSamplingThenFORM: return "directional_sampling_then_form";
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
            else if (method == "cobyla_reliability") return ReliabilityMethodType::ReliabilityCobyla;
            else if (method == "form_then_directional_sampling") return ReliabilityMethodType::ReliabilityFORMthenDirectionalSampling;
            else if (method == "directional_sampling_then_form") return ReliabilityMethodType::ReliabilityDirectionalSamplingThenFORM;
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


