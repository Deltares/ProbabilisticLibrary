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

#include "correlationValueAndType.h"
#include "../Logging/ValidationReport.h"

namespace Deltares::Statistics
{
    ///
    /// base class for Archimedean type Copulas.
    ///
    class BaseCopula
    {
    public:
        /// <summary>
        /// update a pair of correlated stochastic variables
        /// </summary>
        /// <param name="a"> value of first stochast in u-space </param>
        /// <param name="b"> value of second stochast in u-space </param>
        virtual void update_uspace(const double& a, double& b) const;

        /// <summary>
        /// update a pair of correlated stochastic variables
        /// </summary>
        /// <param name="u"> probability of failure of first stochast </param>
        /// <param name="t"> probability of failure of second stochast </param>
        virtual void update(const double& u, double& t) const = 0;

        virtual correlationValueAndType getCorrelation() const { return { 0.0, correlationType::Gaussian }; }

        virtual bool isValid() const;

        virtual void validate(Logging::ValidationReport& report) const = 0;
    };
}

