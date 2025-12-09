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
#include "SparseCorrelationMatrix.h"

namespace Deltares
{
    namespace Statistics
    {
        double SparseCorrelationMatrix::getCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2)
        {
            for (std::shared_ptr<CorrelationValue> correlationValue: this->correlations)
            {
                if (correlationValue->matches(stochast1, stochast2))
                {
                    return correlationValue->value;
                }
            }

            if (stochast1 == stochast2)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        void SparseCorrelationMatrix::setCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2, double value)
        {
            std::shared_ptr<CorrelationValue> correlationValue = nullptr;

            for (std::shared_ptr<CorrelationValue> existingCorrelationValue : this->correlations)
            {
                if (existingCorrelationValue->matches(stochast1, stochast2))
                {
                    correlationValue = existingCorrelationValue;
                    break;
                }
            }

            bool isDefaultCorrelationValue = stochast1 == stochast2 && value == 1.0 || stochast1 != stochast2 && value == 0.0;

            if (correlationValue == nullptr && !isDefaultCorrelationValue)
            {
                correlationValue = std::make_shared<CorrelationValue>(stochast1, stochast2);
                correlationValue->value = value;
                this->correlations.push_back(correlationValue);
            }
            else if (correlationValue != nullptr && !isDefaultCorrelationValue)
            {
                correlationValue->value = value;
            }
            else if (correlationValue == nullptr && isDefaultCorrelationValue)
            {
                // nothing to do
            }
            else if (correlationValue != nullptr && isDefaultCorrelationValue)
            {
                std::erase(this->correlations, correlationValue);
            }
        }

        std::shared_ptr<CorrelationMatrix> SparseCorrelationMatrix::getCorrelationMatrix(std::vector<std::shared_ptr<Stochast>> stochasts)
        {
            std::shared_ptr<CorrelationMatrix> correlationMatrix = std::make_shared<CorrelationMatrix>();

            correlationMatrix->init(stochasts);

            for (std::shared_ptr<CorrelationValue> correlationValue : this->correlations)
            {
                correlationMatrix->SetCorrelation(correlationValue->stochast1, correlationValue->stochast2, correlationValue->value, CorrelationType::Gaussian);
            }

            return correlationMatrix;
        }
    }
}
