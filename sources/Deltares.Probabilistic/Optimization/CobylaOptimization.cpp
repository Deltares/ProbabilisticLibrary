#include "CobylaOptimization.h"
#include "cobyla.h"
#include <functional>

namespace Deltares
{
    namespace Optimization
    {

        OptimizationSample CobylaOptimization::GetCalibrationPoint(SearchArea searchArea, std::shared_ptr<optimizationModel> model)
        {
            unsigned n = static_cast<unsigned>(searchArea.Dimensions.size());
            unsigned m = 5; // TODO

            double* x0 = new double[n];
            double* lb = new double[n];
            double* ub = new double[n];
            for (int i = 0 ; i < n; i++)
            {
                x0[i] = searchArea.Dimensions[i].StartValue;
                lb[i] = searchArea.Dimensions[i].LowerBound;
                ub[i] = searchArea.Dimensions[i].UpperBound;
            }
            long fData[999];
            void* funcData = &fData;

            //auto myfunc1 = [model](Models::Sample s) {return model->GetZValue(s); };
            auto myfunc = [model](unsigned n, const double* x,
                double* gradient, 
                void* func_data) {return model->GetZValue(x); };

            nlopt_func f = myfunc;
            nlopt_constraint* fc = new nlopt_constraint[m];
            nlopt_constraint* h = new nlopt_constraint[m];
            double* minf = new double[n];
            nlopt_stopping stop = nlopt_stopping();
            double dx = 0.1;
            unsigned p = 0;

            auto status = cobyla_minimize(n, f, funcData, m, fc, p, h, lb, ub, x0, minf, &stop, &dx);

            delete[] x0; delete[] lb; delete[] ub;
            delete[] fc; delete[] h;
            OptimizationSample s;
            return s;
        };

    }
}


