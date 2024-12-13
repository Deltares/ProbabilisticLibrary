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

#include "../Utils/DirtySupport.h"
#include <memory>

namespace Deltares
{
    namespace Statistics
    {
        class DiscreteValue
        {
        public:
            DiscreteValue()    { }

            DiscreteValue(double x, double amount)
            {
                this->X = x;
                this->Amount = amount;
            }

            ~DiscreteValue()
            {
            }

            double X = 0;
            double Amount = 0;

            double NormalizedAmount = 0; // for internal use
            double CumulativeNormalizedAmount = 0; // for internal use

            void setDirtyFunction(Utils::SetDirtyLambda setDirtyLambda)
            {
                this->setDirtyLambda = setDirtyLambda;
            }

            void setDirty()
            {
                if (setDirtyLambda != nullptr)
                {
                    setDirtyLambda();
                }
            }

            std::shared_ptr<DiscreteValue> clone()
            {
                std::shared_ptr<DiscreteValue> clone = std::make_shared<DiscreteValue>();

                clone->X = this->X;
                clone->Amount = this->Amount;

                return clone;
            }

        private:
            Utils::SetDirtyLambda setDirtyLambda = nullptr;
        };
    }
}

