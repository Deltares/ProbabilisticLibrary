#include "pch.h"
#include "Deltares.Reliability.Wrapper.h"
#include "../../src/probMethods/crudeMonteCarlo.h"
#include "../../src/probFuncs/stochSettings.h"
#include "../../src/distributions/parseDistribution.h"
#include "../../src/correlation/identity.h"
#include "../../src/probMethods/progress.h"

namespace Deltares {
    namespace Reliability {
        namespace Wrapper {

            void rndDotNet::getRandomVector(vector1D& r)
            {
                auto nrStoch = mr->VaryingStochastCount;
                auto s = mr->GetRandomSample(nrStoch);
                for (size_t i = 0; i < nrStoch; i++)
                {
                    r(i) = s->Values[i];
                }
            }

            double zmodelDotNet::zfunc(const probDraw& d) const
            {
                auto nrStoch = mr->VaryingStochastCount;
                auto s = mr->GetEmptySample(nrStoch);
                for (size_t i = 0; i < nrStoch; i++)
                {
                    auto y = d.getx(i);
                    s->Values[i] = y;
                }
                auto z = mr->GetZValue(s);
                return z;
            }

            double CrudeMC::getDesignPoint(ModelRunner ^ m)
            {
                auto nrStoch = m->VaryingStochastCount;
                auto cm = CMsettings();
                cm.maximumSamples  = mcSettings->MaximumSamples;
                cm.minimumSamples  = mcSettings->MinimumSamples;
                cm.varCoeffFailure = mcSettings->VariationCoefficientFailure;
                auto rnd = rndDotNet(m);
                auto p = progress();
                auto mc = new crudeMonteCarlo(cm, rnd, p);
                auto parser = parseDistribution();
                auto d = listDistribs(nrStoch);
                for (size_t i = 0; i < nrStoch; i++)
                {
                    std::vector<double> params = { 0 };
                    auto nwdist = EnumDistributions::uspace;
                    d.addDistrib(parser.parse(nwdist, "par", params));
                }
                auto c = identity(nrStoch);
                auto zmodel = zmodelDotNet(m);
                auto stochasts = stochSettings(c, d);
                auto result = mc->calc(zmodel, stochasts);
                return result.result.getBeta();
            }
        }
    }
}

