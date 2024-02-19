#include <sstream>
#include "adaptiveImpSampling.h"
#include "../utils/basic_math.h"

namespace Deltares {
    namespace ProbLibCore {

        adaptiveImpSampling::adaptiveImpSampling(const AIMsettings& s, randomForSampling& r, const progress& pg, const bool compatible)
            : settings(s), rnd(r), p(pg), beCompatible(compatible)
        {
            if (settings.varianceFactor < 0.0)
            {
                throw probLibException("variance factor in IM must be > 0.0");
            }
        }

        lsfResult adaptiveImpSampling::calc(const zmodel& m, const stochSettings& s)
        {
            auto startVector = std::vector<double>();
            for (size_t i = 0; i < s.getNrStochasts(); i++)
            {
                startVector.push_back(settings.startVector.size() > 0 ? settings.startVector(i) : 1.0);
            }
            auto fillStartVector = FillStartVector(settings.startMethod, &startVector[0]);
            auto u = fillStartVector.GetU(m, s, 1);

            auto result = loopMCIS(m, s, u);
            result.method = "adaptive Importance Sampling";
            return result;
        }

        void adaptiveImpSampling::printProgress(const int i, const lsfResult& result)
        {
            int numberNonFailed = result.samplesNeeded - result.samplesFailure;
            std::stringstream ss;
            ss << "i,F,NF = " << i << " , " << result.samplesFailure << " , " << numberNonFailed;
            p.printInfo(ss.str());
        }

        lsfResult adaptiveImpSampling::loopMCIS(const zmodel& m, const stochSettings& s, const vector1D& u)
        {
            settings.offsets = u;

            auto bf = IMbuffer(settings.maxSamplesDef, s.getNrStochasts(), false);
            auto IM = ImportanceSampling(settings, rnd, p, bf);

            auto result = lsfResult();
            for (int i = 1; i <= settings.nAdp; i++)
            {
                result = IM.calc(m, s);
                //
                // Next step :
                // If convergence reached, one final step and then done
                // - Enough failures, but no convergence ? Then use the design point as starting value
                // - Not enough failures ? Increase the variance and try again
                //
                int numberNonFailed = result.samplesNeeded - result.samplesFailure;
                bool converged = min(result.samplesFailure, numberNonFailed) >= settings.epsFailed * (result.samplesNeeded);
                printProgress(i, result);
                if (converged || i == settings.nAdp)
                {
                    if (settings.maxSamplesDef > settings.maximumSamples)
                    {
                        if (beCompatible) {
                            auto newOffset = result.result.getAlpha() * -result.result.getBeta();
                            IM.updateOffset(newOffset);
                            rnd.restart();
                        }
                        IM.updateMaxSamples(settings.maxSamplesDef, !beCompatible);
                        result = IM.calc(m, s);
                        printProgress(i + 1, result);
                    }
                    break;
                }
                else if (result.samplesFailure <= settings.minFailed)
                {
                    IM.updateVariance(settings.increaseVariance);
                    rnd.restart();
                }
                else
                {
                    if (beCompatible) {
                        auto newOffset = result.result.getAlpha() * -result.result.getBeta();
                        IM.updateOffset(newOffset);
                    }
                    else {
                        IM.updateOffset(result.u);
                    }
                    rnd.restart();
                }

            }
            return result;
        }
    }
}
