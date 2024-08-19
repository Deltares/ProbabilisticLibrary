#pragma once

#include <memory>
#include "../Deltares.Probabilistic/Model/ModelRunner.h"
#include "../Deltares.Probabilistic/Model/Project.h"
#include "../Deltares.Probabilistic/Sensitivity/SensitivityProject.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class projectBuilder
            {
            public:
                std::shared_ptr<Deltares::Models::ModelRunner> BuildProject();
                std::shared_ptr<Deltares::Models::ModelRunner> BuildProjectWithDeterminist();

                static std::shared_ptr<Sensitivity::SensitivityProject> getSensitivityProject(std::shared_ptr<Deltares::Models::Project> project);
                static std::shared_ptr<Deltares::Models::Project> getAddOneProject();
                static std::shared_ptr<Deltares::Models::Project> getLinearProject();

                bool logZtoScreen = false;
            private:
                void zfunc(std::shared_ptr<Deltares::Models::ModelSample> sample) const;
                void zfuncWithDeterminist(std::shared_ptr<Deltares::Models::ModelSample> sample) const;

                static void sum(std::shared_ptr<Models::ModelSample> sample);
                static void linear(std::shared_ptr<Models::ModelSample> sample);

                static std::shared_ptr<Statistics::Stochast>  getDeterministicStochast(double mean = 0);
                static std::shared_ptr<Statistics::Stochast>  getNormalStochast(double mean = 0, double stddev = 1);
                static std::shared_ptr<Statistics::Stochast>  getLogNormalStochast(double mean = 0, double stddev = 1, double shift = 0);
                static std::shared_ptr<Statistics::Stochast>  getUniformStochast(double min = 0, double max = 1);
                static std::shared_ptr<Statistics::Stochast>  getGumbelStochast(double mean = 0, double stddev = 1);
            };
        }
    }
}
