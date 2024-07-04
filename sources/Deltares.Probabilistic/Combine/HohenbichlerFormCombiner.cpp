#include "HohenbichlerFormCombiner.h"
#include "combineElements.h"
#include "alphaBeta.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> HohenbichlerFormCombiner::combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress)
        {
            elements elm;
            const std::vector<std::shared_ptr<Statistics::Stochast>> stochasts = getUniqueStochasts(designPoints);
            const auto nStochasts = stochasts.size();

            for (const auto& designPoint : designPoints)
            {
                const auto reorderedDesignPoint = designPoint->getSampleForStochasts(stochasts);
                auto alpha = vector1D(nStochasts);
                for (size_t i = 0; i < nStochasts; i++)
                {
                    alpha(i) = -(reorderedDesignPoint->Values[i] / designPoint->Beta);
                }
                auto dp = alphaBeta(designPoint->Beta, alpha);
                elm.push_back(dp);
            }

            auto cmb = combineElements();
            auto rho = vector1D(nStochasts);
            for (size_t i = 0; i < nStochasts; i++)
            {
                rho(i) = selfCorrelationMatrix->getSelfCorrelation(stochasts[i]);
            }

            auto result = cmb.combineMultipleElements(elm, rho, combineMethodType);
            nonConvergedForm += result.n;

            auto dp = std::make_shared<DesignPoint>();
            dp->Beta = result.ab.getBeta();
            for (size_t i = 0; i < nStochasts; i++)
            {
                auto alpha = std::make_shared<StochastPointAlpha>();
                alpha->Stochast = stochasts[i];
                alpha->Alpha = result.ab.getAlphaI(i);
                dp->Alphas.push_back(alpha);
            }
            return dp;
        }

    };
}

