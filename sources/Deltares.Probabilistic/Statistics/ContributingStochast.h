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

#include <memory>
#include "BaseStochast.h"
#include "../Logging/ValidationReport.h"
#include "../Logging/ValidationSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        class ContributingStochast
        {
        public:
            double Probability = 0;
            std::shared_ptr<BaseStochast> Stochast = nullptr;

            void validate(Logging::ValidationReport& report) const
            {
                Logging::ValidationSupport::checkMinimum(report, 0, Probability, "probability");
                Logging::ValidationSupport::checkMaximum(report, 1, Probability, "probability");

                Stochast->validate(report);
            }

            bool isValid() const
            {
                Logging::ValidationReport report;
                this->validate(report);
                return report.isValid();
            }

            std::shared_ptr<ContributingStochast> clone() const
            {
                std::shared_ptr<ContributingStochast> clone = std::make_shared<ContributingStochast>();

                clone->Probability = this->Probability;
                clone->Stochast = this->Stochast;

                return clone;
            }
        };
    }
}

