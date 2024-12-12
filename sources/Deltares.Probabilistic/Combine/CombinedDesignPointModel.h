// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include "CombineType.h"
#include "DesignPointModel.h"
#include "../Reliability/DesignPoint.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Statistics/SelfCorrelationMatrix.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Represents a model consisting of a number of design point models
         */
        class CombinedDesignPointModel
        {
        public:
            /**
             * \brief Series (or-combination) or parallel (and-combination) combination type
             */
            combineAndOr combineType = combineAndOr::combOr;

            /**
             * \brief Stochasts in use by the design point models
             * \remark Do not modify this list
             */
            std::vector<std::shared_ptr<Statistics::Stochast>> stochasts;

            /**
             * \brief Normalized stochasts corresponding with stochasts
             * \remark Do not modify this list
             */
            std::vector<std::shared_ptr<Statistics::Stochast>> standardNormalStochasts;

            /**
             * \brief Adds a design point model for a design point
             * \param designPoint Design point
             * \param ignore Indication whether the design point model should ignore positive values
             * \param invert Indication whether the design point model should invert its calculation result
             */
            void addDesignPointModel(std::shared_ptr<DesignPoint> designPoint, bool ignore = false, bool invert = false);

            /**
             * \brief Adds stochasts and generates standard normal stochasts for invocations to the calculation
             * \param stochasts Stochasts
             * \param selfCorrelationMatrix Administration of self-correlations of the stochasts
             */
            void addStochasts(std::vector<std::shared_ptr<Statistics::Stochast>> stochasts, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix);

            /** 
             * \brief Gets the correlation matrix of the standard normal stochasts
             * \param selfCorrelationMatrix Administration of self-correlations of the stochasts
             * \return Correlation matrix
             */
            std::shared_ptr<Statistics::CorrelationMatrix> getCorrelationMatrix(std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix);

            /**
             * \brief Gets the u-value with the maximum absolute value in the design points for a certain stochast
             * \param stochast Stochast
             * \return Maximum value
             */
            double getStartValue(std::shared_ptr<Statistics::Stochast> stochast);

            /**
             * \brief Calculates a sample
             * \param sample Sample, should contain values defined in u-space
             * \remark The calculation result is stored in the sample z-value
             */
            void calculate(std::shared_ptr<ModelSample> sample);

            /**
             * \brief Indicates whether the beta value in a direction can be calculated
             * \return Indication
             */
            bool canCalculateBetaDirection();

            /**
             * \brief Gets the length of of sample in the direction of a given sample where the calculation result is zero
             * \param sample Sample indicating direction
             * \return Length
             */
            double getBetaDirection(std::shared_ptr<ModelSample> sample);

            /**
             * \brief Replaces standard normal stochasts in a design point with stochasts defined in the design points
             * \param designPoint Design point to be replaced with design point stochasts
             */
            void replaceStandardNormalStochasts(std::shared_ptr<DesignPoint> designPoint);
        private:
            /**
             * \brief List of all design point models
             */
            std::vector<std::shared_ptr<DesignPointModel>> designPointModels;

            /**
             * \brief Mapping of standard normal stochasts to stochasts in design points
             */
            std::map<std::shared_ptr<Statistics::Stochast>, std::shared_ptr<Statistics::Stochast>> stochastsMap;

            /**
             * \brief Mapping of standard normal stochasts to the design point they originate from
             * \remark If not present, the standard normal stochast is applicable to all design points
             */
            std::map<std::shared_ptr<Statistics::Stochast>, std::shared_ptr<Reliability::DesignPoint>> designPointsMap;

            /**
             * \brief Indicates whether a stochast is varying in one of the design points
             * \param stochast Stochast
             * \return Indication
             */
            bool isVarying(std::shared_ptr<Statistics::Stochast> stochast);

            /**
             * \brief Combines two alpha values
             * \param alpha1 First alpha value
             * \param alpha2 Second alpha value 
             * \return Combined alpha value
             */
            double getCombinedAlpha(double alpha1, double alpha2);
        };
    }
}

