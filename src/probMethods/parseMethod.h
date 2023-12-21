#pragma once
#include <string>
#include <vector>

namespace Deltares {
    namespace ProbLibCore {

        enum class ProbMethod
        {
            FORM = 1,
            CM = 3,
            DS = 4,
            NI = 5,
            IM = 6,
            AdaptiveIM = 7,
            FDIR = 11,
            DSFIHR,
            CMFORM,
            CMIMFORM,
            FORMDSFIHR,
            DSFI,
            FORMDSFI,
            FORMSTART = 1234,
        };

        class parseMethod
        {
        public:
            ProbMethod str2enum(const std::string& s) const;
            bool useSampling(const ProbMethod m) const;
            bool useForm(const ProbMethod m) const;
            std::string enum2str(const ProbMethod m) const;
        private:
            std::vector<std::pair<std::string, ProbMethod>> getMethods() const;
        };
    }
}
