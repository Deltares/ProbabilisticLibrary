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
#include "ModelProject.h"

namespace Deltares
{
    namespace Models
    {
        void ModelProject::updateStochasts()
        {
            this->stochasts.clear();

            if (model != nullptr)
            {
                for (std::shared_ptr<ModelInputParameter> parameter : model->inputParameters)
                {
                    std::shared_ptr<Statistics::Stochast> stochast = nullptr;
                    if (this->existingStochasts.contains(parameter->name))
                    {
                        stochast = this->existingStochasts[parameter->name];
                    }
                    else
                    {
                        stochast = std::make_shared<Statistics::Stochast>();
                        stochast->name = parameter->name;
                        stochast->setMean(parameter->defaultValue);
                        existingStochasts[stochast->name] = stochast;
                    }

                    this->stochasts.push_back(stochast);
                }
            }
        }
    }
}

