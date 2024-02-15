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
				SharedPointerProvider<Statistics::CorrelationMatrix>* shared = new SharedPointerProvider(new Statistics::CorrelationMatrix());
				System::Collections::Generic::List<Stochast^> stochasts = gcnew System::Collections::Generic::List<Stochast^>();

			public:
				CorrelationMatrix() { }
				~CorrelationMatrix() { this->!CorrelationMatrix(); }
				!CorrelationMatrix() { delete shared; }

				void Initialize(const int size) { shared->object->init(size); }
				void Initialize(System::Collections::Generic::List<Stochast^>^ stochasts)
				{
					this->stochasts.Clear();

					for (int i = 0; i < stochasts->Count; i++)
					{
						this->stochasts.Add(stochasts[i]);
					}

					shared->object->init(stochasts->Count);
				}

				void SetCorrelation(const int i, const int j, const double value) { shared->object->SetCorrelation(i, j, value); }

				void SetCorrelation(Stochast^ stochast1, Stochast^ stochast2, const double value)
				{
					int index1 = stochasts.IndexOf(stochast1);
					int index2 = stochasts.IndexOf(stochast2);

					shared->object->SetCorrelation(index1, index2, value);
				}

				double GetCorrelation(const int i, const int j) { return shared->object->GetCorrelation(i, j); }

				double GetCorrelation(Stochast^ stochast1, Stochast^ stochast2)
				{
					int index1 = stochasts.IndexOf(stochast1);
					int index2 = stochasts.IndexOf(stochast2);

					return shared->object->GetCorrelation(index1, index2);
				}

				bool IsIdentity()
				{
					return shared->object->IsIdentity();
				}

				int CountCorrelations()
				{
					return shared->object->CountCorrelations();
				}

				bool HasConflictingCorrelations()
				{
					return shared->object->HasConflictingCorrelations();
				}

				void ResolveConflictingCorrelations()
				{
					shared->object->resolveConflictingCorrelations();
				}

				std::shared_ptr<Statistics::CorrelationMatrix> GetCorrelationMatrix()
				{
					return shared->object;
				}
			};
		}
	}
}


