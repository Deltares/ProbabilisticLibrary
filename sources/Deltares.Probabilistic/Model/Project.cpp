#include "Project.h"

namespace Deltares
{
    namespace Models
    {
        void Project::run()
        {
            if (this->initializer != nullptr)
            {
                this->initializer();
            }

            this->reliabilityMethod = this->settings->GetReliabilityMethod();
            this->runSettings = this->settings->RunSettings;

            if (this->settings->ReliabilityResult == Reliability::ResultDesignPoint)
            {
                this->designPoint = this->getDesignPoint();
            }
            else if (this->settings->ReliabilityResult == Reliability::ResultFragilityCurve)
            {
                this->fragilityCurve = this->getFragilityCurve();
            }
        }

        std::shared_ptr<Reliability::DesignPoint> Project::getDesignPoint()
        {
            std::shared_ptr<UConverter> uConverter = std::make_shared<UConverter>(this->stochasts, this->correlationMatrix);
            const std::shared_ptr<ModelRunner> modelRunner = std::make_shared<ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            this->designPoint = this->reliabilityMethod->getDesignPoint(modelRunner);

            return this->designPoint;
        }

        std::shared_ptr<Statistics::Stochast> Project::getFragilityCurve()
        {
            this->fragilityCurve = std::make_shared<Statistics::Stochast>();
            fragilityCurve->setDistributionType(Statistics::CDFCurve);

            std::shared_ptr<UConverter> uConverter = std::make_shared<UConverter>(this->stochasts, this->correlationMatrix);
            const std::shared_ptr<ModelRunner> modelRunner = std::make_shared<ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            this->designPoint = this->reliabilityMethod->getDesignPoint(modelRunner);

            return this->fragilityCurve;
        }

        bool Project::isValid() const
        {
            return  this->model != nullptr && 
                    this->runSettings != nullptr && this->runSettings->isValid();
        }
    }
}

