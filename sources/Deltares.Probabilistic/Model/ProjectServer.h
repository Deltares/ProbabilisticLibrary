#pragma once
#include "Project.h"

namespace Deltares
{
    namespace Models
    {
        class ProjectServer
        {
        public:
            static const void* Create(std::string object_type);
            static void Destroy(void* object);
            static double GetValue(void* object, const std::string property_);
            static void SetValue(void* object, const std::string property_, double value);
            static std::string GetStringValue(void* object, std::string property_);
            static void SetStringValue(void* object, std::string property_, std::string value);
            static double GetArgValue(void* object, std::string property_, double argument);
            static void SetArgValue(void* object, std::string property_, double argument, double value);
        private:
            static Statistics::DistributionType getDistributionType(std::string distributionType);
            static std::string getDistributionTypeString(Statistics::DistributionType distributionType);
        };
    }
}
