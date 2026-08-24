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
#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Reliability/ConvergenceReport.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ConvergenceReport
     */
    class ConvergenceReportHandler : public StoredObjectHandler<Reliability::ConvergenceReport>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::ConvergenceReport;
        }

        double GetValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Reliability::ConvergenceReport> convergence_report = GetObject(id);

            if (property_ == "convergence") return convergence_report->Convergence;
            else if (property_ == "fail_fraction") return convergence_report->FailFraction;
            else if (property_ == "fail_weight") return convergence_report->FailWeight;
            else if (property_ == "max_weight") return convergence_report->MaxWeight;
            else if (property_ == "relaxation_factor") return convergence_report->RelaxationFactor;
            else if (property_ == "variance_factor") return convergence_report->VarianceFactor;
            else if (property_ == "z_margin") return convergence_report->ZMargin;
            else return ObjectHandler::GetValue(id, property_);
        }

        void SetValue(int id, const std::string& property_, double value) override
        {
            std::shared_ptr<Reliability::ConvergenceReport> convergence_report = GetObject(id);

            if (property_ == "convergence") convergence_report->Convergence = value;
            else if (property_ == "fail_fraction") convergence_report->FailFraction = value;
            else if (property_ == "fail_weight") convergence_report->FailWeight = value;
            else if (property_ == "max_weight") convergence_report->MaxWeight = value;
            else if (property_ == "relaxation_factor") convergence_report->RelaxationFactor = value;
            else if (property_ == "variance_factor") convergence_report->VarianceFactor = value;
            else if (property_ == "z_margin") convergence_report->ZMargin = value;
            else ObjectHandler::SetValue(id, property_, value);
        }

        int GetIntValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Reliability::ConvergenceReport> convergence_report = GetObject(id);

            if (property_ == "failed_samples") return convergence_report->FailedSamples;
            else return StoredObjectHandler::GetIntValue(id, property_);
        }

        void SetIntValue(int id, const std::string& property_, int value) override
        {
            std::shared_ptr<Reliability::ConvergenceReport> convergence_report = GetObject(id);

            if (property_ == "failed_samples") convergence_report->FailedSamples = value;
            else return StoredObjectHandler::SetIntValue(id, property_, value);
        }

        bool GetBoolValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Reliability::ConvergenceReport> convergence_report = GetObject(id);

            if (property_ == "is_converged") return convergence_report->IsConverged;
            else return StoredObjectHandler::GetBoolValue(id, property_);
        }

        void SetBoolValue(int id, const std::string& property_, bool value) override
        {
            std::shared_ptr<Reliability::ConvergenceReport> convergence_report = GetObject(id);

            if (property_ == "is_converged") convergence_report->IsConverged = value;
            else StoredObjectHandler::SetBoolValue(id, property_, value);
        }
    };
}

