#include "CobylaOptimization.h"
#include "cobyla.h"

namespace Deltares
{
    namespace Optimization
    {

        OptimizationSample CobylaOptimization::GetCalibrationPoint(SearchArea searchArea, optimizationModel model)
        {
            unsigned n = static_cast<unsigned>(searchArea.Dimensions.size());
            unsigned m = 5; // TODO

            double* x0 = new double[n];
            for (int i = 0 ; i < n; i++)
            {
                x0[i] = searchArea.Dimensions[i].StartValue;
            }
            void* funcData = new long[999];

            nlopt_func f = nullptr;
            nlopt_constraint* fc = nullptr;
            nlopt_constraint* h = nullptr;
            double* lb = new double[n];
            double* ub = new double[n];
            double* minf = new double[n];
            nlopt_stopping* stop = nullptr;
            double dx = 0.1;
            unsigned p = 0;

            auto status = cobyla_minimize(n, f, funcData, m, fc, p, h, lb, ub, x0, minf, stop, &dx);

            OptimizationSample s;
            return s;
        };

    }
}


