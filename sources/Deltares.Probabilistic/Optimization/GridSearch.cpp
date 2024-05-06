#include "GridSearch.h"

namespace Deltares
{
	namespace Optimization
	{
		std::shared_ptr<Models::ModelSample> GridSearch::getOptimizedSample(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::shared_ptr<Models::ZModel> model)
		{
			std::shared_ptr<Models::ModelSample> sample = findGridExtreme(searchArea, model, nullptr, 0);
			counter = 0;
			reusedCounter = 0;

			if (sample != nullptr)
			{
				int gridMoves = 0;
				while (gridMoves < MaxGridMoves && isSampleOnEdge(searchArea, sample))
				{
					moveSampleToCenter(searchArea, sample);
					sample = findGridExtreme(searchArea, model, sample, 1 + gridMoves);
					gridMoves++;

					counter = 0;
					reusedCounter = 0;
				}

				int refinements = 0;
				while (canRefine(searchArea, refinements))
				{
					refineGrid(searchArea, refinements, sample);
					sample = findGridExtreme(searchArea, model, sample, 1 + gridMoves + refinements);
					refinements++;

					counter = 0;
					reusedCounter = 0;
				}
			}

			return sample;
		}

		std::shared_ptr<Models::ModelSample> GridSearch::findGridExtreme(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::shared_ptr<Models::ZModel> model, std::shared_ptr<Models::ModelSample> minSample, int iteration)
		{
			std::vector<std::vector<double>> inputValues;
			for (std::shared_ptr<SearchParameterSettings> dimension : searchArea->Dimensions)
			{
				inputValues.push_back(std::vector<double>(dimension->getValues()));
			}

			std::vector<std::vector<double>> combinations = Numeric::NumericSupport::getFullFactorialCombination(inputValues);

			if (combinations.empty())
			{
				throw Reliability::probLibException("No dimensions or empty dimensions are not allowed");
			}

			int gridCounter = 0;
			int gridIntervalCounter = 0;

			for (auto combination : combinations)
			{
				gridCounter++;
				gridIntervalCounter++;

				const std::shared_ptr<Models::ModelSample> sample = std::make_shared<Models::ModelSample>(combination);

				model->invoke(sample);

				counter++;

				if (!std::isnan(sample->Z) && (minSample == nullptr || sample->Z < minSample->Z))
				{
					minSample = sample;
				}
			}

			return minSample;
		}

		bool GridSearch::isSampleOnEdge(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::shared_ptr<Models::ModelSample> sample)
		{
			for (int i = 0; i < searchArea->Dimensions.size(); i++)
			{
				if (searchArea->Dimensions[i]->Move && searchArea->Dimensions[i]->NumberOfValues > 2)
				{
					const double tolerance = getTolerance(searchArea->Dimensions[i]);

					if (Numeric::NumericSupport::areEqual(searchArea->Dimensions[i]->MinValue, sample->Values[i], tolerance) || Numeric::NumericSupport::areEqual(searchArea->Dimensions[i]->MaxValue, sample->Values[i], tolerance))
					{
						return true;
					}
				}
			}

			return false;
		}

		void GridSearch::moveSampleToCenter(std::shared_ptr<SearchParameterSettingsSet> searchArea, std::shared_ptr<Models::ModelSample> sample)
		{
			bool moved = false;

			for (int i = 0; i < searchArea->Dimensions.size(); i++)
			{
				std::shared_ptr<SearchParameterSettings> dimension = searchArea->Dimensions[i];

				if (!moved && dimension->Move && dimension->NumberOfValues > 2)
				{
					double shift = dimension->getInterval();

					const double tolerance = getTolerance(searchArea->Dimensions[i]);

					if (Numeric::NumericSupport::areEqual(dimension->MinValue, sample->Values[i], tolerance))
					{
						dimension->MinValue -= shift;
						dimension->MaxValue -= shift;
						dimension->UseValues = UseValuesType::MinValue;
						moved = true;
					}
					else if (Numeric::NumericSupport::areEqual(dimension->MaxValue, sample->Values[i], tolerance))
					{
						dimension->MinValue += shift;
						dimension->MaxValue += shift;
						dimension->UseValues = UseValuesType::MaxValue;
						moved = true;
					}
					else
					{
						dimension->UseValues = UseValuesType::AllValues;
					}
				}
				else
				{
					dimension->UseValues = UseValuesType::AllValues;
				}
			}
		}

		bool GridSearch::canRefine(std::shared_ptr<SearchParameterSettingsSet> searchArea, int refinements)
		{
			for (size_t i = 0; i < searchArea->Dimensions.size(); i++)
			{
				if (refinements < searchArea->Dimensions[i]->NumberOfRefinements)
				{
					return true;
				}
			}

			return false;
		}

		void GridSearch::refineGrid(std::shared_ptr<SearchParameterSettingsSet> searchArea, int refinements, std::shared_ptr<Models::ModelSample> sample)
		{
			for (size_t i = 0; i < searchArea->Dimensions.size(); i++)
			{
				std::shared_ptr<SearchParameterSettings> dimension = searchArea->Dimensions[i];
				if (refinements < dimension->NumberOfRefinements)
				{
					// when refinement is allowed, create values for the new grid higher, lower and equal the original sample
					// the values higher and lower are exactly between the sample and its neighbors in the previous grid
					double newInterval = dimension->getInterval() / 2; 
					dimension->MinValue = sample->Values[i] - newInterval;
					dimension->MaxValue = sample->Values[i] + newInterval;
					dimension->NumberOfValues = 3;
				}
				else
				{
					// when refinement is not allowed, repeat the value from the original value
					dimension->MinValue = sample->Values[i];
					dimension->MaxValue = sample->Values[i];
					dimension->NumberOfValues = 1;
				}

				dimension->UseValues = UseValuesType::AllValues;
			}
		}

		double GridSearch::getTolerance(std::shared_ptr<SearchParameterSettings> dimension)
		{
			if (dimension->NumberOfValues > 0) 
			{
				return std::abs((dimension->MaxValue - dimension->MinValue)) / (10 * dimension->NumberOfValues);
			}
			else 
			{
				return 0;
			}
		}
	}
}
