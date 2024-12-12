// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
#include "FORMThenDirectionalSampling.h"
#include "FORM.h"
#include "DirectionalSampling.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> FORMThenDirectionalSampling::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            auto form = FORM();
            form.Settings = formSettings;

            auto formDesignPoint = form.getDesignPoint(modelRunner);

            if (formDesignPoint->convergenceReport->IsConverged)
            {
                // If the resulting beta is below some threshold, we prefer Directional Sampling (DS)
                // But with only 1 stochast, DS will not improve the result.
                if (modelRunner->getVaryingStochastCount() == 1 || formDesignPoint->Beta >= thresholdBeta)
                {
                    return formDesignPoint;
                }
            }

            modelRunner->clear();

            if (modelRunner->Settings->RunAtDesignPoint)
            {
                modelRunner->runDesignPoint(formDesignPoint);
            }

            auto ds = DirectionalSampling();
            ds.Settings = DsSettings;
            auto dsDesignPoint = ds.getDesignPoint(modelRunner);
            dsDesignPoint->convergenceReport->TotalIterations = formDesignPoint->convergenceReport->TotalIterations;
            dsDesignPoint->ContributingDesignPoints.push_back(formDesignPoint);

            return dsDesignPoint;
        }
    }
}
