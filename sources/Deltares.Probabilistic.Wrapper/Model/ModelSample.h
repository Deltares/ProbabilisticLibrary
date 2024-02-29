#pragma once
#include "../Utils/NativeSupport.h"
#include "../Utils/SharedPointerProvider.h"
#include "../../Deltares.Probabilistic/Model/ModelSample.h"


namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Models;
			using namespace Deltares::Utils::Wrappers;

			public ref class ModelSample
			{
			private:
				array<double>^ values = nullptr;
				array<double>^ uValues = nullptr;
				System::Object^ tag = nullptr;
				SharedPointerProvider<Models::ModelSample>* shared = nullptr;

			public:
				ModelSample(std::shared_ptr<Models::ModelSample> sample)
				{
					shared = new SharedPointerProvider(sample);
				}

				ModelSample(array<double>^ values)
				{
					std::vector<double> nativeValues = NativeSupport::toNative(values);

					Models::Sample* sample = new Models::Sample(nativeValues);

					shared = new SharedPointerProvider(sample);

					this->values = NativeSupport::toManaged(shared->object->Values);;
					this->uValues = NativeSupport::toManaged(shared->object->UValues);;
				}

				property array<double>^ Values
				{
					array<double>^ get() { return values; }
				}

				property array<double>^ UValues
				{
					array<double>^ get() { return uValues; }
				}

				property double Weight
				{
					double get() { return shared->object->Weight; }
				}

				property int Iteration
				{
					int get() { return shared->object->IterationIndex; }
				}

				property bool AllowProxy
				{
					bool get() { return shared->object->AllowProxy; }
				}

				property double Z
				{
					double get() { return shared->object->Z; }
					void set(double value) { shared->object->Z = value; }
				}

				property System::Object^ Tag
				{
					System::Object^ get()
					{
						if (tag == nullptr && shared->object->Tag != 0)
						{
							tag = NativeSupport::toManagedObject(shared->object->Tag);
						}

						return tag;
					}
					void set(System::Object^ value)
					{
						tag = value;
						shared->object->Tag = NativeSupport::toNativeObject(value);
					}
				}
			};
		}
	}
}


