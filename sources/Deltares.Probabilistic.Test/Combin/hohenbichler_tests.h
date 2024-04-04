#pragma once

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class HohenbichlerTest
            {
            public:
                void allHohenbichlerTests();
            private:
                void NoCorrelation();
                void AlmostFullCorrelation();
                void FullCorrelation();
                void PartialCorrelation();
                void negativeCorrelation();
                void negativeCorrelationNegativeBeta();
                void negativeCorrelationZeroBeta();
                void RhoLimit();
                const double margin = 1.0e-12;
            };
        }
    }
}
