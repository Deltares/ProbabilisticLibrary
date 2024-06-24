#pragma once
#include "Project.h"
#include "../Reliability/Settings.h"
#include "../Combine/CombineProject.h"
#include "../Combine/CombineSettings.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/DiscreteValue.h"

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
            std::vector<int> GetArrayValue(int id, std::string property_);
            void SetArrayValue(int id, std::string property_, int* values, int size);
            double GetArgValue(int id, std::string property_, double argument);
            void SetArgValue(int id, std::string property_, double argument, double value);
            double GetIndexedValue(int id, std::string property_, int index);
            void SetIndexedValue(int id, std::string property_, int index, double value);
            double GetIndexedIndexedValue(int id, std::string property_, int index1, int index2);
            void SetIndexedIndexedValue(int id, std::string property_, int index1, int index2, double value);
            int GetIndexedIntValue(int id, std::string property_, int index);
            void SetCallBack(int id, std::string property_, ZValuesCallBack callBack);
            void Execute(int id, std::string method_);
        private:
            enum ObjectType {StandardNormal, Project, Stochast, DiscreteValue, HistogramValue, FragilityValue, CorrelationMatrix, Settings, StochastSettings, DesignPoint, Alpha, CombineProject, CombineSettings};

            int counter = 0;
            std::unordered_map<int, Deltares::Models::ProjectServer::ObjectType> types;

            std::unordered_map<int, std::shared_ptr<Statistics::Stochast>> stochasts;
            std::unordered_map<int, std::shared_ptr<Models::Project>> projects;
            std::unordered_map<int, std::shared_ptr<Statistics::DiscreteValue>> discreteValues;
            std::unordered_map<int, std::shared_ptr<Statistics::HistogramValue>> histogramValues;
            std::unordered_map<int, std::shared_ptr<Statistics::FragilityValue>> fragilityValues;
            std::unordered_map<int, std::shared_ptr<Statistics::CorrelationMatrix>> correlationMatrices;
            std::unordered_map<int, std::shared_ptr<Reliability::Settings>> settingsValues;
            std::unordered_map<int, std::shared_ptr<Reliability::StochastSettings>> stochastSettingsValues;
            std::unordered_map<int, std::shared_ptr<Reliability::DesignPoint>> designPoints;
            std::unordered_map<int, std::shared_ptr<Reliability::StochastPointAlpha>> alphas;
            std::unordered_map<int, std::shared_ptr<Reliability::CombineProject>> combineProjects;
            std::unordered_map<int, std::shared_ptr<Reliability::CombineSettings>> combineSettingsValues;

            std::unordered_map<std::shared_ptr<Reliability::DesignPoint>, int> designPointIds;
            std::unordered_map<std::shared_ptr<Reliability::StochastPointAlpha>, int> alphaIds;
            std::unordered_map<std::shared_ptr<Statistics::Stochast>, int> stochastIds;

            int GetDesignPointId(std::shared_ptr<Reliability::DesignPoint> designPoint);
            int GetAlphaId(std::shared_ptr<StochastPointAlpha> alpha);
        };
    }
}
