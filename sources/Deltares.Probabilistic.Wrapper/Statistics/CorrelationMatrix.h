// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
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
                SharedPointerProvider<Statistics::CorrelationMatrix>* shared = nullptr;
                System::Collections::Generic::List<Stochast^> stochasts = gcnew System::Collections::Generic::List<Stochast^>();

            public:
                CorrelationMatrix()
                {
                    shared = new SharedPointerProvider(new Statistics::CorrelationMatrix());
                }

                CorrelationMatrix(std::shared_ptr<Statistics::CorrelationMatrix> nativeCorrelationMatrix)
                {
                    shared = new SharedPointerProvider(nativeCorrelationMatrix);
                }

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


