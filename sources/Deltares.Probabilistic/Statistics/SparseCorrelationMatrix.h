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
#include <vector>

#include "CorrelationMatrix.h"
#include "Stochast.h"

namespace Deltares
{
    namespace Statistics
    {
        class SparseCorrelationMatrix
        {
        public:
            double getCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2);
            void setCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2, double value);

            std::shared_ptr<CorrelationMatrix> getCorrelationMatrix(std::vector<std::shared_ptr<Stochast>> stochasts);
        private:
            class CorrelationValue
            {
            public:
                CorrelationValue(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2)
                {
                    this->stochast1 = stochast1;
                    this->stochast2 = stochast2;
                }

                std::shared_ptr<Stochast> stochast1 = nullptr;
                std::shared_ptr<Stochast> stochast2 = nullptr;
                double value = 0;

                bool matches(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2) const
                {
                    return this->stochast1 == stochast1 && this->stochast2 == stochast2 || this->stochast1 == stochast2 && this->stochast2 == stochast1;
                }
            };

            std::vector<std::shared_ptr<CorrelationValue>> correlations;
        };
    }
}

