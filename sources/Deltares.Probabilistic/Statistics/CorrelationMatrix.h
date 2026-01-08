// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once
#include <map>
#include <memory>

#include "../Math/matrix.h"
#include <vector>

#include "Stochast.h"
#include "BaseCorrelation.h"

namespace Deltares
{
    namespace Statistics
    {
        class correlationPair : public BaseCorrelationPair
        {
        public:
            double correlation;
            bool isFullyCorrelated;
        };

        class CorrelationMatrix : public BaseCorrelation
        {
        public:
            void Init(const int maxStochasts) override;
            void Init(const std::vector<std::shared_ptr<Stochast>>& stochastList) override;

            bool IsValid() const override;
            void Validate(Logging::ValidationReport& report) const override;

            std::vector<double> ApplyCorrelation(const std::vector<double>& uValues) override;
            std::vector<double> InverseCholesky(const std::vector<double>& uValues);

            void SetCorrelation(const int i, const int j, double value, CorrelationType type) override;
            void SetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2,
                double value, CorrelationType type) override;

            CorrelationValueAndType GetCorrelation(const int i, const int j) const override;
            CorrelationValueAndType GetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2) override;

            bool IsIdentity() const override;
            int CountCorrelations() const override;
            int GetDimension() override { return static_cast<int>(dim); }
            std::shared_ptr<Statistics::Stochast> GetStochast(int index) override;
            bool HasConflictingCorrelations() const;
            void resolveConflictingCorrelations();
            void InitializeForRun() override;
            bool IsFullyCorrelated(const int i, const std::vector<int>& varyingIndices) const override;
            void Filter(const std::shared_ptr<BaseCorrelation> source, const std::vector<int>& index) override;
            IndexWithCorrelation FindDependent(const int i) const override;
        private:
            Numeric::Matrix matrix = Numeric::Matrix(0, 0);
            Numeric::Matrix choleskyMatrix = Numeric::Matrix(0, 0);
            Numeric::Matrix inverseCholeskyMatrix = Numeric::Matrix(0, 0);
            void CholeskyDecomposition();
            void InverseCholeskyDecomposition();
            std::map<std::shared_ptr<Stochast>, int> stochastIndex;
            std::vector<std::shared_ptr<Stochast>> stochasts;
            std::vector<IndexWithCorrelation> indexer;
            std::vector<correlationPair> inputCorrelations;
            std::vector<int> GetLinkingCorrelationStochasts(correlationPair correlation, correlationPair otherCorrelation) const;
            size_t dim = 0;
        };
    }
}

