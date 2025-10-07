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
# include "ProjectHandler.h"

#include <iostream>
#include <string>
#include <memory>

namespace Deltares
{
    namespace Server
    {
        using namespace Deltares::Statistics;
        using namespace Deltares::Reliability;

        bool ProjectHandler::CanHandle(std::string object_type)
        {
            return
                (object_type == "standard_normal" ||
                    object_type == "probability_value" ||
                    object_type == "message" ||
                    object_type == "project" ||
                    object_type == "model_parameter" ||
                    object_type == "limit_state_function" ||
                    object_type == "stochast" ||
                    object_type == "discrete_value" ||
                    object_type == "histogram_value" ||
                    object_type == "fragility_value" ||
                    object_type == "contributing_stochast" ||
                    object_type == "conditional_value" ||
                    object_type == "correlation_matrix" ||
                    object_type == "scenario" ||
                    object_type == "settings" ||
                    object_type == "stochast_settings" ||
                    object_type == "design_point" ||
                    object_type == "alpha" ||
                    object_type == "fragility_curve" ||
                    object_type == "fragility_curve_project" ||
                    object_type == "evaluation" ||
                    object_type == "convergence_report" ||
                    object_type == "combine_project" ||
                    object_type == "combine_settings" ||
                    object_type == "excluding_combine_project" ||
                    object_type == "excluding_combine_settings" ||
                    object_type == "self_correlation_matrix" ||
                    object_type == "run_project" ||
                    object_type == "run_project_settings" ||
                    object_type == "sensitivity_project" ||
                    object_type == "sensitivity_settings" ||
                    object_type == "sensitivity_result" ||
                    object_type == "sensitivity_value" ||
                    object_type == "uncertainty_project" ||
                    object_type == "uncertainty_settings" ||
                    object_type == "uncertainty_result" ||
                    object_type == "length_effect_project");
        }

        ProjectHandler::ObjectType ProjectHandler::GetType(std::string object_type)
        {
            if (object_type == "standard_normal") return ObjectType::StandardNormal;
            else if (object_type == "probability_value") return ObjectType::ProbabilityValue;
            else if (object_type == "message") return ObjectType::Message;
            else if (object_type == "project") return ObjectType::Project;
            else if (object_type == "model_parameter") return ObjectType::ModelParameter;
            else if (object_type == "limit_state_function") return  ObjectType::LimitStateFunction;
            else if (object_type == "stochast") return ObjectType::Stochast;
            else if (object_type == "discrete_value") return ObjectType::DiscreteValue;
            else if (object_type == "histogram_value") return ObjectType::HistogramValue;
            else if (object_type == "fragility_value") return ObjectType::FragilityValue;
            else if (object_type == "contributing_stochast") return ObjectType::ContributingStochast;
            else if (object_type == "conditional_value") return ObjectType::ConditionalValue;
            else if (object_type == "correlation_matrix") return ObjectType::CorrelationMatrix;
            else if (object_type == "scenario") return ObjectType::Scenario;
            else if (object_type == "settings") return ObjectType::Settings;
            else if (object_type == "stochast_settings") return  ObjectType::StochastSettings;
            else if (object_type == "design_point") return  ObjectType::DesignPoint;
            else if (object_type == "alpha") return  ObjectType::Alpha;
            else if (object_type == "fragility_curve") return  ObjectType::FragilityCurve;
            else if (object_type == "fragility_curve_project") return ObjectType::FragilityCurveProject;
            else if (object_type == "evaluation") return  ObjectType::Evaluation;
            else if (object_type == "reliability_result") return  ObjectType::ReliabilityResult;
            else if (object_type == "combine_project") return ObjectType::CombineProject;
            else if (object_type == "combine_settings") return ObjectType::CombineSettings;
            else if (object_type == "excluding_combine_project") return ObjectType::ExcludingCombineProject;
            else if (object_type == "excluding_combine_settings") return ObjectType::ExcludingCombineSettings;
            else if (object_type == "self_correlation_matrix") return ObjectType::SelfCorrelationMatrix;
            else if (object_type == "run_project") return ObjectType::RunProject;
            else if (object_type == "run_project_settings") return ObjectType::RunProjectSettings;
            else if (object_type == "sensitivity_project") return ObjectType::SensitivityProject;
            else if (object_type == "sensitivity_settings") return ObjectType::SensitivitySettings;
            else if (object_type == "sensitivity_result") return ObjectType::SensitivityResult;
            else if (object_type == "sensitivity_value") return ObjectType::SensitivityValue;
            else if (object_type == "uncertainty_project") return ObjectType::UncertaintyProject;
            else if (object_type == "uncertainty_settings") return ObjectType::UncertaintySettings;
            else if (object_type == "uncertainty_result") return ObjectType::UncertaintyResult;
            else if (object_type == "length_effect_project") return ObjectType::LengthEffectProject;
            else throw probLibException("type not supported: " + object_type);
        }

        void ProjectHandler::Create(std::string object_type, int id)
        {
            types[id] = GetType(object_type);

            switch (types[id])
            {
            case ObjectType::StandardNormal:
                break; // nothing to do
            case ObjectType::ProbabilityValue:
                probabilityValues[id] = std::make_shared<Deltares::Statistics::ProbabilityValue>();
                probabilityValueIds[probabilityValues[id]] = id;
                break;
            case ObjectType::Message:
                messages[id] = std::make_shared<Deltares::Models::Message>();
                messageIds[messages[id]] = id;
                break;
            case ObjectType::Project:
                projects[id] = std::make_shared<Deltares::Reliability::ReliabilityProject>();
                break;
            case ObjectType::ModelParameter:
                modelParameters[id] = std::make_shared<Deltares::Models::ModelInputParameter>();
                break;
            case ObjectType::LimitStateFunction:
                limitStateFunctions[id] = std::make_shared<Deltares::Reliability::LimitStateFunction>();
                break;
            case ObjectType::Stochast:
                stochasts[id] = std::make_shared<Deltares::Statistics::Stochast>();
                stochastIds[stochasts[id]] = id;
                break;
            case ObjectType::DiscreteValue:
                discreteValues[id] = std::make_shared<Deltares::Statistics::DiscreteValue>();
                discreteValueIds[discreteValues[id]] = id;
                break;
            case ObjectType::HistogramValue:
                histogramValues[id] = std::make_shared<Deltares::Statistics::HistogramValue>();
                histogramValueIds[histogramValues[id]] = id;
                break;
            case ObjectType::FragilityValue:
                fragilityValues[id] = std::make_shared<Deltares::Statistics::FragilityValue>();
                fragilityValueIds[fragilityValues[id]] = id;
                break;
            case ObjectType::ContributingStochast:
                contributingStochasts[id] = std::make_shared<Deltares::Statistics::ContributingStochast>();
                contributingStochastIds[contributingStochasts[id]] = id;
                break;
            case ObjectType::ConditionalValue:
                conditionalValues[id] = std::make_shared<Deltares::Statistics::VariableStochastValue>();
                conditionalValueIds[conditionalValues[id]] = id;
                break;
            case ObjectType::CorrelationMatrix:
                correlationMatrices[id] = std::make_shared<Deltares::Statistics::CorrelationMatrix>();
                correlationMatrixIds[correlationMatrices[id]] = id;
                break;
            case ObjectType::Scenario:
                scenarios[id] = std::make_shared<Deltares::Statistics::Scenario>();
                break;
            case ObjectType::Settings:
                settingsValues[id] = std::make_shared<Deltares::Reliability::Settings>();
                settingsValuesIds[settingsValues[id]] = id;
                break;
            case ObjectType::StochastSettings:
                stochastSettingsValues[id] = std::make_shared<Deltares::Reliability::StochastSettings>();
                break;
            case ObjectType::DesignPoint:
                designPoints[id] = std::make_shared<Deltares::Reliability::DesignPoint>();
                designPointIds[designPoints[id]] = id;
                break;
            case ObjectType::Alpha:
                alphas[id] = std::make_shared<Deltares::Reliability::StochastPointAlpha>();
                alphaIds[alphas[id]] = id;
                break;
            case ObjectType::FragilityCurve:
                fragilityCurves[id] = std::make_shared<Deltares::Reliability::FragilityCurve>();
                fragilityCurveIds[fragilityCurves[id]] = id;
                break;
            case ObjectType::FragilityCurveProject:
                fragilityCurveProjects[id] = std::make_shared<Deltares::Reliability::FragilityCurveProject>();
                break;
            case ObjectType::Evaluation:
                evaluations[id] = std::make_shared<Deltares::Reliability::Evaluation>();
                evaluationIds[evaluations[id]] = id;
                break;
            case ObjectType::ReliabilityResult:
                reliabilityResults[id] = std::make_shared<Deltares::Reliability::ReliabilityResult>();
                reliabilityResultIds[reliabilityResults[id]] = id;
                break;
            case ObjectType::CombineProject:
                combineProjects[id] = std::make_shared<Deltares::Reliability::CombineProject>();
                break;
            case ObjectType::CombineSettings:
                combineSettingsValues[id] = std::make_shared<Deltares::Reliability::CombineSettings>();
                break;
            case ObjectType::ExcludingCombineProject:
                excludingCombineProjects[id] = std::make_shared<Deltares::Reliability::ExcludingCombineProject>();
                break;
            case ObjectType::ExcludingCombineSettings:
                excludingCombineSettings[id] = std::make_shared<Deltares::Reliability::ExcludingCombineSettings>();
                break;
            case ObjectType::SelfCorrelationMatrix:
                selfCorrelationMatrices[id] = std::make_shared<Deltares::Statistics::SelfCorrelationMatrix>();
                break;
            case ObjectType::RunProject:
                runProjects[id] = std::make_shared<Deltares::Models::RunProject>();
                break;
            case ObjectType::RunProjectSettings:
                runProjectSettings[id] = std::make_shared<Deltares::Models::RunProjectSettings>();
                break;
            case ObjectType::UncertaintyProject:
                uncertaintyProjects[id] = std::make_shared<Deltares::Uncertainty::UncertaintyProject>();
                break;
            case ObjectType::UncertaintySettings:
                uncertaintySettingsValues[id] = std::make_shared<Deltares::Uncertainty::SettingsS>();
                break;
            case ObjectType::UncertaintyResult:
                uncertaintyResults[id] = std::make_shared<Deltares::Uncertainty::UncertaintyResult>();
                uncertaintyResultsIds[uncertaintyResults[id]] = id;
                break;
            case ObjectType::SensitivityProject:
                sensitivityProjects[id] = std::make_shared<Deltares::Sensitivity::SensitivityProject>();
                break;
            case ObjectType::SensitivitySettings:
                sensitivitySettingsValues[id] = std::make_shared<Deltares::Sensitivity::SensitivitySettings>();
                break;
            case ObjectType::SensitivityResult:
                sensitivityResults[id] = std::make_shared<Deltares::Sensitivity::SensitivityResult>();
                sensitivityResultsIds[sensitivityResults[id]] = id;
                break;
            case ObjectType::SensitivityValue:
                sensitivityValues[id] = std::make_shared<Deltares::Sensitivity::SensitivityValue>();
                sensitivityValuesIds[sensitivityValues[id]] = id;
                break;
            case ObjectType::LengthEffectProject:
                lengthEffectProjects[id] = std::make_shared<Deltares::Reliability::LengthEffectProject>();
                break;
            default: throw probLibException("object type");
            }
        }

