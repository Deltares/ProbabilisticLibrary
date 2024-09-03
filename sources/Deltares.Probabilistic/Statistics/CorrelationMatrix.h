// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
#include <map>
#include <memory>

#include "../Math/matrix.h"
#include <vector>

#include "Stochast.h"

namespace Deltares
{
    namespace Statistics
    {
        struct indexWithCorrelation
        {
            int index;
            double correlation;
        };

        class correlationPair
        {
        public:
            int index1;
            int index2;
            double correlation;
            bool isFullyCorrelated;
            bool AreLinked(const correlationPair & other) const
            {
                return index1 == other.index1 || index1 == other.index2 || index2 == other.index1 || index2 == other.index2;
            }
        };

        class CorrelationMatrix
        {
        public:
            void init(const int maxStochasts);
            void init(std::vector<std::shared_ptr<Stochast>> stochasts);

            std::vector<double> Cholesky(const std::vector<double>& uValues);
            std::vector<double> InverseCholesky(const std::vector<double>& uValues);

            void SetCorrelation(const int i, const int j, double value);
            void SetCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2, double value);

            double GetCorrelation(const int i, const int j) const;
            double GetCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2);

            bool IsIdentity() const;
            int CountCorrelations() const;
            bool HasConflictingCorrelations() const;
            void resolveConflictingCorrelations();
            void CholeskyDecomposition();
            void InverseCholeskyDecomposition();
            bool isFullyCorrelated(const int i, std::vector<int> varyingIndices) const;
            void filter(const std::shared_ptr<CorrelationMatrix> m, const std::vector<int>& index);
            indexWithCorrelation findDependent(const int i) const;
        private:
            Deltares::Numeric::Matrix matrix = Deltares::Numeric::Matrix(0, 0);
            Deltares::Numeric::Matrix choleskyMatrix = Deltares::Numeric::Matrix(0, 0);
            Deltares::Numeric::Matrix inverseCholeskyMatrix = Deltares::Numeric::Matrix(0, 0);
            int findNewIndex(const std::vector<int> index, const size_t i);
            std::map<std::shared_ptr<Stochast>, int> stochastIndex;
            std::vector<indexWithCorrelation> indexer;
            std::vector<correlationPair> inputCorrelations;
            std::vector<int> GetLinkingCorrelationStochasts(correlationPair correlation, correlationPair otherCorrelation) const;
            size_t dim = 0;
        };
    }
}

