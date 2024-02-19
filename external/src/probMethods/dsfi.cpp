#include "dsfi.h"
#include "dsRootFinderRobust.h"

namespace Deltares {
    namespace ProbLibCore {

        dsfi::dsfi(DSsettings s1, FormSettings s2, randomForSampling& r, dsRootFinder& f, const progress& pg)
            : FormFlags(s2), DSFlags(s1), rnd(r), p(pg), rtf(f) {}

        dsfi::dsfi(DSsettings s1, FormSettings s2, randomForSampling& r, dsRootFinder& f, const size_t nrCores, const progress& pg)
            : FormFlags(s2), DSFlags(s1), rnd(r), numcores(nrCores), p(pg), rtf(f) {}

        lsfResult dsfi::calc(const zmodel& m, const stochSettings& s)
        {
            m.logInfo(1);
            auto ds = directionalSampling(DSFlags, rnd, rtf, numcores, p);
            auto dsresult = ds.calc(m, s);

            if (dsresult.convergence == ConvergenceStatus::abortedByUser) return dsresult;

            m.logInfo(2);
            auto f = form(FormFlags, dsresult.u, p);
            auto fresult = f.calc(m, s);

            dsresult.result.setAlpha(fresult.result.getAlpha());
            dsresult.x = fresult.x;
            dsresult.method = "DSFI";
            dsresult.stepsNeeded = fresult.stepsNeeded;
            return dsresult;
        }

        dsfiHR::dsfiHR(DSsettings s1, FormSettings s2, randomForSampling& r, dsRootFinder& f, const progress& pg)
            : FormFlags(s2), DSFlags(s1), rnd(r), p(pg), rtf(f) {}

        dsfiHR::dsfiHR(DSsettings s1, FormSettings s2, randomForSampling& r, dsRootFinder& f, const size_t nrCores, const progress& pg)
            : FormFlags(s2), DSFlags(s1), rnd(r), numcores(nrCores), p(pg), rtf(f) {}

        lsfResult dsfiHR::calc(const zmodel& m, const stochSettings& s)
        {
            m.logInfo(1);
            auto ds = directionalSampling(DSFlags, rnd, rtf, numcores, p);
            auto dsresult = ds.calc(m, s);

            if (dsresult.convergence == ConvergenceStatus::abortedByUser) return dsresult;

            m.logInfo(2);
            vector1D start = dsresult.result.getAlpha() * -dsresult.result.getBeta();
            auto f = form(FormFlags, start, p);
            auto fresult = f.calc(m, s);

            dsresult.result.setAlpha(fresult.result.getAlpha());
            dsresult.x = fresult.x;
            dsresult.method = "DSFIcompatible";
            dsresult.stepsNeeded = fresult.stepsNeeded;
            return dsresult;
        }
    }
}
