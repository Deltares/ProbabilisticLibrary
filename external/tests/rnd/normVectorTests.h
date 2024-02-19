#pragma once

namespace Deltares
{
    namespace ProbLibTests
    {
        class normVectorTests
        {
        public:
            void allNormVectorTests();
        private:
            void checkMt19937();
            void testGm();
            void testMt1();
            void testMt2();
            void testMt3();
        };
    }
}
