#include "DirectionalSamplingThenFORM.h"
#include "FORM.h"
#include "DirectionalSampling.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> DirectionalSamplingThenFORM::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            auto ds = DirectionalSampling();
            ds.Settings = DsSettings;
            modelRunner->setRelMethodCounter(1);
            auto dsDesignPoint = ds.getDesignPoint(modelRunner);

            auto nStoch = dsDesignPoint->Alphas.size();
            auto startVector = std::vector<double>(nStoch);
            for (size_t i = 0; i < nStoch; i++)
            {
                double u = -dsDesignPoint->Beta * dsDesignPoint->Alphas[i]->Alpha;
                startVector[i] = u;
            }

            auto form = FORM();
            form.Settings = formSettings;
            form.Settings->StartPointSettings->StartMethod = StartMethodType::GivenVector;
            form.Settings->StartPointSettings->startVector = startVector;
            modelRunner->setRelMethodCounter(2);
            auto dsfiDesignPoint = form.getDesignPoint(modelRunner);
            dsfiDesignPoint->convergenceReport->TotalDirections = dsDesignPoint->convergenceReport->TotalDirections;
            dsfiDesignPoint->Beta = dsDesignPoint->Beta;
            return dsfiDesignPoint;
        }
    }
}
