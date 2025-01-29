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
#include "../../Deltares.Probabilistic/Combine/combiner.h"
#include "../../Deltares.Probabilistic/Combine/ExcludingCombiner.h"
#include "../../Deltares.Probabilistic/Combine/alphaBeta.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class CombinerTest
            {
            public:
                void allCombinerTests() const;
            private:
                void HohenbichlerCombinerTest() const;
                void Hohenbichler2CombinerTest() const;
                void HohenbichlerCombiner1StochTest() const;
                void Hohenbichler2Combiner1StochTest() const;
                void DirectionalSamplingCombinerTest() const;
                void ImportanceSamplingCombinerTest() const;
                void ImportanceSamplingCombinerAndTest() const;
                void ImportanceSamplingCombinerInvertedTest() const;
                void ExcludingCombinerTest() const;
                void tester(Reliability::Combiner* comb, const double beta, const Reliability::alphaBeta& ref, const Reliability::combineAndOr AndOr) const;
                void excluding_tester(Reliability::ExcludingCombiner* comb, const double beta) const;
                void addDesignPoint(const double beta, const size_t nStochasts, size_t i, std::vector<double>& alphaInput, std::vector<std::shared_ptr<Deltares::Statistics::Stochast>>& stochasts, std::vector<std::shared_ptr<Deltares::Reliability::DesignPoint>>& designPoints) const;
                void tester1stoch(Reliability::Combiner* comb, const double rho, const double beta, const Reliability::alphaBeta& ref, const Reliability::combineAndOr AndOr) const;
                const double margin = 1.0e-4;
            };
        }
    }
}
