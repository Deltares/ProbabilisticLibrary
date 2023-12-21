#include "sphere_search.h"
#include "sphere_tasks.h"
#include <cmath>
#include <vector>
#include <omp.h>
#include "../utils/basic_math.h"
#include "../utils/probLibException.h"
#include "../rootFinders/linear_loop.h"
#include "../rootFinders/Anderson_Bjork.h"

namespace Deltares {
    namespace ProbLibCore {

        sf_func_wrapper::sf_func_wrapper(const std::vector<vector1D>& tasks, const zmodel& m, const stochSettings& s) :
            model(m), stoch(s), uTasks(tasks) {};


        double sf_func_wrapper::GetValue(const double x)
        {
            const size_t nrStochasts = stoch.getNrStochasts();

            auto zi = std::vector<double>(uTasks.size());
            double zmin = DBL_MAX;

#pragma omp parallel for num_threads(model.numcores)
            for (int j = 0; j < (int)uTasks.size(); j++)
            {
                vector1D u = uTasks[j];
                u *= x;
                auto drw = probDraw(nrStochasts, u, stoch);
                auto z = model.zfunc(drw, omp_get_thread_num());
                zi[j] = z;
            }

            for (size_t j = 0; j < uTasks.size(); j++)
            {
                auto z = zi[j];
                if (z < zmin)
                {
                    zmin = z;
                }
                if (fabs(z) < zabsmin)
                {
                    uFinal = uTasks[j] * x;
                    zabsmin = fabs(z);
                }
                zEvaluations++;
                if (x == 0.0) break;
                if (z < 0.0)
                {
                    double coFactor = (x - 0.05) / x;
                    vector1D u = uTasks[j] * x;
                    u *= coFactor;
                    auto drw = probDraw(nrStochasts, u, stoch);
                    auto z2 = model.zfunc(drw, omp_get_thread_num());
                    z2 = min(z2, 0.0);
                    uFinal = u * (1.0 + z2 / (z2 - z) / coFactor);
                    break;
                }
            }
            return zmin;
        }

        void sf_func_wrapper::getStats(size_t& evals, double& zmin, vector1D& u)
        {
            evals = zEvaluations;
            zmin = zabsmin;
            u = uFinal;
        }

        sphere_search::sphere_search(const zmodel& m, const stochSettings& s) : form_init(m, s) {}

        sphere_search::sphere_search(const zmodel& m, const stochSettings& s, const form_init_settings i)
            : form_init(m, s, i) {}

        vector1D sphere_search::searchu(vector1D& u)
        {
            const size_t maxStepsR = (size_t)(2.0 / settings.du);
            double lengthU = sqrt(u.sumOfSquares());
            if (fabs(lengthU) < 1e-200) throw probLibException("Division by zero in sphere search");
            double radiusFactor = settings.maxuSphereSearch / lengthU;
            u *= radiusFactor;

            auto st = sphereTasks();
            auto tasks = st.examineSurfaceForTasks(u);

            auto func = sf_func_wrapper(tasks, mdl, stoch);
            auto rootFinder = linear_loop(func, 1e-4, maxStepsR - 1, ll_final_step::xminz, 1);
            double firstx = 1.0 / (double)maxStepsR;
            rootFinder.FindRoot(firstx, 1.0);
            vector1D uFinal;
            func.getStats(zEvaluations, zmin, uFinal);
            return uFinal;
        }

        sphere_searchAB::sphere_searchAB(const zmodel& m, const stochSettings& s) : form_init(m, s) {}

        sphere_searchAB::sphere_searchAB(const zmodel& m, const stochSettings& s, const form_init_settings i)
            : form_init(m, s, i) {}

        vector1D sphere_searchAB::searchu(vector1D& u)
        {
            const size_t maxStepsR = (size_t)(2.0 / settings.du);
            double radiusFactor = settings.maxu / sqrt(u.sumOfSquares());
            u *= radiusFactor;

            auto st = sphereTasks();
            auto tasks = st.examineSurfaceForTasks(u);

            auto func = sf_func_wrapper(tasks, mdl, stoch);
            auto rootFinder = Anderson_Bjork(func);
            double firstx = (settings.ABstartZero ? 0.0 : 1.0 / (double)maxStepsR);
            auto rt = rootFinder.FindRoot(firstx, 1.0);
            vector1D uFinal;
            func.getStats(zEvaluations, zmin, uFinal);
            return (rt.first ? uFinal : vector1D(0));
        }
    }
}
