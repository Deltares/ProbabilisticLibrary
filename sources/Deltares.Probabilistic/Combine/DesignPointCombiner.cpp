#include "DesignPointCombiner.h"

namespace Deltares
{
    namespace Reliability
    {
        std::string DesignPointCombiner::getCombineTypeString(combineAndOr type)
        {
            switch (type)
            {
            case combineAndOr::combOr: return "series";
            case combineAndOr::combAnd: return "parallel";
            default: throw probLibException("Combine type");
            }
        }

        combineAndOr DesignPointCombiner::getCombineType(std::string method)
        {
            if (method == "series") return combineAndOr::combOr;
            else if (method == "parallel") return combineAndOr::combAnd;
            else throw probLibException("Combine type");
        }

        std::string DesignPointCombiner::getCombinerMethodString(CombinerType type)
        {
            switch (type)
            {
            case CombinerType::Hohenbichler: return "hohenbichler";
            case CombinerType::ImportanceSampling: return "importance_sampling";
            case CombinerType::DirectionalSampling: return "directional_sampling";
            default: throw probLibException("Combiner method");
            }
        }

        CombinerType DesignPointCombiner::getCombinerMethod(std::string method)
        {
            if (method == "hohenbichler") return CombinerType::Hohenbichler;
            else if (method == "importance_sampling") return CombinerType::ImportanceSampling;
            else if (method == "directional_sampling") return CombinerType::DirectionalSampling;
            else throw probLibException("Combiner method type");

        }

    }
}

