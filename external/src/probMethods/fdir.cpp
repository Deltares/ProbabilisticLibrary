#include "fdir.h"
#include "dsRootFinderRobust.h"

namespace Deltares {
    namespace ProbLibCore {

        fdir::fdir(DSsettings s1, FormSettings s2, randomForSampling& r, dsRootFinder& f, const progress& pg)
            : FormFlags(s2), DSFlags(s1), rnd(r), p(pg), rtf(f) {}

        fdir::fdir(DSsettings s1, FormSettings s2, randomForSampling& r, dsRootFinder& f, vector1D& u, const progress& pg)
            : FormFlags(s2), DSFlags(s1), rnd(r), startVector(u), p(pg), rtf(f) {}

        fdir::fdir(DSsettings s1, FormSettings s2, randomForSampling& r, dsRootFinder& f, const size_t nrCores, const progress& pg)
            : FormFlags(s2), DSFlags(s1), rnd(r), numcores(nrCores), p(pg), rtf(f) {}

        fdir::fdir(DSsettings s1, FormSettings s2, randomForSampling& r, dsRootFinder& f, vector1D& u, const size_t nrCores, const progress& pg)
            : FormFlags(s2), DSFlags(s1), rnd(r), numcores(nrCores), startVector(u), p(pg), rtf(f) {}

        lsfResult fdir::calc(const zmodel& m, const stochSettings& s)
        {
            m.logInfo(1);
            auto f = form(FormFlags, startVector, p);
            auto fresult = f.calc(m, s);

            if (fresult.convergence == ConvergenceStatus::noConvergence || !acceptableBeta(fresult.result.getBeta()))
            {
                m.logInfo(2);
                auto ds = directionalSampling(DSFlags, rnd, rtf, numcores, p);
                auto dsresult = ds.calc(m, s);
                dsresult.stepsNeeded = fresult.stepsNeeded;
                return dsresult;
            }
            else
            {
                return fresult;
            }
        }

        bool fdir::acceptableBeta(const double beta)
        {
            return (beta >= FDthreshold && beta < betaMax);
        }
    }
}
