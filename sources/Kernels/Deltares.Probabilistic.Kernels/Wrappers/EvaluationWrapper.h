#pragma once

#include "NativeSupport.h"
#include "../Model/Evaluation.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class EvaluationWrapper
			{
			private:
				array<double>^ input = nullptr;
				array<double>^ result = nullptr;

				Evaluation* evaluation;
			public:
				EvaluationWrapper()
				{
					evaluation = new Evaluation();
				}
				EvaluationWrapper(Evaluation* evaluation)
				{
					this->evaluation = evaluation;
				}
				~EvaluationWrapper() { this->!EvaluationWrapper(); }
				!EvaluationWrapper() { delete evaluation; }

				property int Iteration
				{
					int get() { return evaluation->Iteration; }
					void set(int value) { evaluation->Iteration = value; }
				}

				property double Z
				{
					double get() { return evaluation->Z; }
					void set(double value) { evaluation->Z = value; }
				}

				property double Weight
				{
					double get() { return evaluation->Weight; }
					void set(double value) { evaluation->Weight = value; }
				}

				property array<double>^ Input
				{
					array<double>^ get()
					{
						if (input == nullptr)
						{
							input = NativeSupport::toManaged(evaluation->X, evaluation->SizeX);
						}

						return input;
					}
				}

				property array<double>^ Result
				{
					array<double>^ get()
					{
						if (result == nullptr)
						{
							input = NativeSupport::toManaged(evaluation->X, evaluation->SizeX);
						}

						return result;
					}
				}

				Evaluation* GetEvaluation()
				{
					return evaluation;
				}
			};
		}
	}
}



