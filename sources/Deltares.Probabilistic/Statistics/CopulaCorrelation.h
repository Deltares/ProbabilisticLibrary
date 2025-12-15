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
    class CopulaPair : public BaseCorrelationPair
    {
    public:
        std::shared_ptr<BaseCopula> copula;
    };

    class CopulaCorrelation : public BaseCorrelation
    {
    public:
        void Init(const int max_stochasts_count) override
        {
            max_stochasts = max_stochasts_count;
        }

        void Init(const std::vector<std::shared_ptr<Stochast>>& stochast_list) override
        {
            max_stochasts = static_cast<int>(stochast_list.size());
            stochasts = stochast_list;
        }

        bool IsValid() const override;
        void Validate(Logging::ValidationReport& report) const override;

        void SetCorrelation(const int i, const int j, double value, CorrelationType type) override;
        void SetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2,
            double value, CorrelationType type) override;

        CorrelationValueAndType GetCorrelation(const int i, const int j) const override;
        CorrelationValueAndType GetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2) override;

        bool IsIdentity() const override {return copulas.empty();}
        int CountCorrelations() const override { return static_cast<int>(copulas.size()); }
        int GetDimension() override { return max_stochasts; }
        std::shared_ptr<Stochast> GetStochast(int index) override { return nullptr; }
        std::vector<double> ApplyCorrelation(const std::vector<double>& uValues) override;
        void InitializeForRun() override
        {
            // empty; after init and setCorrelation this class is ready to use
        }
        bool IsFullyCorrelated(const int i, const std::vector<int>& varyingIndices) const override {return false;}
        void Filter(const std::shared_ptr<BaseCorrelation> source, const std::vector<int>& index) override;
        IndexWithCorrelation FindDependent(const int i) const override { return IndexWithCorrelation(); }
    private:
        int max_stochasts = -1;
        std::vector<CopulaPair> copulas;
        std::vector<std::shared_ptr<Stochast>> stochasts;
        void RemovePair(const CopulaPair& pair);
        size_t FindPair(const CopulaPair& pair) const;
    };
}

