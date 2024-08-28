#pragma once
#include <vector>

#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Statistics/SparseCorrelationMatrix.h"
#include "../Reliability/ReliabilityMethod.h"
#include "../Reliability/DesignPoint.h"
#include "../Reliability/Settings.h"

namespace Deltares
{
    namespace Models
    {
        /**
         * \brief Combines a model, stochastic variables and calculation settings, can perform a calculation and holds results
         */
        class Project
        {
        public:
            /**
             * \brief Collection of all stochastic variables
             */
            std::vector<std::shared_ptr<Statistics::Stochast>> stochasts;

            /**
             * \brief Defines correlations between stochastic variables
             */
            std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = nullptr;

            /**
             * \brief Method which performs a reliability calculation
             */
            std::shared_ptr<Reliability::ReliabilityMethod> reliabilityMethod = nullptr;

            /**
             * \brief Deterministic model which calculates a z-value based on input values
             */
            std::shared_ptr<ZModel> model = nullptr;

            /**
             * \brief Calculation settings
             */
            std::shared_ptr<Reliability::Settings> settings = std::make_shared<Reliability::Settings>();

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
            std::shared_ptr<Reliability::DesignPoint> designPoint = nullptr;

            /**
             * \brief Results of the reliability calculation
             */
            std::shared_ptr<Statistics::Stochast> fragilityCurve = nullptr;

            /**
             * \brief Indicates whether the input is valid
             * \return Indication
             */
            bool isValid() const;

            /**
             * \brief Performs the reliability calculation
             * \return Design point
             */
            std::shared_ptr<Reliability::DesignPoint> getDesignPoint();

            /**
             * \brief Performs the reliability calculation
             * \return Design point
             */
            std::shared_ptr<Statistics::Stochast> getFragilityCurve();

            /**
             * \brief Runs the reliability calculation
             */
            void run();

        private:
            std::shared_ptr<Statistics::CorrelationMatrix> getCorrelationMatrix();

        };
    }
}

