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

    class BaseCorrelationPair
    {
    public:
        int index1;
        int index2;
        bool AreLinked(const BaseCorrelationPair& other) const
        {
            return index1 == other.index1 || index1 == other.index2 || index2 == other.index1 || index2 == other.index2;
        }
    };

    class BaseCorrelation
    {
    public:
        virtual void init(const int maxStochasts) = 0;
        virtual void init(const std::vector<std::shared_ptr<Stochast>>& stochasts) = 0;

        virtual bool isValid() const = 0;
        virtual void validate(Logging::ValidationReport& report) const = 0;

        virtual void SetCorrelation(const int i, const int j, double value, correlationType type = correlationType::UnknownYet) = 0;
        virtual void SetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2, double value, correlationType type = correlationType::UnknownYet) = 0;

        virtual correlationValueAndType GetCorrelation(const int i, const int j) const = 0;
        virtual correlationValueAndType GetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2) = 0;

        virtual bool IsIdentity() const = 0;
        virtual int CountCorrelations() const = 0;
        virtual int getDimension() = 0;
        virtual std::shared_ptr<Stochast> getStochast(int index) = 0;
        virtual std::vector<double> ApplyCorrelation(const std::vector<double>& uValues) = 0;
        virtual void initializeForRun() = 0;
        virtual bool isFullyCorrelated(const int i, const std::vector<int>& varyingIndices) const = 0;
        virtual void filter(const std::shared_ptr<BaseCorrelation> m, const std::vector<int>& index) = 0;
        virtual indexWithCorrelation findDependent(const int i) const = 0;
    protected:
        static int findNewIndex(const std::vector<int>& index, const size_t i);
    };
}


