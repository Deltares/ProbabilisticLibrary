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
    class copulaPair : public BaseCorrelationPair
    {
    public:
        std::shared_ptr<BaseCopula> copula;
    };

    class CopulaCorrelation : public BaseCorrelation
    {
    public:
        void init(const int maxStochastsCnt) override
        {
            maxStochasts = maxStochastsCnt;
        }

        void init(const std::vector<std::shared_ptr<Stochast>>& stochastList) override
        {
            maxStochasts = static_cast<int>(stochastList.size());
            stochasts = stochastList;
        }

        bool isValid() const override;
        void validate(Logging::ValidationReport& report) const override;

        void SetCorrelation(const int i, const int j, double value, CorrelationType type) override;
        void SetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2,
            double value, CorrelationType type) override;

        CorrelationValueAndType GetCorrelation(const int i, const int j) const override;
        CorrelationValueAndType GetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2) override;

        bool IsIdentity() const override {return copulas.empty();}
        int CountCorrelations() const override { return static_cast<int>(copulas.size()); }
        int getDimension() override { return maxStochasts; }
        std::shared_ptr<Stochast> getStochast(int index) override { return nullptr; }
        std::vector<double> ApplyCorrelation(const std::vector<double>& uValues) override;
        void initializeForRun() override
        {
            // empty; after init and setCorrelation this class is ready to use
        }
        bool isFullyCorrelated(const int i, const std::vector<int>& varyingIndices) const override {return false;}
        void filter(const std::shared_ptr<BaseCorrelation> source, const std::vector<int>& index) override;
        IndexWithCorrelation findDependent(const int i) const override { return IndexWithCorrelation(); }
    private:
        int maxStochasts = -1;
        std::vector<copulaPair> copulas;
        std::vector<std::shared_ptr<Stochast>> stochasts;
    };
}

