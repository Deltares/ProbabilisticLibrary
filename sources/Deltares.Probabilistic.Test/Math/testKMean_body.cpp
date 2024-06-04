#include "testKMean.h"
#include "../../Deltares.Probabilistic/Optimization/KMeansClustering.h"
#include "../../Deltares.Probabilistic/Model/Sample.h"

using namespace Deltares::Numeric;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void testKMean::allKMeanTests()
            {
                test1();
            }

            void testKMean::test1()
            {
                auto inputdata = std::vector<std::vector<double>>();
                inputdata.push_back({ 0.65, 0.22 });
                inputdata.push_back({ 0.73, 0.16 });
                inputdata.push_back({ 0.59, 0.11 });
                inputdata.push_back({ 0.61, 0.12 });
                inputdata.push_back({ 0.75, 0.15 });
                inputdata.push_back({ 0.67, 0.24 });
                inputdata.push_back({ 0.68, 0.23 });
                inputdata.push_back({ 0.70, 0.22 });
                inputdata.push_back({ 0.62, 0.13 });
                inputdata.push_back({ 0.66, 0.21 });
                inputdata.push_back({ 0.77, 0.19 });
                inputdata.push_back({ 0.75, 0.18 });
                inputdata.push_back({ 0.74, 0.17 });
                inputdata.push_back({ 0.70, 0.22 });
                inputdata.push_back({ 0.61, 0.11 });
                inputdata.push_back({ 0.58, 0.1 });
                inputdata.push_back({ 0.66, 0.23 });
                inputdata.push_back({ 0.59, 0.12 });
                inputdata.push_back({ 0.68, 0.21 });
                inputdata.push_back({ 0.61, 0.13 });
                auto samples = std::vector < std::shared_ptr<Models::Sample>>();
                for (const auto & input : inputdata)
                {
                    auto s = std::make_shared<Models::Sample>(2);
                    s->Weight = 1.0;
                    s->Values = input;
                    samples.push_back(s);
                }
                auto kmean = Optimization::KMeansClustering();
                kmean.Settings->MaxClusters = 3;
                kmean.Settings->OptimizeNumberOfClusters = true;
                kmean.Settings->generatorType = Numeric::RandomValueGeneratorType::MersenneTwister;
                auto result = kmean.getClusterCenters(samples);
                EXPECT_NEAR(result[0]->Values[0], 0.675, margin);
                EXPECT_NEAR(result[0]->Values[1], 0.2212, margin);
                EXPECT_NEAR(result[1]->Values[0], 0.748, margin);
                EXPECT_NEAR(result[1]->Values[1], 0.17, margin);
                EXPECT_NEAR(result[2]->Values[0], 0.6014, margin);
                EXPECT_NEAR(result[2]->Values[1], 0.1171, margin);
            }

        }
    }
}

