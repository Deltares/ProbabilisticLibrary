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
            unsigned m = 0; // TODO

            double* x0 = new double[n];
            double* lb = new double[n];
            double* ub = new double[n];
            double* dx = new double[n];
            for (int i = 0 ; i < n; i++)
            {
                x0[i] = searchArea.Dimensions[i].StartValue;
                lb[i] = searchArea.Dimensions[i].LowerBound;
                ub[i] = searchArea.Dimensions[i].UpperBound;
                dx[i] = 0.1;
            }
            long long fData;
            void* funcData = &fData;

            auto myfunc = [model](unsigned n, const double* x, double* gradient, void* func_data)
            {
                auto s = Models::Sample(n);
                for (int i = 0; i < n; i++)
                {
                    s.Values[i] = x[i];
                }
                return model->GetZValue(s);
            };

            nlopt_func f = myfunc;
            nlopt_constraint* fc = new nlopt_constraint[m];
            nlopt_constraint* h = new nlopt_constraint[m];
            double* minf = new double[n];
            nlopt_stopping stop = nlopt_stopping();
            stop.nevals_p = new int;
            stop.xtol_rel = 0.001;
            unsigned p = 0;

            auto status = cobyla_minimize(n, f, funcData, m, fc, p, h, lb, ub, x0, minf, &stop, dx);

            OptimizationSample s;
            for (int i = 0; i < n; i++)
            {
                s.Input.push_back(x0[i]);
            }
            delete[] x0; delete[] lb; delete[] ub;
            delete[] fc; delete[] h; delete[] dx; delete[] minf;
            delete stop.nevals_p;
            return s;
        };

    }
}


