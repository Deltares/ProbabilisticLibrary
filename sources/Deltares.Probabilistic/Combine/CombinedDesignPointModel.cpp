#include "CombinedDesignPointModel.h"
#include "../Statistics/StandardNormal.h"
#include "../Statistics/Stochast.h"

#include <cmath>

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Statistics;

        void CombinedDesignPointModel::addStochasts(std::vector<std::shared_ptr<Stochast>> stochasts)
        {
            for (size_t i = 0; i < stochasts.size(); i++)
            {
                this->stochasts.push_back(stochasts[i]);

                std::shared_ptr<Stochast> normalizedStochast = std::make_shared<Stochast>();
                normalizedStochast->setDistributionType(DistributionType::Deterministic);
                normalizedStochast->getProperties()->Location = 0;

                this->standardNormalStochasts.push_back(normalizedStochast);
            }
        }

        void CombinedDesignPointModel::addDesignPointModel(std::shared_ptr<DesignPoint> designPoint, bool invert, bool negate)
        {
            std::shared_ptr<DesignPointModel> designPointModel = std::make_shared<DesignPointModel>();
            designPointModel->designPoint = designPoint;
            designPointModel->inverted = invert;
            designPointModel->negated = negate;

            this->designPointModels.push_back(designPointModel);

            designPointModel->setParameters(this->stochasts);
        }

        void CombinedDesignPointModel::updateStochasts()
        {
            for (size_t i = 0; i < this->standardNormalStochasts.size(); i++)
            {
                bool isVarying = false;

                for (size_t j = 0; j < this->designPointModels.size(); j++)
                {
                    if (this->designPointModels[j]->isVarying(i))
                    {
                        isVarying = true;
                        break;
                    }
                }

                if (isVarying)
                {
                    this->standardNormalStochasts[i]->setDistributionType(DistributionType::Normal);
                    this->standardNormalStochasts[i]->getProperties()->Location = 0;
                    this->standardNormalStochasts[i]->getProperties()->Scale = 1;
                }
            }
        }

        void CombinedDesignPointModel::calculate(std::shared_ptr<ModelSample> sample)
        {
            double result = this->combineType == combineAndOr::combOr ? std::numeric_limits<double>::max() : - std::numeric_limits<double>::max();

            for (std::shared_ptr<DesignPointModel> designPointModel : designPointModels)
            {
                designPointModel->calculate(sample);
                if (isnan(sample->Z))
                {
                    result = nan("");
                    break;
                }
                else
                {
                    result = this->combineType == combineAndOr::combOr ? std::min(result, sample->Z) : std::max(result, sample->Z);
                }
            }

            sample->Z = result;
        }

        double CombinedDesignPointModel::getStartValue(std::shared_ptr<Statistics::Stochast> parameter)
        {
            double startLoad = 0;
            double startResistance = 0;

            for (std::shared_ptr<DesignPointModel> model : designPointModels)
            {
                std::shared_ptr<StochastPointAlpha> alpha = nullptr;

                for (size_t i = 0; i < model->designPoint->Alphas.size(); i++)
                {
                    if (model->designPoint->Alphas[i]->Stochast == parameter)
                    {
                        alpha = model->designPoint->Alphas[i];
                        break;
                    }
                }

                if (alpha != nullptr)
                {
                    if (alpha->U < 0)
                    {
                        startLoad = std::min(startLoad, alpha->U);
                    }
                    else if (alpha->U > 0)
                    {
                        startResistance = std::max(startResistance, alpha->U);
                    }
                }
            }

            if (std::abs(startLoad) > std::abs(startResistance))
            {
                return startLoad;
            }
            else
            {
                return startResistance;
            }
        }

        double CombinedDesignPointModel::getBetaDirection(std::shared_ptr<ModelSample> sample)
        {
            if (this->combineType == combineAndOr::combOr)
            {
                double beta = std::numeric_limits<double>::max();

                for (const std::shared_ptr<DesignPointModel> designPointModel : designPointModels)
                {
                    double designPointBeta = designPointModel->getDirectionBeta(sample);

                    if (designPointBeta >= 0)
                    {
                        beta = std::min(designPointBeta, beta);
                    }
                }

                return beta;
            }
            else
            {
                double beta = 0;

                for (const std::shared_ptr<DesignPointModel> designPointModel : designPointModels)
                {
                    double designPointBeta = designPointModel->getDirectionBeta(sample);

                    if (designPointBeta >= 0)
                    {
                        beta = std::max(beta, designPointBeta);
                    }
                    else
                    {
                        return Statistics::StandardNormal::BetaMax;
                    }
                }

                return beta;
            }
        }

        void CombinedDesignPointModel::replaceStandardNormalStochasts(std::shared_ptr<DesignPoint> designPoint)
        {
            for (size_t i = 0; i < designPoint->Alphas.size(); i++)
            {
                if (designPoint->Alphas[i]->Stochast == this->standardNormalStochasts[i])
                {
                    designPoint->Alphas[i]->Stochast = stochasts[i];
                    designPoint->Alphas[i]->X = stochasts[i]->getXFromU(designPoint->Alphas[i]->U);
                }
            }
        }
    }
}

