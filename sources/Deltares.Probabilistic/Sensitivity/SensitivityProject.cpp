#include "SensitivityProject.h"

namespace Deltares
{
    namespace Sensitivity
    {
        void SensitivityProject::run()
        {
            this->sensitivityMethod = this->settings->GetSensitivityMethod();
            this->runSettings = this->settings->RunSettings;

            this->stochast = this->getStochast();
        }

        std::shared_ptr<Statistics::Stochast> SensitivityProject::getStochast()
        {
            std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(this->stochasts, this->correlationMatrix);
            const std::shared_ptr<Models::ModelRunner> modelRunner = std::make_shared<Models::ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            this->stochast = this->sensitivityMethod->getStochast(modelRunner);

            return this->stochast;
        }

        bool SensitivityProject::isValid() const
        {
            return  this->model != nullptr && 
                    this->runSettings != nullptr && this->runSettings->isValid();
        }
    }
}

