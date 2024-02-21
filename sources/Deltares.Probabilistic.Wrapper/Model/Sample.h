#pragma once
#include "../Utils/NativeSupport.h"
#include "../../Deltares.Probabilistic/Model/Sample.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Models;
			using namespace Deltares::Utils::Wrappers;

			public ref class Sample
			{
			private:
				array<double>^ values = nullptr;
				System::Object^ tag = nullptr;
				SharedPointerProvider<Models::Sample>* shared = nullptr;

			public:
				Sample(std::shared_ptr<Models::Sample> sample)
				{
					shared = new SharedPointerProvider(sample);
				}

				Sample(array<double>^ values)
				{
					std::vector<double> nativeValues = NativeSupport::toNative(values);

					Models::Sample* sample = new Models::Sample(nativeValues);

					shared = new SharedPointerProvider(sample);
					this->values = values;
				}

				property array<double>^ Values
				{
					array<double>^ get()
					{
						if (values == nullptr)
						{
							values = NativeSupport::toManaged(shared->object->Values);
						}

						return values;
					}
				}

				property double Weight
				{
					double get() { return shared->object->Weight; }
					void set(double value) { shared->object->Weight = value; }
				}

				property int Iteration
				{
					int get() { return shared->object->IterationIndex; }
					void set(int value) { shared->object->IterationIndex = value; }
				}

				property bool AllowProxy
				{
					bool get() { return shared->object->AllowProxy; }
					void set(bool value) { shared->object->AllowProxy = value; }
				}

				property double Z
				{
					double get() { return shared->object->Z; }
					void set(double value) { shared->object->Z = value; }
				}

				property double Beta
				{
					double get()
					{
						if (values != nullptr)
						{
							// synchronize values
							for (int i = 0; i < values->Length; i++)
							{
								shared->object->Values[i] = values[i];
							}
						}

						return shared->object->getBeta();
					}
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


