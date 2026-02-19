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

#include "ComputeBetaSectionTests.h"
#include "../../Deltares.Probabilistic/Combine/ComputeBetaSection.h"
#include <numbers>
#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    void ComputeBetaSectionTests::Test1()
    {
        using namespace std::numbers;

        constexpr double beta_cross_section = 5.0;
        constexpr double section_length = 250.0;
        constexpr double rho_z = 0.8;
        constexpr double dz = 125.0;
        double delta_l = std::min(dz / beta_cross_section * sqrt(pi) / sqrt(1.0 - rho_z), section_length);
        delta_l = std::max(delta_l, 0.01);
        auto calculator = Reliability::ComputeBetaSection(section_length, rho_z, dz, delta_l);
        const auto beta_section = calculator.Compute(beta_cross_section);

        constexpr double beta_expected = 4.8170194948342857;

        EXPECT_NEAR(beta_expected, beta_section, margin);

    }
}

