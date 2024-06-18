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
            void Execute(int id, std::string method_);
        private:
            enum ObjectType {Project, Stochast, DiscreteValue, HistogramValue, FragilityValue, Settings, DesignPoint};

            int counter = 0;
            std::map<int, Deltares::Models::ProjectServer::ObjectType> types;

            std::unordered_map<int, std::shared_ptr<Statistics::Stochast>> stochasts;
            std::unordered_map<int, std::shared_ptr<Models::Project>> projects;
            std::unordered_map<int, std::shared_ptr<Statistics::DiscreteValue>> discreteValues;
            std::unordered_map<int, std::shared_ptr<Statistics::HistogramValue>> histogramValues;
            std::unordered_map<int, std::shared_ptr<Statistics::FragilityValue>> fragilityValues;
            std::unordered_map<int, std::shared_ptr<Reliability::Settings>> settingsValues;
            std::unordered_map<int, std::shared_ptr<Reliability::DesignPoint>> designPoints;

            std::unordered_map<std::shared_ptr<Reliability::DesignPoint>, int> designPointIds;

            int GetDesignPointId(std::shared_ptr<Reliability::DesignPoint> designPoint);
        };
    }
}
