#pragma once
#include "StochastWrapper.h"
#include "../Statistics/CorrelationMatrix.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class CorrelationMatrixWrapper
			{
			private:
				CorrelationMatrix* m_correlation_matrix;
				System::Collections::Generic::List<StochastWrapper^> stochasts = gcnew System::Collections::Generic::List<StochastWrapper^>();

			public:
				CorrelationMatrixWrapper() { m_correlation_matrix = new CorrelationMatrix(); }
				~CorrelationMatrixWrapper() { this->!CorrelationMatrixWrapper(); }
				!CorrelationMatrixWrapper() { delete m_correlation_matrix; }

				void Initialize(const int size) { m_correlation_matrix->init(size); }
				void Initialize(System::Collections::Generic::List<StochastWrapper^>^ stochasts)
				{
					this->stochasts.Clear();

					for (int i = 0; i < stochasts->Count; i++)
					{
						this->stochasts.Add(stochasts[i]);
					}

					m_correlation_matrix->init(stochasts->Count);
				}

				void SetCorrelation(const int i, const int j, const double value) { m_correlation_matrix->SetCorrelation(i, j, value); }

				void SetCorrelation(StochastWrapper^ stochast1, StochastWrapper^ stochast2, const double value)
				{
					int index1 = stochasts.IndexOf(stochast1);
					int index2 = stochasts.IndexOf(stochast2);

					m_correlation_matrix->SetCorrelation(index1, index2, value);
				}

				CorrelationMatrix* GetCorrelationMatrix()
				{
					return m_correlation_matrix;
				}
			};
		}
	}
}


