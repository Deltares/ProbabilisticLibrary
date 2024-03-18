#include <gtest/gtest.h>
#include "projectBuilder.h"
#include <iostream>

using namespace Deltares::Reliability;
using namespace Deltares::Statistics;
using namespace Deltares::Models;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            std::shared_ptr<ModelRunner> projectBuilder::BuildProject()
            {
                std::shared_ptr<ZModel> z(new ZModel(ZModel([this](std::shared_ptr<ModelSample> v) { return zfunc(v); })));
                auto stochast = std::vector<std::shared_ptr<Stochast>>();
                auto dist = DistributionType::Normal;
                std::vector<double> params{ 0.0, 1.0 };
                std::shared_ptr<Stochast> s(new Stochast(dist, params));
                stochast.push_back(s);
                stochast.push_back(s);
                std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
                std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
                uConverter->initializeForRun();
                std::shared_ptr<ModelRunner> m(new ModelRunner(z, uConverter));
                return m;
            }

            std::shared_ptr<ModelRunner> projectBuilder::BuildProjectWithDeterminist()
            {
                std::shared_ptr<ZModel> z(new ZModel(ZModel([this](std::shared_ptr<ModelSample> v) { return zfuncWithDeterminist(v); })));
                auto stochast = std::vector<std::shared_ptr<Stochast>>();
                auto dist = DistributionType::Normal;
                std::vector<double> params{ 0.0, 1.0 };
                std::shared_ptr<Stochast> s(new Stochast(dist, params));
                std::shared_ptr<Stochast> determinist(new Stochast(DistributionType::Deterministic, {2.6}));
                stochast.push_back(s);
                stochast.push_back(determinist);
                stochast.push_back(s);
                std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
                std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
                uConverter->initializeForRun();
                std::shared_ptr<ModelRunner> m(new ModelRunner(z, uConverter));
                return m;
            }

            void projectBuilder::zfunc(std::shared_ptr<Deltares::Models::ModelSample> sample) const
            {
                sample->Z = 3.0 + sample->Values[1] - 1.25 * sample->Values[0];
                if (logZtoScreen)
                {
                    std::cout << "u, z = " << sample->Values[0] << " , " << sample->Values[1] << " , " << sample->Z << std::endl;
                }
            }

            void projectBuilder::zfuncWithDeterminist(std::shared_ptr<Deltares::Models::ModelSample> sample) const
            {
                sample->Z = 3.0 + sample->Values[0] - 1.25 * sample->Values[2];
                if (logZtoScreen)
                {
                    std::cout << "u, z = " << sample->Values[0] << " , " << sample->Values[1] << " , " << sample->Z << std::endl;
                }
            }

        }
    }
}
