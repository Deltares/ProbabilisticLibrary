#pragma once

#include "Combiner.h"
#include "DesignPointModel.h"
#include "../Reliability/DesignPoint.h"

namespace Deltares
{
    namespace Reliability
    {
        class CombinedDesignPointModel
        {
        public:
            combineAndOr combineType = combineAndOr::combOr;
            std::vector<std::shared_ptr<DesignPointModel>> designPointModels;
            std::vector<std::shared_ptr<Statistics::Stochast>> stochasts;
            std::vector<std::shared_ptr<Statistics::Stochast>> standardNormalStochasts;

            void addStochasts(std::vector<std::shared_ptr<Statistics::Stochast>> stochasts);
            void addDesignPointModel(std::shared_ptr<DesignPoint> designPoint, bool invert = false, bool negate = false);
            void updateStochasts();
            double getStartValue(std::shared_ptr<Statistics::Stochast> parameter);

            void calculate(std::shared_ptr<ModelSample> sample);
            double getBetaDirection(std::shared_ptr<ModelSample> sample);
            void replaceStandardNormalStochasts(std::shared_ptr<DesignPoint> designPoint);
        };
    }
}

