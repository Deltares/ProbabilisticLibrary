#include "TestLatinHyperCube.h"
#include <gtest/gtest.h>
#include "../../Deltares.Probabilistic/Reliability/LatinHyperCube.h"
#include "../../Deltares.Probabilistic/Reliability/Settings.h"
#include "../ProjectBuilder.h"

namespace Deltares::Probabilistic::Test
{
    using namespace Deltares::Reliability;
    using namespace Deltares::Statistics;
    using namespace Deltares::Models;

    void TestLatinHyperCube::testDifferentChunkSizes()
    {
        const auto chunk_sizes = std::vector<int>({ 1, 15, 2000 });
        for (const auto& chunk_size : chunk_sizes)
        {
            auto calculator = LatinHyperCube();
            calculator.Settings->RunSettings->MaxChunkSize = chunk_size;

            const auto model_runner = ProjectBuilder().BuildProject();

            const auto design_point = calculator.getDesignPoint(model_runner);

            ASSERT_EQ(design_point->Alphas.size(), 2);
            EXPECT_NEAR(design_point->Beta, 1.896, 1e-3);
            EXPECT_NEAR(design_point->Alphas[0]->X, 1.358, 1e-3);
            EXPECT_NEAR(design_point->Alphas[1]->X, -1.322, 1e-3);
        }
    }

    void TestLatinHyperCube::testBoundedProbability()
    {
        struct Case
        {
            double min1;
            double max1;
            double min2;
            double max2;
            bool upper_failure;
            bool remainder_failure;
            double bounded_failure;
        };

        const std::vector<Case> cases = {
            { -1.0, 1.0, -8.0, 8.0, false, false, 0.25 },
            { -1.0, 1.0, -8.0, 8.0, true, true, 0.25 },
            { -0.5, 2.0, -2.0, 0.5, false, true, 0.25 },
            { -0.5, 2.0, -2.0, 0.5, true, false, 0.25 },
            { -1.0, 1.0, -8.0, 8.0, false, false, 0.0 },
            { -1.0, 1.0, -8.0, 8.0, true, true, 0.0 },
            { -1.0, 1.0, -8.0, 8.0, false, false, 1.0 },
            { -1.0, 1.0, -8.0, 8.0, true, true, 1.0 }
        };

        for (const auto& test : cases)
        {
            SCOPED_TRACE(testing::Message() << "bounds: " << test.min1 << ", " << test.max1
                << "; " << test.min2 << ", " << test.max2 << "; upper failure: " << test.upper_failure
                << "; bounded failure: " << test.bounded_failure);

            auto calculator = LatinHyperCube();
            calculator.Settings->MinimumSamples = 100;

            auto stochast1 = ProjectBuilder::getNormalStochast();
            auto stochast2 = ProjectBuilder::getNormalStochast();
            for (const auto& stochast : { stochast1, stochast2 })
            {
                auto settings = std::make_shared<StochastSettings>();
                settings->stochast = stochast;
                settings->MinValue = stochast == stochast1 ? test.min1 : test.min2;
                settings->MaxValue = stochast == stochast1 ? test.max1 : test.max2;
                calculator.Settings->StochastSet->stochastSettings.push_back(settings);
            }

            const double pMin = StandardNormal::getPFromU(test.min1);
            const double pMax = StandardNormal::getPFromU(test.max1);
            const double fraction = test.upper_failure ? 1.0 - test.bounded_failure : test.bounded_failure;
            const double threshold = StandardNormal::getUFromP(pMin + fraction * (pMax - pMin));
            const auto model = std::make_shared<ZModel>([threshold, test](ModelSample& sample)
                {
                    sample.Z = test.upper_failure ? threshold - sample.Values[0] : sample.Values[0] - threshold;
                });
            const auto correlation = std::make_shared<CorrelationMatrix>(true);
            const auto converter = std::make_shared<UConverter>(std::vector<std::shared_ptr<Stochast>>{ stochast1, stochast2 }, correlation);
            converter->initializeForRun();
            const auto runner = std::make_shared<ModelRunner>(model, converter);

            const auto design_point = calculator.getDesignPoint(runner);
            const double range1 = pMax - pMin;
            const double range2 = test.min2 == -8.0 && test.max2 == 8.0 ? 1.0 :
                StandardNormal::getPFromU(test.max2) - StandardNormal::getPFromU(test.min2);
            const double qRange = range1 * range2;
            const double expected_failure = test.bounded_failure * qRange + (test.remainder_failure ? 1.0 - qRange : 0.0);

            EXPECT_NEAR(design_point->convergenceReport->FailFraction, test.bounded_failure, 1e-12);
            EXPECT_NEAR(StandardNormal::getQFromU(design_point->Beta), expected_failure, 1e-10);
        }
    }

