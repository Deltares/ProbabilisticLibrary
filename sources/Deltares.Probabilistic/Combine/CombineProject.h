#pragma once

#include "CombineSettings.h"
#include "../Reliability/DesignPoint.h"
#include "../Statistics/SelfCorrelationMatrix.h"

namespace Deltares
{
    namespace Reliability
    {
        class CombineProject
        {
        public:
            /**
             * \brief Collection of all design points to be combined
             */
            std::vector<std::shared_ptr<Deltares::Reliability::DesignPoint>> designPoints;

            /**
             * \brief Calculation settings
             */
            std::shared_ptr<CombineSettings> settings = std::make_shared<CombineSettings>();

            /**
             * \brief Self correlations
             */
            std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = std::make_shared<Statistics::SelfCorrelationMatrix>();

            /**
             * \brief Results of the combination
             */
            std::shared_ptr<Reliability::DesignPoint> designPoint = nullptr;

            /**
             * \brief Executes the combination
             */
            void run();
        };
    }
}

