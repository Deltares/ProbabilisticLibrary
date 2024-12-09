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

#include "ProbabilityValue.h"
#include "BaseStochastPoint.h"
#include "../Utils/DirtySupport.h"

#include <memory>


namespace Deltares::Models
{
    class StochastPoint;
}

namespace Deltares
{
    namespace Statistics
    {
        /**
         * \brief Point in a fragility curve or empirical distribution
         */
        class FragilityValue : public ProbabilityValue
        {
        public:
            /**
             * \brief Physical value for which the reliability is defined
             */
            double X = 0;

            /**
             * \brief Design point corresponding with the reliability
             * \remark This property is always of the type designPoint. The only reason it is declared as BaseStochastPoint is that C++ does not allow circular references
             */
            std::shared_ptr<Models::StochastPoint> designPoint = nullptr;

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

            std::shared_ptr<FragilityValue> clone()
            {
                std::shared_ptr<FragilityValue> clone = std::make_shared<FragilityValue>();

                clone->X = this->X;
                clone->Reliability = this->Reliability;
                clone->designPoint = this->designPoint;

                return clone;
            }

        private:
            Utils::SetDirtyLambda setDirtyLambda = nullptr;
        };
    }
}

