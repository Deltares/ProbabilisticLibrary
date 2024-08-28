#include "ProjectServer.h"

#include <string>
#include <memory>

namespace Deltares
{
    namespace Models
    {
        using namespace Deltares::Statistics;
        using namespace Deltares::Reliability;

        int ProjectServer::GetNewObjectId()
        {
            return ++id_;
        }

        int ProjectServer::Create(std::string object_type)
        {
            int counter = GetNewObjectId();

            if (object_type == "standard_normal")
            {
                types[counter] = ObjectType::StandardNormal;
            }
            else if (object_type == "project")
            {
                projects[counter] = std::make_shared<Deltares::Models::Project>();
                types[counter] = ObjectType::Project;
            }
            else if (object_type == "stochast")
            {
                stochasts[counter] = std::make_shared<Deltares::Statistics::Stochast>();
                stochastIds[stochasts[counter]] = counter;
                types[counter] = ObjectType::Stochast;
            }
            else if (object_type == "discrete_value")
            {
                discreteValues[counter] = std::make_shared<Deltares::Statistics::DiscreteValue>();
                discreteValueIds[discreteValues[counter]] = counter;
                types[counter] = ObjectType::DiscreteValue;
            }
            else if (object_type == "histogram_value")
            {
                histogramValues[counter] = std::make_shared<Deltares::Statistics::HistogramValue>();
                histogramValueIds[histogramValues[counter]] = counter;
                types[counter] = ObjectType::HistogramValue;
            }
            else if (object_type == "fragility_value")
            {
                fragilityValues[counter] = std::make_shared<Deltares::Statistics::FragilityValue>();
                fragilityValueIds[fragilityValues[counter]] = counter;
                types[counter] = ObjectType::FragilityValue;
            }
            else if (object_type == "contributing_stochast")
            {
                contributingStochasts[counter] = std::make_shared<Deltares::Statistics::ContributingStochast>();
                contributingStochastIds[contributingStochasts[counter]] = counter;
                types[counter] = ObjectType::ContributingStochast;
            }
            else if (object_type == "correlation_matrix")
            {
                correlationMatrices[counter] = std::make_shared<Deltares::Statistics::CorrelationMatrix>();
                types[counter] = ObjectType::CorrelationMatrix;
            }
            else if (object_type == "settings")
            {
                settingsValues[counter] = std::make_shared<Deltares::Reliability::Settings>();
                types[counter] = ObjectType::Settings;
            }
            else if (object_type == "stochast_settings")
            {
                stochastSettingsValues[counter] = std::make_shared<Deltares::Reliability::StochastSettings>();
                types[counter] = ObjectType::StochastSettings;
            }
            else if (object_type == "design_point")
            {
                designPoints[counter] = std::make_shared<Deltares::Reliability::DesignPoint>();
                designPointIds[designPoints[counter]] = counter;
                types[counter] = ObjectType::DesignPoint;
            }
            else if (object_type == "alpha")
            {
                alphas[counter] = std::make_shared<Deltares::Reliability::StochastPointAlpha>();
                alphaIds[alphas[counter]] = counter;
                types[counter] = ObjectType::Alpha;
            }
            else if (object_type == "combine_project")
            {
                combineProjects[counter] = std::make_shared<Deltares::Reliability::CombineProject>();
                types[counter] = ObjectType::CombineProject;
            }
            else if (object_type == "combine_settings")
            {
                combineSettingsValues[counter] = std::make_shared<Deltares::Reliability::CombineSettings>();
                types[counter] = ObjectType::CombineSettings;
            }
            else if (object_type == "self_correlation_matrix")
            {
                selfCorrelationMatrices[counter] = std::make_shared<Deltares::Statistics::SelfCorrelationMatrix>();
                types[counter] = ObjectType::SelfCorrelationMatrix;
            }
            else if (object_type == "sensitivity_project")
            {
                sensitivityProjects[counter] = std::make_shared<Deltares::Sensitivity::SensitivityProject>();
                types[counter] = ObjectType::SensitivityProject;
            }
            else if (object_type == "sensitivity_settings")
            {
                sensitivitySettingsValues[counter] = std::make_shared<Deltares::Sensitivity::SettingsS>();
                types[counter] = ObjectType::SensitivitySettings;
            }

            return counter;
        }

        void ProjectServer::Destroy(int id)
        {
            switch (types[id])
            {
            case ObjectType::StandardNormal: break;
            case ObjectType::Project: projects.erase(id); break;
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

        double ProjectServer::GetValue(int id, std::string property_)
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
            return std::nan("");
        }

        void ProjectServer::SetValue(int id, std::string property_, double value)
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
        }

        int ProjectServer::GetIntValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Models::Project> project = projects[id];

