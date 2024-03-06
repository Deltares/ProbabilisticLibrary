#pragma once

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class testStartPointCalculator
            {
            public:
                void allStartPointTests();
            private:
                void testMethodOne();
                void testMethodRaySearch();
                void testMethodSphereSearch();
                const double margin = 1e-9;
            };
        }
    }
}
