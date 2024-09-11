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

#include "BaseStochastPoint.h"
#include "ModelSample.h"
#include "StochastPointAlpha.h"
#include "../../Deltares.Probabilistic/Model/StochastPoint.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public ref class StochastPoint : public Wrappers::BaseStochastPoint
            {
            private:
                SharedPointerProvider<Models::StochastPoint>* shared = nullptr;

                System::Collections::Generic::List<StochastPointAlpha^>^ alphas = gcnew System::Collections::Generic::List<StochastPointAlpha^>();

            protected:

                void setStochastPoint(std::shared_ptr<Models::StochastPoint> stochastPoint, System::Collections::Generic::IList<Statistics::Wrappers::Stochast^>^ stochasts);

            public:
                StochastPoint()
                {
                    shared = new SharedPointerProvider(new Models::StochastPoint());
                }

                StochastPoint(std::shared_ptr<Models::StochastPoint> stochastPoint, System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts)
                {
                    setStochastPoint(stochastPoint, stochasts);
                }

                ~StochastPoint() { this->!StochastPoint(); }
                !StochastPoint() { delete shared;    }

                property double Beta
                {
                    double get() { return shared->object->Beta; }
                    void set(double value) { shared->object->Beta = value; }
                }

                property System::Collections::Generic::List<StochastPointAlpha^>^ Alphas
                {
                    System::Collections::Generic::List<StochastPointAlpha^>^ get() { return alphas; }
                }

                ModelSample^ GetModelSample()
                {
                    return gcnew ModelSample(shared->object->getModelSample());
                }
            };
        }
    }
}


