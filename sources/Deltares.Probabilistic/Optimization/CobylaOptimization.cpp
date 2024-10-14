#include "CobylaOptimization.h"
#include "Cobyla.h"
#include <functional>

namespace Deltares
{
    namespace Optimization
    {
        OptimizationSample CobylaOptimization::GetCalibrationPoint(const SearchArea& searchArea, std::shared_ptr<optimizationModel> model) const
        {
            unsigned n = static_cast<unsigned>(searchArea.Dimensions.size());
            unsigned m = model->GetNumberOfConstraints();

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
                auto s = std::make_shared<Models::Sample>(n);
                for (int i = 0; i < n; i++)
                {
                    s->Values[i] = x[i];
                }
                return model->GetZValue(s);
            };

            auto myfuncC = [model](unsigned n, const double* x, double* gradient, void* func_data)
                {
                    auto s = std::make_shared<Models::Sample>(n);
                    for (int i = 0; i < n; i++)
                    {
                        s->Values[i] = x[i];
                    }
                    return model->GetConstraintValue(s);
                };

            nlopt_constraint* fc = new nlopt_constraint[m];
            if (m > 0)
            {
                fc[0].f = myfuncC;
                fc[0].m = 1;
                fc[0].tol = new double[1];
                fc[0].tol[0] = settings.EpsilonBeta;
            }
            nlopt_constraint* h = new nlopt_constraint[0];
            double* minf = new double[1];
            nlopt_stopping stop = nlopt_stopping();
            stop.nevals_p = new int;
            stop.xtol_rel = settings.EpsilonBeta;
            stop.maxeval = settings.MaxIterations;
            unsigned p = 0;

            auto status = cobyla_minimize(n, myfunc, funcData, m, fc, p, h, lb, ub, x0, minf, &stop, dx);

            OptimizationSample s;
            s.numberOfSamples = *stop.nevals_p;
            s.minimumValue = *minf;
            switch (status)
            {
            case NLOPT_SUCCESS:
            case NLOPT_STOPVAL_REACHED:
            case NLOPT_FTOL_REACHED:
            case NLOPT_XTOL_REACHED:
                s.success = true;
                break;
            default:
                s.success = false;
                break;
            }

            for (int i = 0; i < n; i++)
            {
                s.Input.push_back(x0[i]);
            }
            delete[] x0; delete[] lb; delete[] ub;
            delete[] dx; delete[] minf;
            delete stop.nevals_p;
            for(int i = 0; i < m; i++)
            {
                delete[] fc[i].tol;
            }
            delete[] fc; delete[] h;
            return s;
        };

    }
}


