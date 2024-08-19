#include "TestSensitivity.h"

#include "../projectBuilder.h"

#include "../../Deltares.Probabilistic/Sensitivity/SensitivityProject.h"
#include "../../Deltares.Probabilistic/Sensitivity/CrudeMonteCarloS.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void TestSensitivity::AllSensitivityTests()
            {
                TestAddOneCrudeMonteCarlo();
                TestLinear();
                TestLinearManySamples();
                TestLinearAutoSamples();
            }

            void TestSensitivity::TestAddOneCrudeMonteCarlo()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getAddOneProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::MersenneTwister;

                project->sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_NEAR(stochast->getMean(), 1.0, margin);

                stochast->setDistributionType(Statistics::DistributionType::Uniform);

                ASSERT_NEAR(0.0, stochast->getProperties()->Minimum, margin);
                ASSERT_NEAR(2.0, stochast->getProperties()->Maximum, margin);
            }

            void TestSensitivity::TestLinear()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::MersenneTwister;

                project->sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_NEAR(1.8, stochast->getMean(), margin);
                ASSERT_NEAR(0.82, stochast->getDeviation(), margin);
            }

            void TestSensitivity::TestLinearManySamples()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::MersenneTwister;
                sensitivityMethod->Settings->Samples = 100000;

                project->sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_NEAR(1.8, stochast->getMean(), margin);
                ASSERT_NEAR(0.82, stochast->getDeviation(), margin);

                stochast->setDistributionType(Statistics::DistributionType::Uniform);

                ASSERT_NEAR(-0.2, stochast->getProperties()->Minimum, 10 * margin);
                ASSERT_NEAR(3.8, stochast->getProperties()->Maximum, 10 * margin);
            }

            void TestSensitivity::TestLinearAutoSamples()
            {
                std::shared_ptr<Sensitivity::SensitivityProject> project = projectBuilder::getSensitivityProject(projectBuilder::getLinearProject());

                std::shared_ptr<Sensitivity::CrudeMonteCarloS> sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();
                sensitivityMethod->Settings->randomSettings->RandomGeneratorType = Deltares::Numeric::MersenneTwister;
                sensitivityMethod->Settings->DeriveSamplesFromVariationCoefficient = true;

                project->sensitivityMethod = std::make_shared<Sensitivity::CrudeMonteCarloS>();

                std::shared_ptr<Statistics::Stochast> stochast = project->getStochast();

                ASSERT_EQ(7600, sensitivityMethod->Settings->getRequiredSamples());
                ASSERT_NEAR(1.8, stochast->getMean(), margin);
                ASSERT_NEAR(0.82, stochast->getDeviation(), margin);
            }
        }
    }
}


