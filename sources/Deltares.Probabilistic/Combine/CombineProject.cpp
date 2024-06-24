#include "CombineProject.h"
#include "DesignPointCombiner.h"

namespace Deltares
{
    namespace Reliability
    {
        void CombineProject::run()
        {
            std::shared_ptr<DesignPointCombiner> combiner = std::make_shared<DesignPointCombiner>(this->settings->combinerMethod);

            this->designPoint = combiner->combineDesignPoints(this->settings->combineType, this->designPoints, this->selfCorrelationMatrix);
        }
    }
}

