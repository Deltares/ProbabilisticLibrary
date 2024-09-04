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
#include <vector>
#include "../Math/vector1D.h"
#include "alphaBeta.h"
#include "combiner.h"

namespace Deltares {
    namespace Reliability {

        typedef std::vector<alphaBeta> elements;

        struct cmbResult
        {
            alphaBeta ab; // beta and alpha vector
            int       n;  // number of non converged Hohenblicher runs
        };

        class combineElements
        {
        public:
            cmbResult combineTwoElementsPartialCorrelation(alphaBeta& element1,
                alphaBeta& element2, const vector1D& rhoP, const combineAndOr combAndOr);

            cmbResult combineTwoElementsPartialCorrelation(alphaBeta& element1,
                alphaBeta& element2, const vector1D& rhoP, const combineAndOr combAndOr,
                vector1D& alphaI, vector1D& alphaII);

            cmbResult combineMultipleElements(const elements& Elements,
                const vector1D& rho, const combineAndOr combAndOrIn);

            cmbResult combineMultipleElementsFull(const elements& Elements, const combineAndOr combAndOr);

            cmbResult combineMultipleElementsProb(elements& Elements,
                const std::vector<double>& percentages, const combineAndOr combAndOr);

            cmbResult combineMultipleElementsSpatialCorrelated(const elements& Element,
                const std::vector<std::vector<vector1D>>& rho, const combineAndOr combAndOr);

        private:
            void checkArraySizes(const size_t nStochasts, const size_t sizeAlpha2, const size_t sizeRhoP);
            std::pair<double, double> setLargestBeta(const double beta1, const double beta2, const double pf1, const double pf2);
            double combinedFailure(const combineAndOr combAndOr, const double pf1, const double pf2,
                const double pfu, const double pf2pf1);
            void calculateCombinationWithLargestCorrelationSpatialCorrelated(const std::vector<std::vector<vector1D>>& rhoP,
                const size_t nElements, const elements& elements, size_t& i1max, size_t& i2max);
            void calculateCombinationWithLargestCorrelation(const vector1D& rhoP, const size_t nElements,
                const std::vector<alphaBeta>& alpha, size_t& i1max, size_t& i2max);
        };
    }
}
