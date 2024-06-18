#include "Settings.h"
#include "FORM.h"
#include "CrudeMonteCarlo.h"
#include "ImportanceSampling.h"
#include "AdaptiveImportanceSampling.h"
#include "DirectionalSampling.h"
#include "SubsetSimulation.h"
#include "NumericalIntegration.h"

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
            default: throw probLibException("Reliability method");
            }
        }

        const std::shared_ptr<Reliability::ReliabilityMethod> Settings::GetFORMMethod()
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

        const std::shared_ptr<Reliability::ReliabilityMethod> Settings::GetNumericalIntegrationMethod()
        {
            std::shared_ptr<NumericalIntegration> numericalIntegration = std::make_shared<NumericalIntegration>();

            return numericalIntegration;
        }

        const std::shared_ptr<Reliability::ReliabilityMethod> Settings::GetCrudeMonteCarloMethod()
        {
            std::shared_ptr<CrudeMonteCarlo> crudeMonteCarlo = std::make_shared<CrudeMonteCarlo>();

            //crudeMonteCarlo->Settings->EpsilonBeta = this->EpsilonBeta;
            //crudeMonteCarlo->Settings->FilterAtNonConvergence = this->FilterAtNonConvergence;
            //crudeMonteCarlo->Settings->MaxIterationsGrowthFactor = this->MaxIterationsGrowthFactor;
            //crudeMonteCarlo->Settings->MaximumIterations = this->MaximumIterations;
            //crudeMonteCarlo->Settings->RelaxationFactor = this->RelaxationFactor;
            //crudeMonteCarlo->Settings->RelaxationLoops = this->RelaxationLoops;
            //crudeMonteCarlo->Settings->GradientSettings = this->GradientSettings;
            //crudeMonteCarlo->Settings->RunSettings = this->RunSettings;
            //crudeMonteCarlo->Settings->StartPointSettings = this->StartPointSettings;
            //crudeMonteCarlo->Settings->StochastSet = this->StochastSet;

            return crudeMonteCarlo;
        }

        const std::shared_ptr<Reliability::ReliabilityMethod> Settings::GetImportanceSamplingMethod()
        {
            std::shared_ptr<ImportanceSampling> importanceSampling = std::make_shared<ImportanceSampling>();

            return importanceSampling;
        }

        const std::shared_ptr<Reliability::ReliabilityMethod> Settings::GetAdaptiveImportanceSamplingMethod()
        {
            std::shared_ptr<AdaptiveImportanceSampling> adaptiveImportanceSampling = std::make_shared<AdaptiveImportanceSampling>();

            return adaptiveImportanceSampling;
        }

        const std::shared_ptr<Reliability::ReliabilityMethod> Settings::GetDirectionalSamplingMethod()
        {
            std::shared_ptr<DirectionalSampling> directionalSampling = std::make_shared<DirectionalSampling>();

            return directionalSampling;
        }

        const std::shared_ptr<Reliability::ReliabilityMethod> Settings::GetSubsetSimulationMethod()
        {
            std::shared_ptr<SubsetSimulation> subsetSimulation = std::make_shared<SubsetSimulation>();

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
            else throw probLibException("Reliability method");
        }

    }
}


