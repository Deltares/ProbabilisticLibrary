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
#include "StochastPoint.h"
#include "../Statistics/Stochast.h"

#include <map>

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            void StochastPoint::setStochastPoint(std::shared_ptr<Models::StochastPoint> stochastPoint, System::Collections::Generic::IList<Statistics::Wrappers::Stochast^>^ stochasts)
            {
                shared = new SharedPointerProvider(stochastPoint);

                std::map<std::shared_ptr<Statistics::Stochast>, int> stochastMapping;

                for (int i = 0; i < stochasts->Count; i++)
                {
                    stochastMapping[stochasts[i]->GetStochast()] = i;
                }

                for (int i = 0; i < stochastPoint->Alphas.size(); i++)
                {
                    StochastPointAlpha^ alphaWrapper = gcnew StochastPointAlpha(stochastPoint->Alphas[i]);
                    alphaWrapper->Parameter = stochasts[stochastMapping[stochastPoint->Alphas[i]->Stochast]];
                    this->Alphas->Add(alphaWrapper);
                }
            }
        }
    }
}

