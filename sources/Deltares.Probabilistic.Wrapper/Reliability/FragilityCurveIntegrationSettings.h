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
#include "DesignPointMethodSettings.h"
#include "../../Deltares.Probabilistic/Reliability/FragilityCurveIntegrationSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class FragilityCurveIntegrationSettings
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Reliability::FragilityCurveIntegrationSettings>* shared = new Utils::Wrappers::SharedPointerProvider(new Reliability::FragilityCurveIntegrationSettings());

            public:
                FragilityCurveIntegrationSettings() {}
                ~FragilityCurveIntegrationSettings() { this->!FragilityCurveIntegrationSettings(); }
                !FragilityCurveIntegrationSettings() { delete shared; }

                property double StepSize
                {
                    double get() { return shared->object->StepSize; }
                    void set(double value) { shared->object->StepSize = value; }
                }

                property Wrappers::DesignPointMethod DesignPointMethod
                {
                    Wrappers::DesignPointMethod get()
                    {
                        switch (shared->object->designPointMethod)
                        {
                        case Reliability::DesignPointMethod::NearestToMean: return Wrappers::DesignPointMethod::NearestToMean;
                        case Reliability::DesignPointMethod::CenterOfGravity: return Wrappers::DesignPointMethod::CentreOfGravity;
                        case Reliability::DesignPointMethod::CenterOfAngles: return Wrappers::DesignPointMethod::CentreOfAngles;
                        default: throw gcnew System::NotSupportedException("Design point method");
                        }
                    }
                    void set(Wrappers::DesignPointMethod value)
                    {
                        switch (value)
                        {
                        case Wrappers::DesignPointMethod::NearestToMean: shared->object->designPointMethod = Reliability::DesignPointMethod::NearestToMean; break;
                        case Wrappers::DesignPointMethod::CentreOfGravity:shared->object->designPointMethod = Reliability::DesignPointMethod::CenterOfGravity; break;
                        case Wrappers::DesignPointMethod::CentreOfAngles: shared->object->designPointMethod = Reliability::DesignPointMethod::CenterOfAngles; break;
                        default: throw gcnew System::NotSupportedException("Design point method");
                        }
                    }
                }

                bool IsValid()
                {
                    return shared->object->isValid();
                }

                std::shared_ptr<Reliability::FragilityCurveIntegrationSettings> GetSettings()
                {
                    return shared->object;
                }
            };
        }
    }
}


