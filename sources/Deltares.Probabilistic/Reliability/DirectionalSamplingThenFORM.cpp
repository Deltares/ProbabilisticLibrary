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
#include "DirectionalSamplingThenFORM.h"
#include "FORM.h"
#include "DirectionalSampling.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> DirectionalSamplingThenFORM::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            auto ds = DirectionalSampling();
            ds.Settings = DsSettings;
            auto dsDesignPoint = ds.getDesignPoint(modelRunner);

            auto nStoch = dsDesignPoint->Alphas.size();
            auto startVector = std::vector<double>(nStoch);
            for (size_t i = 0; i < nStoch; i++)
            {
                double u = -dsDesignPoint->Beta * dsDesignPoint->Alphas[i]->Alpha;
                startVector[i] = u;
            }

            if (modelRunner->Settings->RunAtDesignPoint)
            {
                modelRunner->runDesignPoint(dsDesignPoint);
            }

            auto form = FORM();
            form.Settings = formSettings;
            form.Settings->StartPointSettings->StartMethod = StartMethodType::GivenVector;
            form.Settings->StartPointSettings->startVector = startVector;

            auto dsfiDesignPoint = form.getDesignPoint(modelRunner);
            dsfiDesignPoint->convergenceReport->TotalDirections = dsDesignPoint->convergenceReport->TotalDirections;
            dsfiDesignPoint->Beta = dsDesignPoint->Beta;
            dsfiDesignPoint->ContributingDesignPoints.push_back(dsDesignPoint);

            return dsfiDesignPoint;
        }
    }
}
