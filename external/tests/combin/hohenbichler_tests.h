#pragma once

namespace Deltares
{
    namespace ProbLibTests
    {
        class HohenbichlerTest
        {
        public:
            void NoCorrelation();
            void AlmostFullCorrelation();
            void FullCorrelation();
            void PartialCorrelation();
            void negativeCorrelation();
            void negativeCorrelationNegativeBeta();
            void negativeCorrelationZeroBeta();
            void RhoLimit();
        private:
            const double margin = 1.0e-12;
        };
    }
}
