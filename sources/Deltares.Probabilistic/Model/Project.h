#pragma once
#include <vector>

#include "../Statistics/Stochast.h"
#include "../Statistics/Correlationmatrix.h"
#include "../Reliability/ReliabilityMethod.h"

namespace Deltares
{
    namespace Models
    {
        using namespace Deltares::Statistics;
        using namespace Deltares::Reliability;

        class Project
        {
        public:
            std::vector<std::shared_ptr<Stochast>> variables;
            std::shared_ptr<CorrelationMatrix> correlationMatrix = std::make_shared<CorrelationMatrix>();
            std::shared_ptr<ReliabilityMethod> reliabilityMethod = nullptr;
            std::shared_ptr<ZModel> model = nullptr;
            std::shared_ptr<RunSettings> runSettings = std::make_shared<RunSettings>();
            std::shared_ptr<ProgressIndicator> progressIndicator = nullptr;
            std::shared_ptr<DesignPoint> designPoint = nullptr;

            bool isValid() const;
            std::shared_ptr<DesignPoint> getDesignPoint();
        };
    }
}

