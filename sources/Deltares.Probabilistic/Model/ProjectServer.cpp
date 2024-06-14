#include "ProjectServer.h"

#include <string>

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <format>


namespace Deltares
{
    namespace Models
    {
        using namespace Deltares::Statistics;
        using namespace Deltares::Reliability;

        const void* ProjectServer::Create(std::string object_type)
        {
            if (object_type == "stochast")
            {
                void* ptr = new Stochast();
                return ptr;
            }

            return nullptr;
        }

        void ProjectServer::Destroy(void* object)
        {
            delete (Stochast*) object;
        }

        double ProjectServer::GetValue(void* object, std::string property_)
        {
            Stochast* stochast = (Stochast*)object;

            if (property_ == "location") return stochast->getProperties()->Location;
            else if (property_ == "scale") return stochast->getProperties()->Scale;
            else if (property_ == "shape") return stochast->getProperties()->Shape;
            else if (property_ == "shape_b") return stochast->getProperties()->ShapeB;
            else if (property_ == "shift") return stochast->getProperties()->Shift;
            else if (property_ == "shift_b") return stochast->getProperties()->ShiftB;
            else if (property_ == "minimum") return stochast->getProperties()->Minimum;
            else if (property_ == "maximum") return stochast->getProperties()->Maximum;
            else if (property_ == "observations") return stochast->getProperties()->Observations;
            else if (property_ == "mean") return stochast->getMean();
            else if (property_ == "deviation") return stochast->getDeviation();
            else return std::nan("");
        }


        void ProjectServer::SetValue(void* object, std::string property_, double value)
        {
            Stochast* stochast = (Stochast*)object;

            if (property_ == "location") stochast->getProperties()->Location = value;
            else if (property_ == "scale") stochast->getProperties()->Scale = value;
            else if (property_ == "shape") stochast->getProperties()->Shape = value;
            else if (property_ == "shape_b") stochast->getProperties()->ShapeB = value;
            else if (property_ == "shift") stochast->getProperties()->Shift = value;
            else if (property_ == "shift_b") stochast->getProperties()->ShiftB = value;
            else if (property_ == "minimum") stochast->getProperties()->Minimum = value;
            else if (property_ == "maximum") stochast->getProperties()->Maximum = value;
            else if (property_ == "observations") stochast->getProperties()->Observations = (int) value;
            else if (property_ == "mean") stochast->setMean(value);
            else if (property_ == "deviation") stochast->setDeviation(value);
        }

        std::string ProjectServer::GetStringValue(void* object, std::string property_)
        {
            Stochast* stochast = (Stochast*)object;

            if (property_ == "distribution") return getDistributionTypeString(stochast->getDistributionType());
            else return "";
        }

        void ProjectServer::SetStringValue(void* object, std::string property_, std::string value)
        {
            Stochast* stochast = (Stochast*)object;

            if (property_ == "distribution") stochast->setDistributionType(getDistributionType(value));
        }

        double ProjectServer::GetArgValue(void* object, std::string property_, double argument)
        {
            Stochast* stochast = (Stochast*)object;

            if (property_ == "quantile") return stochast->getQuantile(argument);
            if (property_ == "x_from_u") return stochast->getXFromU(argument);
            if (property_ == "u_from_x") return stochast->getUFromX(argument);
            else return std::nan("");
        }

        void ProjectServer::SetArgValue(void* object, std::string property_, double argument, double value)
        {
            Stochast* stochast = (Stochast*)object;

            if (property_ == "x_at_u") return stochast->setXAtU(value, argument, ConstantParameterType::VariationCoefficient);
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

