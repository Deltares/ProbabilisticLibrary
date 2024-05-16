#pragma once

#include <map>
#include <vector>
#include <memory>

#include "SearchParameterSettingsSet.h"
#include "../Model/ModelSample.h"
#include "../Model/ModelRunner.h"

namespace Deltares
{
	namespace Optimization
	{
		class GridSearch
		{
		public:
			/**
			 * \brief Maximum number of grid moves to be performed
			 */
			int MaxGridMoves = 50;

			/**
			 * \brief Finds the parameter combination which results in the minimum value
			 * \param searchArea Definition of parameter space and settings which will be searched
			 * \param model Model to invoke, the minimum z-value will be used
			 * \return Sample containing values which lead to he minimum value
			 */
			std::shared_ptr<Models::ModelSample> getOptimizedSample(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::shared_ptr<Models::ZModel> model);

		private:
			/**
			 * \brief Finds the parameter combination which results in the minimum value
			 * \param searchArea Definition of parameter space and settings which will be searched
			 * \param model Model to invoke, the minimum z-value will be used
			 * \param minSample The minimum sample found from previous iterations (nullptr if initial)
			 * \return Sample containing values which lead to he minimum value (if no sample leading to a lower value is found, minSample will be returned)
			 */
			std::shared_ptr<Models::ModelSample> findGridExtreme(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::shared_ptr<Models::ZModel> model, std::shared_ptr<Models::ModelSample> minSample, int iteration);

			/**
			 * \brief Indicates whether a sample is located on the edge of the search area
			 * \param searchArea Definition of parameter space and settings which will be searched
			 * \param sample Sample
			 * \return Indication
			 */
			bool isSampleOnEdge(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::shared_ptr<Models::ModelSample> sample);

			/**
			 * \brief Moves the search area n such a way that the sample is not on the edge any more
			 * \param searchArea Definition of parameter space and settings which will be searched
			 * \param sample Sample on edge
			 */
			void moveSampleToCenter(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::shared_ptr<Models::ModelSample> sample);

			/**
			 * \brief Indicates whether refinement is possible
			 * \param searchArea Definition of parameter space and settings which will be searched
			 * \param refinements Number of refinements performed so far
			 * \return Indication
			 */
			bool canRefine(std::shared_ptr<SearchParameterSettingsSet> searchArea, int refinements);

			/**
			 * \brief Refines the grid around the currently found minimum sample
			 * \param searchArea Definition of parameter space and settings which will be searched
			 * \param refinements Number of refinements performed so far
			 * \param sample Minimum sample
			 */
			void refineGrid(std::shared_ptr<SearchParameterSettingsSet> searchArea, int refinements, std::shared_ptr<Models::ModelSample> sample);

			/**
			 * \brief Gets the tolerance for a parameter when determining whether a value is on the edge of a grid 
			 * \param dimension Settings of the parameter
			 * \return Tolerance
			 */
			double getTolerance(std::shared_ptr<SearchParameterSettings> dimension);

        private:

            int counter = 0;
            int reusedCounter = 0;

		};
	}
}
