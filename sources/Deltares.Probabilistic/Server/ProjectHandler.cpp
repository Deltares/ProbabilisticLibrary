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
// Stichting Deltares and remain full property of Stichting Deltares at all times
// All rights reserved 

# include "ProjectHandler.h"
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
                object_type == "message" ||
                object_type == "project" ||
                object_type == "model_parameter" ||
                object_type == "stochast" ||
                object_type == "discrete_value" ||
                object_type == "histogram_value" ||
                object_type == "fragility_value" ||
                object_type == "contributing_stochast" ||
                object_type == "conditional_value" ||
                object_type == "correlation_matrix" ||
                object_type == "settings" ||
                object_type == "stochast_settings" ||
                object_type == "design_point" ||
                object_type == "alpha" ||
                object_type == "combine_project" ||
                object_type == "combine_settings" ||
                object_type == "self_correlation_matrix" ||
                object_type == "sensitivity_project" ||
                object_type == "sensitivity_settings" ||
                object_type == "length_effect_project");
        }

        void ProjectHandler::Create(std::string object_type, int id)
        {
            if (object_type == "standard_normal")
            {
                types[id] = ObjectType::StandardNormal;
            }
            else if (object_type == "message")
            {
                messages[id] = std::make_shared<Deltares::Models::Message>();
                messageIds[messages[id]] = id;
                types[id] = ObjectType::Message;
            }
            else if (object_type == "project")
            {
                projects[id] = std::make_shared<Deltares::Reliability::ReliabilityProject>();
                types[id] = ObjectType::Project;
            }
            else if (object_type == "model_parameter")
            {
                modelParameters[id] = std::make_shared<Deltares::Models::ModelInputParameter>();
                types[id] = ObjectType::ModelParameter;
            }
            else if (object_type == "stochast")
            {
                stochasts[id] = std::make_shared<Deltares::Statistics::Stochast>();
                stochastIds[stochasts[id]] = id;
                types[id] = ObjectType::Stochast;
            }
            else if (object_type == "discrete_value")
            {
                discreteValues[id] = std::make_shared<Deltares::Statistics::DiscreteValue>();
                discreteValueIds[discreteValues[id]] = id;
                types[id] = ObjectType::DiscreteValue;
            }
            else if (object_type == "histogram_value")
            {
                histogramValues[id] = std::make_shared<Deltares::Statistics::HistogramValue>();
                histogramValueIds[histogramValues[id]] = id;
                types[id] = ObjectType::HistogramValue;
            }
            else if (object_type == "fragility_value")
            {
                fragilityValues[id] = std::make_shared<Deltares::Statistics::FragilityValue>();
                fragilityValueIds[fragilityValues[id]] = id;
                types[id] = ObjectType::FragilityValue;
            }
            else if (object_type == "contributing_stochast")
            {
                contributingStochasts[id] = std::make_shared<Deltares::Statistics::ContributingStochast>();
                contributingStochastIds[contributingStochasts[id]] = id;
                types[id] = ObjectType::ContributingStochast;
            }
            else if (object_type == "conditional_value")
            {
                conditionalValues[id] = std::make_shared<Deltares::Statistics::VariableStochastValue>();
                conditionalValueIds[conditionalValues[id]] = id;
                types[id] = ObjectType::ConditionalValue;
            }
            else if (object_type == "correlation_matrix")
            {
                correlationMatrices[id] = std::make_shared<Deltares::Statistics::CorrelationMatrix>();
                correlationMatrixIds[correlationMatrices[id]] = id;
                types[id] = ObjectType::CorrelationMatrix;
            }
            else if (object_type == "settings")
            {
                settingsValues[id] = std::make_shared<Deltares::Reliability::Settings>();
                types[id] = ObjectType::Settings;
            }
            else if (object_type == "stochast_settings")
            {
                stochastSettingsValues[id] = std::make_shared<Deltares::Reliability::StochastSettings>();
                types[id] = ObjectType::StochastSettings;
            }
            else if (object_type == "design_point")
            {
                designPoints[id] = std::make_shared<Deltares::Reliability::DesignPoint>();
                designPointIds[designPoints[id]] = id;
                types[id] = ObjectType::DesignPoint;
            }
            else if (object_type == "alpha")
            {
                alphas[id] = std::make_shared<Deltares::Reliability::StochastPointAlpha>();
                alphaIds[alphas[id]] = id;
                types[id] = ObjectType::Alpha;
            }
            else if (object_type == "combine_project")
            {
                combineProjects[id] = std::make_shared<Deltares::Reliability::CombineProject>();
                types[id] = ObjectType::CombineProject;
            }
            else if (object_type == "combine_settings")
            {
                combineSettingsValues[id] = std::make_shared<Deltares::Reliability::CombineSettings>();
                types[id] = ObjectType::CombineSettings;
            }
            else if (object_type == "self_correlation_matrix")
            {
                selfCorrelationMatrices[id] = std::make_shared<Deltares::Statistics::SelfCorrelationMatrix>();
                types[id] = ObjectType::SelfCorrelationMatrix;
            }
            else if (object_type == "sensitivity_project")
            {
                sensitivityProjects[id] = std::make_shared<Deltares::Sensitivity::SensitivityProject>();
                types[id] = ObjectType::SensitivityProject;
            }
            else if (object_type == "sensitivity_settings")
            {
                sensitivitySettingsValues[id] = std::make_shared<Deltares::Sensitivity::SettingsS>();
                types[id] = ObjectType::SensitivitySettings;
            }
            else if (object_type == "length_effect_project")
            {
                lengthEffectProjects[id] = std::make_shared<Deltares::Reliability::LengthEffectProject>();
                types[id] = ObjectType::LengthEffectProject;
            }
        }

        void ProjectHandler::Destroy(int id)
        {
            switch (types[id])
            {
            case ObjectType::StandardNormal: break;
            case ObjectType::Message: messages.erase(id); break;
            case ObjectType::Project: projects.erase(id); break;
            case ObjectType::ModelParameter: modelParameters.erase(id); break;
            case ObjectType::Stochast: stochasts.erase(id); break;
            case ObjectType::DiscreteValue: discreteValues.erase(id); break;
            case ObjectType::HistogramValue: histogramValues.erase(id); break;
            case ObjectType::FragilityValue: fragilityValues.erase(id); break;
            case ObjectType::ContributingStochast: contributingStochasts.erase(id); break;
            case ObjectType::CorrelationMatrix: correlationMatrices.erase(id); break;
            case ObjectType::Settings: settingsValues.erase(id); break;
            case ObjectType::StochastSettings: stochastSettingsValues.erase(id); break;
            case ObjectType::DesignPoint: designPoints.erase(id); break;
            case ObjectType::Alpha: alphas.erase(id); break;
            case ObjectType::CombineProject: combineProjects.erase(id); break;
            case ObjectType::CombineSettings: combineSettingsValues.erase(id); break;
            case ObjectType::SelfCorrelationMatrix: selfCorrelationMatrices.erase(id); break;
            case ObjectType::SensitivityProject: sensitivityProjects.erase(id); break;
            case ObjectType::SensitivitySettings: sensitivitySettingsValues.erase(id); break;
            default: throw probLibException("object type");
            }
            types.erase(id);
        }

        double ProjectHandler::GetValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
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
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "relaxation_factor") return settings->RelaxationFactor;
                else if (property_ == "variation_coefficient") return settings->VariationCoefficient;
                else if (property_ == "fraction_failed") return settings->FractionFailed;
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "min_value") return stochastSettings->MinValue;
                else if (property_ == "max_value") return stochastSettings->MaxValue;
                else if (property_ == "start_value") return stochastSettings->StartValue;
                else if (property_ == "variation_factor") return stochastSettings->VarianceFactor;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SettingsS> settings = sensitivitySettingsValues[id];

                if (property_ == "relaxation_factor") return settings->RelaxationFactor;
                else if (property_ == "variation_coefficient") return settings->VariationCoefficient;
                else if (property_ == "probability_for_convergence") return settings->ProbabilityForConvergence;
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "reliability_index") return designPoint->Beta;
                else if (property_ == "probability_failure") return designPoint->getFailureProbability();
                else if (property_ == "convergence") return designPoint->convergenceReport->Convergence;
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
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> length_effect = lengthEffectProjects[id];

                if (property_ == "length") return length_effect->length;
                else if (property_ == "minimum_failure_length") return length_effect->minimumFailureLength;
            }
            return std::nan("");
        }

        void ProjectHandler::SetValue(int id, std::string property_, double value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "location") stochast->getProperties()->Location = value;
                else if (property_ == "scale") stochast->getProperties()->Scale = value;
                else if (property_ == "shape") stochast->getProperties()->Shape = value;
                else if (property_ == "shape_b") stochast->getProperties()->ShapeB = value;
                else if (property_ == "shift") stochast->getProperties()->Shift = value;
                else if (property_ == "shift_b") stochast->getProperties()->ShiftB = value;
                else if (property_ == "minimum") stochast->getProperties()->Minimum = value;
                else if (property_ == "maximum") stochast->getProperties()->Maximum = value;
                else if (property_ == "mean") stochast->setMean(value);
                else if (property_ == "deviation") stochast->setDeviation(value);
                else if (property_ == "variation") stochast->setVariation(value);
                else if (property_ == "design_quantile") stochast->designQuantile = value;
                else if (property_ == "design_factor") stochast->designFactor = value;
                else if (property_ == "design_value") stochast->setDesignValue(value);
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
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "relaxation_factor") settings->RelaxationFactor = value;
                else if (property_ == "variation_coefficient") settings->VariationCoefficient = value;
                else if (property_ == "fraction_failed") settings->FractionFailed = value;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SettingsS> settings = sensitivitySettingsValues[id];

                if (property_ == "relaxation_factor") settings->RelaxationFactor = value;
                else if (property_ == "variation_coefficient") settings->VariationCoefficient = value;
                else if (property_ == "probability_for_convergence") settings->ProbabilityForConvergence = value;
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
                else if (property_ == "minimum_failure_length") length_effect->minimumFailureLength = value;
            }
        }

        int ProjectHandler::GetIntValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Reliability::ReliabilityProject> project = projects[id];

                if (property_ == "design_point") return GetDesignPointId(project->designPoint);
                else if (property_ == "index") return project->model->Index;
            }
            else if (objectType == ObjectType::ModelParameter)
            {
                std::shared_ptr<Models::ModelInputParameter> parameter = modelParameters[id];

                if (property_ == "index") return parameter->index;
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "sensitivity_stochast") return GetStochastId(project->sensitivityStochast);
                else if (property_ == "sensitivity_stochasts_count") return (int)project->sensitivityStochasts.size();
                else if (property_ == "output_correlation_matrix") return GetCorrelationMatrixId(project->outputCorrelationMatrix);
                else if (property_ == "index") return project->model->Index;
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "observations") return stochast->getProperties()->Observations;
                else if (property_ == "conditional_source") return GetStochastId(stochast->VariableSource);
                else if (property_ == "histogram_values_count") return (int)stochast->getProperties()->HistogramValues.size();
                else if (property_ == "discrete_values_count") return (int)stochast->getProperties()->DiscreteValues.size();
                else if (property_ == "fragility_values_count") return (int)stochast->getProperties()->FragilityValues.size();
                else if (property_ == "contributing_stochasts_count") return (int)stochast->getProperties()->ContributingStochasts.size();
                else if (property_ == "conditional_values_count") return (int)stochast->ValueSet->StochastValues.size();
            }
            else if (objectType == ObjectType::FragilityValue)
            {
                std::shared_ptr<Statistics::FragilityValue> fragilityValue = fragilityValues[id];

                if (property_ == "design_point") return GetDesignPointId(std::static_pointer_cast<Reliability::DesignPoint>(fragilityValue->designPoint));
            }
            else if (objectType == ObjectType::ContributingStochast)
            {
                std::shared_ptr<Statistics::ContributingStochast> contributingStochast = contributingStochasts[id];

                if (property_ == "variable") return GetStochastId(std::static_pointer_cast<Statistics::Stochast>(contributingStochast->Stochast));
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
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "minimum_samples") return settings->MinimumSamples;
                else if (property_ == "maximum_samples") return settings->MaximumSamples;
                else if (property_ == "maximum_iterations") return settings->MaximumIterations;
                else if (property_ == "minimum_directions") return settings->MinimumDirections;
                else if (property_ == "maximum_directions") return settings->MaximumDirections;
                else if (property_ == "minimum_variance_loops") return settings->MinimumVarianceLoops;
                else if (property_ == "maximum_variance_loops") return settings->MaximumVarianceLoops;
                else if (property_ == "relaxation_loops") return settings->RelaxationLoops;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SettingsS> settings = sensitivitySettingsValues[id];

                if (property_ == "minimum_samples") return settings->MinimumSamples;
                else if (property_ == "maximum_samples") return settings->MaximumSamples;
                else if (property_ == "maximum_iterations") return settings->MaximumIterations;
                else if (property_ == "minimum_directions") return settings->MinimumDirections;
                else if (property_ == "maximum_directions") return settings->MaximumDirections;
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "variable") return GetStochastId(stochastSettings->stochast);
                else if (property_ == "intervals") return stochastSettings->Intervals;
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "contributing_design_points_count") return (int)designPoint->ContributingDesignPoints.size();
                else if (property_ == "alphas_count") return (int)designPoint->Alphas.size();
                else if (property_ == "total_iterations") return designPoint->convergenceReport->TotalIterations;
                else if (property_ == "total_directions") return designPoint->convergenceReport->TotalDirections;
                else if (property_ == "total_model_runs") return designPoint->convergenceReport->TotalModelRuns;
                else if (property_ == "messages_count") return (int)designPoint->Messages.size();
            }
            else if (objectType == ObjectType::Alpha)
            {
                std::shared_ptr<Reliability::StochastPointAlpha> alpha = alphas[id];

                if (property_ == "variable") return GetStochastId(alpha->Stochast);
            }
            else if (objectType == ObjectType::CombineProject)
            {
                std::shared_ptr<Reliability::CombineProject> combineProject = combineProjects[id];

                if (property_ == "design_point") return GetDesignPointId(combineProject->designPoint);
            }
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> project = lengthEffectProjects[id];

                if (property_ == "design_point") return GetDesignPointId(project->designPoint);
                else if (property_ == "correlation_lengths_count") return project->correlationLengths.size();
            }

            return 0;
        }

        void ProjectHandler::SetIntValue(int id, std::string property_, int value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Reliability::ReliabilityProject> project = projects[id];

                if (property_ == "settings") project->settings = settingsValues[value];
                else if (property_ == "correlation_matrix") project->correlationMatrix = correlationMatrices[value];
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "settings") project->settings = sensitivitySettingsValues[value];
                else if (property_ == "correlation_matrix") project->correlationMatrix = correlationMatrices[value];
            }
            else if (objectType == ObjectType::ModelParameter)
            {
                std::shared_ptr<Models::ModelInputParameter> parameter = modelParameters[id];

                if (property_ == "index") parameter->index = value;
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "observations") stochast->getProperties()->Observations = value;
                else if (property_ == "copy_from") stochast->copyFrom(stochasts[value]);
                else if (property_ == "conditional_source") stochast->VariableSource = stochasts[value];
            }
            else if (objectType == ObjectType::FragilityValue)
            {
                std::shared_ptr<Statistics::FragilityValue> fragilityValue = fragilityValues[id];

                if (property_ == "design_point") fragilityValue->designPoint = designPoints[value];
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
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "minimum_samples") settings->MinimumSamples = value;
                else if (property_ == "maximum_samples") settings->MaximumSamples = value;
                else if (property_ == "maximum_iterations") settings->MaximumIterations = value;
                else if (property_ == "minimum_directions") settings->MinimumDirections = value;
                else if (property_ == "maximum_directions") settings->MaximumDirections = value;
                else if (property_ == "minimum_variance_loops") settings->MinimumVarianceLoops = value;
                else if (property_ == "maximum_variance_loops") settings->MaximumVarianceLoops = value;
                else if (property_ == "relaxation_loops") settings->RelaxationLoops = value;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SettingsS> settings = sensitivitySettingsValues[id];

                if (property_ == "minimum_samples") settings->MinimumSamples = value;
                else if (property_ == "maximum_samples") settings->MaximumSamples = value;
                else if (property_ == "maximum_iterations") settings->MaximumIterations = value;
                else if (property_ == "minimum_directions") settings->MinimumDirections = value;
                else if (property_ == "maximum_directions") settings->MaximumDirections = value;
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
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "is_initialization_allowed") return stochastSettings->IsInitializationAllowed;
                else if (property_ == "is_variance_allowed") return stochastSettings->IsVarianceAllowed;

            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "is_converged") return designPoint->convergenceReport->IsConverged;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SettingsS> settings = sensitivitySettingsValues[id];

                if (property_ == "derive_samples_from_variation_coefficient") return settings->DeriveSamplesFromVariationCoefficient;
                else if (property_ == "calculate_correlations") return settings->CalculateCorrelations;
                else if (property_ == "calculate_input_correlations") return settings->CalculateInputCorrelations;
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
            }
            else if (objectType == ObjectType::StochastSettings)
            {
                std::shared_ptr<Reliability::StochastSettings> stochastSettings = stochastSettingsValues[id];

                if (property_ == "is_initialization_allowed") stochastSettings->IsInitializationAllowed = value;
                else if (property_ == "is_variance_allowed") stochastSettings->IsVarianceAllowed = value;
            }
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SettingsS> settings = sensitivitySettingsValues[id];

                if (property_ == "derive_samples_from_variation_coefficient") settings->DeriveSamplesFromVariationCoefficient = value;
                else if (property_ == "calculate_correlations") settings->CalculateCorrelations = value;
                else if (property_ == "calculate_input_correlations") settings->CalculateInputCorrelations = value;
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
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "reliability_method") return Settings::getReliabilityMethodTypeString(settings->ReliabilityMethod);
                else if (property_ == "reliability_result") return Settings::getReliabilityResultTypeString(settings->ReliabilityResult);
                else if (property_ == "design_point_method") return DesignPointBuilder::getDesignPointMethodString(settings->designPointMethod);
                else if (property_ == "sample_method") return SubsetSimulationSettings::getSampleMethodString(settings->sampleMethod);
                else if (property_ == "start_method") return StartPointCalculatorSettings::getStartPointMethodString(settings->StartPointSettings->StartMethod);
                else if (property_ == "random_type") return Numeric::Random::getRandomGeneratorTypeString(settings->RandomSettings->RandomGeneratorType);
            }
            else if (objectType == ObjectType::CombineSettings)
            {
                std::shared_ptr<Reliability::CombineSettings> settings = combineSettingsValues[id];

                if (property_ == "combiner_method") return DesignPointCombiner::getCombinerMethodString(settings->combinerMethod);
                else if (property_ == "combine_type") return DesignPointCombiner::getCombineTypeString(settings->combineType);
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "identifier") return designPoint->Identifier;
            }

            return "";
        }

        void ProjectHandler::SetStringValue(int id, std::string property_, std::string value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
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
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "reliability_method") settings->ReliabilityMethod = Settings::getReliabilityMethodType(value);
                else if (property_ == "reliability_result") settings->ReliabilityResult = Settings::getReliabilityResultType(value);
                else if (property_ == "design_point_method") settings->designPointMethod = DesignPointBuilder::getDesignPointMethod(value);
                else if (property_ == "sample_method") settings->sampleMethod = SubsetSimulationSettings::getSampleMethod(value);
                else if (property_ == "start_method") settings->StartPointSettings->StartMethod = StartPointCalculatorSettings::getStartPointMethod(value);
                else if (property_ == "random_type") settings->RandomSettings->RandomGeneratorType = Numeric::Random::getRandomGeneratorType(value);
            }
            else if (objectType == ObjectType::CombineSettings)
            {
                std::shared_ptr<Reliability::CombineSettings> settings = combineSettingsValues[id];

                if (property_ == "combiner_method") settings->combinerMethod = DesignPointCombiner::getCombinerMethod(value);
                else if (property_ == "combine_type") settings->combineType = DesignPointCombiner::getCombineType(value);
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "identifier") designPoint->Identifier = value;
            }
            else if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Reliability::ReliabilityProject> project = projects[id];

                if (property_ == "model_name") project->model->Name = value;
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> sensitivityProject = sensitivityProjects[id];

                if (property_ == "model_name") sensitivityProject->model->Name = value;
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
                    std::vector<double> fitValues;
                    for (size_t i = 0; i < size; i++)
                    {
                        fitValues.push_back(values[i]);
                    }

                    stochast->fit(fitValues);
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
            ObjectType objectType = types[id];

            return std::vector<int>(0);
        }

        void ProjectHandler::SetArrayIntValue(int id, std::string property_, int* values, int size)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Reliability::ReliabilityProject> project = projects[id];

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
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

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
            else if (objectType == ObjectType::SensitivitySettings)
            {
                std::shared_ptr<Sensitivity::SettingsS> settings = sensitivitySettingsValues[id];

                if (property_ == "stochast_settings")
                {
                    settings->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < size; i++)
                    {
                        settings->StochastSet->stochastSettings.push_back(stochastSettingsValues[values[i]]);
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
                if (property_ == "x_from_u") return stochast->getXFromU(argument);
                if (property_ == "u_from_x") return stochast->getUFromX(argument);
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
            if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> lengthEffect = lengthEffectProjects[id];
                if (property_ == "correlation_lengths")
                {
                    return lengthEffect->correlationLengths[index];
                }
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
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "histogram_values") return this->GetHistogramValueId(stochast->getProperties()->HistogramValues[index]);
                else if (property_ == "discrete_values") return this->GetDiscreteValueId(stochast->getProperties()->DiscreteValues[index]);
                else if (property_ == "fragility_values") return this->GetFragilityValueId(stochast->getProperties()->FragilityValues[index]);
                else if (property_ == "contributing_stochasts") return this->GetContributingStochastId(stochast->getProperties()->ContributingStochasts[index]);
                else if (property_ == "conditional_values") return this->GetConditionalValueId(stochast->ValueSet->StochastValues[index]);
            }
            else if (objectType == ObjectType::CorrelationMatrix)
            {
                std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = correlationMatrices[id];

                if (property_ == "variables") return this->GetStochastId(correlationMatrix->getStochast(index));
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "contributing_design_points") return this->GetDesignPointId(designPoint->ContributingDesignPoints[index]);
                else if (property_ == "alphas") return this->GetAlphaId(designPoint->Alphas[index]);
                else if (property_ == "messages") return this->GetMessageId(designPoint->Messages[index]);
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "sensitivity_stochasts") return this->GetStochastId(project->sensitivityStochasts[index]);
            }

            return 0;
        }

        void ProjectHandler::SetCallBack(int id, std::string property_, ZValuesCallBack callBack)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Reliability::ReliabilityProject> project = projects[id];

                if (property_ == "model") project->model = std::make_shared<ZModel>(callBack);
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "model") project->model = std::make_shared<ZModel>(callBack);
            }
        }

        void ProjectHandler::SetEmptyCallBack(int id, std::string property_, ZEmptyCallBack callBack)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Reliability::ReliabilityProject> project = projects[id];

                if (property_ == "initialize") project->setInitializer(callBack);
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "initialize") project->setInitializer(callBack);
            }
        }

        void ProjectHandler::Execute(int id, std::string method_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (method_ == "initialize_for_run") stochast->initializeForRun(); 
                else if (method_ == "initialize_conditional_values") stochast->ValueSet->initializeForRun(stochast->getProperties(), stochast->getDistributionType(), stochast->isTruncated(), stochast->isInverted());
            }
            else if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Reliability::ReliabilityProject> project = projects[id];

                if (method_ == "run") project->run();
            }
            else if (objectType == ObjectType::CombineProject)
            {
                std::shared_ptr<Reliability::CombineProject> project = combineProjects[id];

                if (method_ == "run") project->run();
            }
            else if (objectType == ObjectType::LengthEffectProject)
            {
                std::shared_ptr<Reliability::LengthEffectProject> project = lengthEffectProjects[id];

                if (method_ == "run") project->run();
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (method_ == "run") project->run();
            }
        }

        int ProjectHandler::GetStochastId(std::shared_ptr<Statistics::Stochast> stochast)
        {
            if (stochast == nullptr)
            {
                return 0;
            }
            else
            {
                if (!stochastIds.contains(stochast))
                {
                    int counter = this->server->GetNewObjectId(this->handlerIndex);

                    stochasts[counter] = stochast;
                    types[counter] = ObjectType::Stochast;
                    stochastIds[stochast] = counter;
                }

                return stochastIds[stochast];
            }
        }

        int ProjectHandler::GetCorrelationMatrixId(std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix)
        {
            if (correlationMatrix == nullptr)
            {
                return 0;
            }
            else
            {
                if (!correlationMatrixIds.contains(correlationMatrix))
                {
                    int counter = this->server->GetNewObjectId(this->handlerIndex);

                    correlationMatrices[counter] = correlationMatrix;
                    types[counter] = ObjectType::CorrelationMatrix;
                    correlationMatrixIds[correlationMatrix] = counter;
                }

                return correlationMatrixIds[correlationMatrix];
            }
        }

        int ProjectHandler::GetDesignPointId(std::shared_ptr<Reliability::DesignPoint> designPoint)
        {
            if (designPoint == nullptr)
            {
                return 0;
            }
            else
            {
                if (!designPointIds.contains(designPoint))
                {
                    int counter = this->server->GetNewObjectId(this->handlerIndex);

                    designPoints[counter] = designPoint;
                    types[counter] = ObjectType::DesignPoint;
                    designPointIds[designPoint] = counter;
                }

                return designPointIds[designPoint];
            }
        }

        int ProjectHandler::GetAlphaId(std::shared_ptr<StochastPointAlpha> alpha)
        {
            if (!alphaIds.contains(alpha))
            {
                int counter = this->server->GetNewObjectId(this->handlerIndex);

                alphas[counter] = alpha;
                types[counter] = ObjectType::Alpha;
                alphaIds[alpha] = counter;
            }

            return alphaIds[alpha];
        }

        int ProjectHandler::GetHistogramValueId(std::shared_ptr<Statistics::HistogramValue> histogramValue)
        {
            if (!histogramValueIds.contains(histogramValue))
            {
                int counter = this->server->GetNewObjectId(this->handlerIndex);

                histogramValues[counter] = histogramValue;
                types[counter] = ObjectType::HistogramValue;
                histogramValueIds[histogramValue] = counter;
            }

            return histogramValueIds[histogramValue];
        }

        int ProjectHandler::GetDiscreteValueId(std::shared_ptr<Statistics::DiscreteValue> discreteValue)
        {
            if (!discreteValueIds.contains(discreteValue))
            {
                int counter = this->server->GetNewObjectId(this->handlerIndex);

                discreteValues[counter] = discreteValue;
                types[counter] = ObjectType::DiscreteValue;
                discreteValueIds[discreteValue] = counter;
            }

            return discreteValueIds[discreteValue];
        }

        int ProjectHandler::GetFragilityValueId(std::shared_ptr<Statistics::FragilityValue> fragilityValue)
        {
            if (!fragilityValueIds.contains(fragilityValue))
            {
                int counter = this->server->GetNewObjectId(this->handlerIndex);

                fragilityValues[counter] = fragilityValue;
                types[counter] = ObjectType::FragilityValue;
                fragilityValueIds[fragilityValue] = counter;
            }

            return fragilityValueIds[fragilityValue];
        }

        int ProjectHandler::GetContributingStochastId(std::shared_ptr<Statistics::ContributingStochast> contributingStochast)
        {
            if (!contributingStochastIds.contains(contributingStochast))
            {
                int counter = this->server->GetNewObjectId(this->handlerIndex);

                contributingStochasts[counter] = contributingStochast;
                types[counter] = ObjectType::FragilityValue;
                contributingStochastIds[contributingStochast] = counter;
            }

            return contributingStochastIds[contributingStochast];
        }

        int ProjectHandler::GetConditionalValueId(std::shared_ptr<Statistics::VariableStochastValue> conditionalValue)
        {
            if (!conditionalValueIds.contains(conditionalValue))
            {
                int counter = this->server->GetNewObjectId(this->handlerIndex);

                conditionalValues[counter] = conditionalValue;
                types[counter] = ObjectType::ConditionalValue;
                conditionalValueIds[conditionalValue] = counter;
            }

            return conditionalValueIds[conditionalValue];
        }

        int ProjectHandler::GetMessageId(std::shared_ptr<Deltares::Models::Message> message)
        {
            if (!messageIds.contains(message))
            {
                int counter = this->server->GetNewObjectId(this->handlerIndex);

                messages[counter] = message;
                types[counter] = ObjectType::Message;
                messageIds[message] = counter;
            }

            return messageIds[message];
        }

    }
}

