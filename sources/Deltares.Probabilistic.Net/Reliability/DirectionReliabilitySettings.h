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
#include "../../Deltares.Probabilistic/Reliability/DirectionReliabilitySettings.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public enum class VaryingType
            {
                Monotone,
                Varying
            };


            public ref class DirectionReliabilitySettings
            {
            private:
                SharedPointerProvider<Reliability::DirectionReliabilitySettings>* shared = new SharedPointerProvider(new Reliability::DirectionReliabilitySettings());
            public:
                DirectionReliabilitySettings() {}
                ~DirectionReliabilitySettings() { this->!DirectionReliabilitySettings(); }
                !DirectionReliabilitySettings() { delete shared; }

                property double Dsdu
                {
                    double get() { return shared->object->Dsdu; }
                    void set(double value) { shared->object->Dsdu = value; }
                }

                property double EpsilonUStepSize
                {
                    double get() { return shared->object->EpsilonUStepSize; }
                    void set(double value) { shared->object->EpsilonUStepSize = value; }
                }

                property double EpsilonZStepSize
                {
                    double get() { return shared->object->EpsilonZStepSize; }
                    void set(double value) { shared->object->EpsilonZStepSize = value; }
                }

                property int MaximumIterations
                {
                    int get() { return shared->object->MaximumIterations; }
                    void set(int value) { shared->object->MaximumIterations = value; }
                }

                property double MaximumLengthU
                {
                    double get() { return shared->object->MaximumLengthU; }
                    void set(double value) { shared->object->MaximumLengthU = value; }
                }

                property Wrappers::VaryingType ModelVaryingType
                {
                    Wrappers::VaryingType get()
                    {
                        switch (shared->object->modelVaryingType)
                        {
                        case Reliability::ModelVaryingType::Monotone: return Wrappers::VaryingType::Monotone;
                        case Reliability::ModelVaryingType::Varying: return Wrappers::VaryingType::Varying;
                        default: throw gcnew System::NotSupportedException("Model varying type");
                        }
                    }
                    void set(Wrappers::VaryingType value)
                    {
                        switch (value)
                        {
                        case Wrappers::VaryingType::Monotone: shared->object->modelVaryingType = Reliability::ModelVaryingType::Monotone; break;
                        case Wrappers::VaryingType::Varying: shared->object->modelVaryingType = Reliability::ModelVaryingType::Varying; break;
                        default: throw gcnew System::NotSupportedException("Model varying type");
                        }
                    }
                }

                void SetStartPoint(DesignPoint^ designPoint)
                {
                    shared->object->StochastSet = std::make_shared<Reliability::StochastSettingsSet>(designPoint->getDesignPoint());
                }

                std::shared_ptr<Reliability::DirectionReliabilitySettings> GetSettings()
                {
                    return shared->object;
                }
            };
        }
    }
}




