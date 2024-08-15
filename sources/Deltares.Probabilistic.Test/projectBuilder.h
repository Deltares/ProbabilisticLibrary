#pragma once

#include <memory>
#include "../Deltares.Probabilistic/Model/Sample.h"
#include "../Deltares.Probabilistic/Model/ModelRunner.h"

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
                static const bool logZtoScreen = false;
            private:
                void zfunc(std::shared_ptr<Deltares::Models::ModelSample> sample) const;
                void zfuncWithDeterminist(std::shared_ptr<Deltares::Models::ModelSample> sample) const;
            };
        }
    }
}
