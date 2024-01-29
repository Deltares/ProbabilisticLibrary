#pragma once

#include "../../Deltares.Probabilistic.Kernels/Statistics/FragilityValue.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class FragilityValueWrapper
			{
			private:
				Statistics::FragilityValue* m_value;

			public:
				FragilityValueWrapper()
				{
					m_value = new Statistics::FragilityValue();
				}
				~FragilityValueWrapper() { this->!FragilityValueWrapper(); }
				!FragilityValueWrapper() { delete m_value; }

				property double X
				{
					double get() { return m_value->X; }
					void set(double value) { m_value->X = value; }
				}

				property double Reliability
				{
					double get() { return m_value->Reliability; }
					void set(double value) { m_value->Reliability = value; }
				}

				Statistics::FragilityValue* GetValue()
				{
					return m_value;
				}
			};
		}
	}
}