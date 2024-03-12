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

			/**
			 * \brief Sample with values defined in x-space
			 */
			public ref class ModelSample
			{
			private:
				array<double>^ values = nullptr;
				System::Object^ tag = nullptr;
				SharedPointerProvider<Models::ModelSample>* shared = nullptr;

			public:
				ModelSample(std::shared_ptr<Models::ModelSample> sample)
				{
					shared = new SharedPointerProvider(sample);

					this->values = NativeSupport::toManaged(sample->Values);
				}

				ModelSample(array<double>^ values)
				{
					std::vector<double> nativeValues = NativeSupport::toNative(values);

					shared = new SharedPointerProvider(new Models::ModelSample(nativeValues));

					this->values = NativeSupport::toManaged(shared->object->Values);
				}

				property array<double>^ Values
				{
					array<double>^ get() { return values; }
				}

				property double Beta
				{
					double get() { return shared->object->Beta; }
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
					void set(bool value) { shared->object->AllowProxy = value; }
				}

				property bool IsRestartRequired
				{
					bool get() { return shared->object->IsRestartRequired; }
					void set(bool value) { shared->object->IsRestartRequired = value; }
				}

				property double Z
				{
					double get() { return shared->object->Z; }
					void set(double value) { shared->object->Z = value; }
				}

				bool AreValuesEqual(ModelSample^ other)
				{
					if (this == other)
					{
						return true;
					}

					if (this->Values->Length != this->Values->Length)
					{
						return false;
					}

					for (int i = 0; i < this->Values->Length; i++)
					{
						if (!isnan(this->Values[i]) || !isnan(other->Values[i]))
						{
							if (this->Values[i] != other->Values[i])
							{
								return false;
							}
						}
					}

					return true;
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

				std::shared_ptr<Models::ModelSample> GetModelSample()
				{
					return shared->object;
				}
			};
		}
	}
}


