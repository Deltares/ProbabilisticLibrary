#pragma once

namespace Deltares
{
    namespace Numeric
    {
        class RandomValueGenerator
        {
        public:
            virtual void initialize(bool repeatable, int seed, int seedB) {};
            virtual double next() { return 0; };
        };
    }
}


