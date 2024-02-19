#include "parseMethod.h"
#include <vector>
#include "../utils/probLibException.h"
#include "../utils/probLibString.h"

namespace Deltares {
    namespace ProbLibCore {

        std::vector<std::pair<std::string, ProbMethod>> parseMethod::getMethods() const
        {
            auto availableMethod = std::vector<std::pair<std::string, ProbMethod>>();
            availableMethod.push_back({ "form", ProbMethod::FORM });
            availableMethod.push_back({ "fdir", ProbMethod::FDIR });
            availableMethod.push_back({ "dsfi", ProbMethod::DSFI });
            availableMethod.push_back({ "ds",   ProbMethod::DS });
            availableMethod.push_back({ "cm"  , ProbMethod::CM });
            availableMethod.push_back({ "im",   ProbMethod::IM });
            availableMethod.push_back({ "adaptiveIm", ProbMethod::AdaptiveIM });
            availableMethod.push_back({ "ni",   ProbMethod::NI });
            return availableMethod;
        }

        ProbMethod parseMethod::str2enum(const std::string& s) const
        {
            auto availableMethod = getMethods();
            auto pls = probLibString();
            for (const auto& m : availableMethod)
            {
                if (pls.iStrcmp(s, m.first)) return m.second;
            }

            throw probLibException("unknown method " + s);
        }

        std::string parseMethod::enum2str(const ProbMethod method) const
        {
            auto availableMethod = getMethods();
            for (const auto& m : availableMethod)
            {
                if (m.second == method) return m.first;
            }

            throw probLibException("unknown method in enum2str.");
        }

        bool parseMethod::useSampling(const ProbMethod m) const
        {
            switch (m)
            {
            case ProbMethod::DS:
            case ProbMethod::DSFI:
            case ProbMethod::FDIR:
            case ProbMethod::CM:
            case ProbMethod::IM:
            case ProbMethod::AdaptiveIM:
                return true;
                break;
            default:
                return false;
                break;
            }
        }

        bool parseMethod::useForm(const ProbMethod m) const
        {
            switch (m)
            {
            case ProbMethod::FORM:
            case ProbMethod::DSFI:
            case ProbMethod::FDIR:
                return true;
                break;
            default:
                return false;
                break;
            }
        }
    }
}
