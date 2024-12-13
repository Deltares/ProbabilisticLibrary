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
#include "Sample.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Models
    {
        using namespace Deltares::Numeric;

        int Sample::getSize()
        {
            return size;
        }

        double Sample::getBeta()
        {
            return Numeric::NumericSupport::GetLength(Values);
        }

        double Sample::getDistance(const std::shared_ptr<Sample>& other) const
        {
            return Numeric::NumericSupport::getDistance(this->Values, other->Values);
        }

        double Sample::getDistance2(const std::shared_ptr<Sample>& other) const
        {
            return Numeric::NumericSupport::getDistance2(this->Values, other->Values);
        }

        void Sample::setInitialValues(double beta)
        {
            double value = sqrt(beta * beta / size) * Numeric::NumericSupport::GetSign(beta);

            for (int i = 0; i < size; i++)
            {
                Values[i] = value;
            }
        }

        std::shared_ptr<Sample> Sample::getSampleAtBeta(double newBeta)
        {
            const double actualBeta = this->getBeta();

            std::shared_ptr<Sample> normalizedSample = this->clone();

            if (actualBeta == 0)
            {
                const double defaultValue = newBeta * sqrt(1.0 / this->size);
                for (int k = 0; k < this->size; k++)
                {
                    normalizedSample->Values[k] = defaultValue;
                }
            }
            else
            {
                const double factor = newBeta / actualBeta;

                for (int k = 0; k < this->size; k++)
                {
                    normalizedSample->Values[k] = factor * this->Values[k];
                }
            }

            return normalizedSample;
        }

        std::shared_ptr<Sample> Sample::getMultipliedSample(double factor)
        {
            std::shared_ptr<Sample> multipliedSample = this->clone();

            for (int i = 0; i < this->size; i++)
            {
                multipliedSample->Values[i] = factor * this->Values[i];
            }

            return multipliedSample;
        }

        void Sample::correctSmallValues(double tolerance)
        {
            for (int k = 0; k < this->getSize(); k++)
            {
                if (std::abs(this->Values[k]) < tolerance)
                {
                    this->Values[k] = 0;
                }
            }
        }

        bool Sample::areValuesEqual(std::shared_ptr<Sample> other)
        {
            if (this->Values.size() != other->Values.size())
            {
                return false;
            }

            for (size_t i = 0; i < this->Values.size(); i++)
            {
                if (this->Values[i] != other->Values[i])
                {
                    return false;
                }
            }

            return true;
        }

        std::shared_ptr<Sample> Sample::clone()
        {
            std::shared_ptr<Sample> clonedSample = std::make_shared<Sample>(this->Values);

            clonedSample->AllowProxy = this->AllowProxy;
            clonedSample->IterationIndex = this->IterationIndex;
            clonedSample->threadId = this->threadId;
            clonedSample->Weight = this->Weight;

            return clonedSample;
        }
    }
}


