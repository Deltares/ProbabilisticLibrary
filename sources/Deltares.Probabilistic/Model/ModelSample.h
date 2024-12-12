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
#pragma once

#include <cmath>
#include <vector>
#include <memory>

namespace Deltares
{
    namespace Models
    {
        class ModelSample
        {
        public:
            ModelSample(std::vector<double> values)
            {
                this->Values = values;
            }

            /**
             * \brief Resets all contents of the sample to its default values
             * \remark Values are not cleared, since they are provided in the constructor
             */
            void clear()
            {
                IterationIndex = -1;
                threadId = 0;
                Weight = std::nan("");
                AllowProxy = true;
                IsRestartRequired = false;
                Z = std::nan("");
                Beta = 0;
                Tag = 0;
            }

            std::vector<double> Values;
            std::vector<double> OutputValues;

            int IterationIndex = -1;
            int threadId = 0;
            double Weight = 1;
            bool AllowProxy = true;
            bool IsRestartRequired = false;
            double Beta = 0;
            double Z = nan("");

            bool ExtendedLogging = false;

            /**
             * Indication of a logging message
             */
            int LoggingCounter = 0;

            int Tag = 0;

            bool hasSameValues(std::shared_ptr<ModelSample> other)
            {
                if (this->Values.size() != other->Values.size())
                {
                    return false;
                }

                for (int i = 0; i < this->Values.size(); i++)
                {
                    if (this->Values[i] != other->Values[i])
                    {
                        return false;
                    }
                }

                return true;
            }
        };
    }
}

