#pragma once
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

			public:
				CorrelationMatrixWrapper() { m_correlation_matrix = new CorrelationMatrix(); }
				~CorrelationMatrixWrapper() { this->!CorrelationMatrixWrapper(); }
				!CorrelationMatrixWrapper() { delete m_correlation_matrix; }

				void init(const int size) { m_correlation_matrix->init(size); }
				void SetCorrelation(const int i, const int j, const double value) { m_correlation_matrix->SetCorrelation(i, j, value); }

				CorrelationMatrix* GetCorrelationMatrix()
				{
					return m_correlation_matrix;
				}

			};
		}
	}
}


