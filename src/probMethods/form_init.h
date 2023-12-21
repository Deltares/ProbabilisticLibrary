#pragma once
#include "../utils/vector1D.h"
#include "zmodel.h"
#include "../probFuncs/stochSettings.h"

namespace Deltares {
    namespace ProbLibCore {

        struct form_init_settings
        {
            double du = 0.1;
            double maxu = 18.1;
            double maxuSphereSearch = 10.0;
            bool ABstartZero = false;
        };

        class form_init
        {
        public:
            form_init(const zmodel& m, const stochSettings& s);
            form_init(const zmodel& m, const stochSettings& s, const form_init_settings i);
            vector1D search();
            vector1D search(const std::initializer_list<double>& m);
            virtual vector1D searchu(vector1D& m) = 0;
            void getStats(double& z, size_t& i);
        protected:
            const zmodel& mdl;
            const stochSettings& stoch;
            const form_init_settings settings;
            double zmin;
            size_t zEvaluations;
        };
    }
}
