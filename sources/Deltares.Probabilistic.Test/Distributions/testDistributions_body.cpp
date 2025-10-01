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
        testCompositeStochast();
        testCompositeTruncatedStochast();
        testCompositeConditionalStochast();
        testCompositeConditionalTruncatedStochast();
        testDesignValue();
        testVariationCoefficient();
        testPoisson();
        testGamma();
        testFitNormal();
        testFitLogNormal();
        testFitBernoulli();
        testValidation();
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
            EXPECT_NEAR(u, uCalculated, 5.0 * margin);
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

    double testDistributions::getPdfNumerical(Stochast& s, const double x)
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
            EXPECT_NEAR(cdf, expectedValues[i - 1], 1e-4);
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

        std::shared_ptr<Stochast> variableDefinition1 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector({ 0.0, 1.0 }));
        realizedStochast->ValueSet->StochastValues.push_back(std::make_shared<VariableStochastValue>(0.0, variableDefinition1->getProperties()));

        std::shared_ptr<Stochast> variableDefinition2 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector({ 11.0, 12.0 }));
        realizedStochast->ValueSet->StochastValues.push_back(std::make_shared<VariableStochastValue>(2.0, variableDefinition2->getProperties()));

        std::shared_ptr<Stochast> variableDefinition3 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector({ 1.0, 2.0 }));
        realizedStochast->ValueSet->StochastValues.push_back(std::make_shared<VariableStochastValue>(1.0, variableDefinition3->getProperties()));

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

    void testDistributions::testCompositeStochast()
    {
        constexpr double margin = 1e-9;

        std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>();
        stochast->setDistributionType(DistributionType::Composite);

        std::shared_ptr<Stochast> subStochast1 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector<double>{0.0, 1.0});
        stochast->getProperties()->ContributingStochasts.push_back(std::make_shared<ContributingStochast>(0.4, subStochast1));

        std::shared_ptr<Stochast> subStochast2 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector<double>{5.0, 6.0});
        stochast->getProperties()->ContributingStochasts.push_back(std::make_shared<ContributingStochast>(0.6, subStochast2));

        EXPECT_NEAR(stochast->getPDF(0.5), 0.4, margin);
        EXPECT_NEAR(stochast->getPDF(2.0), 0.0, margin);

        EXPECT_NEAR(stochast->getMean(), 0.4 * 0.5 + 0.6 * 5.5, margin);

        double u = StandardNormal::getUFromP(0.7);
        EXPECT_NEAR(stochast->getXFromU(u), 5.5, margin);
    }

    void testDistributions::testCompositeTruncatedStochast()
    {
        constexpr double margin = 1e-9;

        std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>();
        stochast->setDistributionType(DistributionType::Composite);

        std::shared_ptr<Stochast> subStochast1 = std::make_shared<Stochast>(DistributionType::Deterministic, std::vector<double>{0.0});
        stochast->getProperties()->ContributingStochasts.push_back(std::make_shared<ContributingStochast>(0.4, subStochast1));

        std::shared_ptr<Stochast> subStochast2 = std::make_shared<Stochast>(DistributionType::Normal, std::vector<double>{1.0, 0.2});
        subStochast2->setTruncated(true);
        subStochast2->getProperties()->Minimum = 0;
        stochast->getProperties()->ContributingStochasts.push_back(std::make_shared<ContributingStochast>(0.6, subStochast2));

        EXPECT_NEAR(stochast->getMean(), 0.6 * 1.0, 0.01);
        EXPECT_NEAR(stochast->getXFromU(-1), 0.0, margin);
    }

    void testDistributions::testCompositeConditionalStochast()
    {
        constexpr double margin = 1e-5;

        std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>();
        stochast->setDistributionType(DistributionType::Composite);

        std::shared_ptr<Stochast> subStochast1 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector<double>{0.0, 1.0});
        stochast->getProperties()->ContributingStochasts.push_back(std::make_shared<ContributingStochast>(0.4, subStochast1));

        std::shared_ptr<Stochast> subConditional = std::make_shared<Stochast>(DistributionType::Uniform, std::vector<double>{5.0, 6.0});
        subConditional->IsVariableStochast = true;

        std::shared_ptr<Stochast> variableDefinition1 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector<double> {0.0, 1.0 });
        subConditional->ValueSet->StochastValues.push_back(std::make_shared<VariableStochastValue>(0.0, variableDefinition1->getProperties()));

        std::shared_ptr<Stochast> variableDefinition2 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector<double> {11.0, 12.0 });
        subConditional->ValueSet->StochastValues.push_back(std::make_shared<VariableStochastValue>(2.0, variableDefinition2->getProperties()));

        std::shared_ptr<Stochast> variableDefinition3 = std::make_shared<Stochast>(DistributionType::Uniform, std::vector<double> {1.0, 2.0 });
        subConditional->ValueSet->StochastValues.push_back(std::make_shared<VariableStochastValue>(1.0, variableDefinition3->getProperties()));

        stochast->getProperties()->ContributingStochasts.push_back(std::make_shared<ContributingStochast>(0.6, subConditional));

        stochast->initializeForRun();

        EXPECT_NEAR(stochast->getPDF(0.5), 0.4, margin);
        EXPECT_NEAR(stochast->getPDF(2.0), 0.0, margin);

        EXPECT_NEAR(stochast->getMean(), 0.4 * 0.5 + 0.6 * 5.5, margin);

        double u1 = StandardNormal::getUFromP(0.2);
        EXPECT_NEAR(stochast->getXFromUAndSource(3, u1), 0.5, margin);

        double u2 = StandardNormal::getUFromP(0.7);
        EXPECT_NEAR(stochast->getXFromUAndSource(0.5, u2), 1.0, margin);
        EXPECT_NEAR(stochast->getXFromUAndSource(1, u2), 1.5, margin);
    }

    void testDistributions::testCompositeConditionalTruncatedStochast()
    {
        std::shared_ptr<Stochast> stochast1 = std::make_shared<Stochast>();
        stochast1->setDistributionType(DistributionType::Deterministic);
        stochast1->getProperties()->Location = 0;

        std::shared_ptr<Stochast> stochast2 = std::make_shared<Stochast>();
        stochast2->setDistributionType(DistributionType::Normal);
        stochast2->getProperties()->Location = 0;
        stochast2->setTruncated(true);

        stochast2->IsVariableStochast = true;

        std::shared_ptr<VariableStochastValue> conditional1 = std::make_shared<VariableStochastValue>();
        conditional1->X = 0;
        conditional1->Stochast->Location = 0;
        conditional1->Stochast->Scale = 1;
        conditional1->Stochast->Minimum = 0;
        conditional1->Stochast->Maximum = std::numeric_limits<double>::infinity();
        stochast2->ValueSet->StochastValues.push_back(conditional1);

        std::shared_ptr<VariableStochastValue> conditional2 = std::make_shared<VariableStochastValue>();
        conditional2->X = 10;
        conditional2->Stochast->Location = 10;
        conditional2->Stochast->Scale = 5;
        conditional2->Stochast->Minimum = 0;
        conditional2->Stochast->Maximum = std::numeric_limits<double>::infinity();
        stochast2->ValueSet->StochastValues.push_back(conditional2);

        std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>();
        stochast->setDistributionType(DistributionType::Composite);
        std::shared_ptr<ContributingStochast> contributing1 = std::make_shared<ContributingStochast>();
        contributing1->Probability = 0.4;
        contributing1->Stochast = stochast1;
        stochast->getProperties()->ContributingStochasts.push_back(contributing1);
        std::shared_ptr<ContributingStochast> contributing2 = std::make_shared<ContributingStochast>();
        contributing2->Probability = 0.6;
        contributing2->Stochast = stochast2;
        stochast->getProperties()->ContributingStochasts.push_back(contributing2);

        for (int u = -5; u <= 5; u++)
        {
            for (int xSource = 0; xSource <= 10; xSource++)
            {
                double x = stochast->getXFromUAndSource(xSource, u);
                EXPECT_TRUE(x >= 0);
            }
        }
    }

    void testDistributions::testDesignValue()
    {
        constexpr double margin = 1e-9;
        std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>(DistributionType::Normal, std::vector({ 10.0, 2.0 }));

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

        auto dist = Stochast(DistributionType::Normal, { mean, 1.0 });
        dist.constantParameterType = ConstantParameterType::VariationCoefficient;
        dist.setVariation(var);
        auto sd = dist.getDeviation();
        EXPECT_NEAR(sd, mean * var, 1e-9);

        dist.setMean(mean2);
        auto sd2 = dist.getDeviation();
        EXPECT_NEAR(sd2, mean2 * var, 1e-9);
    }

    void testDistributions::testPoisson()
    {
        auto dist = Stochast(DistributionType::Poisson, { 4.0 });

        double pdf4 = dist.getPDF(4.0);
        EXPECT_NEAR(pdf4, 0.19537, 0.0001);
        double cdf4p1 = dist.getCDF(4.1);
        double cdf4p2 = dist.getCDF(4.2);
        EXPECT_NEAR(cdf4p1, 0.62884, 0.0001);
        EXPECT_EQ(cdf4p1, cdf4p2) << "x from 4.0001... 4.9999 gives the same cdf";

        double pdf10 = dist.getPDF(10.0);
        EXPECT_NEAR(pdf10, 0.00529, 0.0001);
        double cdf10p1 = dist.getCDF(10.1);
        double cdf10p2 = dist.getCDF(10.2);
        EXPECT_NEAR(cdf10p2, 0.99716, 0.0001);
        EXPECT_EQ(cdf10p1, cdf10p2) << "x from 10.0001... 10.9999 gives the same cdf";
    }

    void testDistributions::testGamma()
    {
        constexpr double margin = 1e-12;
        auto dist = Stochast(DistributionType::Gamma, { 8.0, 4.0 });
        EXPECT_NEAR(4.0, dist.getProperties()->Shape, margin); // if shape is integer, first term in pdf is < 0 for x < 0
        EXPECT_NEAR(0.0, dist.getPDF(-1.0), margin) << "x <= 0 should give 0";
        EXPECT_NEAR(0.0, dist.getPDF(0.0), margin) << "x <= 0 should give 0";
        EXPECT_NEAR(0.0063180277053, dist.getPDF(1.0), margin);

        EXPECT_NEAR(0.0, dist.getCDF(-1.0), margin) << "x <= 0 should give 0";
        EXPECT_NEAR(0.0, dist.getCDF(0.0), margin) << "x <= 0 should give 0";
        EXPECT_NEAR(0.00175162254855, dist.getCDF(1.0), margin);
    }

    void testDistributions::testFitNormal()
    {
        constexpr double margin = 1e-3;

        std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>();
        stochast->setDistributionType(DistributionType::Normal);

        std::vector<double> values = { 4.2, 4.3, 4.7, 4.8 };

        stochast->fit(values);

        EXPECT_NEAR(stochast->getMean(), 4.5, margin);
        EXPECT_NEAR(stochast->getDeviation(), 0.294, margin);
        EXPECT_EQ(stochast->getProperties()->Observations, 4);

        std::shared_ptr<Stochast> prior = std::make_shared<Stochast>();
        prior->setDistributionType(DistributionType::Normal);
        prior->setMean(3.0);
        prior->setDeviation(0.5);

        stochast->fitPrior(prior, values);

        EXPECT_NEAR(stochast->getMean(), 4.380, margin);
        EXPECT_NEAR(stochast->getDeviation(), 0.141, margin);
    }

    void testDistributions::testFitLogNormal()
    {
        constexpr double margin = 1e-3;

        std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>();
        stochast->setDistributionType(DistributionType::LogNormal);

        std::vector<double> values = { 4.2, 4.3, 4.7, 4.8 };

        stochast->fit(values);

        EXPECT_NEAR(stochast->getMean(), 4.5, margin);
        EXPECT_NEAR(stochast->getDeviation(), 0.262, margin);
        EXPECT_EQ(stochast->getProperties()->Observations, 4);

        std::shared_ptr<Stochast> prior = std::make_shared<Stochast>();
        prior->setDistributionType(DistributionType::Normal);
        prior->setMean(3.0);
        prior->setDeviation(0.5);

        stochast->fitPrior(prior, values);

        EXPECT_NEAR(stochast->getMean(), 4.503, margin);
        EXPECT_NEAR(stochast->getDeviation(), 0.0, margin);

        stochast->setMeanAndDeviation(2.0, 0.5);

        std::vector<double> negativeValues = { -2.2, -0.7, 1.2, 1.3 };
        stochast->fitPrior(prior, negativeValues);

        EXPECT_NEAR(stochast->getMean(), 1.670, margin);
        EXPECT_NEAR(stochast->getDeviation(), 1.216, margin);
        EXPECT_NEAR(stochast->getProperties()->Shift, -4.4, margin);
    }

    void testDistributions::testFitBernoulli()
    {
        constexpr double margin = 1e-3;

        std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>();
        stochast->setDistributionType(DistributionType::Bernoulli);

        std::vector<double> values = { 1, 1, 0, 1, 1 };

        stochast->fit(values);

        EXPECT_NEAR(stochast->getMean(), 0.8, margin);
        EXPECT_NEAR(stochast->getDeviation(), sqrt(0.8 * 0.2), margin);
        EXPECT_EQ(stochast->getProperties()->Observations, 5);

        std::shared_ptr<Stochast> prior = std::make_shared<Stochast>();
        prior->setDistributionType(DistributionType::Bernoulli);
        prior->setMean(0.4);
        prior->getProperties()->Observations = 5;

        stochast->fitPrior(prior, values);

        EXPECT_NEAR(stochast->getMean(), 0.6, margin);
        EXPECT_NEAR(stochast->getDeviation(), sqrt(0.6 * 0.4), margin);
        EXPECT_EQ(stochast->getProperties()->Observations, 10);
    }

    void testDistributions::testValidation()
    {
        std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>();
        stochast->setDistributionType(DistributionType::Normal);
        stochast->setMean(3);
        stochast->setDeviation(1);

        EXPECT_EQ(true, stochast->isValid());

        Logging::ValidationReport report = stochast->getValidationReport();

        EXPECT_EQ(0, report.messages.size());

        stochast->setDeviation(-1);

        EXPECT_EQ(false, stochast->isValid());

        Logging::ValidationReport report2 = stochast->getValidationReport();

        EXPECT_EQ(1, report2.messages.size());
    }
}

