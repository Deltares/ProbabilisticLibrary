#include "StartPointCalculatorSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        std::string StartPointCalculatorSettings::getStartPointMethodString(StartMethodType method)
        {
            switch (method)
            {
            case StartMethodType::None: return "none";
            case StartMethodType::One: return "one";
            case StartMethodType::RaySearch: return "ray_search";
            case StartMethodType::SphereSearch: return "sphere_search";
            case StartMethodType::SensitivitySearch: return "sensitivity_search";
            default: throw probLibException("Start point method");
            }
        }

        StartMethodType StartPointCalculatorSettings::getStartPointMethod(std::string method)
        {
            if (method == "none") return StartMethodType::None;
            else if (method == "one") return StartMethodType::One;
            else if (method == "ray_search") return StartMethodType::RaySearch;
            else if (method == "sphere_search") return StartMethodType::SphereSearch;
            else if (method == "sensitivity_search") return StartMethodType::SensitivitySearch;
            else throw probLibException("Start point method");
        }
    }
}

