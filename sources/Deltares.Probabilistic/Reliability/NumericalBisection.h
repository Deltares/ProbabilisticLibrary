#pragma once

#include <vector>
#include "ReliabilityMethod.h"
#include "NumericalBisectionSettings.h"
#include "../Math/NumericSupport.h"
#include "IntegrationGrid.h"

namespace Deltares
{
    namespace Reliability
    {
        class NumericalBisection : public ReliabilityMethod
        {
        public:
            std::shared_ptr<NumericalBisectionSettings> Settings = std::make_shared<NumericalBisectionSettings>();
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            std::vector<double> getStartPoint(const int nStochasts) const;
            static size_t getChunkSize(const int nStochasts);
            void splitUnknownCells(std::shared_ptr<Models::ModelRunner> model, int step, std::vector<std::shared_ptr<IntegrationCell>>& unknownCells) const;
            void derivePoints(std::shared_ptr<IntegrationDomain> domain);
            static void updateProbabilities(std::shared_ptr<IntegrationDomain> domain, double& probUnknown, double& probExcluded, double& probFail);
            bool isReady(double diff, int step) const;
            static std::shared_ptr<IntegrationPoint> getMostProbableFailingPoint(double beta, std::shared_ptr<IntegrationDomain> domain);
        };
    }
}

