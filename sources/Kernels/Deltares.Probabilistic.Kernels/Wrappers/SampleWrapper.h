#pragma once
#include "NativeSupport.h"
#include "../Model/Sample.h"


namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class SampleWrapper
			{
			private:
				Sample* sample;
				array<double>^ values = nullptr;
				System::Object^ tag = nullptr;

			public:
				SampleWrapper(Sample* sample)
				{
					this->sample = sample;
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