        void ProjectHandler::Destroy(int id)
        {
            if (!types.contains(id))
            {
                // already destroyed or never existed
                return;
            }

            switch (types[id])
            {
            case ObjectType::StandardNormal: break;
            case ObjectType::ProbabilityValue: probabilityValueIds.erase(probabilityValues[id]); probabilityValues.erase(id); break;
            case ObjectType::Message: messageIds.erase(messages[id]); messages.erase(id); break;
            case ObjectType::Project: projects.erase(id); break;
            case ObjectType::ModelParameter: modelParameters.erase(id); break;
            case ObjectType::LimitStateFunction: limitStateFunctionIds.erase(limitStateFunctions[id]); limitStateFunctions.erase(id); break;
            case ObjectType::Stochast: stochastIds.erase(stochasts[id]); stochasts.erase(id); break;
            case ObjectType::DiscreteValue: discreteValueIds.erase(discreteValues[id]); discreteValues.erase(id); break;
            case ObjectType::HistogramValue: histogramValueIds.erase(histogramValues[id]); histogramValues.erase(id); break;
            case ObjectType::FragilityValue: fragilityCurveIds.erase(fragilityCurves[id]); fragilityValues.erase(id); break;
            case ObjectType::ContributingStochast: contributingStochastIds.erase(contributingStochasts[id]); contributingStochasts.erase(id); break;
            case ObjectType::ConditionalValue: conditionalValueIds.erase(conditionalValues[id]);  conditionalValues.erase(id); break;
            case ObjectType::CorrelationMatrix: correlationMatrixIds.erase(correlationMatrices[id]); correlationMatrices.erase(id); break;
            case ObjectType::Scenario: scenarios.erase(id); break;
            case ObjectType::Settings: settingsValuesIds.erase(settingsValues[id]); settingsValues.erase(id); break;
            case ObjectType::StochastSettings: stochastSettingsValues.erase(id); break;
            case ObjectType::DesignPoint: designPointIds.erase(designPoints[id]); designPoints.erase(id); break;
            case ObjectType::Alpha: alphaIds.erase(alphas[id]); alphas.erase(id); break;
            case ObjectType::FragilityCurve: fragilityCurveIds.erase(fragilityCurves[id]); fragilityCurves.erase(id); break;
            case ObjectType::FragilityCurveProject: fragilityCurveProjects.erase(id); break;
            case ObjectType::Evaluation: evaluationIds.erase(evaluations[id]); evaluations.erase(id); break;
            case ObjectType::ReliabilityResult: reliabilityResultIds.erase(reliabilityResults[id]); reliabilityResults.erase(id); break;
            case ObjectType::CombineProject: combineProjects.erase(id); break;
            case ObjectType::CombineSettings: combineSettingsValues.erase(id); break;
            case ObjectType::ExcludingCombineProject: excludingCombineProjects.erase(id); break;
            case ObjectType::ExcludingCombineSettings: excludingCombineSettings.erase(id); break;
            case ObjectType::SelfCorrelationMatrix: selfCorrelationMatrices.erase(id); break;
            case ObjectType::RunProject: runProjects.erase(id); break;
            case ObjectType::RunProjectSettings: runProjectSettings.erase(id); break;
            case ObjectType::UncertaintyProject: uncertaintyProjects.erase(id); break;
            case ObjectType::UncertaintySettings: uncertaintySettingsValues.erase(id); break;
            case ObjectType::UncertaintyResult: uncertaintyResultsIds.erase(uncertaintyResults[id]); uncertaintyResults.erase(id); break;
            case ObjectType::SensitivityProject: sensitivityProjects.erase(id); break;
            case ObjectType::SensitivitySettings: sensitivitySettingsValues.erase(id); break;
            case ObjectType::SensitivityResult: sensitivityResultsIds.erase(sensitivityResults[id]); sensitivityResults.erase(id); break;
            case ObjectType::SensitivityValue: sensitivityValuesIds.erase(sensitivityValues[id]); sensitivityValues.erase(id); break;
            case ObjectType::LengthEffectProject: lengthEffectProjects.erase(id); break;
            default: throw probLibException("object type");
            }
            types.erase(id);
        }

        bool ProjectHandler::ShouldClose()
        {
            return types.empty();
        }

