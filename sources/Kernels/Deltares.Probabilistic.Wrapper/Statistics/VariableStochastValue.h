#pragma once

#include "../../Deltares.Probabilistic/Statistics/VariableStochastValue.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public ref class VariableStochastValue : System::IComparable<VariableStochastValue^>
			{
			private:
				SharedPointerProvider<Statistics::VariableStochastValue>* shared = new SharedPointerProvider(new Statistics::VariableStochastValue());
			public:
				VariableStochastValue()	{}
				~VariableStochastValue() { this->!VariableStochastValue(); }
				!VariableStochastValue() { delete shared; }

				property double X
				{
					double get() { return shared->object->X; }
					void set(double value) { shared->object->X = value; }
				}

				// TODO: Implement mean
				property double Mean
				{
					double get() { return shared->object->Stochast->Location; }
					void set(double value) { shared->object->Stochast->Location = value; }
				}

				// TODO: Implement deviation
				property double Deviation
				{
					double get() { return shared->object->Stochast->Scale; }
					void set(double value) { shared->object->Stochast->Scale = value; }
				}

				property double Location
				{
					double get() { return shared->object->Stochast->Location; }
					void set(double value)
					{
						// TODO: implement properly
						if (shared->object->Stochast->Scale == 0)
						{
							shared->object->Stochast->Location = value;
						}
					}
				}

				property double Scale
				{
					double get() { return shared->object->Stochast->Scale; }
					void set(double value)
					{
						// TODO: implement properly
						if (shared->object->Stochast->Scale == 0)
						{
							shared->object->Stochast->Scale = value;
						}
					}
				}

				property double Shift
				{
					double get() { return shared->object->Stochast->Shift; }
					void set(double value) { shared->object->Stochast->Shift = value; }
				}

				property double ShiftB
				{
					double get() { return shared->object->Stochast->ShiftB; }
					void set(double value) { shared->object->Stochast->ShiftB = value; }
				}

				property double Shape
				{
					double get() { return shared->object->Stochast->Shape; }
					void set(double value) { shared->object->Stochast->Shape = value; }
				}

				property double ShapeB
				{
					double get() { return shared->object->Stochast->ShapeB; }
					void set(double value) { shared->object->Stochast->ShapeB = value; }
				}

				property double Minimum
				{
					double get() { return shared->object->Stochast->Minimum; }
					void set(double value) { shared->object->Stochast->Minimum = value; }
				}

				property double Maximum
				{
					double get() { return shared->object->Stochast->Maximum; }
					void set(double value) { shared->object->Stochast->Maximum = value; }
				}

				property int Observations
				{
					int get() { return shared->object->Stochast->Observations; }
					void set(int value) { shared->object->Stochast->Observations = value; }
				}

				std::shared_ptr<Statistics::VariableStochastValue> GetValue()
				{
					return shared->object;
				}

				virtual int CompareTo(VariableStochastValue^ other)
				{
					return this->X.CompareTo(other->X);
				}
			};
		}
	}
}








