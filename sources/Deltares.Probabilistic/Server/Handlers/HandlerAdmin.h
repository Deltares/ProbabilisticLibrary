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
#pragma once

#include <unordered_map>

#include "CombinedLimitStateFunctionHandler.h"
#include "ContributingStochastHandler.h"
#include "../ProjectEntries.h"
#include "ConvergenceReportHandler.h"
#include "CopulaCorrelationHandler.h"
#include "CorrelationMatrixHandler.h"
#include "DesignPointHandler.h"
#include "DiscreteValueHandler.h"
#include "EvaluationHandler.h"
#include "FragilityCurveHandler.h"
#include "FragilityValueHandler.h"
#include "HistogramValueHandler.h"
#include "LimitStateFunctionHandler.h"
#include "MessageHandler.h"
#include "ModelParameterHandler.h"
#include "ProbabilityLimitStateFunctionHandler.h"
#include "ProbabilityValueHandler.h"
#include "ReliabilityResultHandler.h"
#include "ScenarioHandler.h"
#include "SelfCorrelationMatrixHandler.h"
#include "SensitivityResultHandler.h"
#include "SensitivityValueHandler.h"
#include "StandardNormalHandler.h"
#include "StochastHandler.h"
#include "StochastPointAlphaHandler.h"
#include "StochastPointHandler.h"
#include "UncertaintyResultHandler.h"
#include "ValidationReportHandler.h"

namespace Deltares::Server
{
    class HandlerAdmin
    {
    public:
        HandlerAdmin();

        HistogramValueHandler histogramValueHandler;
        DiscreteValueHandler discreteValueHandler;
        FragilityValueHandler fragilityValueHandler;
        MessageHandler messageHandler;
        EvaluationHandler evaluationHandler;
        ProbabilityValueHandler probabilityValueHandler;
        StandardNormalHandler standardNormalHandler;
        StochastHandler stochastHandler;
        FragilityCurveHandler fragilityCurveHandler;
        ScenarioHandler scenarioHandler;
        ValidationReportHandler validationReportHandler;
        ModelParameterHandler modelParameterHandler;
        LimitStateFunctionHandler limitStateFunctionHandler;
        ProbabilityLimitStateFunctionHandler probabilityLimitStateFunctionHandler;
        CombinedLimitStateFunctionHandler combinedLimitStateFunctionHandler;
        ReliabilityResultHandler reliabilityResultHandler;
        ConvergenceReportHandler convergenceReportHandler;
        ContributingStochastHandler contributingStochastHandler;
        ConditionalValueHandler conditionalValueHandler;
        CorrelationMatrixHandler correlationMatrixHandler;
        CopulaCorrelationHandler copulaCorrelationHandler;
        SelfCorrelationMatrixHandler selfCorrelationMatrixHandler;
        StochastPointAlphaHandler alphaHandler;
        StochastPointHandler stochastPointHandler;
        DesignPointHandler designPointHandler;
        UncertaintyResultHandler uncertaintyResultHandler;
        SensitivityResultHandler sensitivityResultHandler;
        SensitivityValueHandler sensitivityValueHandler;

        int GetNewId()
        {
            return admin.GetNewId();
        }

        bool IsSupported(ObjectType objectType);

        void RegisterType(int id, ObjectType objectType)
        {
            admin.RegisterType(id, objectType);
        }

        ObjectType GetObjectType(int id)
        {
            return admin.GetObjectType(id);
        }

        int Create(ObjectType objectType)
        {
            return handlers[objectType]->Create();
        }

        bool Contains(int id)
        {
            return admin.Contains(id);
        }

        void Destroy(int id)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->Destroy(id);
        }

        void Remove(int id)
        {
            admin.Remove(id);
        }

        bool IsEmpty()
        {
            return admin.IsEmpty();
        }

        int GetSize() const
        {
            return admin.GetSize();
        }

        double GetValue(int id, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetValue(id, property_);
        }

        void SetValue(int id, const std::string& property_, double value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetValue(id, property_, value);
        }

        std::string GetStringValue(int id, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetStringValue(id, property_);
        }

        void SetStringValue(int id, const std::string& property_, std::string value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetStringValue(id, property_, value);
        }

        int GetIntValue(int id, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetIntValue(id, property_);
        }

        int GetIdValue(int id, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetIdValue(id, property_);
        }

        void SetIntValue(int id, const std::string& property_, int value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetIntValue(id, property_, value);
        }

        bool GetBoolValue(int id, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetBoolValue(id, property_);
        }

        void SetBoolValue(int id, const std::string& property_, bool value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetBoolValue(id, property_, value);
        }

        double GetIntArgValue(int id1, int id2, const std::string& property_)
        {
            ObjectType objectType = admin.GetObjectType(id1);
            return handlers[objectType]->GetIntArgValue(id1, id2, property_);
        }

        void SetIntArgValue(int id1, int id2, const std::string& property_, double value)
        {
            ObjectType objectType = admin.GetObjectType(id1);
            return handlers[objectType]->SetIntArgValue(id1, id2, property_, value);
        }

        int GetIndexedIdValue(int id, const std::string& property_, int index)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetIndexedIdValue(id, property_, index);
        }

        void SetArrayValue(int id, const std::string& property_, double* values, int size)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetArrayValue(id, property_, values, size);
        }

        void SetArrayIntValue(int id, const std::string& property_, int* values, int size)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetArrayIntValue(id, property_, values, size);
        }

        double GetArgValue(int id, const std::string& property_, double argument)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetArgValue(id, property_, argument);
        }

        void SetArgValue(int id, const std::string& property_, double argument, double value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->SetArgValue(id, property_, argument, value);
        }

        double GetIndexedValue(int id, const std::string& property_, int index)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetIndexedValue(id, property_, index);
        }

        double GetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->GetIndexedIndexedValue(id, property_, index1, index2);
        }

        void SetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2, double value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->SetIndexedIndexedValue(id, property_, index1, index2, value);
        }

        void SetIndexedIndexedIntValue(int id, const std::string& property_, int index1, int index2, int value)
        {
            ObjectType objectType = admin.GetObjectType(id);
            return handlers[objectType]->SetIndexedIndexedIntValue(id, property_, index1, index2, value);
        }

        void Execute(int id, const std::string& method_)
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->Execute(id, method_);
        }

    private:

        ObjectHandlerAdmin admin;
        std::unordered_map<ObjectType, ObjectHandler*> handlers;

    };
}
