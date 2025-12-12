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

#include "CorrelationValueAndType.h"
#include "Stochast.h"

namespace Deltares::Statistics
{
    /// <summary>
    /// class with index and value of a single correlation
    /// </summary>
    struct IndexWithCorrelation
    {
        /// <summary>
        /// index of the correlation
        /// </summary>
        int index;

        /// <summary>
        /// value of the correlation
        /// </summary>
        double correlation;
    };

    /// <summary>
    /// base class for a pair of stochastic variables
    /// </summary>
    class BaseCorrelationPair
    {
    public:
        /// <summary>
        /// index of the first stochastic variable
        /// </summary>
        int index1;

        /// <summary>
        /// index of the second stochastic variable
        /// </summary>
        int index2;

        /// <summary>
        /// checks whether one of the current variables is also present in the other object
        /// </summary>
        /// <param name="other"> the other object to compare with </param>
        /// <returns> flag: true = is present </returns>
        bool AreLinked(const BaseCorrelationPair& other) const
        {
            return index1 == other.index1 || index1 == other.index2 || index2 == other.index1 || index2 == other.index2;
        }
    };

    /// <summary>
    /// Base class for correlations
    /// </summary>
    class BaseCorrelation
    {
    public:
        /// <summary>
        /// initialize with the maximum number of stochastic variables
        /// </summary>
        /// <param name="maxStochasts"> maximum number of stochastic variables </param>
        virtual void Init(const int maxStochasts) = 0;

        /// <summary>
        /// initialize with the list of stochastic variables
        /// </summary>
        /// <param name="stochasts"> the list of stochastic variables </param>
        virtual void Init(const std::vector<std::shared_ptr<Stochast>>& stochasts) = 0;

        /// <summary>
        /// checks the object is valid
        /// </summary>
        /// <returns> flag: true = valid </returns>
        virtual bool IsValid() const = 0;

        /// <summary>
        /// fill the report with validation messages for this object
        /// </summary>
        /// <param name="report"> report to extend with validation messages </param>
        virtual void Validate(Logging::ValidationReport& report) const = 0;

        /// <summary>
        /// set the correlation between two stochastic variables \n
        /// note that the behaviour of multiple calls with the same set of stochastic variables is undefined
        /// </summary>
        /// <param name="i"> index first stochast </param>
        /// <param name="j"> index second stochast </param>
        /// <param name="value"> correlation value </param>
        /// <param name="type"> correlation type </param>
        virtual void SetCorrelation(const int i, const int j, double value, CorrelationType type) = 0;

        /// <summary>
        /// set the correlation between two stochastic variable \n
        /// note that the behaviour of multiple calls with the same set of stochastic variables is undefined
        /// </summary>
        /// <param name="stochast1"> the first stochast </param>
        /// <param name="stochast2"> the second stochast </param>
        /// <param name="value"> correlation value </param>
        /// <param name="type"> correlation type </param>
        virtual void SetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2, double value, CorrelationType type) = 0;

        /// <summary>
        /// get the correlation between two stochastic variables
        /// </summary>
        /// <param name="i"> index first stochast </param>
        /// <param name="j"> index second stochast </param>
        /// <returns> correlation value and type </returns>
        virtual CorrelationValueAndType GetCorrelation(const int i, const int j) const = 0;

        /// <summary>
        /// get the correlation between two stochastic variables
        /// </summary>
        /// <param name="stochast1"> the first stochast </param>
        /// <param name="stochast2"> the second stochast </param>
        /// <returns> correlation value and type </returns>
        virtual CorrelationValueAndType GetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2) = 0;

        /// <summary>
        /// checks the object has no correlations
        /// </summary>
        /// <returns> flag: true = no correlations </returns>
        virtual bool IsIdentity() const = 0;

        /// <summary>
        /// get the number of pairs of correlated stochastic variables
        /// </summary>
        /// <returns> the number of pairs </returns>
        virtual int CountCorrelations() const = 0;

        /// <summary>
        /// get the number stochastic variables
        /// </summary>
        /// <returns> the number of stochastic variables </returns>
        virtual int GetDimension() = 0;

        /// <summary>
        /// get the stochastic variable for a given index
        /// </summary>
        /// <returns> the stochastic variable </returns>
        virtual std::shared_ptr<Stochast> GetStochast(int index) = 0;

        /// <summary>
        /// apply the correlation on a vector of U-values
        /// </summary>
        /// <param name="uValues"> the input U-values </param>
        /// <returns> the updated vector of U-values </returns>
        virtual std::vector<double> ApplyCorrelation(const std::vector<double>& uValues) = 0;

        /// <summary>
        /// make the object ready-to-run
        /// </summary>
        virtual void InitializeForRun() = 0;

        /// <summary>
        /// checks whether a stochastic variable is fully correlated with another variable
        /// </summary>
        /// <param name="i"> the input U-values </param>
        /// <param name="varyingIndices"> a list of indices </param>
        /// <returns> flag : true = fully correlated </returns>
        virtual bool IsFullyCorrelated(const int i, const std::vector<int>& varyingIndices) const = 0;

        /// <summary>
        /// creates a new object with a sub set from the source
        /// </summary>
        /// <param name="source"> the input correlation object </param>
        /// <param name="index"> the index for filtering </param>
        virtual void Filter(const std::shared_ptr<BaseCorrelation> source, const std::vector<int>& index) = 0;

        /// <summary>
        /// get the stochastic variable where the variable depends on
        /// </summary>
        /// <param name="i"> the index of the stochast </param>
        /// <returns> index of dependent stochastic variable with value </returns>
        virtual IndexWithCorrelation FindDependent(const int i) const = 0;

    protected:
        static int FindNewIndex(const std::vector<int>& index, const size_t i);
    };
}


