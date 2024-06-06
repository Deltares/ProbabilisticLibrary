#include "HohenbichlerCombiner.h"
#include "combineElements.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> HohenbichlerCombiner::combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress)
        {
            std::vector<double> rho;

            for (size_t i = 0; i < designPoints[0]->Alphas.size(); i++)
            {
                if (selfCorrelationMatrix != nullptr)
                {
                    rho.push_back(selfCorrelationMatrix->getSelfCorrelation(designPoints[0]->Alphas[i]->Stochast));
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
            for (size_t i = 0; i < designPoints.size(); i++)
            {
                auto dp = copyAlphaBeta(designPoints[i]);
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