        double ProjectHandler::GetValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::ProbabilityValue)
            {
                std::shared_ptr<Statistics::ProbabilityValue> probabilityValue = probabilityValues[id];

                if (property_ == "reliability_index") return probabilityValue->Reliability;
                else if (property_ == "probability_of_failure") return probabilityValue->getProbabilityOfFailure();
                else if (property_ == "probability_of_non_failure") return probabilityValue->getProbabilityOfNonFailure();
                else if (property_ == "return_period") return probabilityValue->getReturnPeriod();
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "location") return stochast->getProperties()->Location;
                else if (property_ == "scale") return stochast->getProperties()->Scale;
                else if (property_ == "shape") return stochast->getProperties()->Shape;
                else if (property_ == "shape_b") return stochast->getProperties()->ShapeB;
                else if (property_ == "shift") return stochast->getProperties()->Shift;
                else if (property_ == "shift_b") return stochast->getProperties()->ShiftB;
                else if (property_ == "minimum") return stochast->getProperties()->Minimum;
                else if (property_ == "maximum") return stochast->getProperties()->Maximum;
                else if (property_ == "mean") return stochast->getMean();
                else if (property_ == "deviation") return stochast->getDeviation();
                else if (property_ == "variation") return stochast->getVariation();
                else if (property_ == "design_quantile") return stochast->designQuantile;
                else if (property_ == "design_factor") return stochast->designFactor;
                else if (property_ == "design_value") return stochast->getDesignValue();
                else if (property_ == "ks_test") return stochast->getKSTest(tempValues["data"]);
                else if (property_ == "x_from_u_and_source") return stochast->getXFromUAndSource(tempValues["u_and_x"][1], tempValues["u_and_x"][0]);
                else return std::nan("");
            }
            else if (objectType == ObjectType::DiscreteValue)
            {
                std::shared_ptr<Statistics::DiscreteValue> discreteValue = discreteValues[id];

                if (property_ == "x") return discreteValue->X;
                else if (property_ == "amount") return discreteValue->Amount;
            }
            else if (objectType == ObjectType::HistogramValue)
            {
                std::shared_ptr<Statistics::HistogramValue> histogramValue = histogramValues[id];

                if (property_ == "lower_bound") return histogramValue->LowerBound;
                else if (property_ == "upper_bound") return histogramValue->UpperBound;
                else if (property_ == "amount") return histogramValue->Amount;
            }
            else if (objectType == ObjectType::FragilityValue)
            {
                std::shared_ptr<Statistics::FragilityValue> fragilityValue = fragilityValues[id];

                if (property_ == "x") return fragilityValue->X;
                else if (property_ == "reliability_index") return fragilityValue->Reliability;
                else if (property_ == "probability_of_failure") return fragilityValue->getProbabilityOfFailure();
                else if (property_ == "probability_of_non_failure") return fragilityValue->getProbabilityOfNonFailure();
                else if (property_ == "return_period") return fragilityValue->getReturnPeriod();
            }
            else if (objectType == ObjectType::ContributingStochast)
            {
                std::shared_ptr<Statistics::ContributingStochast> contributingStochast = contributingStochasts[id];

                if (property_ == "probability") return contributingStochast->Probability;
            }
            else if (objectType == ObjectType::LimitStateFunction)
            {
                std::shared_ptr<Reliability::LimitStateFunction> limitStateFunction = limitStateFunctions[id];

                if (property_ == "critical_value") return limitStateFunction->criticalValue;
            }
            else if (objectType == ObjectType::ModelParameter)
            {
                std::shared_ptr<Models::ModelInputParameter> parameter = modelParameters[id];

                if (property_ == "default_value") return parameter->defaultValue;
            }
            else if (objectType == ObjectType::ConditionalValue)
            {
                std::shared_ptr<Statistics::VariableStochastValue> conditionalValue = conditionalValues[id];

                if (property_ == "x") return conditionalValue->X;
                else if (property_ == "location") return conditionalValue->Stochast->Location;
                else if (property_ == "scale") return conditionalValue->Stochast->Scale;
                else if (property_ == "shape") return conditionalValue->Stochast->Shape;
                else if (property_ == "shape_b") return conditionalValue->Stochast->ShapeB;
                else if (property_ == "shift") return conditionalValue->Stochast->Shift;
                else if (property_ == "shift_b") return conditionalValue->Stochast->ShiftB;
                else if (property_ == "minimum") return conditionalValue->Stochast->Minimum;
                else if (property_ == "maximum") return conditionalValue->Stochast->Maximum;
                else if (property_ == "mean") return conditionalValue->mean;
                else if (property_ == "deviation") return conditionalValue->deviation;
                else return std::nan("");
            }
            else if (objectType == ObjectType::Scenario)
            {
                std::shared_ptr<Statistics::Scenario> scenario = scenarios[id];

                if (property_ == "probability") return scenario->probability;
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "relaxation_factor") return settings->RelaxationFactor;
                else if (property_ == "variation_coefficient") return settings->VariationCoefficient;
                else if (property_ == "fraction_failed") return settings->FractionFailed;
                else if (property_ == "epsilon_beta") return settings->EpsilonBeta;
                else if (property_ == "step_size") return settings->GradientSettings->StepSize;
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "min_value") return stochastSettings->MinValue;
                else if (property_ == "max_value") return stochastSettings->MaxValue;
                else if (property_ == "start_value") return stochastSettings->StartValue;
                else if (property_ == "variation_factor") return stochastSettings->VarianceFactor;
            }
            else if (objectType == ObjectType::UncertaintySettings)
            {
                std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

                if (property_ == "variation_coefficient") return settings->VariationCoefficient;
                else if (property_ == "probability_for_convergence") return settings->ProbabilityForConvergence;
                else if (property_ == "minimum_u") return settings->MinimumU;
                else if (property_ == "maximum_u") return settings->MaximumU;
                else if (property_ == "step_size") return settings->GradientSettings->StepSize;
                else if (property_ == "global_step_size") return settings->GlobalStepSize;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

                if (property_ == "low_value") return settings->LowValue;
                else if (property_ == "high_value") return settings->HighValue;
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "reliability_index") return designPoint->Beta;
                else if (property_ == "probability_failure") return designPoint->getFailureProbability();
                else if (property_ == "convergence" && designPoint->convergenceReport != nullptr)
                {
                    return designPoint->convergenceReport->Convergence;
                }
            }
            else if (objectType == ObjectType::Alpha)
            {
                std::shared_ptr<Reliability::StochastPointAlpha> alpha = alphas[id];

                if (property_ == "alpha") return alpha->Alpha;
                else if (property_ == "alpha_correlated") return alpha->AlphaCorrelated;
                else if (property_ == "u") return alpha->U;
                else if (property_ == "x") return alpha->X;
                else if (property_ == "influence_factor") return alpha->InfluenceFactor;
            }
            else if (objectType == ObjectType::FragilityCurve)
            {
                std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = fragilityCurves[id];

                if (property_ == "mean") return fragilityCurve->getMean();
                else if (property_ == "deviation") return fragilityCurve->getDeviation();
                else if (property_ == "variation") return fragilityCurve->getVariation();
                else return std::nan("");
            }
            else if (objectType == ObjectType::Evaluation)
            {
                std::shared_ptr<Reliability::Evaluation> evaluation = evaluations[id];

                if (property_ == "z") return evaluation->Z;
                else if (property_ == "quantile") return evaluation->Quantile;
                else if (property_ == "beta") return evaluation->Beta;
                else if (property_ == "weight") return evaluation->Weight;
            }
            else if (objectType == ObjectType::ReliabilityResult)
            {
                std::shared_ptr<Reliability::ReliabilityResult> result = reliabilityResults[id];

                if (property_ == "reliability_index") return result->Reliability;
                else if (property_ == "convergence") return std::isnan(result->ConvBeta) ? result->Variation : result->ConvBeta;
            }
            else if (objectType == ObjectType::SensitivityValue)
            {
                std::shared_ptr<Sensitivity::SensitivityValue> sensitivity_value = sensitivityValues[id];

                if (property_ == "low") return sensitivity_value->low;
                else if (property_ == "medium") return sensitivity_value->medium;
                else if (property_ == "high") return sensitivity_value->high;
                else if (property_ == "first_order_index") return sensitivity_value->firstOrderIndex;
                else if (property_ == "total_index") return sensitivity_value->totalIndex;
            }
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> length_effect = lengthEffectProjects[id];

                if (property_ == "length") return length_effect->length;
            }
            return std::nan("");
        }

        void ProjectHandler::SetValue(int id, std::string property_, double value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::ProbabilityValue)
            {
                std::shared_ptr<Statistics::ProbabilityValue> probabilityValue = probabilityValues[id];

                if (property_ == "reliability_index") probabilityValue->Reliability = value;
                else if (property_ == "probability_of_failure") probabilityValue->setProbabilityOfFailure(value);
                else if (property_ == "probability_of_non_failure") probabilityValue->setProbabilityOfNonFailure(value);
                else if (property_ == "return_period") probabilityValue->setReturnPeriod(value);
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "location") stochast->getProperties()->Location = value;
                else if (property_ == "scale") stochast->getProperties()->Scale = value;
                else if (property_ == "shape") stochast->getProperties()->Shape = value;
                else if (property_ == "shape_b") stochast->getProperties()->ShapeB = value;
                else if (property_ == "shift") stochast->setShift(value);
                else if (property_ == "shift_b") stochast->getProperties()->ShiftB = value;
                else if (property_ == "minimum") stochast->getProperties()->Minimum = value;
                else if (property_ == "maximum") stochast->getProperties()->Maximum = value;
                else if (property_ == "mean") stochast->setMean(value);
                else if (property_ == "deviation") stochast->setDeviation(value);
                else if (property_ == "variation") stochast->setVariation(value);
                else if (property_ == "design_quantile") stochast->designQuantile = value;
                else if (property_ == "design_factor") stochast->designFactor = value;
                else if (property_ == "design_value") stochast->setDesignValue(value);
                else if (property_ == "shift_for_fit") argValue = value;
            }
            else if (objectType == ObjectType::DiscreteValue)
            {
                std::shared_ptr<Statistics::DiscreteValue> discreteValue = discreteValues[id];

                if (property_ == "x") discreteValue->X = value;
                else if (property_ == "amount") discreteValue->Amount = value;

                discreteValue->setDirty();
            }
            else if (objectType == ObjectType::HistogramValue)
            {
                std::shared_ptr<Statistics::HistogramValue> histogramValue = histogramValues[id];

                if (property_ == "lower_bound") histogramValue->LowerBound = value;
                else if (property_ == "upper_bound") histogramValue->UpperBound = value;
                else if (property_ == "amount") histogramValue->Amount = value;

                histogramValue->setDirty();
            }
            else if (objectType == ObjectType::FragilityValue)
            {
                std::shared_ptr<Statistics::FragilityValue> fragilityValue = fragilityValues[id];

                if (property_ == "x") fragilityValue->X = value;
                else if (property_ == "reliability_index") fragilityValue->Reliability = value;
                else if (property_ == "probability_of_failure") fragilityValue->setProbabilityOfFailure(value);
                else if (property_ == "probability_of_non_failure") fragilityValue->setProbabilityOfNonFailure(value);
                else if (property_ == "return_period") fragilityValue->setReturnPeriod(value);

                fragilityValue->setDirty();
            }
            else if (objectType == ObjectType::ContributingStochast)
            {
                std::shared_ptr<Statistics::ContributingStochast> contributingStochast = contributingStochasts[id];

                if (property_ == "probability") contributingStochast->Probability = value;
            }
            else if (objectType == ObjectType::LimitStateFunction)
            {
                std::shared_ptr<Reliability::LimitStateFunction> limitStateFunction = limitStateFunctions[id];

                if (property_ == "critical_value") limitStateFunction->criticalValue = value;
            }
            else if (objectType == ObjectType::ModelParameter)
            {
                std::shared_ptr<Models::ModelInputParameter> parameter = modelParameters[id];

                if (property_ == "default_value") parameter->defaultValue = value;
            }
            else if (objectType == ObjectType::ConditionalValue)
            {
                std::shared_ptr<Statistics::VariableStochastValue> conditionalValue = conditionalValues[id];

                if (property_ == "x") conditionalValue->X = value;
                else if (property_ == "location") conditionalValue->Stochast->Location = value;
                else if (property_ == "scale") conditionalValue->Stochast->Scale = value;
                else if (property_ == "shape") conditionalValue->Stochast->Shape = value;
                else if (property_ == "shape_b") conditionalValue->Stochast->ShapeB = value;
                else if (property_ == "shift") conditionalValue->Stochast->Shift = value;
                else if (property_ == "shift_b") conditionalValue->Stochast->ShiftB = value;
                else if (property_ == "minimum") conditionalValue->Stochast->Minimum = value;
                else if (property_ == "maximum") conditionalValue->Stochast->Maximum = value;
                else if (property_ == "mean") conditionalValue->mean = value;
                else if (property_ == "deviation") conditionalValue->deviation = value;
            }
            else if (objectType == ObjectType::Scenario)
            {
                std::shared_ptr<Statistics::Scenario> scenario = scenarios[id];

                if (property_ == "probability") scenario->probability = value;
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "relaxation_factor") settings->RelaxationFactor = value;
                else if (property_ == "variation_coefficient") settings->VariationCoefficient = value;
                else if (property_ == "fraction_failed") settings->FractionFailed = value;
                else if (property_ == "epsilon_beta") settings->EpsilonBeta = value;
                else if (property_ == "step_size") settings->GradientSettings->StepSize = value;
            }
            else if (objectType == ObjectType::UncertaintySettings)
            {
                std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

                if (property_ == "variation_coefficient") settings->VariationCoefficient = value;
                else if (property_ == "probability_for_convergence") settings->ProbabilityForConvergence = value;
                else if (property_ == "minimum_u") settings->MinimumU = value;
                else if (property_ == "maximum_u") settings->MaximumU = value;
                else if (property_ == "step_size") settings->GradientSettings->StepSize = value;
                else if (property_ == "global_step_size") settings->GlobalStepSize = value;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

                if (property_ == "low_value") settings->LowValue = value;
                else if (property_ == "high_value") settings->HighValue = value;
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "min_value") stochastSettings->MinValue = value;
                else if (property_ == "max_value") stochastSettings->MaxValue = value;
                else if (property_ == "start_value") stochastSettings->StartValue = value;
                else if (property_ == "variance_factor") stochastSettings->VarianceFactor = value;
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "reliability_index") designPoint->Beta = value;
            }
            else if (objectType == ObjectType::Alpha)
            {
                std::shared_ptr<Reliability::StochastPointAlpha> alpha = alphas[id];

                if (property_ == "alpha") alpha->Alpha = value;
                else if (property_ == "u") alpha->U = value;
                else if (property_ == "x") alpha->X = value;
            }
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> length_effect = lengthEffectProjects[id];

                if (property_ == "length") length_effect->length = value;
            }
        }

        int ProjectHandler::GetIntValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (IsModelProjectType(objectType))
            {
                std::shared_ptr<Models::ModelProject> project = GetProject(id);

                if (property_ == "index") return project->model->Index;
                else if (property_ == "stochasts_count") return static_cast<int>(project->stochasts.size());
                else if (property_ == "total_model_runs") return project->modelRuns;
            }

            if (objectType == ObjectType::ModelParameter)
            {
                std::shared_ptr<Models::ModelInputParameter> parameter = modelParameters[id];

                if (property_ == "index") return parameter->index;
                else if (property_ == "array_size") return parameter->arraySize;
            }
            else if (objectType == ObjectType::Alpha)
            {
                std::shared_ptr<Reliability::StochastPointAlpha> alpha = alphas[id];

                if (property_ == "index") return alpha->Index;
            }
            else if (objectType == ObjectType::UncertaintyProject)
            {
                std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

                if (property_ == "uncertainty_stochasts_count") return static_cast<int>(project->uncertaintyResults.size());
                else if (property_ == "uncertainty_results_count") return static_cast<int>(project->uncertaintyResults.size());
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "results_count") return static_cast<int>(project->sensitivityResults.size());
            }
            else if (objectType == ObjectType::SensitivityResult)
            {
                std::shared_ptr<Sensitivity::SensitivityResult> result = sensitivityResults[id];

                if (property_ == "values_count") return static_cast<int>(result->values.size());
                else if (property_ == "evaluations_count") return static_cast<int>(result->evaluations.size());
                else if (property_ == "messages_count") return static_cast<int>(result->messages.size());
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "observations") return stochast->getProperties()->Observations;
                else if (property_ == "array_size") return stochast->modelParameter->arraySize;
                else if (property_ == "histogram_values_count") return static_cast<int>(stochast->getProperties()->HistogramValues.size());
                else if (property_ == "discrete_values_count") return static_cast<int>(stochast->getProperties()->DiscreteValues.size());
                else if (property_ == "fragility_values_count") return static_cast<int>(stochast->getProperties()->FragilityValues.size());
                else if (property_ == "contributing_stochasts_count") return static_cast<int>(stochast->getProperties()->ContributingStochasts.size());
                else if (property_ == "conditional_values_count") return static_cast<int>(stochast->ValueSet->StochastValues.size());
                else if (property_ == "array_variables_count") return static_cast<int>(stochast->ArrayVariables.size());
                else if (property_ == "special_values_count") tempValues["special_values"] = stochast->getSpecialXValues(); return static_cast<int>(tempValues["special_values"].size());
            }
            else if (objectType == ObjectType::ConditionalValue)
            {
                std::shared_ptr<Statistics::VariableStochastValue> conditionalValue = conditionalValues[id];

                if (property_ == "observations") return conditionalValue->Stochast->Observations;
            }
            else if (objectType == ObjectType::CorrelationMatrix)
            {
                std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = correlationMatrices[id];

                if (property_ == "variables_count") return correlationMatrix->getDimension();
            }
            else if (objectType == ObjectType::FragilityCurve)
            {
                std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = fragilityCurves[id];

                if (property_ == "fragility_values_count") return static_cast<int>(fragilityCurve->getProperties()->FragilityValues.size());
                else if (property_ == "special_values_count") tempValues["special_values"] = fragilityCurve->getSpecialXValues(); return static_cast<int>(tempValues["special_values"].size());
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "max_parallel_processes") return settings->RunSettings->MaxParallelProcesses;
                else if (property_ == "minimum_samples") return settings->MinimumSamples;
                else if (property_ == "maximum_samples") return settings->MaximumSamples;
                else if (property_ == "minimum_iterations") return settings->MinimumIterations;
                else if (property_ == "maximum_iterations") return settings->MaximumIterations;
                else if (property_ == "minimum_directions") return settings->MinimumDirections;
                else if (property_ == "maximum_directions") return settings->MaximumDirections;
                else if (property_ == "minimum_variance_loops") return settings->MinimumVarianceLoops;
                else if (property_ == "maximum_variance_loops") return settings->MaximumVarianceLoops;
                else if (property_ == "random_seed") return settings->RandomSettings->Seed;
                else if (property_ == "relaxation_loops") return settings->RelaxationLoops;
                else if (property_ == "max_steps_sphere_search") return settings->StartPointSettings->maxStepsSphereSearch;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

                if (property_ == "max_parallel_processes") return settings->RunSettings->MaxParallelProcesses;
                else if (property_ == "iterations") return settings->Iterations;
            }
            else if (objectType == ObjectType::UncertaintySettings)
            {
                std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

                if (property_ == "max_parallel_processes") return settings->RunSettings->MaxParallelProcesses;
                else if (property_ == "minimum_samples") return settings->MinimumSamples;
                else if (property_ == "maximum_samples") return settings->MaximumSamples;
                else if (property_ == "maximum_iterations") return settings->MaximumIterations;
                else if (property_ == "minimum_directions") return settings->MinimumDirections;
                else if (property_ == "maximum_directions") return settings->MaximumDirections;
                else if (property_ == "random_seed") return settings->RandomSettings->Seed;
                else if (property_ == "quantiles_count") return static_cast<int>(settings->RequestedQuantiles.size());
            }
            else if (objectType == ObjectType::UncertaintyResult)
            {
                std::shared_ptr<Uncertainty::UncertaintyResult> result = uncertaintyResults[id];

                if (property_ == "evaluations_count") return static_cast<int>(result->evaluations.size());
                else if (property_ == "quantile_evaluations_count") return static_cast<int>(result->quantileEvaluations.size());
                else if (property_ == "messages_count") return static_cast<int>(result->messages.size());
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "intervals") return stochastSettings->Intervals;
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (designPoint->convergenceReport != nullptr)
                {
                    if (property_ == "total_iterations") return designPoint->convergenceReport->TotalIterations;
                    else if (property_ == "total_directions") return designPoint->convergenceReport->TotalDirections;
                    else if (property_ == "total_model_runs") return designPoint->convergenceReport->TotalModelRuns;
                }
                if (property_ == "contributing_design_points_count") return static_cast<int>(designPoint->ContributingDesignPoints.size());
                else if (property_ == "alphas_count") return static_cast<int>(designPoint->Alphas.size());
                else if (property_ == "total_iterations") return designPoint->convergenceReport->TotalIterations;
                else if (property_ == "total_directions") return designPoint->convergenceReport->TotalDirections;
                else if (property_ == "total_model_runs") return designPoint->convergenceReport->TotalModelRuns;
                else if (property_ == "evaluations_count") return static_cast<int>(designPoint->Evaluations.size());
                else if (property_ == "reliability_results_count") return static_cast<int>(designPoint->ReliabililityResults.size());
                else if (property_ == "messages_count") return static_cast<int>(designPoint->Messages.size());
            }
            else if (objectType == ObjectType::Evaluation)
            {
                std::shared_ptr<Reliability::Evaluation> evaluation = evaluations[id];

                if (property_ == "iteration") return evaluation->Iteration;
                else if (property_ == "input_values_count") return static_cast<int>(evaluation->InputValues.size());
                else if (property_ == "output_values_count") return static_cast<int>(evaluation->OutputValues.size());
            }
            else if (objectType == ObjectType::ReliabilityResult)
            {
                std::shared_ptr<Reliability::ReliabilityResult> result = reliabilityResults[id];

                if (property_ == "index") return result->Index;
            }
            else if (objectType == ObjectType::ExcludingCombineProject)
            {
                if (property_ == "validation_messages_count") return static_cast<int>(validationMessages.size());
            }
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> project = lengthEffectProjects[id];

                if (property_ == "correlation_lengths_count") return static_cast<int>(project->correlationLengths.size());
            }

            return 0;
        }

        int ProjectHandler::GetIdValue(int id, std::string property_, int newId)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Reliability::ReliabilityProject> project = projects[id];

                if (property_ == "limit_state_function") return GetLimitStateFunctionId(project->limitStateFunction, newId);
                else if (property_ == "design_point") return GetDesignPointId(project->designPoint, newId);
            }
            else if (objectType == ObjectType::FragilityCurveProject)
            {
                std::shared_ptr<Reliability::FragilityCurveProject> project = fragilityCurveProjects[id];

                if (property_ == "design_point") return GetDesignPointId(project->designPoint, newId);
            }
            else if (objectType == ObjectType::RunProject)
            {
                std::shared_ptr<Models::RunProject> project = runProjects[id];

                if (property_ == "realization") return GetEvaluationId(project->evaluation, newId);
            }
            else if (objectType == ObjectType::UncertaintyProject)
            {
                std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

                if (property_ == "uncertainty_stochast") return GetStochastId(project->uncertaintyResult->stochast, newId);
                else if (property_ == "uncertainty_result") return GetUncertaintyResultId(project->uncertaintyResult, newId);
                else if (property_ == "output_correlation_matrix") return GetCorrelationMatrixId(project->outputCorrelationMatrix, newId);
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "result") return GetSensitivityResultId(project->sensitivityResult, newId);
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "conditional_source") return GetStochastId(stochast->VariableSource, newId);
            }
            else if (objectType == ObjectType::FragilityValue)
            {
                std::shared_ptr<Statistics::FragilityValue> fragilityValue = fragilityValues[id];

                if (property_ == "design_point") return GetDesignPointId(std::static_pointer_cast<Reliability::DesignPoint>(fragilityValue->designPoint), newId);
            }
            else if (objectType == ObjectType::ContributingStochast)
            {
                std::shared_ptr<Statistics::ContributingStochast> contributingStochast = contributingStochasts[id];

                if (property_ == "variable") return GetStochastId(std::static_pointer_cast<Statistics::Stochast>(contributingStochast->Stochast), newId);
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "variable") return GetStochastId(stochastSettings->stochast, newId);
            }
            else if (objectType == ObjectType::Alpha)
            {
                std::shared_ptr<Reliability::StochastPointAlpha> alpha = alphas[id];

                if (property_ == "variable")
                {
                    if (alpha->Stochast == nullptr) return 0;

                    std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = std::dynamic_pointer_cast<Reliability::FragilityCurve>(alpha->Stochast);
                    if (fragilityCurve == nullptr) return GetStochastId(alpha->Stochast, newId);
                    else return GetFragilityCurveId(fragilityCurve, newId);
                }
            }
            else if (objectType == ObjectType::UncertaintyResult)
            {
                std::shared_ptr<Uncertainty::UncertaintyResult> result = uncertaintyResults[id];

                if (property_ == "variable") return GetStochastId(result->stochast, newId);
            }
            else if (objectType == ObjectType::SensitivityValue)
            {
                std::shared_ptr<Sensitivity::SensitivityValue> result = sensitivityValues[id];

                if (property_ == "variable") return GetStochastId(result->stochast, newId);
            }
            else if (objectType == ObjectType::CombineProject)
            {
                std::shared_ptr<Reliability::CombineProject> combineProject = combineProjects[id];

                if (property_ == "design_point") return GetDesignPointId(combineProject->designPoint, newId);
            }
            else if (objectType == ObjectType::ExcludingCombineProject)
            {
                std::shared_ptr<Reliability::ExcludingCombineProject> combineProject = excludingCombineProjects[id];

                if (property_ == "design_point") return GetDesignPointId(combineProject->designPoint, newId);
            }
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> project = lengthEffectProjects[id];

                if (property_ == "design_point") return GetDesignPointId(project->designPoint, newId);
            }

            return 0;
        }

        void ProjectHandler::SetIntValue(int id, std::string property_, int value)
        {
            ObjectType objectType = types[id];

            if (IsModelProjectType(objectType))
            {
                std::shared_ptr<Models::ModelProject> project = GetProject(id);

                if (property_ == "settings") project->setSettings(GetSettings(value));
                else if (property_ == "correlation_matrix") project->correlationMatrix = correlationMatrices[value];
                else if (property_ == "share_project") project->shareStochasts(GetProject(value));
            }
            else if (objectType == ObjectType::FragilityCurveProject)
            {
                std::shared_ptr<Reliability::FragilityCurveProject> project = fragilityCurveProjects[id];

                if (property_ == "integrand") project->integrand = stochasts[value];
                else if (property_ == "fragility_curve") project->fragilityCurve = fragilityCurves[value];
            }
            else if (objectType == ObjectType::ModelParameter)
            {
                std::shared_ptr<Models::ModelInputParameter> parameter = modelParameters[id];

                if (property_ == "index") parameter->index = value;
                else if (property_ == "array_size") parameter->arraySize = value;
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "observations") stochast->getProperties()->Observations = value;
                else if (property_ == "array_size") stochast->modelParameter->arraySize = value;
                else if (property_ == "copy_from") stochast->copyFrom(stochasts[value]);
                else if (property_ == "conditional_source") stochast->VariableSource = stochasts[value];
                else if (property_ == "prior") tempIntValue = value;
            }
            else if (objectType == ObjectType::FragilityValue)
            {
                std::shared_ptr<Statistics::FragilityValue> fragilityValue = fragilityValues[id];

                if (property_ == "design_point")
                {
                    fragilityValue->designPoint = designPoints.contains(value) ? designPoints[value] : nullptr;
                    if (fragilityValue->designPoint != nullptr)
                    {
                        fragilityValue->Reliability = fragilityValue->designPoint->Beta;
                    }
                }
            }
            else if (objectType == ObjectType::ContributingStochast)
            {
                std::shared_ptr<Statistics::ContributingStochast> contributingStochast = contributingStochasts[id];

                if (property_ == "variable") contributingStochast->Stochast = stochasts[value];
            }
            else if (objectType == ObjectType::ConditionalValue)
            {
                std::shared_ptr<Statistics::VariableStochastValue> conditionalValue = conditionalValues[id];

                if (property_ == "observations") conditionalValue->Stochast->Observations = value;
            }
            else if (objectType == ObjectType::FragilityCurve)
            {
                std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = fragilityCurves[id];

                if (property_ == "copy_from") fragilityCurve->copyFrom(fragilityCurves[value]);
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "max_parallel_processes") settings->RunSettings->MaxParallelProcesses = value;
                else if (property_ == "minimum_samples") settings->MinimumSamples = value;
                else if (property_ == "maximum_samples") settings->MaximumSamples = value;
                else if (property_ == "minimum_iterations") settings->MinimumIterations = value;
                else if (property_ == "maximum_iterations") settings->MaximumIterations = value;
                else if (property_ == "minimum_directions") settings->MinimumDirections = value;
                else if (property_ == "maximum_directions") settings->MaximumDirections = value;
                else if (property_ == "minimum_variance_loops") settings->MinimumVarianceLoops = value;
                else if (property_ == "maximum_variance_loops") settings->MaximumVarianceLoops = value;
                else if (property_ == "random_seed") settings->RandomSettings->Seed = value;
                else if (property_ == "relaxation_loops") settings->RelaxationLoops = value;
                else if (property_ == "max_steps_sphere_search") settings->StartPointSettings->maxStepsSphereSearch = value;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

                if (property_ == "max_parallel_processes") settings->RunSettings->MaxParallelProcesses = value;
                else if (property_ == "iterations") settings->Iterations = value;
            }
            else if (objectType == ObjectType::UncertaintySettings)
            {
                std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

                if (property_ == "max_parallel_processes") settings->RunSettings->MaxParallelProcesses = value;
                else if (property_ == "minimum_samples") settings->MinimumSamples = value;
                else if (property_ == "maximum_samples") settings->MaximumSamples = value;
                else if (property_ == "maximum_iterations") settings->MaximumIterations = value;
                else if (property_ == "minimum_directions") settings->MinimumDirections = value;
                else if (property_ == "maximum_directions") settings->MaximumDirections = value;
                else if (property_ == "random_seed") settings->RandomSettings->Seed = value;
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "variable") stochastSettings->stochast = stochasts[value];
                else if (property_ == "intervals") stochastSettings->Intervals = value;
            }
            else if (objectType == ObjectType::CombineProject)
            {
                std::shared_ptr<Reliability::CombineProject> combineProject = combineProjects[id];

                if (property_ == "settings") combineProject->settings = combineSettingsValues[value];
                else if (property_ == "correlation_matrix") combineProject->selfCorrelationMatrix = selfCorrelationMatrices[value];
                else if (property_ == "design_point_correlation_matrix") combineProject->correlationMatrix = correlationMatrices[value];
            }
            else if (objectType == ObjectType::ExcludingCombineProject)
            {
                std::shared_ptr<Reliability::ExcludingCombineProject> combineProject = excludingCombineProjects[id];

                if (property_ == "settings") combineProject->settings = excludingCombineSettings[value];
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "ids") designPoint->Ids = GetDesignPointIds(value);
            }
            else if (objectType == ObjectType::Alpha)
            {
                std::shared_ptr<Reliability::StochastPointAlpha> alpha = alphas[id];

                if (property_ == "variable") alpha->Stochast = stochasts[value];
            }
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> project = lengthEffectProjects[id];

                if (property_ == "correlation_matrix") project->selfCorrelationMatrix = selfCorrelationMatrices[value];
                else if (property_ == "design_point_cross_section") project->designPointCrossSection = designPoints[value];
            }
        }

        double ProjectHandler::GetIntArgValue(int id1, int id2, std::string property_)
        {
            ObjectType objectType = types[id1];

            if (objectType == ObjectType::SelfCorrelationMatrix)
            {
                std::shared_ptr<Statistics::SelfCorrelationMatrix> correlationMatrix = selfCorrelationMatrices[id1];
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id2];

                if (property_ == "rho") return correlationMatrix->getSelfCorrelation(stochast);
            }
            return std::nan("");
        }

        void ProjectHandler::SetIntArgValue(int id1, int id2, std::string property_, double value)
        {
            ObjectType objectType = types[id1];

            if (objectType == ObjectType::SelfCorrelationMatrix)
            {
                std::shared_ptr<Statistics::SelfCorrelationMatrix> correlationMatrix = selfCorrelationMatrices[id1];
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id2];

                if (property_ == "rho") correlationMatrix->setSelfCorrelation(stochast, value);
            }
        }

        bool ProjectHandler::GetBoolValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "inverted") return stochast->isInverted();
                else if (property_ == "truncated") return stochast->isTruncated();
                else if (property_ == "conditional") return stochast->IsVariableStochast;
                else if (property_ == "can_fit_prior") return stochast->canFitPrior();
                else if (property_ == "is_array") return stochast->modelParameter->isArray;
                else if (property_ == "is_valid") return stochast->isValid();
                else if (property_ == "is_used_mean") return true;
                else if (property_ == "is_used_deviation") return stochast->getDistributionType() != DistributionType::Deterministic;
                else if (property_ == "is_used_location") return stochast->hasParameter(DistributionPropertyType::Location);
                else if (property_ == "is_used_scale") return stochast->hasParameter(DistributionPropertyType::Scale);
                else if (property_ == "is_used_minimum") return stochast->hasParameter(DistributionPropertyType::Minimum);
                else if (property_ == "is_used_maximum") return stochast->hasParameter(DistributionPropertyType::Maximum);
                else if (property_ == "is_used_shift") return stochast->hasParameter(DistributionPropertyType::Shift);
                else if (property_ == "is_used_shift_b") return stochast->hasParameter(DistributionPropertyType::ShiftB);
                else if (property_ == "is_used_shape") return stochast->hasParameter(DistributionPropertyType::Shape);
                else if (property_ == "is_used_shape_b") return stochast->hasParameter(DistributionPropertyType::ShapeB);
                else if (property_ == "is_used_observations") return stochast->hasParameter(DistributionPropertyType::Observations);
            }
            else if (objectType == ObjectType::ModelParameter)
            {
                std::shared_ptr<Models::ModelInputParameter> parameter = modelParameters[id];

                if (property_ == "is_array") return parameter->isArray;
            }
            else if (objectType == ObjectType::LimitStateFunction)
            {
                std::shared_ptr<Reliability::LimitStateFunction> limitStateFunction = limitStateFunctions[id];

                if (property_ == "use_compare_parameter") return limitStateFunction->useCompareParameter;
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "is_initialization_allowed") return stochastSettings->IsInitializationAllowed;
                else if (property_ == "is_variance_allowed") return stochastSettings->IsVarianceAllowed;

            }
            else if (IsModelProjectType(objectType))
            {
                std::shared_ptr<Models::ModelProject> project = GetProject(id);

                if (property_ == "is_valid") return project->isValid();
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "is_converged" && designPoint->convergenceReport != nullptr)
                {
                    return designPoint->convergenceReport->IsConverged;
                }
            }
            else if (objectType == ObjectType::UncertaintySettings)
            {
                std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

                if (property_ == "derive_samples_from_variation_coefficient") return settings->DeriveSamplesFromVariationCoefficient;
                else if (property_ == "calculate_correlations") return settings->CalculateCorrelations;
                else if (property_ == "calculate_input_correlations") return settings->CalculateInputCorrelations;
                else if (property_ == "is_repeatable_random") return settings->RandomSettings->IsRepeatableRandom;
                else if (property_ == "save_realizations") return settings->RunSettings->SaveEvaluations;
                else if (property_ == "save_convergence") return settings->RunSettings->SaveConvergence;
                else if (property_ == "save_messages") return settings->RunSettings->SaveMessages;
                else if (property_ == "reuse_calculations") return settings->RunSettings->ReuseCalculations;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

                if (property_ == "save_realizations") return settings->RunSettings->SaveEvaluations;
                else if (property_ == "save_convergence") return settings->RunSettings->SaveConvergence;
                else if (property_ == "save_messages") return settings->RunSettings->SaveMessages;
                else if (property_ == "reuse_calculations") return settings->RunSettings->ReuseCalculations;
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> setting = settingsValues[id];

                if (property_ == "all_quadrants") return setting->StartPointSettings->allQuadrants;
                else if (property_ == "is_repeatable_random") return setting->RandomSettings->IsRepeatableRandom;
                else if (property_ == "save_realizations") return setting->RunSettings->SaveEvaluations;
                else if (property_ == "save_convergence") return setting->RunSettings->SaveConvergence;
                else if (property_ == "save_messages") return setting->RunSettings->SaveMessages;
                else if (property_ == "reuse_calculations") return setting->RunSettings->ReuseCalculations;
            }
            else if (objectType == ObjectType::RunProjectSettings)
            {
                std::shared_ptr<Models::RunProjectSettings> settings = runProjectSettings[id];

                if (property_ == "reuse_calculations") return settings->RunSettings->ReuseCalculations;
                else if (property_ == "save_messages") return settings->RunSettings->SaveMessages;
            }
            else if (objectType == ObjectType::CombineProject)
            {
                std::shared_ptr<Reliability::CombineProject> project = combineProjects[id];

                if (property_ == "is_valid") return project->isValid();
            }
            else if (objectType == ObjectType::ExcludingCombineProject)
            {
                std::shared_ptr<Reliability::ExcludingCombineProject> project = excludingCombineProjects[id];

                if (property_ == "is_valid") return project->is_valid();
            }

            return false;
        }

        void ProjectHandler::SetBoolValue(int id, std::string property_, bool value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "inverted") stochast->setInverted(value);
                else if (property_ == "truncated") stochast->setTruncated(value);
                else if (property_ == "conditional") stochast->IsVariableStochast = value;
                else if (property_ == "is_array") stochast->modelParameter->isArray = value;
            }
            else if (objectType == ObjectType::ModelParameter)
            {
                std::shared_ptr<Models::ModelInputParameter> parameter = modelParameters[id];

                if (property_ == "is_array") parameter->isArray = value;
            }
            else if (objectType == ObjectType::LimitStateFunction)
            {
                std::shared_ptr<Reliability::LimitStateFunction> limitStateFunction = limitStateFunctions[id];

                if (property_ == "use_compare_parameter") limitStateFunction->useCompareParameter = value;
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "is_initialization_allowed") stochastSettings->IsInitializationAllowed = value;
                else if (property_ == "is_variance_allowed") stochastSettings->IsVarianceAllowed = value;
            }
            else if (objectType == ObjectType::UncertaintySettings)
            {
                std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

                if (property_ == "derive_samples_from_variation_coefficient") settings->DeriveSamplesFromVariationCoefficient = value;
                else if (property_ == "calculate_correlations") settings->CalculateCorrelations = value;
                else if (property_ == "calculate_input_correlations") settings->CalculateInputCorrelations = value;
                else if (property_ == "is_repeatable_random") settings->RandomSettings->IsRepeatableRandom = value;
                else if (property_ == "save_realizations") settings->RunSettings->SaveEvaluations = value;
                else if (property_ == "save_convergence") settings->RunSettings->SaveConvergence = value;
                else if (property_ == "save_messages") settings->RunSettings->SaveMessages = value;
                else if (property_ == "reuse_calculations") settings->RunSettings->ReuseCalculations = value;
                else if (property_ == "use_openmp_in_reliability") settings->RunSettings->UseOpenMPinReliability = value;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

                if (property_ == "save_realizations") settings->RunSettings->SaveEvaluations = value;
                else if (property_ == "save_convergence") settings->RunSettings->SaveConvergence = value;
                else if (property_ == "save_messages") settings->RunSettings->SaveMessages = value;
                else if (property_ == "reuse_calculations") settings->RunSettings->ReuseCalculations = value;
                else if (property_ == "use_openmp_in_reliability") settings->RunSettings->UseOpenMPinReliability = value;
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> setting = settingsValues[id];

                if (property_ == "all_quadrants") setting->StartPointSettings->allQuadrants = value;
                else if (property_ == "is_repeatable_random") setting->RandomSettings->IsRepeatableRandom = value;
                else if (property_ == "save_realizations") setting->RunSettings->SaveEvaluations = value;
                else if (property_ == "save_convergence") setting->RunSettings->SaveConvergence = value;
                else if (property_ == "save_messages") setting->RunSettings->SaveMessages = value;
                else if (property_ == "reuse_calculations") setting->RunSettings->ReuseCalculations = value;
                else if (property_ == "use_openmp_in_reliability") setting->RunSettings->UseOpenMPinReliability = value;
            }
            else if (objectType == ObjectType::RunProjectSettings)
            {
                std::shared_ptr<Models::RunProjectSettings> settings = runProjectSettings[id];

                if (property_ == "reuse_calculations") settings->RunSettings->ReuseCalculations = value;
                else if (property_ == "save_messages") settings->RunSettings->SaveMessages = value;
            }
        }

        std::string ProjectHandler::GetStringValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Message)
            {
                std::shared_ptr<Models::Message> message = messages[id];

                if (property_ == "type") return Message::getMessageTypeString(message->Type);
                else if (property_ == "text") return message->Text;
                else return "";
            }
            else if (objectType == ObjectType::ModelParameter)
            {
                std::shared_ptr<Models::ModelInputParameter> parameter = modelParameters[id];

                if (property_ == "name") return parameter->name;
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "distribution") return Stochast::getDistributionTypeString(stochast->getDistributionType());
                else if (property_ == "constant_parameter") return Stochast::getConstantParameterTypeString(stochast->constantParameterType);
                else if (property_ == "name") return stochast->name;
                else return "";
            }
            else if (objectType == ObjectType::FragilityCurve)
            {
                std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = fragilityCurves[id];

                if (property_ == "name") return fragilityCurve->name;
                else return "";
            }
            else if (objectType == ObjectType::LimitStateFunction)
            {
                std::shared_ptr<Reliability::LimitStateFunction> limitStateFunction = limitStateFunctions[id];

                if (property_ == "parameter") return limitStateFunction->criticalParameter;
                else if (property_ == "compare_parameter") return limitStateFunction->compareParameter;
                else if (property_ == "compare_type") return LimitStateFunction::GetCompareTypeString(limitStateFunction->compareType);
            }
            else if (objectType == ObjectType::Scenario)
            {
                std::shared_ptr<Statistics::Scenario> scenario = scenarios[id];

                if (property_ == "name") return scenario->name;
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "reliability_method") return Settings::getReliabilityMethodTypeString(settings->ReliabilityMethod);
                else if (property_ == "reliability_result") return Settings::getReliabilityResultTypeString(settings->ReliabilityResult);
                else if (property_ == "design_point_method") return DesignPointBuilder::getDesignPointMethodString(settings->designPointMethod);
                else if (property_ == "sample_method") return SubsetSimulationSettings::getSampleMethodString(settings->sampleMethod);
                else if (property_ == "start_method") return StartPointCalculatorSettings::getStartPointMethodString(settings->StartPointSettings->StartMethod);
                else if (property_ == "gradient_type") return GradientSettings::getGradientTypeString(settings->GradientSettings->gradientType);
                else if (property_ == "random_type") return Numeric::Random::getRandomGeneratorTypeString(settings->RandomSettings->RandomGeneratorType);
            }
            else if (objectType == ObjectType::RunProjectSettings)
            {
                std::shared_ptr<Models::RunProjectSettings> settings = runProjectSettings[id];

                if (property_ == "run_values_type") return Models::RunProjectSettings::getRunValuesTypeString(settings->runValuesType);
            }
            else if (objectType == ObjectType::UncertaintySettings)
            {
                std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

                if (property_ == "uncertainty_method") return Uncertainty::SettingsS::getUncertaintyMethodTypeString(settings->UncertaintyMethod);
                else if (property_ == "gradient_type") return GradientSettings::getGradientTypeString(settings->GradientSettings->gradientType);
            }
            else if (objectType == ObjectType::UncertaintyProject)
            {
                std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

                if (property_ == "parameter") return project->parameter;
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "parameter") return project->parameter;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

                if (property_ == "sensitivity_method") return Sensitivity::SensitivitySettings::getSensitivityMethodTypeString(settings->SensitivityMethod);
            }
            else if (objectType == ObjectType::CombineSettings)
            {
                std::shared_ptr<Reliability::CombineSettings> settings = combineSettingsValues[id];

                if (property_ == "combiner_method") return DesignPointCombiner::getCombinerMethodString(settings->combinerMethod);
                else if (property_ == "combine_type") return DesignPointCombiner::getCombineTypeString(settings->combineType);
            }
            else if (objectType == ObjectType::ExcludingCombineSettings)
            {
                std::shared_ptr<Reliability::ExcludingCombineSettings> settings = excludingCombineSettings[id];

                if (property_ == "combiner_method") return DesignPointCombiner::getExcludingCombinerMethodString(settings->combinerMethod);
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "identifier") return designPoint->Identifier;
            }
            else if (objectType == ObjectType::Alpha)
            {
                std::shared_ptr<Reliability::StochastPointAlpha> alpha = alphas[id];

                if (property_ == "identifier") return alpha->getIdentifier();
            }
            else if (objectType == ObjectType::UncertaintyResult)
            {
                std::shared_ptr<Uncertainty::UncertaintyResult> result = uncertaintyResults[id];

                if (property_ == "identifier") return result->getIdentifier();
            }
            else if (objectType == ObjectType::SensitivityResult)
            {
                std::shared_ptr<Sensitivity::SensitivityResult> result = sensitivityResults[id];

                if (property_ == "identifier") return result->identifier;
            }

            return "";
        }

        void ProjectHandler::SetStringValue(int id, std::string property_, std::string value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Message)
            {
                std::shared_ptr<Models::Message> message = messages[id];

                if (property_ == "type") message->Type = Message::getMessageType(value);
                else if (property_ == "text") message->Text = value;
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "distribution") stochast->setDistributionType(Stochast::getDistributionType(value));
                else if (property_ == "constant_parameter") stochast->constantParameterType = Stochast::getConstantParameterType(value);
                else if (property_ == "name") stochast->name = value;
            }
            else if (objectType == ObjectType::ModelParameter)
            {
                std::shared_ptr<Models::ModelInputParameter> parameter = modelParameters[id];

                if (property_ == "name") parameter->name = value;
            }
            else if (objectType == ObjectType::FragilityCurve)
            {
                std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = fragilityCurves[id];

                if (property_ == "name") fragilityCurve->name = value;
            }
            else if (objectType == ObjectType::LimitStateFunction)
            {
                std::shared_ptr<Reliability::LimitStateFunction> limitStateFunction = limitStateFunctions[id];

                if (property_ == "parameter") limitStateFunction->criticalParameter = value;
                else if (property_ == "compare_parameter") limitStateFunction->compareParameter = value;
                else if (property_ == "compare_type") limitStateFunction->compareType = LimitStateFunction::GetCompareType(value);
            }
            else if (objectType == ObjectType::Scenario)
            {
                std::shared_ptr<Statistics::Scenario> scenario = scenarios[id];

                if (property_ == "name") scenario->name = value;
            }
            else if (objectType == ObjectType::UncertaintyProject)
            {
                std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

                if (property_ == "parameter") project->parameter = value;
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "parameter") project->parameter = value;
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "reliability_method") settings->ReliabilityMethod = Settings::getReliabilityMethodType(value);
                else if (property_ == "reliability_result") settings->ReliabilityResult = Settings::getReliabilityResultType(value);
                else if (property_ == "design_point_method") settings->designPointMethod = DesignPointBuilder::getDesignPointMethod(value);
                else if (property_ == "sample_method") settings->sampleMethod = SubsetSimulationSettings::getSampleMethod(value);
                else if (property_ == "start_method") settings->StartPointSettings->StartMethod = StartPointCalculatorSettings::getStartPointMethod(value);
                else if (property_ == "gradient_type") settings->GradientSettings->gradientType = GradientSettings::getGradientType(value);
                else if (property_ == "random_type") settings->RandomSettings->RandomGeneratorType = Numeric::Random::getRandomGeneratorType(value);
            }
            else if (objectType == ObjectType::RunProjectSettings)
            {
                std::shared_ptr<Models::RunProjectSettings> settings = runProjectSettings[id];

                if (property_ == "run_values_type") settings->runValuesType = Models::RunProjectSettings::getRunValuesType(value);
            }
            else if (objectType == ObjectType::UncertaintySettings)
            {
                std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

                if (property_ == "uncertainty_method") settings->UncertaintyMethod = Uncertainty::SettingsS::getUncertaintyMethodType(value);
                else if (property_ == "gradient_type") settings->GradientSettings->gradientType = GradientSettings::getGradientType(value);
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SensitivitySettings> settings = sensitivitySettingsValues[id];

                if (property_ == "sensitivity_method") settings->SensitivityMethod = Sensitivity::SensitivitySettings::getSensitivityMethodType(value);
            }
            else if (objectType == ObjectType::CombineSettings)
            {
                std::shared_ptr<Reliability::CombineSettings> settings = combineSettingsValues[id];

                if (property_ == "combiner_method") settings->combinerMethod = DesignPointCombiner::getCombinerMethod(value);
                else if (property_ == "combine_type") settings->combineType = DesignPointCombiner::getCombineType(value);
            }
            else if (objectType == ObjectType::ExcludingCombineSettings)
            {
                std::shared_ptr<Reliability::ExcludingCombineSettings> settings = excludingCombineSettings[id];

                if (property_ == "combiner_method") settings->combinerMethod = DesignPointCombiner::getExcludingCombinerMethod(value);
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "identifier") designPoint->Identifier = value;
            }
            else if (IsModelProjectType(objectType))
            {
                std::shared_ptr<Models::ModelProject> project = GetProject(id);

                if (property_ == "model_name") project->model->name = value;
            }
        }

        void ProjectHandler::SetArrayValue(int id, std::string property_, double* values, int size)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "fit")
                {
                    std::vector<double> fitValues(size);
                    for (int i = 0; i < size; i++)
                    {
                        fitValues[i] = values[i];
                    }

                    double shift = argValue;
                    argValue = nan("");

                    stochast->fit(fitValues, shift);
                }
                else if (property_ == "fit_prior")
                {
                    std::vector<double> fitValues(size);
                    for (int i = 0; i < size; i++)
                    {
                        fitValues[i] = values[i];
                    }

                    double shift = argValue;
                    argValue = nan("");

                    stochast->fitPrior(fitValues, stochasts[tempIntValue], shift);
                    tempIntValue = 0;
                }
                else if (property_ == "data")
                {
                    std::vector<double> data;
                    for (size_t i = 0; i < size; i++)
                    {
                        data.push_back(values[i]);
                    }

                    tempValues["data"] = data;
                }
                else if (property_ == "u_and_x")
                {
                    std::vector<double> u_and_x;
                    for (size_t i = 0; i < size; i++)
                    {
                        u_and_x.push_back(values[i]);
                    }

                    tempValues["u_and_x"] = u_and_x;
                }
            }
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> lengthEffect = lengthEffectProjects[id];
                if (property_ == "correlation_lengths")
                {
                    lengthEffect->correlationLengths.clear();
                    for (size_t i = 0; i < size; i++)
                    {
                        lengthEffect->correlationLengths.push_back(values[i]);
                    }
                }
            }
        }

        std::vector<int> ProjectHandler::GetArrayIntValue(int id, std::string property_)
        {
            return std::vector<int>(0);
        }

        void ProjectHandler::SetArrayIntValue(int id, std::string property_, int* values, int size)
        {
            ObjectType objectType = types[id];

            if (IsModelProjectType(objectType))
            {
                std::shared_ptr<Models::ModelProject> project = GetProject(id);

                if (property_ == "variables")
                {
                    project->stochasts.clear();
                    for (int i = 0; i < size; i++)
                    {
                        project->stochasts.push_back(stochasts[values[i]]);
                    }
                }
                else if (property_ == "input_parameters")
                {
                    project->model->inputParameters.clear();
                    for (int i = 0; i < size; i++)
                    {
                        project->model->inputParameters.push_back(modelParameters[values[i]]);
                    }
                    project->updateStochasts();
                }
                else if (property_ == "output_parameters")
                {
                    project->model->outputParameters.clear();
                    for (int i = 0; i < size; i++)
                    {
                        project->model->outputParameters.push_back(modelParameters[values[i]]);
                    }
                }
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "discrete_values")
                {
                    stochast->getProperties()->setDirty();
                    stochast->getProperties()->DiscreteValues.clear();
                    for (int i = 0; i < size; i++)
                    {
                        stochast->getProperties()->DiscreteValues.push_back(discreteValues[values[i]]);
                    }
                }
                else if (property_ == "histogram_values")
                {
                    stochast->getProperties()->setDirty();
                    stochast->getProperties()->HistogramValues.clear();
                    for (int i = 0; i < size; i++)
                    {
                        stochast->getProperties()->HistogramValues.push_back(histogramValues[values[i]]);
                    }
                }
                else if (property_ == "fragility_values")
                {
                    stochast->getProperties()->setDirty();
                    stochast->getProperties()->FragilityValues.clear();
                    for (int i = 0; i < size; i++)
                    {
                        stochast->getProperties()->FragilityValues.push_back(fragilityValues[values[i]]);
                    }
                }
                else if (property_ == "contributing_stochasts")
                {
                    stochast->getProperties()->setDirty();
                    stochast->getProperties()->ContributingStochasts.clear();
                    for (int i = 0; i < size; i++)
                    {
                        stochast->getProperties()->ContributingStochasts.push_back(contributingStochasts[values[i]]);
                    }
                }
                else if (property_ == "conditional_values")
                {
                    stochast->ValueSet->StochastValues.clear();
                    for (int i = 0; i < size; i++)
                    {
                        stochast->ValueSet->StochastValues.push_back(conditionalValues[values[i]]);
                    }
                }
                else if (property_ == "array_variables")
                {
                    stochast->ArrayVariables.clear();
                    for (int i = 0; i < size; i++)
                    {
                        stochast->ArrayVariables.push_back(stochasts[values[i]]);
                    }
                }
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "alphas")
                {
                    designPoint->Alphas.clear();
                    for (int i = 0; i < size; i++)
                    {
                        designPoint->Alphas.push_back(alphas[values[i]]);
                    }
                }
            }
            else if (objectType == ObjectType::FragilityCurve)
            {
                std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = fragilityCurves[id];

                if (property_ == "fragility_values")
                {
                    fragilityCurve->getProperties()->setDirty();
                    fragilityCurve->getProperties()->FragilityValues.clear();
                    for (int i = 0; i < size; i++)
                    {
                        fragilityCurve->getProperties()->FragilityValues.push_back(fragilityValues[values[i]]);
                    }
                }
            }
            else if (objectType == ObjectType::CorrelationMatrix)
            {
                std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = correlationMatrices[id];

                if (property_ == "variables")
                {
                    std::vector<std::shared_ptr<Statistics::Stochast>> correlationMatrixStochasts;
                    for (int i = 0; i < size; i++)
                    {
                        correlationMatrixStochasts.push_back(stochasts[values[i]]);
                    }

                    correlationMatrix->init(correlationMatrixStochasts);
                }
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "stochast_settings")
                {
                    settings->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < size; i++)
                    {
                        settings->StochastSet->stochastSettings.push_back(stochastSettingsValues[values[i]]);
                    }
                }
            }
            else if (objectType == ObjectType::UncertaintySettings)
            {
                std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

                if (property_ == "stochast_settings")
                {
                    settings->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < size; i++)
                    {
                        settings->StochastSet->stochastSettings.push_back(stochastSettingsValues[values[i]]);
                    }
                }
                else if (property_ == "quantiles")
                {
                    settings->RequestedQuantiles.clear();
                    for (int i = 0; i < size; i++)
                    {
                        settings->RequestedQuantiles.push_back(probabilityValues[values[i]]);
                    }
                }
            }
            else if (objectType == ObjectType::CombineProject)
            {
                std::shared_ptr<Reliability::CombineProject> project = combineProjects[id];

                if (property_ == "design_points")
                {
                    project->designPoints.clear();

                    for (int i = 0; i < size; i++)
                    {
                        project->designPoints.push_back(designPoints[values[i]]);
                    }
                }
            }
            else if (objectType == ObjectType::ExcludingCombineProject)
            {
                std::shared_ptr<Reliability::ExcludingCombineProject> project = excludingCombineProjects[id];

                if (property_ == "design_points")
                {
                    project->designPoints.clear();

                    for (int i = 0; i < size; i++)
                    {
                        project->designPoints.push_back(designPoints[values[i]]);
                    }
                }
                else if (property_ == "scenarios")
                {
                    project->scenarios.clear();

                    for (int i = 0; i < size; i++)
                    {
                        project->scenarios.push_back(scenarios[values[i]]);
                    }
                }
            }
        }

        double ProjectHandler::GetArgValue(int id, std::string property_, double argument)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::StandardNormal)
            {
                if (property_ == "u_from_q") return StandardNormal::getUFromQ(argument);
                else if (property_ == "u_from_p") return StandardNormal::getUFromP(argument);
                else if (property_ == "q_from_u") return StandardNormal::getQFromU(argument);
                else if (property_ == "p_from_u") return StandardNormal::getPFromU(argument);
                else if (property_ == "t_from_p") return StandardNormal::getTFromP(argument);
                else if (property_ == "p_from_t") return StandardNormal::getPFromT(argument);
                else if (property_ == "t_from_u") return StandardNormal::getTFromU(argument);
                else if (property_ == "u_from_t") return StandardNormal::getUFromT(argument);
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "quantile") return stochast->getQuantile(argument);
                else if (property_ == "x_from_u") return stochast->getXFromU(argument);
                else if (property_ == "u_from_x") return stochast->getUFromX(argument);
                else if (property_ == "pdf") return stochast->getPDF(argument);
                else if (property_ == "cdf") return stochast->getCDF(argument);
            }
            else if (objectType == ObjectType::FragilityCurve)
            {
                std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = fragilityCurves[id];

                if (property_ == "quantile") return fragilityCurve->getQuantile(argument);
                else if (property_ == "x_from_u") return fragilityCurve->getXFromU(argument);
                else if (property_ == "u_from_x") return fragilityCurve->getUFromX(argument);
                else if (property_ == "pdf") return fragilityCurve->getPDF(argument);
                else if (property_ == "cdf") return fragilityCurve->getCDF(argument);
            }

            return std::nan("");
        }

        void ProjectHandler::SetArgValue(int id, std::string property_, double argument, double value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "x_at_u") stochast->setXAtU(value, argument, ConstantParameterType::VariationCoefficient);
            }
        }

        double ProjectHandler::GetIndexedValue(int id, std::string property_, int index)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                if (property_ == "special_values")
                {
                    return tempValues["special_values"][index];
                }
            }
            else if (objectType == ObjectType::FragilityCurve)
            {
                if (property_ == "special_values")
                {
                    return tempValues["special_values"][index];
                }
            }
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> lengthEffect = lengthEffectProjects[id];
                if (property_ == "correlation_lengths")
                {
                    return lengthEffect->correlationLengths[index];
                }
            }
            else if (objectType == ObjectType::Evaluation)
            {
                std::shared_ptr<Reliability::Evaluation> evaluation = evaluations[id];

                if (property_ == "input_values") return evaluation->InputValues[index];
                else if (property_ == "output_values") return evaluation->OutputValues[index];
            }

            return std::nan("");
        }

        void ProjectHandler::SetIndexedValue(int id, std::string property_, int index, double value)
        {
            // not needed yet
        }

        double ProjectHandler::GetIndexedIndexedValue(int id, std::string property_, int index1, int index2)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::CorrelationMatrix)
            {
                std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = correlationMatrices[id];

                if (property_ == "correlation")
                {
                    return correlationMatrix->GetCorrelation(stochasts[index1], stochasts[index2]);
                }
            }

            return std::nan("");
        }

        void ProjectHandler::SetIndexedIndexedValue(int id, std::string property_, int index1, int index2, double value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::CorrelationMatrix)
            {
                std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = correlationMatrices[id];

                if (property_ == "correlation")
                {
                    correlationMatrix->SetCorrelation(stochasts[index1], stochasts[index2], value);
                }
            }
        }

        int ProjectHandler::GetIndexedIntValue(int id, std::string property_, int index)
        {
            return 0;
        }

        int ProjectHandler::GetIndexedIdValue(int id, std::string property_, int index, int newId)
        {
            ObjectType objectType = types[id];

            if (IsModelProjectType(objectType))
            {
                std::shared_ptr<Models::ModelProject> project = GetProject(id);

                if (property_ == "stochasts") return GetStochastId(project->stochasts[index], newId);
            }

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "histogram_values") return GetHistogramValueId(stochast->getProperties()->HistogramValues[index], newId);
                else if (property_ == "discrete_values") return GetDiscreteValueId(stochast->getProperties()->DiscreteValues[index], newId);
                else if (property_ == "fragility_values") return GetFragilityValueId(stochast->getProperties()->FragilityValues[index], newId);
                else if (property_ == "contributing_stochasts") return GetContributingStochastId(stochast->getProperties()->ContributingStochasts[index], newId);
                else if (property_ == "conditional_values") return GetConditionalValueId(stochast->ValueSet->StochastValues[index], newId);
                else if (property_ == "array_variables") return GetStochastId(stochast->ArrayVariables[index], newId);
            }
            else if (objectType == ObjectType::FragilityCurve)
            {
                std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = fragilityCurves[id];

                if (property_ == "fragility_values") return GetFragilityValueId(fragilityCurve->getProperties()->FragilityValues[index], newId);
            }
            else if (objectType == ObjectType::CorrelationMatrix)
            {
                std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = correlationMatrices[id];

                if (property_ == "variables") return GetStochastId(correlationMatrix->getStochast(index), newId);
            }
            else if (objectType == ObjectType::UncertaintyProject)
            {
                std::shared_ptr<Uncertainty::UncertaintyProject> project = uncertaintyProjects[id];

                if (property_ == "uncertainty_stochasts") return GetStochastId(project->uncertaintyResults[index]->stochast, newId);
                else if (property_ == "uncertainty_results") return GetUncertaintyResultId(project->uncertaintyResults[index], newId);
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "results") return GetSensitivityResultId(project->sensitivityResults[index], newId);
            }
            else if (objectType == ObjectType::SensitivityResult)
            {
                std::shared_ptr<Sensitivity::SensitivityResult> result = sensitivityResults[id];

                if (property_ == "values") return GetSensitivityValueId(result->values[index], newId);
                else if (property_ == "evaluations") return GetEvaluationId(result->evaluations[index], newId);
                else if (property_ == "messages") return GetMessageId(result->messages[index], newId);
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "contributing_design_points") return GetDesignPointId(designPoint->ContributingDesignPoints[index], newId);
                else if (property_ == "alphas") return GetAlphaId(designPoint->Alphas[index], newId);
                else if (property_ == "evaluations") return GetEvaluationId(designPoint->Evaluations[index], newId);
                else if (property_ == "reliability_results") return GetReliabilityResultId(designPoint->ReliabililityResults[index], newId);
                else if (property_ == "messages") return GetMessageId(designPoint->Messages[index], newId);
            }
            else if (objectType == ObjectType::UncertaintySettings)
            {
                std::shared_ptr<Uncertainty::SettingsS> settings = uncertaintySettingsValues[id];

                if (property_ == "quantiles") return GetProbabilityValueId(settings->RequestedQuantiles[index], newId);
            }
            else if (objectType == ObjectType::UncertaintyResult)
            {
                std::shared_ptr<Uncertainty::UncertaintyResult> result = uncertaintyResults[id];

                if (property_ == "evaluations") return GetEvaluationId(result->evaluations[index], newId);
                else if (property_ == "quantile_evaluations") return GetEvaluationId(result->quantileEvaluations[index], newId);
                else if (property_ == "messages") return GetMessageId(result->messages[index], newId);
            }
            else if (objectType == ObjectType::ExcludingCombineProject)
            {
                if (property_ == "validation_messages") return GetMessageId(validationMessages[index], newId);
            }

            return 0;
        }

        void ProjectHandler::SetCallBack(int id, std::string property_, ZValuesCallBack callBack)
        {
            ObjectType objectType = types[id];

            if (IsModelProjectType(objectType))
            {
                std::shared_ptr<Models::ModelProject> project = GetProject(id);

                if (property_ == "model") project->model = std::make_shared<ZModel>(callBack);
            }
        }

        void ProjectHandler::SetMultipleCallBack(int id, std::string property_, ZValuesMultipleCallBack callBack)
        {
            ObjectType objectType = types[id];

            if (IsModelProjectType(objectType))
            {
                std::shared_ptr<Models::ModelProject> project = GetProject(id);

                if (property_ == "model") project->model->setMultipleCallback(callBack);
            }
        }

        void ProjectHandler::SetEmptyCallBack(int id, std::string property_, EmptyCallBack callBack)
        {
            ObjectType objectType = types[id];

            if (IsModelProjectType(objectType))
            {
                std::shared_ptr<Models::ModelProject> project = GetProject(id);

                if (property_ == "run_samples") project->model->setRunMethod(callBack);
            }
        }

        void ProjectHandler::Execute(int id, std::string method_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (method_ == "initialize_for_run") stochast->initializeForRun();
                else if (method_ == "initialize_conditional_values") stochast->initializeConditionalValues();
            }
            else if (IsModelProjectType(objectType))
            {
                std::shared_ptr<Models::ModelProject> project = GetProject(id);

                if (method_ == "run") project->run();
            }
            else if (objectType == ObjectType::FragilityCurveProject)
            {
                std::shared_ptr<Reliability::FragilityCurveProject> project = fragilityCurveProjects[id];

                if (method_ == "run") project->run();
            }
            else if (objectType == ObjectType::CombineProject)
            {
                std::shared_ptr<Reliability::CombineProject> project = combineProjects[id];

                if (method_ == "run") project->run();
            }
            else if (objectType == ObjectType::ExcludingCombineProject)
            {
                std::shared_ptr<Reliability::ExcludingCombineProject> project = excludingCombineProjects[id];

                if (method_ == "run") project->run();
                else if (method_ == "validate") UpdateValidationMessages(project->validate());
                else if (method_ == "clear_validate") validationMessages.clear();
            }
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> project = lengthEffectProjects[id];

                if (method_ == "run") project->run();
            }
        }

        int ProjectHandler::GetProbabilityValueId(std::shared_ptr<Statistics::ProbabilityValue> probability, int newId)
        {
            if (probability == nullptr)
            {
                return 0;
            }
            else
            {
                if (!probabilityValueIds.contains(probability))
                {
                    probabilityValues[newId] = probability;
                    types[newId] = ObjectType::ProbabilityValue;
                    probabilityValueIds[probability] = newId;
                }

                return probabilityValueIds[probability];
            }
        }

        int ProjectHandler::GetStochastId(std::shared_ptr<Statistics::Stochast> stochast, int newId)
        {
            if (stochast == nullptr)
            {
                return 0;
            }
            else
            {
                if (!stochastIds.contains(stochast))
                {
                    stochasts[newId] = stochast;
                    types[newId] = ObjectType::Stochast;
                    stochastIds[stochast] = newId;
                }

                return stochastIds[stochast];
            }
        }

        int ProjectHandler::GetFragilityCurveId(std::shared_ptr<Reliability::FragilityCurve> fragilityCurve, int newId)
        {
            if (fragilityCurve == nullptr)
            {
                return 0;
            }
            else
            {
                if (!fragilityCurveIds.contains(fragilityCurve))
                {
                    fragilityCurves[newId] = fragilityCurve;
                    types[newId] = ObjectType::FragilityCurve;
                    fragilityCurveIds[fragilityCurve] = newId;
                }

                return fragilityCurveIds[fragilityCurve];
            }
        }

        int ProjectHandler::GetCorrelationMatrixId(std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, int newId)
        {
            if (correlationMatrix == nullptr)
            {
                return 0;
            }
            else
            {
                if (!correlationMatrixIds.contains(correlationMatrix))
                {
                    correlationMatrices[newId] = correlationMatrix;
                    types[newId] = ObjectType::CorrelationMatrix;
                    correlationMatrixIds[correlationMatrix] = newId;
                }

                return correlationMatrixIds[correlationMatrix];
            }
        }

        int ProjectHandler::GetLimitStateFunctionId(std::shared_ptr<Reliability::LimitStateFunction> limitStateFunction, int newId)
        {
            if (limitStateFunction == nullptr)
            {
                return 0;
            }
            else
            {
                if (!limitStateFunctionIds.contains(limitStateFunction))
                {
                    limitStateFunctions[newId] = limitStateFunction;
                    types[newId] = ObjectType::LimitStateFunction;
                    limitStateFunctionIds[limitStateFunction] = newId;
                }

                return limitStateFunctionIds[limitStateFunction];
            }
        }

        int ProjectHandler::GetDesignPointId(std::shared_ptr<Reliability::DesignPoint> designPoint, int newId)
        {
            if (designPoint == nullptr)
            {
                return 0;
            }
            else
            {
                if (!designPointIds.contains(designPoint))
                {
                    designPoints[newId] = designPoint;
                    types[newId] = ObjectType::DesignPoint;
                    designPointIds[designPoint] = newId;
                }

                return designPointIds[designPoint];
            }
        }

        int ProjectHandler::GetAlphaId(std::shared_ptr<StochastPointAlpha> alpha, int newId)
        {
            if (!alphaIds.contains(alpha))
            {
                alphas[newId] = alpha;
                types[newId] = ObjectType::Alpha;
                alphaIds[alpha] = newId;
            }

            return alphaIds[alpha];
        }

        int ProjectHandler::GetUncertaintyResultId(std::shared_ptr<Uncertainty::UncertaintyResult> result, int newId)
        {
            if (result == nullptr)
            {
                return 0;
            }
            else
            {
                if (!uncertaintyResultsIds.contains(result))
                {
                    uncertaintyResults[newId] = result;
                    types[newId] = ObjectType::UncertaintyResult;
                    uncertaintyResultsIds[result] = newId;
                }

                return uncertaintyResultsIds[result];
            }
        }

        int ProjectHandler::GetSensitivityResultId(std::shared_ptr<Sensitivity::SensitivityResult> result, int newId)
        {
            if (result == nullptr)
            {
                return 0;
            }
            else
            {
                if (!sensitivityResultsIds.contains(result))
                {
                    sensitivityResults[newId] = result;
                    types[newId] = ObjectType::SensitivityResult;
                    sensitivityResultsIds[result] = newId;
                }

                return sensitivityResultsIds[result];
            }
        }

        int ProjectHandler::GetSensitivityValueId(std::shared_ptr<Sensitivity::SensitivityValue> result, int newId)
        {
            if (result == nullptr)
            {
                return 0;
            }
            else
            {
                if (!sensitivityValuesIds.contains(result))
                {
                    sensitivityValues[newId] = result;
                    types[newId] = ObjectType::SensitivityValue;
                    sensitivityValuesIds[result] = newId;
                }

                return sensitivityValuesIds[result];
            }
        }

        int ProjectHandler::GetHistogramValueId(std::shared_ptr<Statistics::HistogramValue> histogramValue, int newId)
        {
            if (!histogramValueIds.contains(histogramValue))
            {
                histogramValues[newId] = histogramValue;
                types[newId] = ObjectType::HistogramValue;
                histogramValueIds[histogramValue] = newId;
            }

            return histogramValueIds[histogramValue];
        }

        int ProjectHandler::GetDiscreteValueId(std::shared_ptr<Statistics::DiscreteValue> discreteValue, int newId)
        {
            if (!discreteValueIds.contains(discreteValue))
            {
                discreteValues[newId] = discreteValue;
                types[newId] = ObjectType::DiscreteValue;
                discreteValueIds[discreteValue] = newId;
            }

            return discreteValueIds[discreteValue];
        }

        int ProjectHandler::GetFragilityValueId(std::shared_ptr<Statistics::FragilityValue> fragilityValue, int newId)
        {
            if (!fragilityValueIds.contains(fragilityValue))
            {
                fragilityValues[newId] = fragilityValue;
                types[newId] = ObjectType::FragilityValue;
                fragilityValueIds[fragilityValue] = newId;
            }

            return fragilityValueIds[fragilityValue];
        }

        int ProjectHandler::GetContributingStochastId(std::shared_ptr<Statistics::ContributingStochast> contributingStochast, int newId)
        {
            if (!contributingStochastIds.contains(contributingStochast))
            {
                contributingStochasts[newId] = contributingStochast;
                types[newId] = ObjectType::FragilityValue;
                contributingStochastIds[contributingStochast] = newId;
            }

            return contributingStochastIds[contributingStochast];
        }

        int ProjectHandler::GetConditionalValueId(std::shared_ptr<Statistics::VariableStochastValue> conditionalValue, int newId)
        {
            if (!conditionalValueIds.contains(conditionalValue))
            {
                conditionalValues[newId] = conditionalValue;
                types[newId] = ObjectType::ConditionalValue;
                conditionalValueIds[conditionalValue] = newId;
            }

            return conditionalValueIds[conditionalValue];
        }

        int ProjectHandler::GetEvaluationId(std::shared_ptr<Deltares::Reliability::Evaluation> evaluation, int newId)
        {
            if (!evaluationIds.contains(evaluation))
            {
                evaluations[newId] = evaluation;
                types[newId] = ObjectType::Evaluation;
                evaluationIds[evaluation] = newId;
            }

            return evaluationIds[evaluation];
        }

        int ProjectHandler::GetReliabilityResultId(std::shared_ptr<Deltares::Reliability::ReliabilityResult> result, int newId)
        {
            if (!reliabilityResultIds.contains(result))
            {
                reliabilityResults[newId] = result;
                types[newId] = ObjectType::ReliabilityResult;
                reliabilityResultIds[result] = newId;
            }

            return reliabilityResultIds[result];
        }

        int ProjectHandler::GetMessageId(std::shared_ptr<Deltares::Models::Message> message, int newId)
        {
            if (!messageIds.contains(message))
            {
                messages[newId] = message;
                types[newId] = ObjectType::Message;
                messageIds[message] = newId;
            }

            return messageIds[message];
        }

        std::shared_ptr<Reliability::DesignPointIds> ProjectHandler::GetDesignPointIds(int id)
        {
            return nullptr;
        }

        std::shared_ptr<Models::ModelProject> ProjectHandler::GetProject(int id)
        {
            if (projects.contains(id))
            {
                return projects[id];
            }
            else if (runProjects.contains(id))
            {
                return runProjects[id];
            }
            else if (uncertaintyProjects.contains(id))
            {
                return uncertaintyProjects[id];
            }
            else if (sensitivityProjects.contains(id))
            {
                return sensitivityProjects[id];
            }
            else
            {
                return nullptr;
            }
        }

        std::shared_ptr<Models::ModelProjectSettings> ProjectHandler::GetSettings(int id)
        {
            if (settingsValues.contains(id))
            {
                return settingsValues[id];
            }
            else if (runProjectSettings.contains(id))
            {
                return runProjectSettings[id];
            }
            else if (uncertaintySettingsValues.contains(id))
            {
                return uncertaintySettingsValues[id];
            }
            else if (sensitivitySettingsValues.contains(id))
            {
                return sensitivitySettingsValues[id];
            }
            else
            {
                return nullptr;
            }
        }

        bool ProjectHandler::IsModelProjectType(ObjectType objectType)
        {
            return objectType == ObjectType::Project ||
                objectType == ObjectType::RunProject ||
                objectType == ObjectType::UncertaintyProject ||
                objectType == ObjectType::SensitivityProject;
        }

        void ProjectHandler::UpdateValidationMessages(const std::vector<std::shared_ptr<Models::Message>>& newMessages)
        {
            validationMessages.clear();
            for (const std::shared_ptr<Models::Message>& message : newMessages)
            {
                validationMessages.push_back(message);
            }
        }
    }
}

