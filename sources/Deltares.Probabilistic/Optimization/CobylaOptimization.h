#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "CobylaOptimizationSettings.h"
#include "../Model/Sample.h"

namespace Deltares
{
    namespace Optimization
    {
        class SearchDimension
        {
        public:
            double StartValue = 0.0;
            double LowerBound = -1e30;
            double UpperBound = 1e30;
        };

        class SearchArea
        {
        public:
            std::vector<SearchDimension> Dimensions;
        };

        class OptimizationSample
        {
        public:
            std::vector<double> Input;
            std::vector<double> Deviations;
            int numberOfSamples;
            double minimumValue;
            bool success;
        };

        class optimizationModel
        {
        public:
            virtual double GetZValue(const std::shared_ptr<Models::Sample> sample) const { return -1; };
            virtual double GetConstraintValue(const std::shared_ptr<Models::Sample> sample) const { return -1; };
            virtual unsigned GetNumberOfConstraints() const { return 0; };
        };

        /**
         * \brief Wrapper for Cobyla optimization algorithm
         * \note At this moment: this wrapper handles only zero or one constraints
         */
        class CobylaOptimization
        {
        public:
            CobylaOptimizationSettings settings;
            OptimizationSample GetCalibrationPoint(const SearchArea& searchArea, std::shared_ptr<optimizationModel> model) const;
        };

    }
}

