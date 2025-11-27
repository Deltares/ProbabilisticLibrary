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

#include "correlationValueAndType.h"
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

        virtual bool isValid() {return false;}
        virtual void validate(Logging::ValidationReport& report) const {}

        virtual void SetCorrelation(const int i, const int j, double value, correlationType type = correlationType::UnknownYet) {}
        virtual void SetCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2, double value, correlationType type = correlationType::UnknownYet) {}

        virtual correlationValueAndType GetCorrelation(const int i, const int j) const
        {
            return { -1, correlationType::Gaussian };
        }
        virtual correlationValueAndType GetCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2)
        {
            return { -1, correlationType::Gaussian };
        }

        virtual bool IsIdentity() const { return false; }
        virtual int CountCorrelations() const { return -1; }
        virtual int getDimension() { return -1; }
        virtual std::shared_ptr<Stochast> getStochast(int index) { return nullptr; }
        virtual bool HasConflictingCorrelations() const { return false; }
        virtual void resolveConflictingCorrelations() {}
        virtual std::vector<double> ApplyCorrelation(const std::vector<double>& uValues) { return {}; }
        virtual void CholeskyDecomposition() {}
        virtual void InverseCholeskyDecomposition() {}
        virtual bool isFullyCorrelated(const int i, std::vector<int> varyingIndices) const { return false; }
        virtual void filter(const std::shared_ptr<BaseCorrelation> m, const std::vector<int>& index) {}
        virtual indexWithCorrelation findDependent(const int i) const { return indexWithCorrelation(); }
    protected:
        static int findNewIndex(const std::vector<int>& index, const size_t i);
    };
}


