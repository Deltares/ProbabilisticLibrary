#pragma once
#include <vector>

namespace Deltares
{
    namespace ProbLibTests
    {

        class distributionsTests
        {
        public:
            void allTests();
        private:
            void betaDistTest1();
            void betaDistTest2();
            void betaInverseTestActualData();
            void uniform_dist_test();
            void logNormalInverseTestProbOneHalf();
            void logNormalInverseTypeIITestProbOneHalf();
            void logNormalTypeIITest();
            void weibullTest();
            void gumbelInverseTest();
            void gumbelIIandModifiedTests();
            void TruncatedNormalTests();
            void TriangularInverseTestProbZero();
            void TriangularInverseTestProbOne();
            void TriangularInverseTestProbContinuous();
            void testPareto();
            void rayleigh5000DistributionTest();
            void exponentialTestProbZero();
            std::vector<double[7]> refValues();
        };
    }
}
