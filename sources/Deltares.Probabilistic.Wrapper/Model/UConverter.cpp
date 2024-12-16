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
#include "UConverter.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            UConverter::UConverter(System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts, Statistics::Wrappers::CorrelationMatrix^ correlationMatrix)
            {
                std::vector<std::shared_ptr<Statistics::Stochast>> nativeStochasts;

                for (int i = 0; i < stochasts->Count; i++)
                {
                    nativeStochasts.push_back(stochasts[i]->GetStochast());
                }

                std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(nativeStochasts, correlationMatrix->GetCorrelationMatrix());
                this->shared = new SharedPointerProvider(uConverter);

                this->shared->object->initializeForRun();
            }

            bool UConverter::IsVaryingStochast(int index)
            {
                return shared->object->isVaryingStochast(index);
            }

            StochastPoint^ UConverter::GetBaseStochastPoint(double beta, array<double>^ alphas, System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts)
            {
                std::shared_ptr<Models::StochastPoint> stochastPoint = shared->object->GetStochastPoint(beta, NativeSupport::toNative(alphas));

                return gcnew StochastPoint(stochastPoint, stochasts);
            }
        }
    }
}
