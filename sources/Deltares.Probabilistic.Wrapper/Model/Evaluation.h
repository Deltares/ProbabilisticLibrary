#pragma once

#include <memory>

#include "../Utils/NativeSupport.h"
#include "../../Deltares.Probabilistic/Model/Evaluation.h"

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
                    this->Tag = evaluation->Tag;
                }

                property int Iteration;
                property double Z;
                property double Weight;
                property int Tag;
            };
        }
    }
}



