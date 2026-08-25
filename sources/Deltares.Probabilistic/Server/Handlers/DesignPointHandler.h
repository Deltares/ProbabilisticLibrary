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

#include <string>

#include "EvaluationHandler.h"
#include "MessageHandler.h"
#include "ReliabilityResultHandler.h"
#include "StochastPointAlphaHandler.h"
#include "StoredObjectHandler.h"
#include "../../Reliability/DesignPoint.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class DesignPoint
     */
    class DesignPointHandler : public StoredObjectHandler<Reliability::DesignPoint>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::DesignPoint;
        }

        double GetValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_) override
        {
            if (property_ == "beta") return designPoint->Beta;
            else if (property_ == "reliability_index") return designPoint->Beta;
            else if (property_ == "probability_failure") return designPoint->getFailureProbability();
            else if (property_ == "probability_non_failure") return designPoint->getNonFailureProbability();
            else if (property_ == "return_period") return designPoint->getReturnPeriod();
            else if (property_ == "convergence" && designPoint->convergenceReport != nullptr)
            {
                return designPoint->convergenceReport->Convergence;
            }
            else return StoredObjectHandler::GetValue(designPoint, property_);
        }

        void SetValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_, double value) override
        {
            if (property_ == "beta") designPoint->Beta = value;
            else if (property_ == "reliability_index") designPoint->Beta = value;
            else if (property_ == "convergence")
            {
                if (designPoint->convergenceReport == nullptr) designPoint->convergenceReport = std::make_shared<Reliability::ConvergenceReport>();
                designPoint->convergenceReport->Convergence = value;
            }
            else StoredObjectHandler::SetValue(designPoint, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_) override
        {
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
            else if (property_ == "reliability_results_count") return static_cast<int>(designPoint->ReliabilityResults.size());
            else if (property_ == "messages_count") return static_cast<int>(designPoint->Messages.size());
            else return StoredObjectHandler::GetIntValue(designPoint, property_);
        }

        int GetIdValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_) override
        {
            if (property_ == "convergence_report") return convergenceReportHandler->GetObjectId(designPoint->convergenceReport);
            else return StoredObjectHandler::GetIdValue(designPoint, property_);
        }

        void SetIntValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_, int value) override
        {
            //if (property_ == "ids") designPoint->Ids = GetDesignPointIds(value);
            if (property_ == "total_iterations") designPoint->convergenceReport->TotalIterations = value;
            else if (property_ == "total_directions") designPoint->convergenceReport->TotalDirections = value;
            else if (property_ == "total_model_runs") designPoint->convergenceReport->TotalModelRuns = value;
            else StoredObjectHandler::SetIntValue(designPoint, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_) override
        {
            if (property_ == "is_converged") return designPoint->convergenceReport != nullptr && designPoint->convergenceReport->IsConverged;
            else return StoredObjectHandler::GetBoolValue(designPoint, property_);
        }

        void SetBoolValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_, bool value) override
        {
            if (property_ == "is_converged") designPoint->convergenceReport->IsConverged = value;
            else StoredObjectHandler::SetBoolValue(designPoint, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_) override
        {
            if (property_ == "identifier") return designPoint->Identifier;
            else return StoredObjectHandler::GetStringValue(designPoint, property_);
        }

        void SetStringValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_, const std::string& value) override
        {
            if (property_ == "identifier") designPoint->Identifier = value;
            else StoredObjectHandler::SetStringValue(designPoint, property_, value);
        }

        void SetArrayIntValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_, int* values, int size) override
        {
            if (property_ == "alphas")
            {
                designPoint->Alphas.clear();
                for (int i = 0; i < size; i++)
                {
                    designPoint->Alphas.push_back(alphaHandler->GetObject(values[i]));
                }
            }
            else StoredObjectHandler::SetArrayIntValue(designPoint, property_, values, size);
        }

        int GetIndexedIdValue(const std::shared_ptr<Reliability::DesignPoint>& designPoint, const std::string& property_, int index) override
        {
            if (property_ == "contributing_design_points") return GetObjectId(designPoint->ContributingDesignPoints[index]);
            else if (property_ == "alphas") return alphaHandler->GetObjectId(designPoint->Alphas[index]);
            else if (property_ == "evaluations") return evaluationHandler->GetObjectId(designPoint->Evaluations[index]);
            else if (property_ == "reliability_results") return reliabilityResultHandler->GetObjectId(designPoint->ReliabilityResults[index]);
            else if (property_ == "messages") return messageHandler->GetObjectId(designPoint->Messages[index]);
            else return StoredObjectHandler::GetIndexedIdValue(designPoint, property_, index);
        }

        ConvergenceReportHandler* convergenceReportHandler = nullptr;
        EvaluationHandler* evaluationHandler = nullptr;
        ReliabilityResultHandler* reliabilityResultHandler = nullptr;
        MessageHandler* messageHandler = nullptr;
        StochastPointAlphaHandler* alphaHandler = nullptr;
    };
}

