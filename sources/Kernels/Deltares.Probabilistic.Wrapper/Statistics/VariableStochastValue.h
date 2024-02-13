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

			public ref class VariableStochastValue
			{
			private:
				Statistics::VariableStochastValue* m_value;
				SharedPointerProvider<Statistics::VariableStochastValue>* sharedPointer = new SharedPointerProvider<Statistics::VariableStochastValue>();
			public:
				VariableStochastValue()
				{
					m_value = new Statistics::VariableStochastValue();
				}
				~VariableStochastValue() { this->!VariableStochastValue(); }
				!VariableStochastValue() { delete sharedPointer; }

				property double X
				{
					double get() { return m_value->X; }
					void set(double value) { m_value->X = value; }
				}

				// TODO: Implement mean
				property double Mean
				{
					double get() { return m_value->Stochast->Location; }
					void set(double value) { m_value->Stochast->Location = value; }
				}

				// TODO: Implement deviation
				property double Deviation
				{
					double get() { return m_value->Stochast->Scale; }
					void set(double value) { m_value->Stochast->Scale = value; }
				}

				// TODO: Implement is varying
				bool IsVarying()
				{
					return true;
				}

				property double Location
				{
					double get() { return m_value->Stochast->Location; }
					void set(double value) { m_value->Stochast->Location = value; }
				}

				property double Scale
				{
					double get() { return m_value->Stochast->Scale; }
					void set(double value) { m_value->Stochast->Scale = value; }
				}

				property double Shift
				{
					double get() { return m_value->Stochast->Shift; }
					void set(double value) { m_value->Stochast->Shift = value; }
				}

				property double ShiftB
				{
					double get() { return m_value->Stochast->ShiftB; }
					void set(double value) { m_value->Stochast->ShiftB = value; }
				}

				property double Shape
				{
					double get() { return m_value->Stochast->Shape; }
					void set(double value) { m_value->Stochast->Shape = value; }
				}

				property double ShapeB
				{
					double get() { return m_value->Stochast->ShapeB; }
					void set(double value) { m_value->Stochast->ShapeB = value; }
				}

				property double Minimum
				{
					double get() { return m_value->Stochast->Minimum; }
					void set(double value) { m_value->Stochast->Minimum = value; }
				}

				property double Maximum
				{
					double get() { return m_value->Stochast->Maximum; }
					void set(double value) { m_value->Stochast->Maximum = value; }
				}

				property int Observations
				{
					int get() { return m_value->Stochast->Observations; }
					void set(int value) { m_value->Stochast->Observations = value; }
				}

				std::shared_ptr<Statistics::VariableStochastValue> GetValue()
				{
					return sharedPointer->getSharedPointer(m_value);
				}
			};
		}
	}
}








