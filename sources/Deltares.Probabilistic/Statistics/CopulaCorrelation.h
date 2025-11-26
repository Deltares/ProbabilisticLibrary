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
#include "BaseCopula.h"

namespace Deltares::Statistics
{
    struct copulaPair
    {
        int i;
        int j;
        std::shared_ptr<BaseCopula> copula;
    };

    class CopulaCorrelation : public BaseCorrelation
    {
    public:
        void init(const int maxStochastsCnt) override
        {
            maxStochasts = maxStochastsCnt;
        }

        void init(std::vector<std::shared_ptr<Stochast>> stochastList) override
        {
            maxStochasts = static_cast<int>(stochastList.size());
            stochasts = stochastList;
        }

        bool isValid() override;

        void SetCorrelation(const int i, const int j, double value, correlationType type) override;
        void SetCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2, double value, correlationType type = correlationType::UnknownYet) override;

        correlationValueAndType GetCorrelation(const int i, const int j) const override;
        correlationValueAndType GetCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2) override;

        bool IsIdentity() const override {return false;}
        int CountCorrelations() const override { return -1; }
        int getDimension() override { return -1; }
        std::shared_ptr<Stochast> getStochast(int index) override { return nullptr; }
        bool HasConflictingCorrelations() const override {return false;}
        void resolveConflictingCorrelations() override {}
        std::vector<double> ApplyCorrelation(const std::vector<double>& uValues) override;
        void CholeskyDecomposition() override {}
        void InverseCholeskyDecomposition() override {}
        bool isFullyCorrelated(const int i, std::vector<int> varyingIndices) const override {return false;}
        void filter(const std::shared_ptr<BaseCorrelation> m, const std::vector<int>& index) override;
        indexWithCorrelation findDependent(const int i) const override { return indexWithCorrelation(); }
        void copyFrom(BaseCorrelation& source);
    private:
        int maxStochasts = -1;
        std::vector<copulaPair> copulas;
        std::vector<std::shared_ptr<Stochast>> stochasts;
        double lastValue = 0.0;
    };
}

