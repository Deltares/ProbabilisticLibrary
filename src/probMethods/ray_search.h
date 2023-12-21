#pragma once
#include "form_init.h"
#include "../rootFinders/root_finder.h"

namespace Deltares {
    namespace ProbLibCore {

        class rs_func_wrapper : public FuncWrapper
        {
        public:
            rs_func_wrapper(vector1D& d, const zmodel& m, const stochSettings& s, const int threadId);
            void updateDir(vector1D& d);
            virtual double GetValue(const double x);
            virtual double GetValue(const double x, const int threadId);
        private:
            vector1D& dir;
            vector1D xx;
            const zmodel& model;
            const stochSettings& stoch;
            const int thread;
        };

        class ray_search : public form_init
        {
        public:
            ray_search(const zmodel& m, const stochSettings& s, const int numCores);
            ray_search(const zmodel& m, const stochSettings& s, const form_init_settings i);
            virtual vector1D searchu(vector1D& m);
        private:
            const int numCores;
        };

        class ray_searchAB : public form_init
        {
        public:
            ray_searchAB(const zmodel& m, const stochSettings& s);
            ray_searchAB(const zmodel& m, const stochSettings& s, const form_init_settings i);
            virtual vector1D searchu(vector1D& m);
        };
    }
}
