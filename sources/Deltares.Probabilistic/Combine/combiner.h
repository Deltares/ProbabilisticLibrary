#pragma once
#include <vector>
#include <memory>
#include "../Reliability/DesignPoint.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Statistics/SelfCorrelationMatrix.h"
#include "../Model/ProgressIndicator.h"
#include "../Model/Project.h"
#include "CombinedDesignPointModel.h"
#include "CombineType.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Super class for all design point combination algorithms
         */
        class Combiner
        {
        public:
            virtual std::shared_ptr<DesignPoint> CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints, const std::vector<double>& rho, const combineAndOr cmbType) = 0;

            /**
             * \brief Combines a number of design points
             * \param combineMethodType Series (or-combination) or Parallel (and-combination) combination type
             * \param designPoints Design points to be combined
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \param progress Progress indicator (optional)
             * \return Design point resembling the combined reliability and alpha values
             */
            virtual std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress) = 0;
        protected:
            /**
             * \brief Gets all stochasts in a number of design points in the design point without doubling
             * \param designPoints design points
             * \return All stochasts without doubling
             */
            std::vector<std::shared_ptr<Statistics::Stochast>> getUniqueStochasts(std::vector<std::shared_ptr<Reliability::DesignPoint>>& designPoints);

            /**
             * \brief Gets the linearized model representing the design points to be combined
             * \param combineMethodType Combination type
             * \param currentDesignPoint Current design point
             * \param previousDesignPoints Previous design points
             * \param stochasts All stochasts
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \return Linearized model
             */
            std::shared_ptr<CombinedDesignPointModel> getModel(combineAndOr combineMethodType, std::shared_ptr<Reliability::DesignPoint> currentDesignPoint, std::vector<std::shared_ptr<Reliability::DesignPoint>>& previousDesignPoints, std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix);

            /**
             * \brief Gets a project containing the linearized model
             * \param model Linearized model
             * \param correlationMatrix Correlation matrix
             * \param selfCorrelationMatrix Self correlation administration
             * \return Project
             */
            std::shared_ptr<Project> getProject(std::shared_ptr<Reliability::CombinedDesignPointModel> model, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix);

        };
    };
}
