#pragma once
#include "../Utils/NativeSupport.h"
#include "../../Deltares.Probabilistic.Kernels/Model/Sample.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Models;
			using namespace Deltares::Utils::Wrappers;

			public ref class SampleWrapper
			{
			private:
				Models::Sample* sample;
				array<double>^ values = nullptr;
				System::Object^ tag = nullptr;

			public:
				SampleWrapper(std::shared_ptr<Models::Sample> sample)
				{
					this->sample = sample.get();
				}

				property array<double>^ Values
				{
					array<double>^ get()
					{
						if (values == nullptr)
						{
							values = NativeSupport::toManaged(sample->Values);
						}

						return values;
					}
				}

				property double Weight
				{
					double get() { return sample->Weight; }
				}

				property int Iteration
				{
					int get() { return sample->IterationIndex; }
				}

				property double Z
				{
					double get() { return sample->Z; }
					void set(double value) { sample->Z = value; }
				}

				property System::Object^ Tag
				{
					System::Object^ get()
					{
						if (tag == nullptr && sample->Tag != 0)
						{
							tag = NativeSupport::toManagedObject(sample->Tag);
						}

						return tag;
					}
					void set(System::Object^ value)
					{
						tag = value;
						sample->Tag = NativeSupport::toNativeObject(value);
					}
				}
			};
		}
	}
}


