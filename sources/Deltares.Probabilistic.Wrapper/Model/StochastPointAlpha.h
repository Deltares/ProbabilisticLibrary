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

#include "../../Deltares.Probabilistic/Model/StochastPointAlpha.h"
#include "../Statistics/Stochast.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models;

            public ref class StochastPointAlpha
            {
            private:
                Statistics::Wrappers::Stochast^ parameter = gcnew Statistics::Wrappers::Stochast();
                SharedPointerProvider<Models::StochastPointAlpha>* shared = nullptr;

            public:
                StochastPointAlpha()
                {
                    shared = new SharedPointerProvider(new Models::StochastPointAlpha());
                }
                StochastPointAlpha(std::shared_ptr<Models::StochastPointAlpha> alpha)
                {
                    shared = new SharedPointerProvider(alpha);
                }
                ~StochastPointAlpha() { this->!StochastPointAlpha(); }
                !StochastPointAlpha() { delete shared; }

                property Statistics::Wrappers::Stochast^ Parameter
                {
                    Statistics::Wrappers::Stochast ^ get() { return parameter; }
                    void set(Statistics::Wrappers::Stochast ^ value) { parameter = value; }
                }

                property double Alpha
                {
                    double get() { return shared->object->Alpha; }
                    void set(double value) { shared->object->Alpha = value; }
                }

                property double AlphaCorrelated
                {
                    double get() { return shared->object->AlphaCorrelated; }
                    void set(double value) { shared->object->AlphaCorrelated = value; }
                }

                property double U
                {
                    double get() { return shared->object->U; }
                    void set(double value) { shared->object->U = value; }
                }

                property double X
                {
                    double get() { return shared->object->X; }
                    void set(double value) { shared->object->X = value; }
                }

                property double Index
                {
                    double get() { return shared->object->Index; }
                    void set(double value) { shared->object->Index = value; }
                }

                property double InfluenceFactor
                {
                    double get() { return shared->object->InfluenceFactor; }
                    void set(double value) { shared->object->InfluenceFactor = value; }
                }

                    std::shared_ptr<Models::StochastPointAlpha> GetNativeAlpha()
                {
                    shared->object->Stochast = this->Parameter != nullptr ? this->Parameter->GetStochast() : nullptr;
                    return shared->object;
                }
            };
        }
    }
}

