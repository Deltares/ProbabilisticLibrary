#include "FORMThenDirectionalSampling.h"
#include "FORM.h"
#include "DirectionalSampling.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> FORMThenDirectionalSampling::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            auto form = FORM();
            form.Settings = formSettings;

            auto formDesignPoint = form.getDesignPoint(modelRunner);

            if (formDesignPoint->convergenceReport->IsConverged)
            {
                // If the resulting beta is below some threshold, we prefer Directional Sampling (DS)
                // But with only 1 stochast, DS will not improve the result.
                if (modelRunner->getVaryingStochastCount() == 1 || formDesignPoint->Beta >= thresholdBeta)
                {
                    return formDesignPoint;
                }
            }

            modelRunner->clear();

            if (modelRunner->Settings->RunAtDesignPoint)
            {
                modelRunner->runDesignPoint(formDesignPoint);
            }

            auto ds = DirectionalSampling();
            ds.Settings = DsSettings;
            auto dsDesignPoint = ds.getDesignPoint(modelRunner);
            dsDesignPoint->convergenceReport->TotalIterations = formDesignPoint->convergenceReport->TotalIterations;
            dsDesignPoint->ContributingDesignPoints.push_back(formDesignPoint);

            return dsDesignPoint;
        }
    }
}
