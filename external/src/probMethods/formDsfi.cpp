#include "formDsfi.h"

namespace Deltares {
    namespace ProbLibCore {

        formDsfi::formDsfi(const DSsettings& s1, const FormSettings& s2, randomForSampling& r, dsRootFinder& f, const progress& pg)
            : DSFlags(s1), FormFlags(s2), rnd(r), p(pg), rtf(f) {}

        formDsfi::formDsfi(const DSsettings& s1, const FormSettings& s2, randomForSampling& r, dsRootFinder& f, const size_t nrCores, const progress& pg)
            : DSFlags(s1), FormFlags(s2), rnd(r), numcores(nrCores), p(pg), rtf(f) {}

        lsfResult formDsfi::calc(const zmodel& m, const stochSettings& s)
        {
            auto f = form(FormFlags, p);
            auto fresult = f.calc(m, s);

            if (fresult.convergence == ConvergenceStatus::noConvergence)
            {
                auto ds = dsfi(DSFlags, FormFlags, rnd, rtf, numcores, p);
                auto dsresult = ds.calc(m, s);
                dsresult.method = "FORM_DSFI";
                return dsresult;
            }
            else
            {
                fresult.method = "FORM_DSFI";
                return fresult;
            }
        }

        formDsfiHR::formDsfiHR(const DSsettings& s1, const FormSettings& s2, randomForSampling& r, dsRootFinder& f, const progress& pg)
            : DSFlags(s1), FormFlags(s2), rnd(r), p(pg), rtf(f) {}

        formDsfiHR::formDsfiHR(const DSsettings& s1, const FormSettings& s2, randomForSampling& r, dsRootFinder& f, const size_t nrCores, const progress& pg)
            : DSFlags(s1), FormFlags(s2), rnd(r), numcores(nrCores), p(pg), rtf(f) {}

        lsfResult formDsfiHR::calc(const zmodel& m, const stochSettings& s)
        {
            auto f = form(FormFlags, p);
            auto fresult = f.calc(m, s);

            if (fresult.convergence == ConvergenceStatus::noConvergence)
            {
                auto ds = dsfiHR(DSFlags, FormFlags, rnd, rtf, numcores, p);
                auto dsresult = ds.calc(m, s);
                dsresult.method = "FORM_DSFI_compatible";
                return dsresult;
            }
            else
            {
                fresult.method = "FORM_DSFI_compatible";
                return fresult;
            }
        }
    }
}
