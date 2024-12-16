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
#include <gtest/gtest.h>
#include "testDistributions.h"

using namespace Deltares::Statistics;

namespace Deltares::Probabilistic::Test
{
    void testDistributions::allDistributionsTests()
    {
        testConditionalWeibull();
        testConditionalWeibullNonIntegerShape();
        testConditionalWeibullMeanDeviation();
        testConditionalWeibullCdfPdf();
        testConditionalWeibullCdfPdf2();
        testConditionalStochast();
        testDesignValue();
        testVariationCoefficient();
    }

    void testDistributions::testConditionalWeibull()
    {
        constexpr double margin = 1e-9;
        std::shared_ptr< StochastProperties> params = std::make_shared< StochastProperties>();
        params->Scale = 1.0;
        params->Shape = 1.0;
        params->Shift = 1.0;
        auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

        std::vector<double> expectedValues = 
        {
            1.36651292058166,
            2.75588794089519,
            4.77169994533666,
            7.60705089255662,
            11.3600856506974,
            16.0649982505082,
            21.7367689494396,
            28.3843074961794
        };

        for (int i = 0; i < expectedValues.size(); i++)
        {
            auto u = (double)i;
            auto x = distCondWeibull.getXFromU(u);
            EXPECT_NEAR(x, expectedValues[i], margin);
            auto uCalculated = distCondWeibull.getUFromX(x);
            EXPECT_NEAR(u, uCalculated, 5.0*margin);
        }

    }

    void testDistributions::testConditionalWeibullNonIntegerShape()
    {
        constexpr double margin = 1e-9;
        std::shared_ptr< StochastProperties> params = std::make_shared< StochastProperties>();
        params->Scale = 1.0;
        params->Shape = 1.001;
        params->Shift = 1.0;
        auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

        for (int i = -8; i < -2; i++)
        {
            auto x = distCondWeibull.getXFromU(i);
            EXPECT_NEAR(x, 0.0, margin);
        }
    }

    void testDistributions::testConditionalWeibullMeanDeviation()
    {
        constexpr double margin = 1e-9;
        std::shared_ptr< StochastProperties> params = std::make_shared< StochastProperties>();
        params->Scale = 1.0;
        params->Shape = 1.0;
        params->Shift = 1.0;
        auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

        auto mean = distCondWeibull.getMean();
        EXPECT_NEAR(mean, 1.5956017719, margin);
        auto deviation = distCondWeibull.getDeviation();
        EXPECT_NEAR(deviation, 1.2539175933, margin);
    }

    double testDistributions::getPdfNumerical(Stochast & s, const double x)
    {
        const double dx = 1e-4;
        double p2 = s.getCDF(x + dx);
        double p1 = s.getCDF(x - dx);
        double pdf = (p2 - p1) / (2.0 * dx);
        return pdf;
    }

    void testDistributions::testConditionalWeibullCdfPdf()
    {
        auto params = std::make_shared<StochastProperties>();
        params->Scale = 1.0;
        params->Shape = 1.0;
        params->Shift = 1.0;
        auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

        std::vector<double> expectedValues = {
            0.981851,
            0.999877,
            0.999999,
            1 };

        for (int i = 1; i < 5; i++)
        {
            double x = (double)(i * 5);
            double cdf = distCondWeibull.getCDF(x);
            EXPECT_NEAR(cdf, expectedValues[i-1], 1e-4);
        }

        for (int i = 1; i < 10; i++)
        {
            double x = (double)i;
            double pdf = distCondWeibull.getPDF(x);
            double expectedPdf = getPdfNumerical(distCondWeibull, x);
            EXPECT_NEAR(pdf, expectedPdf, 1e-4);
        }
    }

    void testDistributions::testConditionalWeibullCdfPdf2()
    {
        auto params = std::make_shared<StochastProperties>();
        params->Scale = 2.0;
        params->Shape = 2.0;
        params->Shift = 2.0;
        auto distCondWeibull = Stochast(DistributionType::ConditionalWeibull, params);

        for (int i = 1; i < 10; i++)
        {
            double x = (double)i;
            double pdf = distCondWeibull.getPDF(x);
            double expectedPdf = getPdfNumerical(distCondWeibull, x);
            EXPECT_NEAR(pdf, expectedPdf, 1e-4);
        }
    }

    void testDistributions::testConditionalStochast()
    {
        constexpr double margin = 1e-9;
        std::shared_ptr<StochastProperties> params = std::make_shared<StochastProperties>();
        params->Minimum = 0.0;
        params->Maximum = 1.0;
        std::shared_ptr<Stochast> realizedStochast = std::make_shared<Stochast>(DistributionType::Uniform, params);

        realizedStochast->IsVariableStochast = true;

        std::shared_ptr<Stochast> variableDefinition1 = std::make_shared<Stochast> (DistributionType::Uniform, std::vector({0.0, 1.0 }));
        realizedStochast->ValueSet->StochastValues.push_back(std::make_shared<VariableStochastValue> (0.0, variableDefinition1->getProperties()));

        std::shared_ptr<Stochast> variableDefinition2 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector({ 11.0, 12.0 }));
        realizedStochast->ValueSet->StochastValues.push_back(std::make_shared<VariableStochastValue> (2.0, variableDefinition2->getProperties()));

        std::shared_ptr<Stochast> variableDefinition3 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector({ 1.0, 2.0 }));
        realizedStochast->ValueSet->StochastValues.push_back(std::make_shared<VariableStochastValue> (1.0, variableDefinition3->getProperties()));

        realizedStochast->initializeForRun();

        // exact value
        EXPECT_NEAR(0.5, realizedStochast->getXFromUAndSource(0, 0), margin);
        EXPECT_NEAR(1.5, realizedStochast->getXFromUAndSource(1, 0), margin);
        EXPECT_NEAR(11.5, realizedStochast->getXFromUAndSource(2, 0), margin);

        // interpolated
        EXPECT_NEAR(1, realizedStochast->getXFromUAndSource(0.5, 0), margin);
        EXPECT_NEAR(6.5, realizedStochast->getXFromUAndSource(1.5, 0), margin);

        // extrapolated
        EXPECT_NEAR(0.5, realizedStochast->getXFromUAndSource(-1, 0), margin);
        EXPECT_NEAR(11.5, realizedStochast->getXFromUAndSource(3, 0), margin);
    }

    void testDistributions::testDesignValue()
    {
        constexpr double margin = 1e-9;
        std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>(DistributionType::Normal, std::vector({10.0, 2.0}));

        // not modified
        EXPECT_NEAR(10.0, stochast->getDesignValue(), margin);

        stochast->designFactor = 2;
        stochast->designQuantile = StandardNormal::getPFromU(2.0);

        EXPECT_NEAR((10.0 + 2 * 2) / 2.0, stochast->getDesignValue(), margin);
    }

    void testDistributions::testVariationCoefficient()
    {
        constexpr double mean = 2.5;
        constexpr double mean2 = 5.2;
        constexpr double var = 2.4;

        auto dist = Stochast(DistributionType::Normal, {mean, 1.0});
        dist.constantParameterType = ConstantParameterType::VariationCoefficient;
        dist.setVariation(var);
        auto sd = dist.getDeviation();
        EXPECT_NEAR(sd, mean*var, 1e-9);

        dist.setMean(mean2);
        auto sd2 = dist.getDeviation();
        EXPECT_NEAR(sd2, mean2*var, 1e-9);
    }
}

