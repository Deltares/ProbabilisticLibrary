#pragma once

#include <memory>

#include "../Utils/NativeSupport.h"
#include "../../Deltares.Probabilistic.Kernels/Model/Evaluation.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models;

			public ref class EvaluationWrapper
			{
			public:
				EvaluationWrapper() {}
				EvaluationWrapper(std::shared_ptr<Evaluation> evaluation)
				{
					this->Z = evaluation->Z;
					this->Iteration = evaluation->Iteration;
					this->Weight = evaluation->Weight;
					this->Input = NativeSupport::toManaged(evaluation->X, evaluation->SizeX);
					this->Result = NativeSupport::toManaged(evaluation->X, evaluation->SizeX);
					this->Tag = NativeSupport::toManagedObject(evaluation->Tag);
				}

				property int Iteration;
				property double Z;
				property double Weight;
				property array<double>^ Input;
				property array<double>^ Result;
				property System::Object^ Tag;
			};
		}
	}
}



