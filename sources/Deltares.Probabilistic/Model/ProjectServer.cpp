#include "ProjectServer.h"

#include <string>
#include <memory>
#include <map>

namespace Deltares
{
    namespace Models
    {
        using namespace Deltares::Statistics;
        using namespace Deltares::Reliability;

        int ProjectServer::Create(std::string object_type)
        {
            counter++;

            if (object_type == "stochast")
            {
                stochasts[counter] = std::make_shared<Deltares::Statistics::Stochast>();
                types[counter] = ObjectType::Stochast;
            }
            else if (object_type == "discrete_value")
            {
                discreteValues[counter] = std::make_shared<Deltares::Statistics::DiscreteValue>();
                types[counter] = ObjectType::DiscreteValue;
            }
            else if (object_type == "histogram_value")
            {
                histogramValues[counter] = std::make_shared<Deltares::Statistics::HistogramValue>();
                types[counter] = ObjectType::HistogramValue;
            }
            else if (object_type == "fragility_value")
            {
                fragilityValues[counter] = std::make_shared<Deltares::Statistics::FragilityValue>();
                types[counter] = ObjectType::FragilityValue;
            }
            else if (object_type == "settings")
            {
                settingsValues[counter] = std::make_shared<Deltares::Reliability::Settings>();
                types[counter] = ObjectType::Settings;
            }

            return counter;
        }

