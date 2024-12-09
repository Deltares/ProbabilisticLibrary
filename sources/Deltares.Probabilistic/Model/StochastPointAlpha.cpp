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
#include "StochastPointAlpha.h"

namespace Deltares
{
    namespace Models
    {
        std::shared_ptr<StochastPointAlpha> StochastPointAlpha::clone()
        {
            std::shared_ptr<StochastPointAlpha> clone = std::make_shared<StochastPointAlpha>();
            clone->Alpha = this->Alpha;
            clone->AlphaCorrelated = this->AlphaCorrelated;
            clone->Stochast = this->Stochast;
            clone->U = this->U;
            clone->X = this->X;

            return clone;
        }

        void StochastPointAlpha::invert()
        {
            this->Alpha = - this->Alpha;
            this->AlphaCorrelated = - this->AlphaCorrelated;
            this->U = - this->U;
            this->X = this->Stochast->getXFromU(this->U);
        }

        std::string StochastPointAlpha::getIdentifier()
        {
            if (this->Stochast == nullptr)
            {
                return "";
            }
            else if (this->Stochast->modelParameter->isArray)
            {
                return this->Stochast->name + "[" + std::to_string(this->Index) + "]";
            }
            else
            {
                return this->Stochast->name;
            }
        }
    }
}


