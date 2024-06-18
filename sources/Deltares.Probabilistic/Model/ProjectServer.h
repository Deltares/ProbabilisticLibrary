#pragma once
#include "Project.h"
#include "../Reliability/Settings.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/DiscreteValue.h"
#include <map>

namespace Deltares
{
    namespace Models
    {
        class ProjectServer
        {
        public:
            int Create(std::string object_type);
            void Destroy(int id);
            double GetValue(int id, const std::string property_);
            void SetValue(int id, const std::string property_, double value);
            int GetIntValue(int id, std::string property_);
            void SetIntValue(int id, std::string property_, int value);
            bool GetBoolValue(int id, std::string property_);
            void SetBoolValue(int id, std::string property_, bool value);
            std::string GetStringValue(int id, std::string property_);
            void SetStringValue(int id, std::string property_, std::string value);
            void SetArrayValue(int id, std::string property_, int* values, int size);
            double GetArgValue(int id, std::string property_, double argument);
            void SetArgValue(int id, std::string property_, double argument, double value);
        private:
            enum ObjectType {Stochast, DiscreteValue, HistogramValue, FragilityValue, Settings};

            int counter = 0;
            std::map<int, Deltares::Models::ProjectServer::ObjectType> types;
            std::map<int, std::shared_ptr<Statistics::Stochast>> stochasts;
            std::unordered_map<int, std::shared_ptr<Statistics::DiscreteValue>> discreteValues;
            std::unordered_map<int, std::shared_ptr<Statistics::HistogramValue>> histogramValues;
            std::unordered_map<int, std::shared_ptr<Statistics::FragilityValue>> fragilityValues;
            std::unordered_map<int, std::shared_ptr<Reliability::Settings>> settingsValues;

            static Statistics::DistributionType getDistributionType(std::string distributionType);
            static std::string getDistributionTypeString(Statistics::DistributionType distributionType);
        };
    }
}
