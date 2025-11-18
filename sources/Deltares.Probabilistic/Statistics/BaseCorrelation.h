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

namespace Deltares::Statistics
{
    struct indexWithCorrelation
    {
        int index;
        double correlation;
    };

    class BaseCorrelation
    {
    public:
        virtual void init(const int maxStochasts) {}
        virtual void init(std::vector<std::shared_ptr<Stochast>> stochasts) {}

        virtual void SetCorrelation(const int i, const int j, double value) {}
        virtual void SetCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2, double value) {}

        virtual double GetCorrelation(const int i, const int j) const { return -1; }
        virtual double GetCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2) { return -1; }

        virtual bool IsIdentity() const { return false; }
        virtual int CountCorrelations() const { return -1; }
        virtual int getDimension() { return -1; }
        virtual std::shared_ptr<Stochast> getStochast(int index) { return nullptr; }
        virtual bool HasConflictingCorrelations() const { return false; }
        virtual void resolveConflictingCorrelations() {}
        virtual void CholeskyDecomposition() {}
        virtual void InverseCholeskyDecomposition() {}
        virtual bool isFullyCorrelated(const int i, std::vector<int> varyingIndices) const { return false; }
        virtual void filter(const std::shared_ptr<BaseCorrelation> m, const std::vector<int>& index) {}
        virtual indexWithCorrelation findDependent(const int i) const { return indexWithCorrelation(); }
    };
}


