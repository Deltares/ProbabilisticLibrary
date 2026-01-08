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
#include "TestXfromU.h"
#include "../../Deltares.Probabilistic/Statistics/Distributions/NormalDistribution.h"
#include "../../Deltares.Probabilistic/Statistics/Distributions/UniformDistribution.h"
#include "../../Deltares.Probabilistic/Statistics/Distributions/TriangularDistribution.h"
#include "../../Deltares.Probabilistic/Statistics/Distributions/ExponentialDistribution.h"
#include "../../Deltares.Probabilistic/Statistics/Distributions/GumbelDistribution.h"
#include "../../Deltares.Probabilistic/Statistics/Distributions/WeibullDistribution.h"
#include "../../Deltares.Probabilistic/Statistics/Distributions/ParetoDistribution.h"
#include "../../Deltares.Probabilistic/Statistics/Distributions/RayleighDistribution.h"
#include "../../Deltares.Probabilistic/Statistics/Distributions/RayleighNDistribution.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"

#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    void TestXfromU::TestNormal()
    {
        constexpr double margin = 1e-9;
        auto normal_distribution = Statistics::NormalDistribution();
        auto properties = Statistics::StochastProperties();
        normal_distribution.setMeanAndDeviation(properties, 2.0, 0.5);
        EXPECT_NEAR(2.0, normal_distribution.getXFromU(properties, 0.0), margin);
        TestRange(normal_distribution, properties, margin);
    }

    void TestXfromU::TestUniform()
    {
        constexpr double margin = 1e-9;
        auto uniform_distribution = Statistics::UniformDistribution();
        auto properties = Statistics::StochastProperties();
        properties.Minimum = -1;
        properties.Maximum = 1;
        EXPECT_NEAR(0.0, uniform_distribution.getXFromU(properties, 0.0), margin);
        TestRange(uniform_distribution, properties, 1e-2);
    }

    void TestXfromU::TestTriangular()
    {
        constexpr double margin = 1e-3;
        auto triangular_distribution = Statistics::TriangularDistribution();
        auto properties = Statistics::StochastProperties();
        properties.Minimum = 0.0;
        properties.Shift = 2.0;
        properties.Maximum = 3.0;
        EXPECT_NEAR(1.732, triangular_distribution.getXFromU(properties, 0.0), margin);
        TestRange(triangular_distribution, properties, 1e-2);
    }

    void TestXfromU::TestExponential()
    {
        constexpr double margin = 1e-3;
        auto exponential_distribution = Statistics::ExponentialDistribution();
        auto properties = Statistics::StochastProperties();
        properties.Scale = 2.0;
        properties.Shift = 0.5;
        EXPECT_NEAR(1.886, exponential_distribution.getXFromU(properties, 0.0), margin);
        TestRange(exponential_distribution, properties, 1e-2);
    }

    void TestXfromU::TestGumbel()
    {
        constexpr double margin = 1e-3;
        auto gumbel_distribution = Statistics::GumbelDistribution();
        auto properties = Statistics::StochastProperties();
        properties.Scale = 2.0;
        properties.Shift = 3.0;
        EXPECT_NEAR(3.733, gumbel_distribution.getXFromU(properties, 0.0), margin);
        TestRange(gumbel_distribution, properties, 1e-2);
    }

    void TestXfromU::TestWeibull()
    {
        constexpr double margin = 1e-3;
        auto weibull_distribution = Statistics::WeibullDistribution();
        auto properties = Statistics::StochastProperties();
        properties.Scale = 1.0;
        properties.Shift = 1.0;
        properties.Shape = 2.0;
        EXPECT_NEAR(1.8326, weibull_distribution.getXFromU(properties, 0.0), margin);
        TestRange(weibull_distribution, properties, 1e-2);
    }

    void TestXfromU::TestPareto()
    {
        constexpr double margin = 1e-3;
        auto pareto_distribution = Statistics::ParetoDistribution();
        auto properties = Statistics::StochastProperties();
        properties.Scale = 1.0;
        properties.Shape = 3.0;
        EXPECT_NEAR(1.2599, pareto_distribution.getXFromU(properties, 0.0), margin);
        TestRange(pareto_distribution, properties, 1e-2);
    }

    void TestXfromU::TestRayleigh()
    {
        constexpr double margin = 1e-3;
        auto rayleigh_distribution = Statistics::RayleighDistribution();
        auto properties = Statistics::StochastProperties();
        properties.Scale = 0.5;
        properties.Shift = 1.0;
        EXPECT_NEAR(1.5887, rayleigh_distribution.getXFromU(properties, 0.0), margin);
        TestRange(rayleigh_distribution, properties, 1e-2);
    }

    void TestXfromU::TestRayleighN()
    {
        constexpr double margin = 1e-3;
        auto rayleigh_n_distribution = Statistics::RayleighNDistribution();
        auto properties = Statistics::StochastProperties();
        properties.Scale = 1.0;
        properties.Shift = 0.0;
        properties.Shape = 50.0;
        EXPECT_NEAR(2.9276, rayleigh_n_distribution.getXFromU(properties, 0.0), margin);
        TestRange(rayleigh_n_distribution, properties, 1e-3, -7, 7);
    }

    void TestXfromU::TestTruncatedNormal()
    {
        constexpr double margin = 1e-3;
        auto stochast = Statistics::Stochast();
        stochast.setDistributionType(Statistics::DistributionType::Normal);
        stochast.setTruncated(true);
        stochast.setMeanAndDeviation(0.0, 1.0);
        stochast.getProperties()->Minimum = -0.5;
        EXPECT_NEAR(0.39687, stochast.getXFromU(0.0), margin);
        for (int i = -7; i <= 7; i++)
        {
            const double u = i;
            const double x = stochast.getXFromU(u);
            const double u2 = stochast.getUFromX(x);
            EXPECT_NEAR(u, u2, margin);
        }
    }

    void TestXfromU::TestRange(Statistics::Distribution& distribution, Statistics::StochastProperties& properties, const double margin,
        const int start, const int stop)
    {
        for (int i = -start; i <= stop; i++)
        {
            const double u = i;
            const double x = distribution.getXFromU(properties, u);
            const double u2 = distribution.getUFromX(properties, x);
            EXPECT_NEAR(u, u2, margin);
        }
    }

}

