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
#include <memory>
#include <vector>
#include "Stochast.h"
#include "BaseCorrelation.h"

namespace Deltares::Statistics
{

    class CorrelationMatrix : public BaseCorrelation
    {
    public:
        void init(const int maxStochasts) override;
        void init(std::vector<std::shared_ptr<Stochast>> stochasts) override;

        void SetCorrelation(const int i, const int j, double value) override;
        void SetCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2, double value) override;

        double GetCorrelation(const int i, const int j) const override;
        double GetCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2) override;

        bool IsIdentity() const override;
        int CountCorrelations() const override;
        int getDimension() override;
        std::shared_ptr<Statistics::Stochast> getStochast(int index) override;
        bool HasConflictingCorrelations() const override;
        void resolveConflictingCorrelations() override;
        void CholeskyDecomposition() override;
        void InverseCholeskyDecomposition() override;
        bool isFullyCorrelated(const int i, std::vector<int> varyingIndices) const override;
        void filter(const std::shared_ptr<BaseCorrelation> m, const std::vector<int>& index) override;
        indexWithCorrelation findDependent(const int i) const override;
    };
}

