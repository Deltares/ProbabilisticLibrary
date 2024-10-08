#pragma once
#include <vector>
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
            double StartValue;
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
        };

        class optimizationModel
        {
        public:
            double GetZValue(const Models::Sample& sample);
        };

        class CobylaOptimization
        {
        public:

            CobylaOptimizationSettings settings;
            OptimizationSample GetCalibrationPoint(SearchArea searchArea, optimizationModel model);
        private:
            double calcfcWrapper(int n, int m, const double x[], double con[], void* data);

        };

    }
}

