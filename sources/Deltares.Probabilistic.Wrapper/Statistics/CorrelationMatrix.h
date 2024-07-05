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

                    std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> nativeStochasts;

                    for (int i = 0; i < stochasts->Count; i++)
                    {
                        this->stochasts.Add(stochasts[i]);
                        nativeStochasts.push_back(stochasts[i]->GetStochast());
                    }

                    shared->object->init(nativeStochasts);
                }

                void SetCorrelation(const int i, const int j, const double value)
                {
                    shared->object->SetCorrelation(i, j, value);
                }
                
                void SetCorrelation(Stochast^ stochast1, Stochast^ stochast2, const double value)
                {
                    shared->object->SetCorrelation(stochast1->GetStochast(), stochast2->GetStochast(), value);
                }

                double GetCorrelation(const int i, const int j)
                {
                    return shared->object->GetCorrelation(i, j);
                }

                double GetCorrelation(Stochast^ stochast1, Stochast^ stochast2)
                {
                    return shared->object->GetCorrelation(stochast1->GetStochast(), stochast2->GetStochast());
                }

                virtual bool IsIdentity()
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

                virtual void UpdateFullMatrix() {}

                std::shared_ptr<Statistics::CorrelationMatrix> GetCorrelationMatrix()
                {
                    UpdateFullMatrix();

                    return shared->object;
                }
            };
        }
    }
}


