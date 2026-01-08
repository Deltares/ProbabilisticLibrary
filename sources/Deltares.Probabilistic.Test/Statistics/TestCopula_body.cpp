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
#include "TestCopula.h"
#include "../../Deltares.Probabilistic/Statistics/ClaytonCopula.h"
#include "../../Deltares.Probabilistic/Statistics/FrankCopula.h"
#include "../../Deltares.Probabilistic/Statistics/GumbelCopula.h"
#include "../../Deltares.Probabilistic/Statistics/GaussianCopula.h"
#include "../../Deltares.Probabilistic/Statistics/DiagonalBandCopula.h"
#include "../../Deltares.Probabilistic/Statistics/CopulaCorrelation.h"
#include "../../Deltares.Probabilistic/Statistics/CorrelationMatrix.h"

#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    void TestCopula::TestClayton()
    {
        constexpr double margin = 1e-9;

        auto clayton_copula = Statistics::ClaytonCopula(0.7);
        double a = 1.0;
        double b = 1.0;
        clayton_copula.update_uspace(a, b);
        EXPECT_NEAR(b, 1.23779153247, margin);
    }

    void TestCopula::TestFrank()
    {
        constexpr double margin = 1e-9;

        auto frank_copula = Statistics::FrankCopula(0.7);
        double a = 1.0;
        double b = 1.0;
        frank_copula.update_uspace(a, b);
        EXPECT_NEAR(b, 1.1216995066, margin);
    }

    void TestCopula::TestGaussian()
    {
        constexpr double margin = 1e-12;

        auto correlation = Statistics::GaussianCopula(0.5);
        auto correlation_with_type = correlation.getCorrelation();
        EXPECT_NEAR(0.5, correlation_with_type.value, margin);
        constexpr double u1 = 0.9;
        double u2 = 0.8;
        correlation.update_uspace(u1, u2);
        const double expected = u1 * 0.5 + 0.8 * sqrt(0.75);
        EXPECT_NEAR(expected, u2, margin);
    }

    void TestCopula::TestGumbel()
    {
        constexpr double margin = 1e-4;

        auto gumbel_copula = Statistics::GumbelCopula(5.0);
        double a = 0.2;
        double b = 0.2;
        gumbel_copula.update(a, b);
        EXPECT_NEAR(b, 0.1355, margin) << "comparison with Matlab fails";
    }

    void TestCopula::TestValidation()
    {
        auto gaussian_copula = Statistics::GaussianCopula(1.5);
        EXPECT_FALSE(gaussian_copula.isValid());

        auto frank_copula = Statistics::FrankCopula(0.0);
        EXPECT_FALSE(frank_copula.isValid());

        auto clayton_copula = Statistics::ClaytonCopula(-2.0);
        EXPECT_FALSE(clayton_copula.isValid());

        auto gumbel_copula = Statistics::GumbelCopula(0.99);
        EXPECT_FALSE(gumbel_copula.isValid());

        auto diagonal_band_copula = Statistics::DiagonalBandCopula(-1.0);
        EXPECT_FALSE(diagonal_band_copula.isValid());

        auto copulas = Statistics::CopulaCorrelation();
        copulas.Init(2);
        copulas.SetCorrelation(0, 1, 0.1, CorrelationType::Gaussian);
        copulas.SetCorrelation(1, 0, 2.0, CorrelationType::Frank);
        EXPECT_TRUE(copulas.IsValid());

        auto copulas_empty = Statistics::CopulaCorrelation();
        copulas_empty.Init(2);
        copulas_empty.SetCorrelation(1, 0, 2.0, CorrelationType::Frank);
        copulas_empty.SetCorrelation(0, 1, 0.0, CorrelationType::Gaussian);
        EXPECT_EQ(copulas_empty.CountCorrelations() , 0);
    }

    void TestCopula::TestValidationMessages()
    {
        auto report = Logging::ValidationReport();
        auto gaussian_copula = Statistics::GaussianCopula(1.5);
        gaussian_copula.validate(report);
        EXPECT_EQ("Rho value 1.5 is greater than 1.", report.messages[0]->Text);

        report.messages.clear();
        auto frank_copula = Statistics::FrankCopula(0.0);
        frank_copula.validate(report);
        EXPECT_EQ("Theta in Frank copula should be <> 0.0, but is 0.0", report.messages[0]->Text);

        report.messages.clear();
        auto clayton_copula = Statistics::ClaytonCopula(-2.0);
        clayton_copula.validate(report);
        EXPECT_EQ("Theta in Clayton copula should be >= -1.0 and <> 0.0, but is -2.000000", report.messages[0]->Text);

        report.messages.clear();
        auto gumbel_copula = Statistics::GumbelCopula(0.99);
        gumbel_copula.validate(report);
        EXPECT_EQ("Theta value 0.99 is less than 1.", report.messages[0]->Text);

        report.messages.clear();
        auto diagonal_band_copula = Statistics::DiagonalBandCopula(-1.0);
        diagonal_band_copula.validate(report);
        EXPECT_EQ("Alpha value -1 is less than 0.", report.messages[0]->Text);

        report.messages.clear();
        auto copulas = Statistics::CopulaCorrelation();
        copulas.Init(2);
        copulas.SetCorrelation(0, 1, 0.1, CorrelationType::Gaussian);
        copulas.SetCorrelation(1, 0, 2.0, CorrelationType::Frank);
        copulas.Validate(report);
        EXPECT_EQ(0, report.messages.size());

        report.messages.clear();
        auto copulas2 = Statistics::CopulaCorrelation();
        auto stochast1 = std::make_shared<Statistics::Stochast>();
        stochast1->name = "A";
        auto stochast2 = std::make_shared<Statistics::Stochast>();
        stochast2->name = "B";
        auto stochast3 = std::make_shared<Statistics::Stochast>();
        stochast3->name = "C";
        auto stochasts = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochasts.push_back(stochast1);
        stochasts.push_back(stochast2);
        stochasts.push_back(stochast3);
        copulas2.Init(stochasts);
        copulas2.SetCorrelation(stochast1, stochast2, 0.1, CorrelationType::Gaussian);
        copulas2.SetCorrelation(stochast3, stochast1, 2.0, CorrelationType::Frank);
        copulas2.Validate(report);
        EXPECT_EQ("Multiple correlations not allowed for copulas, found for correlations C-A and A-B", report.messages[0]->Text);
    }

    void TestCopula::TestGaussianValidationMessages()
    {
        auto matrix = Statistics::CorrelationMatrix();
        matrix.Init(3);
        matrix.SetCorrelation(0, 1, -0.9, CorrelationType::Gaussian);
        matrix.SetCorrelation(0, 2, -0.9, CorrelationType::Gaussian);
        matrix.SetCorrelation(1, 2, -0.9, CorrelationType::Gaussian);
        auto report = Logging::ValidationReport();
        matrix.Validate(report);
        ASSERT_EQ(report.messages.size(), 1);
        EXPECT_NE(report.messages[0]->Text.find("Cholesky decomposition fails"), std::string::npos);
    }

}

