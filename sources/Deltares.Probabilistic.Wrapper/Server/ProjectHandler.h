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
#pragma once
#include "../../Deltares.Probabilistic/Server/ProjectHandler.h"
#include "../Statistics/Stochast.h"
#include "../Reliability/Settings.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Utils/NativeSupport.h"

namespace Deltares
{
    namespace Server
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public ref class ProjectHandler
            {
            private:
                SharedPointerProvider<Server::ProjectHandler>* shared = new SharedPointerProvider(new Server::ProjectHandler());
            public:
                ProjectHandler() {}
                ~ProjectHandler() { this->!ProjectHandler(); }
                !ProjectHandler() { delete shared; }

                virtual bool CanHandle(System::String^ property)
                {
                    return shared->object->CanHandle(NativeSupport::toNative(property));
                }

                virtual void Create(System::String^ type, int id)
                {
                    shared->object->Create(NativeSupport::toNative(type), id);
                }

                virtual void Destroy(int id)
                {
                    shared->object->Destroy(id);
                }

                virtual double GetValue(int id, System::String^ property)
                {
                    return shared->object->GetValue(id, NativeSupport::toNative(property));
                }

                virtual void SetValue(int id, System::String^ property, double value)
                {
                    shared->object->SetValue(id, NativeSupport::toNative(property), value);
                }

                virtual double GetArgValue(int id, System::String^ property, double argument)
                {
                    return shared->object->GetArgValue(id, NativeSupport::toNative(property), argument);
                }

                virtual int GetIntValue(int id, System::String^ property)
                {
                    return shared->object->GetIntValue(id, NativeSupport::toNative(property));
                }

                virtual void SetIntValue(int id, System::String^ property, int value)
                {
                    shared->object->SetIntValue(id, NativeSupport::toNative(property), value);
                }

                virtual int GetIdValue(int id, System::String^ property, int newId)
                {
                    return shared->object->GetIdValue(id, NativeSupport::toNative(property), newId);
                }

                virtual bool GetBoolValue(int id, System::String^ property)
                {
                    return shared->object->GetBoolValue(id, NativeSupport::toNative(property));
                }

                virtual void SetBoolValue(int id, System::String^ property, bool value)
                {
                    shared->object->SetBoolValue(id, NativeSupport::toNative(property), value);
                }

                virtual System::String^ GetStringValue(int id, System::String^ property)
                {
                    return NativeSupport::toManaged(shared->object->GetStringValue(id, NativeSupport::toNative(property)));
                }

                virtual void SetStringValue(int id, System::String^ property, System::String^ value)
                {
                    shared->object->SetStringValue(id, NativeSupport::toNative(property), NativeSupport::toNative(value));
                }

                virtual double GetIndexedValue(int id, System::String^ property, int index)
                {
                    return shared->object->GetIndexedValue(id, NativeSupport::toNative(property), index);
                }

                virtual int GetIndexedIntValue(int id, System::String^ property, int index)
                {
                    return shared->object->GetIndexedIntValue(id, NativeSupport::toNative(property), index);
                }

                virtual int GetIndexedIdValue(int id, System::String^ property, int index, int newId)
                {
                    return shared->object->GetIndexedIdValue(id, NativeSupport::toNative(property), index, newId);
                }

                virtual System::String^ GetIndexedStringValue(int id, System::String^ property, int index)
                {
                    return NativeSupport::toManaged(shared->object->GetIndexedStringValue(id, NativeSupport::toNative(property), index));
                }

                virtual void SetArrayIntValue(int id, System::String^ property, array<int>^ values)
                {
                    shared->object->SetArrayIntValue(id, NativeSupport::toNative(property), NativeSupport::toNative(values).data(), values->Length);
                }

                virtual void GetArgValues(int id, System::String^ property, array<double>^ values, array<double>^ outputValues)
                {
                    shared->object->GetArgValues(id, NativeSupport::toNative(property), NativeSupport::toNative(values).data(), values->Length,NativeSupport::toNative(outputValues).data());
                }

                virtual void Execute(int id, System::String^ method)
                {
                    return shared->object->Execute(id, NativeSupport::toNative(method));
                }

            protected:
                int GetStochastId(Statistics::Wrappers::Stochast^ stochast, int newId)
                {
                    return shared->object->GetStochastId(stochast->GetStochast(), newId);
                }

                int GetSettingsId(Reliability::Wrappers::Settings^ settings)
                {
                    return shared->object->GetSettingsId(settings->GetSettings());
                }
            };
        }
    }
}