    void TestLatinHyperCube::testFailedEvaluations()
    {
        for (bool fail_remainder : { false, true })
        {
            auto calculator = LatinHyperCube();
            calculator.Settings->MinimumSamples = 100;

            const auto stochast = ProjectBuilder::getNormalStochast();
            auto settings = std::make_shared<StochastSettings>();
            settings->stochast = stochast;
            settings->MinValue = -1.0;
            settings->MaxValue = 1.0;
            calculator.Settings->StochastSet->stochastSettings.push_back(settings);

            const auto model = std::make_shared<ZModel>([fail_remainder](ModelSample& sample)
                {
                    double x = sample.Values[0];
                    sample.Z = ((x > 0.0 && x < 1.0) || (fail_remainder && x > 1.0)) ? std::nan("") : -1.0;
                });
            auto correlation = std::make_shared<CorrelationMatrix>(true);
            auto converter = std::make_shared<UConverter>(std::vector<std::shared_ptr<Stochast>>{ stochast }, correlation);
            converter->initializeForRun();
            const auto runner = std::make_shared<ModelRunner>(model, converter);

            const auto design_point = calculator.getDesignPoint(runner);
            EXPECT_EQ(design_point->convergenceReport->FailedSamples, 50);
            EXPECT_DOUBLE_EQ(design_point->convergenceReport->FailFraction, 1.0);
            if (fail_remainder)
            {
                EXPECT_TRUE(std::isnan(design_point->Beta));
            }
            else
            {
                EXPECT_DOUBLE_EQ(design_point->Beta, -StandardNormal::BetaMax);
            }
        }
    }

    void TestLatinHyperCube::testModelReturnTypes()
    {
        for (auto returnType : { ModelReturnType::ProbabilityFailure, ModelReturnType::ReliabilityIndex })
        {
            for (bool bounded : { false, true })
            {
                for (bool inverse : { false, true })
                {
                    SCOPED_TRACE(testing::Message() << "return type: " << static_cast<int>(returnType)
                        << "; bounded: " << bounded << "; inverse: " << inverse);

                    auto settings = Settings();
                    settings.ReliabilityMethod = ReliabilityMethodType::ReliabilityLatinHyperCube;
                    settings.MinimumSamples = 100;
                    settings.RunSettings->modelReturnType = returnType;
                    settings.RunSettings->MaxChunkSize = 15;

                    const auto stochast = ProjectBuilder::getNormalStochast();
                    if (bounded)
                    {
                        auto stochastSettings = std::make_shared<StochastSettings>();
                        stochastSettings->stochast = stochast;
                        stochastSettings->MinValue = -1.0;
                        stochastSettings->MaxValue = 1.0;
                        settings.StochastSet->stochastSettings.push_back(stochastSettings);
                    }

                    Logging::ValidationReport report;
                    settings.validate(report);
                    EXPECT_TRUE(report.isValid());
                    auto calculator = settings.GetReliabilityMethod();
                    const auto model = std::make_shared<ZModel>([returnType, inverse](ModelSample& sample)
                        {
                            double probability = sample.Values[0] > 1.0 ? 0.4 : (sample.Values[0] < 0.0 ? 0.2 : 0.8);
                            if (inverse) probability = 1.0 - probability;
                            sample.Z = returnType == ModelReturnType::ProbabilityFailure
                                ? probability : StandardNormal::getUFromQ(probability);
                        });
                    auto correlation = std::make_shared<CorrelationMatrix>(true);
                    auto converter = std::make_shared<UConverter>(std::vector<std::shared_ptr<Stochast>>{ stochast }, correlation);
                    converter->initializeForRun();
                    auto runner = std::make_shared<ModelRunner>(model, converter);
                    const auto designPoint = calculator->getDesignPoint(runner);

                    const double qRange = bounded ? StandardNormal::getPFromU(1.0) - StandardNormal::getPFromU(-1.0) : 1.0;
                    // Sixteen of the 100 unbounded midpoint samples lie above 1.
                    const double upperTail = bounded ? 0.0 : 0.16;
                    const double expectedFraction = inverse ? 0.5 + 0.4 * upperTail : 0.5 - 0.4 * upperTail;
                    const double remainder = inverse ? 0.6 : 0.4;
                    EXPECT_NEAR(designPoint->convergenceReport->FailFraction, expectedFraction, 1e-12);
                    EXPECT_EQ(designPoint->convergenceReport->FailedSamples, 100);
                    EXPECT_NEAR(StandardNormal::getQFromU(designPoint->Beta), qRange * expectedFraction + (1.0 - qRange) * remainder, 1e-10);
                    ASSERT_EQ(designPoint->Alphas.size(), 2);
                    if (bounded)
                    {
                        EXPECT_LT(designPoint->Alphas[0]->U, 0.0);
                        EXPECT_GT(designPoint->Alphas[1]->U, 0.0);
                    }
                }
            }
        }
    }

}
