// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include <vector>

#include "Sample.h"
#include "ModelSample.h"
#include "StochastPointAlpha.h"

#include "../Statistics/BaseStochastPoint.h"

namespace Deltares
{
    namespace Models
    {
        /**
         * \brief Defines a point in the parameter space
         */
        class StochastPoint : public Statistics::BaseStochastPoint
        {
        public:
            std::vector<std::shared_ptr<StochastPointAlpha>> Alphas;

            /**
             * \brief Gets a sample corresponding with the alpha values (u-value) of this stochast point
             * \return Sample
             */
            std::shared_ptr<Sample> getSample();

            /**
             * \brief Gets a model sample corresponding with the alpha values (x-value) of this stochast point
             * \return Model sample
             */
            std::shared_ptr<ModelSample> getModelSample();

            /**
             * \brief Gets a sample corresponding with the alpha values (u-value) of this stochast point, but with values only for specified stochasts
             * \param stochasts Specified stochasts
             * \return Sample
             */
            std::shared_ptr<Sample> getSampleForStochasts(std::vector<std::shared_ptr<Statistics::Stochast>> stochasts);

            /**
             * \brief Gets the alpha values for a given set of stochasts
             * \param stochasts Given set of stochasts
             * \return Alpha values, nullptr if not found
             */
            std::vector<std::shared_ptr<StochastPointAlpha>> getAlphas(const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts) const;

            /**
             * \brief Gets the alpha value for a given stochast
             * \param stochast Given stochast
             * \return Alpha value, nullptr if not found
             */
            std::shared_ptr<StochastPointAlpha> getAlpha(std::shared_ptr<Statistics::Stochast> stochast);

            /**
             * \brief Gets a copy at a given beta
             * \param beta Beta
             * \param alphaMargin Minimum alpha value to be copied
             */
            std::shared_ptr<StochastPoint> getCopy(double beta, double alphaMargin = 1E-8);

            /**
             * \brief Updates the influence factors in the alpha values
             */
            void updateInfluenceFactors();

            virtual ~StochastPoint() { }

        };
    }
}

