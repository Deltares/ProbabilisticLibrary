// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include <memory>

#include "../../Deltares.Probabilistic/Reliability/ConvergenceReport.h"
#include "../Model/Sample.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class ConvergenceReport
            {
            public:
                ConvergenceReport() {}
                ConvergenceReport(std::shared_ptr<Reliability::ConvergenceReport> report)
                {
                    this->Convergence = report->Convergence;
                    this->FailedSamples = report->FailedSamples;
                    this->FailFraction = report->FailFraction;
                    this->RelaxationFactor = report->RelaxationFactor;
                    this->VarianceFactor = report->VarianceFactor;
                    this->MaxWeight = report->MaxWeight;
                    this->FailWeight = report->FailWeight;
                    this->FailedSamples = report->FailedSamples;
                    this->ZMargin = report->ZMargin;
                    this->IsConverged = report->IsConverged;
                }

                property double Convergence;
                property int FailedSamples;
                property double FailFraction;
                property double RelaxationFactor;
                property double VarianceFactor;
                property double MaxWeight;
                property double FailWeight;
                property double ZMargin;
                property bool IsConverged;

                Deltares::Models::Wrappers::Sample^ NearestStochastPoint = nullptr;
            };
        }
    }
}


