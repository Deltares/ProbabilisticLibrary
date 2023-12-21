#pragma once
#include <float.h>
#include "form_init.h"
#include "../rootFinders/root_finder.h"

namespace Deltares {
    namespace ProbLibCore {

        class sf_func_wrapper : public FuncWrapper
        {
        public:
            sf_func_wrapper(const std::vector<vector1D>& tasks, const zmodel& m, const stochSettings& s);
            virtual double GetValue(const double x);
            void getStats(size_t& evals, double& zmin, vector1D& u);
        private:
            const zmodel& model;
            const stochSettings& stoch;
            const std::vector<vector1D>& uTasks;
            vector1D uFinal;
            double zabsmin = DBL_MAX;
            size_t zEvaluations = 0;
        };

        class sphere_search : public form_init
        {
        public:
            sphere_search(const zmodel& m, const stochSettings& s);
            sphere_search(const zmodel& m, const stochSettings& s, const form_init_settings i);
            vector1D searchu(vector1D& m);
        };

        class sphere_searchAB : public form_init
        {
        public:
            sphere_searchAB(const zmodel& m, const stochSettings& s);
            sphere_searchAB(const zmodel& m, const stochSettings& s, const form_init_settings i);
            vector1D searchu(vector1D& m);
        };
    }
}
