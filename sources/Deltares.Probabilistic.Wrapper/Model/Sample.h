#pragma once
#include "../Utils/NativeSupport.h"
#include "../../Deltares.Probabilistic/Model/Sample.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			public ref class SampleX
			{
			public:
				SampleX(array<double>^ x, array<double>^ xInput, array<double>^ xFull)
				{
					this->X = x;
					this->XInput = xInput;
					this->XFull = xFull;
				}

				array<double>^ X;
				array<double>^ XInput;
				array<double>^ XFull;
			};


			/// <summary>
			/// Indicates the meaning of <see cref="Sample.Values"/> in a <see cref="Sample"/>
			/// </summary>
			public enum class SpaceType { U, X };

			/// <summary>
			/// Indicates whether a <see cref="Sample"/> should be logged
			/// </summary>
			public enum class LogSampleType { Never, OnlyModel, Always };

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

				std::shared_ptr<Models::Sample> GetSample()
				{
					return shared->object;
				}

				// TODO: next methods and properties should be removed after after c++ conversion

				Sample(SpaceType spaceType, int count) : Sample(spaceType, -1, count)
				{
				}

				Sample(SpaceType spaceType, int iteration, int count) : Sample(spaceType, iteration, gcnew array<double>(count))
				{
				}

				Sample(SpaceType spaceType, array<double>^ values) : Sample(spaceType, -1, values)
				{
				}

				Sample(SpaceType spaceType, int iteration, array<double>^ values) : Sample(values)
				{
					this->SpaceType = spaceType;
					this->Iteration = iteration;
				}

				Sample(SpaceType spaceType, int iteration, array<double>^ values, LogSampleType logSampleType) : Sample(values)
				{
					this->SpaceType = spaceType;
					this->Iteration = iteration;
					this->LogSampleType = logSampleType;
				}

				/// <summary>
				/// The space type in which the <see cref="Values"/> are defined
				/// </summary>
				SpaceType SpaceType = SpaceType::U;

				LogSampleType LogSampleType = LogSampleType::Always;

				double Factor = 0;

				SampleX^ X = nullptr;

				bool IsNew()
				{
					return this->X == nullptr;
				}

				Sample^ GetSampleAtBeta(double beta)
				{
					UpdateValues();

					array<double>^ values = NativeSupport::toManaged(shared->object->getSampleAtBeta(beta)->Values);

					return gcnew Sample(this->SpaceType, this->Iteration, values);
				}

				Sample^ GetNormalizedSample()
				{
					UpdateValues();

					array<double>^ values = NativeSupport::toManaged(shared->object->getNormalizedSample()->Values);

					return gcnew Sample(this->SpaceType, this->Iteration, values);
				}

				Sample^ GetMultipliedSample(double factor)
				{
					UpdateValues();

					array<double>^ values = NativeSupport::toManaged(shared->object->getMultipliedSample(factor)->Values);

					return gcnew Sample(this->SpaceType, this->Iteration, values);
				}

				Sample^ Clone()
				{
					UpdateValues();

					array<double>^ values = NativeSupport::toManaged(shared->object->clone()->Values);

					return gcnew Sample(this->SpaceType, this->Iteration, values);
				}

				/// <summary>
				/// Tells whether this sample should be saved
				/// </summary>
				/// <param name="reused"></param>
				/// <returns></returns>
				bool ShouldSave(bool reused)
				{
					return this->LogSampleType == LogSampleType::Always || (this->LogSampleType == LogSampleType::OnlyModel && !reused);
				}

				int ScenarioIndex = -1;

				void UpdateValues()
				{
					if (values != nullptr)
					{
						// synchronize values
						for (int i = 0; i < values->Length; i++)
						{
							shared->object->Values[i] = values[i];
						}
					}
				}

				// TODO: remove after c++ conversion
				property double Beta
				{
					double get()
					{
						UpdateValues();
						return shared->object->getBeta();
					}
				}

				/// <summary>
/// Gets the squared distance to another sample
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
				double GetDistance2(Sample^ other, bool hasWeighting)
				{
					double sum = 0;
					for (int j = 0; j < other->Values->Length; ++j)
					{
						double weight = 1;
						if (hasWeighting) weight = other->Weight;
						double diff = (this->Values[j] - other->Values[j]) * weight;
						sum += diff * diff;
					}

					return sum;
				}

				bool AreValuesEqual(Sample^ other)
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
			};
		}
	}
}


