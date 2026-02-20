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
#include <cmath>
#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    /// <summary>
    /// Testing the upscaling from a cross-section to a section.
    /// BetaCrossSection = 5, sectionLength = 250, breach length = 250, rhoZ = 0.8, correlation length dz = 125.
    /// The resulting betaSection is compared to a precomputed result.
    /// note that the breach length is not used anymore.
    /// </summary>
    void ComputeBetaSectionTests::Test1()
    {
        constexpr double beta_cross_section = 5.0;
        constexpr double section_length = 250.0;
        constexpr double rho_z = 0.8;
        constexpr double dz = 125.0;
        auto calculator = Reliability::ComputeBetaSection({ section_length, rho_z, dz, beta_cross_section });
        const auto beta_section = calculator.Compute(beta_cross_section);

        constexpr double beta_expected = 4.81701949483;
        constexpr double delta_l_expected = 99.08318244015;

        EXPECT_NEAR(beta_expected, beta_section, margin);
        EXPECT_NEAR(delta_l_expected, calculator.getDeltaL(), margin);
    }

    /// <summary>
    /// Testing the upscaling from a cross-section to a section.
    /// BetaCrossSection= 5, sectionLength= 250,  rhoZ= 0.00100001, correlation length dz= 125.
    /// The resulting betaSection is compared to a precomputed result.
    /// </summary>
    void ComputeBetaSectionTests::Test2()
    {
        constexpr double beta_cross_section = 5.0;
        constexpr double section_length = 250.0;
        constexpr double rho_z = 0.00100001; // Just above the limit of rhoZ=0.001 where Hohenbichler is used
        constexpr double dz = 125.0;
        auto calculator = Reliability::ComputeBetaSection({ section_length, rho_z, dz, beta_cross_section });
        const auto beta_section = calculator.Compute(beta_cross_section);

        constexpr double beta_expected = 4.6492091943;
        constexpr double delta_l_expected = 44.33351879828;
        const std::string expected_message = "Intermediate results: Delta L = 44.333519; rhoZ = 0.001000; dZ = 125.000000";

        EXPECT_NEAR(beta_expected, beta_section, margin);
        EXPECT_NEAR(delta_l_expected, calculator.getDeltaL(), margin);
        EXPECT_TRUE(calculator.hasLengthEffect());
        EXPECT_EQ(calculator.createMessage(), expected_message);
    }

    /// <summary>
    /// Testing the upscaling from a cross-section to a section.
    /// BetaCrossSection= 5, sectionLength= 250,  rhoZ= 0.00099999, correlation length dz= 125.
    /// The resulting betaSection is compared to a precomputed result.
    /// </summary>
    void ComputeBetaSectionTests::Test3()
    {
        constexpr double beta_cross_section = 5.0;
        constexpr double section_length = 250.0;
        constexpr double rho_z = 0.00099999; // Just below the limit of rhoZ=0.001 where Hohenbichler is used
        constexpr double dz = 125.0;
        auto calculator = Reliability::ComputeBetaSection({ section_length, rho_z, dz, beta_cross_section });
        const auto beta_section = calculator.Compute(beta_cross_section);

        constexpr double beta_expected = 4.6554398262568437;
        constexpr double delta_l_expected = 44.3335183545;
        const std::string expected_message = "Intermediate results: Delta L = 44.333518; rhoZ = 0.001000; dZ = 125.000000";

        EXPECT_NEAR(beta_expected, beta_section, margin);
        EXPECT_NEAR(delta_l_expected, calculator.getDeltaL(), margin);
        EXPECT_TRUE(calculator.hasLengthEffect());
        EXPECT_EQ(calculator.createMessage(), expected_message);
        EXPECT_EQ(calculator.getCounterNonConv(), 0);
    }

}

