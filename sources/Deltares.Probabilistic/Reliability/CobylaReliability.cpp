#include "CobylaReliability.h"
#include "../Optimization/CobylaOptimization.h"

using namespace Deltares::Optimization;
using namespace Deltares::Models;

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint>CobylaReliability::getDesignPoint(std::shared_ptr<ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(Settings->StochastSet);

            const int nStochasts = modelRunner->getVaryingStochastCount();

            auto sampleProvider = SampleProvider(Settings->StochastSet, false);
            auto initialSample = sampleProvider.getSample();
            double z0Fac = getZFactor(modelRunner->getZValue(initialSample));

            auto optModel = std::make_shared<wrappedOptimizationModel>(modelRunner);

            auto optimizer = CobylaOptimization();

            auto searchArea = SearchArea();
            searchArea.Dimensions = std::vector<SearchDimension>(nStochasts);
            std::shared_ptr<Sample> startPoint = Settings->StochastSet->getStartPoint();
            for( int i = 0; i < nStochasts; i++)
            {
                searchArea.Dimensions[i].LowerBound = Settings->StochastSet->VaryingStochastSettings[i]->MinValue;
                searchArea.Dimensions[i].UpperBound = Settings->StochastSet->VaryingStochastSettings[i]->MaxValue;
                searchArea.Dimensions[i].StartValue = startPoint->Values[i];
            }

            auto result = optimizer.GetCalibrationPoint(searchArea, optModel);
            double beta = 0.0;
            for (int i = 0; i < nStochasts; i++)
            {
                beta += pow(result.Input[i], 2);
            }
            beta = z0Fac * std::sqrt(beta);

            auto dp = std::make_shared<DesignPoint>();
            dp->Beta = beta;
            for (int i = 0; i < nStochasts; i++)
            {
                auto alpha = std::make_shared<StochastPointAlpha>();
                alpha->Stochast = Settings->StochastSet->stochastSettings[i]->stochast;
                alpha->U = result.Input[i];
                alpha->Alpha = -alpha->U / beta;
                alpha->X = alpha->Stochast->getXFromU(alpha->U);
                dp->Alphas.push_back(alpha);
            }

            return dp;
        };

        double wrappedOptimizationModel::GetConstraintValue(const std::shared_ptr<Sample> sample) const
        {
            auto z = modelRunner->getZValue(sample);
            return std::abs(z);
        }

        double wrappedOptimizationModel::GetZValue(std::shared_ptr<Sample> sample) const
        {
            double beta = 0.0;
            for (const auto & val : sample->Values)
            {
                beta += pow(val, 2);
            }
            return std::sqrt(beta);
        }
    }
}


