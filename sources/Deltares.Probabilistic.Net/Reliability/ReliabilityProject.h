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

#include "ReliabilityMethod.h"
#include "ReliabilitySettings.h"
#include "DesignPoint.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Statistics/Stochast.h"
#include "../../Deltares.Probabilistic/Reliability/ReliabilityProject.h"
#include "../Model/ModelProject.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class ReliabilityProject : Models::Wrappers::ModelProject
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Reliability::ReliabilityProject>* shared = new Utils::Wrappers::SharedPointerProvider(new Reliability::ReliabilityProject());

            public:
                ReliabilityProject()
                {
                    setNativeObject(shared->object);
                }

                ~ReliabilityProject() { this->!ReliabilityProject(); }
                !ReliabilityProject() { delete shared; }

                Reliability::Wrappers::ReliabilityMethod^ ReliabilityMethod = nullptr;

                Reliability::Wrappers::ReliabilitySettings^ ReliabilitySettings = gcnew Reliability::Wrappers::ReliabilitySettings();
                Models::Wrappers::RunSettings^ Settings = gcnew Models::Wrappers::RunSettings();
                Models::Wrappers::ProgressIndicator^ ProgressIndicator = nullptr;

                bool IsValid() override;
                Reliability::Wrappers::DesignPoint^ GetDesignPoint();

                Reliability::Wrappers::DesignPoint^ DesignPoint = nullptr;
            };
        }
    }
}