        void ProjectServer::Destroy(int id)
        {
            switch (types[id])
            {
            case ObjectType::Stochast: stochasts.erase(id); break;
            case ObjectType::DiscreteValue: discreteValues.erase(id); break;
            case ObjectType::HistogramValue: histogramValues.erase(id); break;
            case ObjectType::FragilityValue: fragilityValues.erase(id); break;
            case ObjectType::Settings: settingsValues.erase(id); break;
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
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "relaxation_factor") return settings->RelaxationFactor;
                else if (property_ == "variation_coefficient") return settings->VariationCoefficient;
                else if (property_ == "fraction_failed") return settings->FractionFailed;
            }
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
            }
            else if (objectType == ObjectType::HistogramValue)
            {
                std::shared_ptr<Statistics::HistogramValue> histogramValue = histogramValues[id];

                if (property_ == "lower_bound") histogramValue->LowerBound = value;
                else if (property_ == "upper_bound") histogramValue->UpperBound = value;
                else if (property_ == "amount") histogramValue->Amount = value;
            }
            else if (objectType == ObjectType::FragilityValue)
            {
                std::shared_ptr<Statistics::FragilityValue> fragilityValue = fragilityValues[id];

                if (property_ == "x") fragilityValue->X;
                else if (property_ == "reliability_index") fragilityValue->Reliability = value;
                else if (property_ == "probability_of_failure") fragilityValue->setProbabilityOfFailure(value);
                else if (property_ == "probability_of_non_failure") fragilityValue->setProbabilityOfNonFailure(value);
                else if (property_ == "return_period") fragilityValue->setReturnPeriod(value);
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "relaxation_factor") settings->RelaxationFactor = value;
                else if (property_ == "variation_coefficient") settings->VariationCoefficient = value;
                else if (property_ == "fraction_failed") settings->FractionFailed = value;
            }
        }

        int ProjectServer::GetIntValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "observations") return stochast->getProperties()->Observations;
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



            return 0;
        }

        void ProjectServer::SetIntValue(int id, std::string property_, int value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "observations") stochast->getProperties()->Observations = value;
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
        }

        bool ProjectServer::GetBoolValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "inverted") return stochast->isInverted();
                if (property_ == "truncated") return stochast->isTruncated();
                else return false;
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
        }

        std::string ProjectServer::GetStringValue(int id, std::string property_)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "distribution") return getDistributionTypeString(stochast->getDistributionType());
                else return "";
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "reliability_method") return Settings::getReliabilityMethodTypeString(settings->ReliabilityMethod);
                if (property_ == "design_point_method") return DesignPointBuilder::getDesignPointMethodString(settings->DesignPointMethod);
                if (property_ == "start_method") return StartPointCalculatorSettings::getStartPointMethodString(settings->StartPointSettings->StartMethod);
            }

            return "";
        }

        void ProjectServer::SetStringValue(int id, std::string property_, std::string value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "distribution") stochast->setDistributionType(getDistributionType(value));
            }
            else if (objectType == ObjectType::Settings)
            {
                std::shared_ptr<Reliability::Settings> settings = settingsValues[id];

                if (property_ == "reliability_method") settings->ReliabilityMethod = Settings::getReliabilityMethodType(value);
                if (property_ == "design_point_method") settings->DesignPointMethod = DesignPointBuilder::getDesignPointMethod(value);
                if (property_ == "start_method") settings->StartPointSettings->StartMethod = StartPointCalculatorSettings::getStartPointMethod(value);
            }
        }

        void ProjectServer::SetArrayValue(int id, std::string property_, int* values, int size)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "discrete_values")
                {
                    stochast->getProperties()->DiscreteValues.clear();
                    for (int i = 0; i < size; i++)
                    {
                        stochast->getProperties()->DiscreteValues.push_back(discreteValues[values[i]]);
                    }
                }
                else if (property_ == "histogram_values")
                {
                    stochast->getProperties()->HistogramValues.clear();
                    for (int i = 0; i < size; i++)
                    {
                        stochast->getProperties()->HistogramValues.push_back(histogramValues[values[i]]);
                    }
                }
                else if (property_ == "fragility_values")
                {
                    stochast->getProperties()->FragilityValues.clear();
                    for (int i = 0; i < size; i++)
                    {
                        stochast->getProperties()->FragilityValues.push_back(fragilityValues[values[i]]);
                    }
                }
            }
        }


        double ProjectServer::GetArgValue(int id, std::string property_, double argument)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "quantile") return stochast->getQuantile(argument);
                if (property_ == "x_from_u") return stochast->getXFromU(argument);
                if (property_ == "u_from_x") return stochast->getUFromX(argument);
                else return std::nan("");
            }

            return std::nan("");
        }

        void ProjectServer::SetArgValue(int id, std::string property_, double argument, double value)
        {
            ObjectType objectType = types[id];

            if (objectType == ObjectType::Stochast)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[id];

                if (property_ == "x_at_u") return stochast->setXAtU(value, argument, ConstantParameterType::VariationCoefficient);
            }
        }

        Statistics::DistributionType ProjectServer::getDistributionType(std::string distributionType)
        {
            if (distributionType == "deterministic") return Statistics::DistributionType::Deterministic;
            else if (distributionType == "Normal") return Statistics::DistributionType::Normal;
            else if (distributionType == "LogNormal") return Statistics::DistributionType::LogNormal;
            else if (distributionType == "Uniform") return Statistics::DistributionType::Uniform;
            else if (distributionType == "Gumbel") return Statistics::DistributionType::Gumbel;
            else if (distributionType == "Weibull") return Statistics::DistributionType::Weibull;
            else if (distributionType == "ConditionalWeibull") return Statistics::DistributionType::ConditionalWeibull;
            else if (distributionType == "Frechet") return Statistics::DistributionType::Frechet;
            else if (distributionType == "GeneralizedExtremeValue") return Statistics::DistributionType::GeneralizedExtremeValue;
            else if (distributionType == "Rayleigh") return Statistics::DistributionType::Rayleigh;
            else if (distributionType == "RayleighN") return Statistics::DistributionType::RayleighN;
            else if (distributionType == "Discrete") return Statistics::DistributionType::Discrete;
            else if (distributionType == "Qualitative") return Statistics::DistributionType::Qualitative;
            else throw probLibException("distribution type");
        }

        std::string ProjectServer::getDistributionTypeString(Statistics::DistributionType distributionType)
        {
            switch (distributionType)
            {
            case Statistics::DistributionType::Deterministic: return "Deterministic";
            case Statistics::DistributionType::Normal: return "Normal";
            case Statistics::DistributionType::LogNormal: return "LogNormal";
            case Statistics::DistributionType::Uniform: return "Uniform";
            case Statistics::DistributionType::Gumbel: return "Gumbel";
            case Statistics::DistributionType::Weibull: return "Weibull";
            case Statistics::DistributionType::ConditionalWeibull: return "ConditionalWeibull";
            case Statistics::DistributionType::Frechet: return "Frechet";
            case Statistics::DistributionType::GeneralizedExtremeValue: return "GeneralizedExtremeValue";
            case Statistics::DistributionType::Rayleigh: return "Rayleigh";
            case Statistics::DistributionType::RayleighN: return "RayleighN";
            case Statistics::DistributionType::Discrete: return "Discrete";
            case Statistics::DistributionType::Qualitative: return "Qualitative";
            default: throw probLibException("distribution type");
            }
        }
    }
}

