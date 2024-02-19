#include "importanceSamplingForm.h"

namespace Deltares {
    namespace ProbLibCore {

        importanceSamplingForm::importanceSamplingForm(const IMsettings& s1, const FormSettings& s2, randomForSampling& r, const progress& pg) :
            IMFlags(s1), FormFlags(s2), rnd(r), p(pg) {}

        importanceSamplingForm::importanceSamplingForm(const IMsettings& s1, const FormSettings& s2, randomForSampling& r, const size_t nrCores, const progress& pg) :
            IMFlags(s1), FormFlags(s2), rnd(r), numcores(nrCores), p(pg) {}

        lsfResult importanceSamplingForm::calc(const zmodel& m, const stochSettings& s)
        {
            auto bf = IMbuffer(IMFlags.chunkSize, s.getNrStochasts(), true);
            auto cm = ImportanceSampling(IMFlags, rnd, p, bf);
            auto l = cm.calc(m, s);

            if (l.convergence == ConvergenceStatus::abortedByUser) return l;

            auto f = form(FormFlags, l.u, p);
            auto l2 = f.calc(m, s);
            l.result.setAlpha(l2.result.getAlpha());
            l.x = l2.x;
            l.method = "CMIM_FORM";
            l.convergence = l2.convergence;
            return l;
        }
    }
}
