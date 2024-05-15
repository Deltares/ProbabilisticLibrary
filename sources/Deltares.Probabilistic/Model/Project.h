#pragma once
#include <vector>

#include "../Statistics/Stochast.h"
#include "../Statistics/Correlationmatrix.h"
#include "../Reliability/ReliabilityMethod.h"

namespace Deltares
{
    namespace Models
    {
        using namespace Deltares::Statistics;
        using namespace Deltares::Reliability;

        /**
         * \brief Combines a model, stochastic variables and calculation settings, can perform a calculation and holds results
         */
        class Project
        {
        public:
            /**
             * \brief Collection of all stochastic variables
             */
            std::vector<std::shared_ptr<Stochast>> stochasts;

            /**
             * \brief Defines correlations between stochastic variables
             */
            std::shared_ptr<CorrelationMatrix> correlationMatrix = std::make_shared<CorrelationMatrix>();

            /**
             * \brief Method which performs a reliability calculation
             */
            std::shared_ptr<ReliabilityMethod> reliabilityMethod = nullptr;

            /**
             * \brief Deterministic model which calculates a z-value based on input values
             */
            std::shared_ptr<ZModel> model = nullptr;

            /**
             * \brief Settings for performing a calculation
             * \remark Settings of the reliability calculation are held in the settings of the reliability method
             */
            std::shared_ptr<RunSettings> runSettings = std::make_shared<RunSettings>();

            /**
             * \brief Callback for progress during the calculation
             */
            std::shared_ptr<ProgressIndicator> progressIndicator = nullptr;

            /**
             * \brief Results of the reliability calculation
             */
            std::shared_ptr<DesignPoint> designPoint = nullptr;

            /**
             * \brief Indicates whether the input is valid
             * \return Indication
             */
            bool isValid() const;

            /**
             * \brief Performs the reliability calculation
             * \return Design point
             */
            std::shared_ptr<DesignPoint> getDesignPoint();
        };
    }
}

