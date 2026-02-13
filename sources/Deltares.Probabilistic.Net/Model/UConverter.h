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

#include "../Utils/SharedPointerProvider.h"
#include "../../Deltares.Probabilistic/Model/UConverter.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Model/StochastPoint.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            public ref class UConverter
            {
            private:
                array<double>^ values = nullptr;
                System::Object^ tag = nullptr;
                Utils::Wrappers::SharedPointerProvider<Models::UConverter>* shared = nullptr;

            public:
                UConverter(System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts, Statistics::Wrappers::CorrelationMatrix^ correlationMatrix);
                ~UConverter() { this->!UConverter(); }
                !UConverter() { delete shared; }

                virtual property int VaryingStochastCount
                {
                    int get() { return shared->object->getVaryingStochastCount(); }
                }

                bool IsVaryingStochast(int index);

                virtual Wrappers::StochastPoint^ GetBaseStochastPoint(double beta, array<double>^ alphas, System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts);

                std::shared_ptr<Models::UConverter> GetConverter()
                {
                    return shared->object;
                }
            };
        }
    }
}

