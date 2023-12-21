#include "distrib.h"
#include "../utils/probLibString.h"

namespace Deltares {
    namespace ProbLibCore {

        std::string distrib::getSummary(const std::vector<double>& p4) const
        {
            std::string s;
            auto pls = probLibString();
            for (int j = 0; j < nrParams; j++)
            {
                s += pls.trim(pls.double2str(p4[j]), " ") + " ";
            }

            return s;
        }
    }
}
