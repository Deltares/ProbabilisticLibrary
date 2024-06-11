#include "Project.h"

namespace Deltares
{
    namespace Models
    {
        std::shared_ptr<DesignPoint> Project::getDesignPoint()
        {
            std::shared_ptr<UConverter> uConverter = std::make_shared<UConverter>(this->variables, this->correlationMatrix);
            const std::shared_ptr<ModelRunner> modelRunner = std::make_shared<ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            this->designPoint = this->reliabilityMethod->getDesignPoint(modelRunner);

            return this->designPoint;
        }

        bool Project::isValid() const
        {
            return  this->model != nullptr && 
                    this->runSettings != nullptr && this->runSettings->isValid();
        }

    }
}

