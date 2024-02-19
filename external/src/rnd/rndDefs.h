#pragma once
#include <string>

namespace Deltares {
    namespace ProbLibCore {

        enum class rndTypes {
            GeorgeMarsaglia = 1,
            MersenneTwister
        };

        class rndDefs
        {
        public:
            static rndTypes convert(std::string s);
        };
    }
}
