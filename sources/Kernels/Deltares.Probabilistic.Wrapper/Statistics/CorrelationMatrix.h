#pragma once
#include "Stochast.h"
#include "../../Deltares.Probabilistic/Statistics/CorrelationMatrix.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public ref class CorrelationMatrix
			{
			private:
				Statistics::CorrelationMatrix* m_correlation_matrix;
				SharedPointerProvider<Statistics::CorrelationMatrix>* sharedPointer = new SharedPointerProvider<Statistics::CorrelationMatrix>();
				System::Collections::Generic::List<Stochast^> stochasts = gcnew System::Collections::Generic::List<Stochast^>();

			public:
				CorrelationMatrix() { m_correlation_matrix = new Statistics::CorrelationMatrix(); }
				~CorrelationMatrix() { this->!CorrelationMatrix(); }
				!CorrelationMatrix() { delete sharedPointer; }

				void Initialize(const int size) { m_correlation_matrix->init(size); }
				void Initialize(System::Collections::Generic::List<Stochast^>^ stochasts)
				{
					this->stochasts.Clear();

					for (int i = 0; i < stochasts->Count; i++)
					{
						this->stochasts.Add(stochasts[i]);
					}

					m_correlation_matrix->init(stochasts->Count);
				}

				void SetCorrelation(const int i, const int j, const double value) { m_correlation_matrix->SetCorrelation(i, j, value); }

				void SetCorrelation(Stochast^ stochast1, Stochast^ stochast2, const double value)
				{
					int index1 = stochasts.IndexOf(stochast1);
					int index2 = stochasts.IndexOf(stochast2);

					m_correlation_matrix->SetCorrelation(index1, index2, value);
				}

				void ResolveConflictingCorrelations()
				{
					m_correlation_matrix->resolveConflictingCorrelations();
				}

				std::shared_ptr<Statistics::CorrelationMatrix> GetCorrelationMatrix()
				{
					return sharedPointer->getSharedPointer(m_correlation_matrix);
				}
			};
		}
	}
}


