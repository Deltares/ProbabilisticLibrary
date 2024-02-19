#include "crudeMonteCarloForm.h"

namespace Deltares {
    namespace ProbLibCore {

        crudeMonteCarloForm::crudeMonteCarloForm(const CMsettings& s1, const FormSettings& s2, randomForSampling& r, const progress& pg) :
            CMFlags(s1), FormFlags(s2), rnd(r), p(pg) {}

        crudeMonteCarloForm::crudeMonteCarloForm(const CMsettings& s1, const FormSettings& s2, randomForSampling& r, const size_t nrCores, const progress& pg) :
            CMFlags(s1), FormFlags(s2), rnd(r), numcores(nrCores), p(pg) {}

        lsfResult crudeMonteCarloForm::calc(const zmodel& m, const stochSettings& s)
        {
            auto cm = crudeMonteCarlo(CMFlags, rnd, p);
            auto l = cm.calc(m, s);

            if (l.convergence == ConvergenceStatus::abortedByUser) return l;

            auto f = form(FormFlags, l.u, p);
            auto l2 = f.calc(m, s);
            l.result.setAlpha(l2.result.getAlpha());
            l.x = l2.x;
            l.method = "CMC_FORM";
            l.convergence = l2.convergence;
            return l;
        }
    }
}
