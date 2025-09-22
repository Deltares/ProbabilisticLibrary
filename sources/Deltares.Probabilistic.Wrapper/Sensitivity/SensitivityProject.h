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
#include "../Statistics/Stochast.h"
#include "../Sensitivity/SensitivityMethod.h"
#include "../../Deltares.Probabilistic/Sensitivity/SensitivityProject.h"
#include "../Statistics/CorrelationMatrix.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            public ref class SensitivityProject
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Sensitivity::SensitivityProject>* shared = new Utils::Wrappers::SharedPointerProvider(new Sensitivity::SensitivityProject());
                Models::ZLambda getZLambda();
                void invokeSample(std::shared_ptr<Models::ModelSample> sample);
                System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();
            public:
                SensitivityProject() {}
                ~SensitivityProject() { this->!SensitivityProject(); }
                !SensitivityProject()
                {
                    delete shared;
                }

                void ReleaseHandles()
                {
                    for (int i = 0; i < handles->Count; i++)
                    {
                        handles[i].Free();
                    }

                    handles->Clear();
                    shared->object->model->releaseCallBacks();
                }

                System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ Stochasts = gcnew System::Collections::Generic::List<Statistics::Wrappers::Stochast^>();
                Statistics::Wrappers::CorrelationMatrix^ CorrelationMatrix = gcnew Statistics::Wrappers::CorrelationMatrix();

                Models::Wrappers::ZSampleDelegate^ ZFunction = nullptr;

                Utils::Wrappers::TagRepository^ TagRepository = nullptr;

                SensitivityMethod^ SensitivityMethod = nullptr;

                Wrappers::SensitivityResult^ GetResult();
                SensitivityResult^ Result = nullptr;

                Models::Wrappers::RunSettings^ Settings = gcnew Models::Wrappers::RunSettings();
                Models::Wrappers::ProgressIndicator^ ProgressIndicator = nullptr;

                bool IsValid();
            };
        }
    }
}

