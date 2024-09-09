#include "SensitivityProject.h"

namespace Deltares
{
    namespace Sensitivity
    {
        void SensitivityProject::run()
        {
            this->sensitivityStochast = nullptr;
            this->sensitivityStochasts.clear();

            this->sensitivityMethod = this->settings->GetSensitivityMethod();
            this->runSettings = this->settings->RunSettings;

            for (std::shared_ptr<Models::ModelParameter> modelParameter : this->model->outputParameters)
            {
                this->model->Index = modelParameter->index;

                std::shared_ptr<Statistics::Stochast> stochast = this->getStochast();
                this->sensitivityStochasts.push_back(stochast);
            }

            if (this->sensitivityStochasts.size() > 0)
            {
                this->sensitivityStochast = this->sensitivityStochasts[0];
            }

            // reset the index
            this->model->Index = 0;

            this->outputCorrelationMatrix = nullptr;
            if (this->settings->CalculateCorrelations)
            {
                this->outputCorrelationMatrix = this->sensitivityMethod->getCorrelationMatrix();
            }
        }

        std::shared_ptr<Statistics::Stochast> SensitivityProject::getStochast()
        {
            if (this->initializer != nullptr)
            {
                initializer();
            }

            std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(this->stochasts, this->correlationMatrix);
            const std::shared_ptr<Models::ModelRunner> modelRunner = std::make_shared<Models::ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            std::shared_ptr<Statistics::Stochast> sensitivityStochast = this->sensitivityMethod->getStochast(modelRunner);
            sensitivityStochast->name = this->model->getIndexedName();

            return sensitivityStochast;
        }

        bool SensitivityProject::isValid() const
        {
            return  this->model != nullptr &&
                this->runSettings != nullptr && this->runSettings->isValid();
        }
    }
}

