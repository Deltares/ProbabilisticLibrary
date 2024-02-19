#include "form_init.h"

namespace Deltares {
    namespace ProbLibCore {

        form_init::form_init(const zmodel& m, const stochSettings& s) : mdl(m), stoch(s) {}

        form_init::form_init(const zmodel& m, const stochSettings& s, const form_init_settings i)
            : mdl(m), stoch(s), settings(i) {}

        vector1D form_init::search()
        {
            const size_t nrStochasts = stoch.getNrStochasts();
            auto dir = vector1D(nrStochasts);
            dir.assign(1.0);
            return searchu(dir);
        }

        vector1D form_init::search(const std::initializer_list<double>& m)
        {
            auto dir = vector1D(m);
            return searchu(dir);
        }

        void form_init::getStats(double& z, size_t& i)
        {
            z = zmin;
            i = zEvaluations;
        }
    }
}
