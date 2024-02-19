#include "rndDefs.h"
#include "../utils/probLibString.h"
#include "../utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

        rndTypes rndDefs::convert(std::string s)
        {
            auto pls = probLibString();
            if (pls.iStrcmp(s, "gm"))
                return rndTypes::GeorgeMarsaglia;
            else if (pls.iStrcmp(s, "mt"))
                return rndTypes::MersenneTwister;
            else
                throw probLibException("Unknown random number generator: " + s);
        }
    }
}
