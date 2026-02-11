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
#include "../../Deltares.Probabilistic/Model/RunSettings.h"
#include "../../Deltares.Probabilistic/Logging/Message.h"
#include "../Utils/SharedPointerProvider.h"
#include "Message.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            public ref class RunSettings
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Models::RunSettings>* shared = new Utils::Wrappers::SharedPointerProvider(new Models::RunSettings());

            public:
                RunSettings() { }
                ~RunSettings() { this->!RunSettings(); }
                !RunSettings() { delete shared; }

                property int MaxChunkSize
                {
                    int get() { return shared->object->MaxChunkSize; }
                    void set(int value) { shared->object->MaxChunkSize = value; }
                }

                property int MaxParallelProcesses
                {
                    int get() { return shared->object->MaxParallelProcesses; }
                    void set(int value) { shared->object->MaxParallelProcesses = value; }
                }

                property bool SaveConvergence
                {
                    bool get() { return shared->object->SaveConvergence; }
                    void set(bool value) { shared->object->SaveConvergence = value; }
                }

                property bool SaveEvaluations
                {
                    bool get() { return shared->object->SaveEvaluations; }
                    void set(bool value) { shared->object->SaveEvaluations = value; }
                }

                property bool SaveMessages
                {
                    bool get() { return shared->object->SaveMessages; }
                    void set(bool value) { shared->object->SaveMessages = value; }
                }

                property bool ReuseCalculations
                {
                    bool get() { return shared->object->ReuseCalculations; }
                    void set(bool value) { shared->object->ReuseCalculations = value; }
                }

                property int MaxMessages
                {
                    int get() { return shared->object->MaxMessages; }
                    void set(int value) { shared->object->MaxMessages = value; }
                }

                property Wrappers::MessageType LowestMessageType
                {
                    Wrappers::MessageType get()
                    {
                        switch (shared->object->LowestMessageType)
                        {
                        case Logging::MessageType::Debug: return Wrappers::MessageType::Debug;
                        case Logging::MessageType::Info: return Wrappers::MessageType::Info;
                        case Logging::MessageType::Warning: return Wrappers::MessageType::Warning;
                        case Logging::MessageType::Error: return Wrappers::MessageType::Error;
                        default: throw gcnew System::NotSupportedException("Message type");
                        }
                    }
                    void set(Wrappers::MessageType value)
                    {
                        switch (value)
                        {
                        case Wrappers::MessageType::Debug: shared->object->LowestMessageType = Logging::MessageType::Debug; break;
                        case Wrappers::MessageType::Info: shared->object->LowestMessageType = Logging::MessageType::Info; break;
                        case Wrappers::MessageType::Warning: shared->object->LowestMessageType = Logging::MessageType::Warning; break;
                        case Wrappers::MessageType::Error: shared->object->LowestMessageType = Logging::MessageType::Error; break;
                        default: throw gcnew System::NotSupportedException("Message type");
                        }
                    }
                }

                std::shared_ptr<Models::RunSettings> GetSettings()
                {
                    return shared->object;
                }
            };

            public interface class IHasRunSettings
            {
                property Deltares::Models::Wrappers::RunSettings^ RunSettings;
            };
        }
    }
}


