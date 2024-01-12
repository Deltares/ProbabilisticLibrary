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
				array<double>^ xValues = nullptr;

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
							values = NativeSupport::toManaged(sample->Values, sample->getSize());
						}

						return values;
					}
				}

				property array<double>^ XValues
				{
					array<double>^ get()
					{
						if (xValues == nullptr)
						{
							xValues = NativeSupport::toManaged(sample->XValues, sample->getSize());
						}

						return xValues;
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
			};
		}
	}
}


