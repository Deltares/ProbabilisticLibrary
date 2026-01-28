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

#include <memory>

#include "../Utils/NativeSupport.h"
#include "../../Deltares.Probabilistic/Model/Evaluation.h"
#include "../Utils/TagRepository.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public ref class Evaluation
            {
            public:
                Evaluation() {}
                Evaluation(std::shared_ptr<Models::Evaluation> evaluation)
                {
                    this->Z = evaluation->Z;
                    this->Iteration = evaluation->Iteration;
                    this->Weight = evaluation->Weight;
                    this->tagIndex = evaluation->Tag;
                }

                property int Iteration;
                property double Z;
                property double Weight;

                System::Object^ Tag = nullptr;

                void AssignTag(TagRepository^ tagRepository)
                {
                    if (tagRepository != nullptr)
                    {
                        this->Tag = tagRepository->RetrieveTag(this->tagIndex);
                    }
                }

            private:
                int tagIndex = 0;
            };
        }
    }
}



