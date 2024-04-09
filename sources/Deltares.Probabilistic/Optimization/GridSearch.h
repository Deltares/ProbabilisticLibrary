#pragma once

#include <map>
#include <vector>
#include <memory>

#include "SearchParameterSettings.h"
#include "SearchParameterSettingsSet.h"
#include "../Model/ModelSample.h"
#include "../Model/ModelRunner.h"

namespace Deltares
{
	namespace Optimization
	{
		class GridSearch
		{
		private:

			static constexpr double tolerance = 1E-6;

			int counter = 0;
			int reusedCounter = 0;

		public:
			int MaxGridMoves = 50;

			/// <summary>
			/// Finds the extreme 
			/// </summary>
			/// <param name="searchArea">The area where the searching takes place</param>
			/// <param name="model">Method which produces the model result</param>
			std::shared_ptr<Models::ModelSample> getOptimizedSample(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::shared_ptr<Models::ZModel> model);

		private:
			/// <summary>
			/// Finds the extreme 
			/// </summary>
			/// <param name="searchArea"></param>
			/// <param name="model"></param>
			/// <param name="minSample">The minimum sample found so far</param>
			/// <param name="iteration"></param>
			/// <returns>Sample with extreme model value</returns>
			std::shared_ptr<Models::ModelSample> findGridExtreme(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::shared_ptr<Models::ZModel> model, std::shared_ptr<Models::ModelSample> minSample, int iteration);

			/// <summary>
			/// Indicates whether a sample is located on the edge of the search area
			/// </summary>
			/// <param name="searchArea"></param>
			/// <param name="sample"></param>
			/// <returns></returns>
			bool isSampleOnEdge(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::vector<double>& sample);

			/// <summary>
			/// Moves the center of the search area to the sample location
			/// </summary>
			/// <param name="searchArea"></param>
			/// <param name="sample"></param>
			void moveSampleToCenter(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::vector<double>& sample);

			/// <summary>
			/// Indicates whether refinement is possible
			/// </summary>
			/// <param name="searchArea"></param>
			/// <param name="refinements"></param>
			/// <returns></returns>
			bool canRefine(std::shared_ptr<SearchParameterSettingsSet> searchArea, int refinements);

			/// <summary>
			/// Refines the grid around the currently found minimum
			/// </summary>
			/// <param name="searchArea"></param>
			/// <param name="refinements"></param>
			/// <param name="input"></param>
			void refineGrid(std::shared_ptr<SearchParameterSettingsSet> searchArea, int refinements, std::vector<double>& input);
		};
	}
}
