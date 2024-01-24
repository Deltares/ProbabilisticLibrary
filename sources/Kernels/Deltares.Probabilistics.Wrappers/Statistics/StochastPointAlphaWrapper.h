#pragma once

#include "StochastWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Model/StochastPointAlpha.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class StochastPointAlphaWrapper
			{
			private:
				StochastPointAlpha* m_alpha;
				StochastWrapper^ parameter = gcnew StochastWrapper();

			public:
				StochastPointAlphaWrapper()
				{
					m_alpha = new StochastPointAlpha();
				}
				StochastPointAlphaWrapper(StochastPointAlpha* alpha)
				{
					m_alpha = alpha;
				}
				~StochastPointAlphaWrapper() { this->!StochastPointAlphaWrapper(); }
				!StochastPointAlphaWrapper() { delete m_alpha; }

				property StochastWrapper^ Parameter
				{
					StochastWrapper^ get() { return parameter; }
					void set(StochastWrapper^ value) { parameter = value; }
				}

				property double Alpha
				{
					double get() { return m_alpha->Alpha; }
					void set(double value) { m_alpha->Alpha = value; }
				}

				property double AlphaCorrelated
				{
					double get() { return m_alpha->AlphaCorrelated; }
					void set(double value) { m_alpha->AlphaCorrelated = value; }
				}

				property double U
				{
					double get() { return m_alpha->U; }
					void set(double value) { m_alpha->U = value; }
				}

				property double X
				{
					double get() { return m_alpha->X; }
					void set(double value) { m_alpha->X = value; }
				}

				property double InfluenceFactor
				{
					double get() { return m_alpha->InfluenceFactor; }
					void set(double value) { m_alpha->InfluenceFactor = value; }
				}

				void SetAlpha(StochastPointAlpha* alpha)
				{
					m_alpha = alpha;
				}
			};
		}
	}
}

