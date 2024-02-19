#pragma once

namespace Deltares
{
    namespace ProbLibTests
    {

        class gaussian_correlation_tests
        {
        public:
            void testCorrelationGaussianModel1a();
            void testCorrelationGaussianModel2a();
            void testCorrelationGaussianModel1b();
            void testCorrelationGaussianModel2b();
        private:
            const double margin = 1.0e-6;
        };
    }
}
