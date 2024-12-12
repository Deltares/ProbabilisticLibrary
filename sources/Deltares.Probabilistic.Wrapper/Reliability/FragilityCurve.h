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

#include "DesignPoint.h"
#include "../Statistics/Stochast.h"
#include "../../Deltares.Probabilistic/Reliability/FragilityCurve.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class FragilityCurve : public Statistics::Wrappers::Stochast
            {
            private:
                SharedPointerProvider<Reliability::FragilityCurve>* shared = nullptr;

                bool HasMatchingFragilityValues();
                void  updateNativeObject();

                bool synchronizing = false;
            protected:
                void setNativeObject(std::shared_ptr<Statistics::Stochast> nativeStochast) override
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(static_pointer_cast<Reliability::FragilityCurve>(nativeStochast));
                    Stochast::setNativeObject(nativeStochast);
                }
            public:
                FragilityCurve() : Stochast(std::make_shared<Reliability::FragilityCurve>()) {}
                ~FragilityCurve() { this->!FragilityCurve(); }
                !FragilityCurve() { delete shared; }

                virtual property bool Inverted
                {
                    bool get() override { return shared->object->inverted; }
                    void set(bool value) override { shared->object->inverted = value; }
                }

                virtual property bool Fixed
                {
                    bool get() { return shared->object->fixed; }
                    void set(bool value) { shared->object->fixed = value; }
                }

                virtual property double FixedValue
                {
                    double get() { return shared->object->fixedValue; }
                    void set(double value) { shared->object->fixedValue = value; }
                }

                virtual Deltares::Models::Wrappers::StochastPoint^ GetRealization(double x);

                // TODO: PROBL-42 remove when fragility curve integration to c++ is complete
                bool IsGloballyDescending()
                {
                    this->updateNativeObject();
                    return shared->object->isGloballyDescending();
                }

                std::shared_ptr<Reliability::FragilityCurve> GetNativeValue()
                {
                    this->updateNativeObject();
                    return shared->object;
                }

            };
        }
    }
}
