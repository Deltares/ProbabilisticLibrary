#include "SubsetSimulationSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        std::string SubsetSimulationSettings::getSampleMethodString(SampleMethodType method)
        {
            switch (method)
            {
            case SampleMethodType::MarkovChain: return "markov_chain";
            case SampleMethodType::AdaptiveConditional: return "adaptive_conditional";
            default: throw probLibException("Sample method");
            }
        }

        SampleMethodType SubsetSimulationSettings::getSampleMethod(std::string method)
        {
            if (method == "markov_chain") return SampleMethodType::MarkovChain;
            else if (method == "adaptive_conditional") return SampleMethodType::AdaptiveConditional;
            else throw probLibException("Sample method");
        }
    }
}
