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

#include "../../Deltares.Probabilistic/Model/ProgressIndicator.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Utils/NativeSupport.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            public enum class ProgressType {Global, Detailed};

            public delegate void ProgressDelegate(double);
            public delegate void DetailedProgressDelegate(int, int, double, double);
            public delegate void TextualProgressDelegate(Wrappers::ProgressType, System::String^ text);

            public ref class ProgressIndicator
            {

            public:
                ProgressIndicator(ProgressDelegate^ progressDelegate, DetailedProgressDelegate^ detailedProgressDelegate, TextualProgressDelegate^ textualProgressDelegate);
                ~ProgressIndicator() { this->!ProgressIndicator(); }
                !ProgressIndicator()
                {
                    for (int i = 0; i < handles->Count; i++)
                    {
                        handles[i].Free();
                    }
                    delete shared;
                }

                void DoProgress(double progress);
                void DoDetailedProgress(int step, int loop, double reliability, double convergence);
                void DoTextualProgress(Wrappers::ProgressType progressType, System::String^ text);

                void ReleaseHandles()
                {
                    for (int i = 0; i < handles->Count; i++)
                    {
                        handles[i].Free();
                    }

                    handles->Clear();
                }

                void Reset()
                {
                    shared->object->reset();
                }

                void Complete()
                {
                    shared->object->complete();
                }

                void Initialize(double factor, double offset)
                {
                    shared->object->initialize(factor, offset);
                }

                void SetTask(System::String^ task)
                {
                    shared->object->setTask(Utils::Wrappers::NativeSupport::toNative(task));
                }

                void IncreaseOffset()
                {
                    shared->object->increaseOffset();
                }

                std::shared_ptr<Models::ProgressIndicator> GetProgressIndicator()
                {
                    return shared->object;
                }

            private:
                ProgressDelegate^ progressDelegate;
                DetailedProgressDelegate^ detailedProgressDelegate;
                TextualProgressDelegate^ textualProgressDelegate;

                Utils::Wrappers::SharedPointerProvider<Deltares::Models::ProgressIndicator>* shared = nullptr;

                Models::ProgressLambda getProgressLambda();
                Models::DetailedProgressLambda getDetailedProgressLambda();
                Models::TextualProgressLambda getTextualProgressLambda();

                void DoTextualProgress(Models::ProgressType progressType, std::string text);

                ProgressType GetProgressType(Models::ProgressType progressType);

                System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();
            };
        }
    }
}



