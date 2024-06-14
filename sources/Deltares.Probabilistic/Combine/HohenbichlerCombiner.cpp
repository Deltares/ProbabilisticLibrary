#include "HohenbichlerCombiner.h"
#include "combineElements.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> HohenbichlerCombiner::combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress)
        {
            std::vector<double> rho;

            for (const auto& Alpha : designPoints[0]->Alphas)
            {
                if (selfCorrelationMatrix != nullptr)
                {
                    rho.push_back(selfCorrelationMatrix->getSelfCorrelation(Alpha->Stochast));
                }
                else
                {
                    rho.push_back(1.0);
                }
            }

            return this->CombineDesignPoints(designPoints, rho, combineMethodType);
        }

        std::shared_ptr<DesignPoint> HohenbichlerCombiner::CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints, const std::vector<double>& r, const combineAndOr cmbType)
        {
            auto nStoch = designPoints[0]->Alphas.size();
            elements elm;
            for (const auto& designPoint : designPoints)
            {
                auto dp = copyAlphaBeta(designPoint);
                elm.push_back(dp);
            }

            auto cmb = combineElements();
            auto rho = vector1D(nStoch);
            for (size_t i = 0; i < nStoch; i++)
            {
                rho(i) = r[i];
            }

            auto result = cmb.combineMultipleElements(elm, rho, cmbType);

            auto dp = std::make_shared<DesignPoint>();
            dp->Beta = result.ab.getBeta();
            for (size_t i = 0; i < nStoch; i++)
            {
                auto alpha = std::make_shared<StochastPointAlpha>();
                alpha->Alpha = result.ab.getAlphaI(i);
                dp->Alphas.push_back(alpha);
            }
            return dp;
        }

        alphaBeta HohenbichlerCombiner::copyAlphaBeta(const std::shared_ptr<DesignPoint>& dp)
        {
            auto nStoch = dp->Alphas.size();
            auto alphas = vector1D(nStoch);
            for (size_t i = 0; i < nStoch; i++)
            {
                alphas(i) = dp->Alphas[i]->Alpha;
            }
            return alphaBeta(dp->Beta, alphas);
        }

    };
}

