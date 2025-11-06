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

#include "FORMSettings.h"
#include "DirectionalSamplingSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
    namespace Reliability
    {
        // class for FORMThenDirectionalSampling: FORM, but in case of non convergence fall back on Directional Sampling
        class FORMThenDirectionalSampling : public ReliabilityMethod
        {
        public:
            FORMThenDirectionalSampling() {}
            FORMThenDirectionalSampling(const double threshold) : thresholdBeta(threshold) {}
            std::shared_ptr<FORMSettings> formSettings = std::make_shared<FORMSettings>();
            std::shared_ptr<DirectionalSamplingSettings> DsSettings = std::make_shared<DirectionalSamplingSettings>();
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;

            bool isValid() override
            {
                return formSettings->isValid() && DsSettings->isValid();
            }

        private:
            const double thresholdBeta = -999.0; // minimal beta value to accept as result from FORM
        };
    }
}
