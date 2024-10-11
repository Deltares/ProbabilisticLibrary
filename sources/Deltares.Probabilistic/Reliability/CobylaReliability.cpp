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
            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            const int nStochasts = modelRunner->getVaryingStochastCount();

            auto optModel = std::make_shared<wrappedOptimizationModel>(modelRunner);

            auto optimizer = CobylaOptimization();

            auto searchArea = SearchArea();
            searchArea.Dimensions = std::vector<SearchDimension>(nStochasts);
            std::shared_ptr<Sample> startPoint = this->Settings->StochastSet->getStartPoint();
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
                beta += pow(result.Input[0], 2);
            }
            beta = std::sqrt(beta);

            auto dp = std::make_shared<DesignPoint>();
            dp->Beta = beta;
            for (int i = 0; i < nStochasts; i++)
            {
                auto alpha = std::make_shared<StochastPointAlpha>();
                alpha->U = result.Input[i];
                dp->Alphas.push_back(alpha);
            }

            return dp;
        };

        double wrappedOptimizationModel::GetZValue(std::shared_ptr<Sample> sample) const
        {
            auto z = modelRunner->getZValue(sample);
            return std::abs(z);
        }
    }
}


