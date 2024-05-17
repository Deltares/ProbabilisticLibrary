#pragma once

#include <vector>

#include "Sample.h"
#include "ModelSample.h"
#include "StochastPointAlpha.h"

namespace Deltares
{
	namespace Models
	{
        /**
         * \brief Defines a point in the parameter space
         */
        class StochastPoint
		{
		public:
			double Beta;
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
             * \brief Gets the alpha value for a given stochast
             * \param stochast Given stochast
             * \return Alpha value, nullptr if not found
             */
            std::shared_ptr<StochastPointAlpha> getAlpha(std::shared_ptr<Statistics::Stochast> stochast);

            /**
             * \brief Updates the influence factors in the alpha values
             */
            void updateInfluenceFactors();

			virtual ~StochastPoint() { ; }
		};
	}
}

