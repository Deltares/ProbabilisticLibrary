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
        };

        class optimizationModel
        {
        public:
            virtual double GetZValue(const std::shared_ptr<Models::Sample> sample) const { return -1; };
            virtual double GetConstraintValue(const std::shared_ptr<Models::Sample> sample) const { return -1; };
            virtual unsigned GetNumberOfConstraints() const { return 0; };
        };

        class CobylaOptimization
        {
        public:

            CobylaOptimizationSettings settings;
            OptimizationSample GetCalibrationPoint(SearchArea searchArea, std::shared_ptr<optimizationModel> model);
        private:
            double calcfcWrapper(int n, int m, const double x[], double con[], void* data);

        };

    }
}