                if (property_ == "design_point") return GetDesignPointId(project->designPoint);
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "stochast") return GetStochastId(project->stochast);
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "observations") return stochast->getProperties()->Observations;
                else if (property_ == "histogram_values_count") return (int)stochast->getProperties()->HistogramValues.size();
                else if (property_ == "discrete_values_count") return (int)stochast->getProperties()->DiscreteValues.size();
                else if (property_ == "fragility_values_count") return (int)stochast->getProperties()->FragilityValues.size();
                else if (property_ == "contributing_stochasts_count") return (int)stochast->getProperties()->ContributingStochasts.size();
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

            return 0;
        }

        void ProjectServer::SetIntValue(int id, std::string property_, int value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Models::Project> project = projects[id];

                if (property_ == "settings") project->settings = settingsValues[value];
                else if (property_ == "correlation_matrix") project->correlationMatrix = correlationMatrices[value];
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "settings") project->settings = sensitivitySettingsValues[value];
                else if (property_ == "correlation_matrix") project->correlationMatrix = correlationMatrices[value];
            }
            else if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "observations") stochast->getProperties()->Observations = value;
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
        }

        double ProjectServer::GetIntArgValue(int id1, int id2, std::string property_)
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

        void ProjectServer::SetIntArgValue(int id1, int id2, std::string property_, double value)
        {
            ObjectType objectType = types[id1];

            if (objectType == ObjectType::SelfCorrelationMatrix)
            {
                std::shared_ptr<Statistics::SelfCorrelationMatrix> correlationMatrix = selfCorrelationMatrices[id1];
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id2];

                if (property_ == "rho") correlationMatrix->setSelfCorrelation(stochast, value);
            }
        }

        bool ProjectServer::GetBoolValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "inverted") return stochast->isInverted();
                if (property_ == "truncated") return stochast->isTruncated();
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
            }

            return false;
        }

        void ProjectServer::SetBoolValue(int id, std::string property_, bool value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "inverted") stochast->setInverted(value);
                else if (property_ == "truncated") stochast->setTruncated(value);
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
            }

        }

        std::string ProjectServer::GetStringValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "distribution") return Stochast::getDistributionTypeString(stochast->getDistributionType());
                else if (property_ == "name") return stochast->name;
                else return "";
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "reliability_method") return Settings::getReliabilityMethodTypeString(settings->ReliabilityMethod);
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

        void ProjectServer::SetStringValue(int id, std::string property_, std::string value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "distribution") stochast->setDistributionType(Stochast::getDistributionType(value));
                else if (property_ == "name") stochast->name = value;
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "reliability_method") settings->ReliabilityMethod = Settings::getReliabilityMethodType(value);
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
        }

        void ProjectServer::SetArrayValue(int id, std::string property_, double* values, int size)
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
            }
        }

        std::vector<int> ProjectServer::GetArrayIntValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            return std::vector<int>(0);
        }

        void ProjectServer::SetArrayIntValue(int id, std::string property_, int* values, int size)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Models::Project> project = projects[id];

                if (property_ == "variables")
                {
                    project->stochasts.clear();
                    for (int i = 0; i < size; i++)
                    {
                        project->stochasts.push_back(stochasts[values[i]]);
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


        double ProjectServer::GetArgValue(int id, std::string property_, double argument)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::StandardNormal)
            {
                if (property_ == "u_from_q") return StandardNormal::getUFromQ(argument);
                else if (property_ == "u_from_p") return StandardNormal::getUFromP(argument);
                else if (property_ == "q_from_u") return StandardNormal::getQFromU(argument);
                else if (property_ == "p_from_u") return StandardNormal::getPFromU(argument);
                else if (property_ == "r_from_p") return StandardNormal::getRFromP(argument);
                else if (property_ == "p_from_r") return StandardNormal::getPFromR(argument);
                else if (property_ == "r_from_u") return StandardNormal::getRFromU(argument);
                else if (property_ == "u_from_r") return StandardNormal::getUFromR(argument);
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

        void ProjectServer::SetArgValue(int id, std::string property_, double argument, double value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "x_at_u") stochast->setXAtU(value, argument, ConstantParameterType::VariationCoefficient);
            }
        }

        double ProjectServer::GetIndexedValue(int id, std::string property_, int index)
        {
            return std::nan("");
        }

        void ProjectServer::SetIndexedValue(int id, std::string property_, int index, double value)
        {
            // not needed yet
        }

        double ProjectServer::GetIndexedIndexedValue(int id, std::string property_, int index1, int index2)
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

        void ProjectServer::SetIndexedIndexedValue(int id, std::string property_, int index1, int index2, double value)
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

        int ProjectServer::GetIndexedIntValue(int id, std::string property_, int index)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "histogram_values") return this->GetHistogramValueId(stochast->getProperties()->HistogramValues[index]);
                else if (property_ == "discrete_values") return this->GetDiscreteValueId(stochast->getProperties()->DiscreteValues[index]);
                else if (property_ == "fragility_values") return this->GetFragilityValueId(stochast->getProperties()->FragilityValues[index]);
                else if (property_ == "contributing_stochasts") return this->GetContributingStochastId(stochast->getProperties()->ContributingStochasts[index]);
            }
            else if (objectType == ObjectType::DesignPoint)
            {
                std::shared_ptr<Reliability::DesignPoint> designPoint = designPoints[id];

                if (property_ == "contributing_design_points") return this->GetDesignPointId(designPoint->ContributingDesignPoints[index]);
                else if (property_ == "alphas") return this->GetAlphaId(designPoint->Alphas[index]);
            }

            return 0;
        }

        void ProjectServer::SetCallBack(int id, std::string property_, ZValuesCallBack callBack)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Models::Project> project = projects[id];

                if (property_ == "model") project->model = std::make_shared<ZModel>(callBack);
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (property_ == "model") project->model = std::make_shared<ZModel>(callBack);
            }
        }

        void ProjectServer::Execute(int id, std::string method_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (method_ == "initialize_for_run") stochast->initializeForRun();
            }
            else if (objectType == ObjectType::Project)
            {
                std::shared_ptr<Models::Project> project = projects[id];

                if (method_ == "run") project->run();
            }
            else if (objectType == ObjectType::CombineProject)
            {
                std::shared_ptr<Reliability::CombineProject> project = combineProjects[id];

                if (method_ == "run") project->run();
            }
            else if (objectType == ObjectType::SensitivityProject)
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = sensitivityProjects[id];

                if (method_ == "run") project->run();
            }
        }

        int ProjectServer::GetStochastId(std::shared_ptr<Statistics::Stochast> stochast)
        {
            if (stochast == nullptr)
            {
                return 0;
            }
            else
            {
                if (!stochastIds.contains(stochast))
                {
                    int counter = GetNewObjectId();

                    stochasts[counter] = stochast;
                    types[counter] = ObjectType::Stochast;
                    stochastIds[stochast] = counter;
                }

                return stochastIds[stochast];
            }
        }

        int ProjectServer::GetDesignPointId(std::shared_ptr<Reliability::DesignPoint> designPoint)
        {
            if (designPoint == nullptr)
            {
                return 0;
            }
            else
            {
                if (!designPointIds.contains(designPoint))
                {
                    int counter = GetNewObjectId();

                    designPoints[counter] = designPoint;
                    types[counter] = ObjectType::DesignPoint;
                    designPointIds[designPoint] = counter;
                }

                return designPointIds[designPoint];
            }
        }

        int ProjectServer::GetAlphaId(std::shared_ptr<StochastPointAlpha> alpha)
        {
            if (!alphaIds.contains(alpha))
            {
                int counter = GetNewObjectId();

                alphas[counter] = alpha;
                types[counter] = ObjectType::Alpha;
                alphaIds[alpha] = counter;
            }

            return alphaIds[alpha];
        }

        int ProjectServer::GetHistogramValueId(std::shared_ptr<Statistics::HistogramValue> histogramValue)
        {
            if (!histogramValueIds.contains(histogramValue))
            {
                int counter = GetNewObjectId();

                histogramValues[counter] = histogramValue;
                types[counter] = ObjectType::HistogramValue;
                histogramValueIds[histogramValue] = counter;
            }

            return histogramValueIds[histogramValue];
        }

        int ProjectServer::GetDiscreteValueId(std::shared_ptr<Statistics::DiscreteValue> discreteValue)
        {
            if (!discreteValueIds.contains(discreteValue))
            {
                int counter = GetNewObjectId();

                discreteValues[counter] = discreteValue;
                types[counter] = ObjectType::DiscreteValue;
                discreteValueIds[discreteValue] = counter;
            }

            return discreteValueIds[discreteValue];
        }

        int ProjectServer::GetFragilityValueId(std::shared_ptr<Statistics::FragilityValue> fragilityValue)
        {
            if (!fragilityValueIds.contains(fragilityValue))
            {
                int counter = GetNewObjectId();

                fragilityValues[counter] = fragilityValue;
                types[counter] = ObjectType::FragilityValue;
                fragilityValueIds[fragilityValue] = counter;
            }

            return fragilityValueIds[fragilityValue];
        }

        int ProjectServer::GetContributingStochastId(std::shared_ptr<Statistics::ContributingStochast> contributingStochast)
        {
            if (!contributingStochastIds.contains(contributingStochast))
            {
                int counter = GetNewObjectId();

                contributingStochasts[counter] = contributingStochast;
                types[counter] = ObjectType::FragilityValue;
                contributingStochastIds[contributingStochast] = counter;
            }

            return contributingStochastIds[contributingStochast];
        }
    }
}

