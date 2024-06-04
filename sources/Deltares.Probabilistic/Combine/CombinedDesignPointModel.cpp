#include "CombinedDesignPointModel.h"
#include "../Statistics/StandardNormal.h"
#include "../Statistics/Stochast.h"
#include "../Math/NumericSupport.h"

#include <cmath>

namespace Deltares
{
    namespace Statistics
    {
        class SelfCorrelationMatrix;
        class CorrelationMatrix;
    }
}

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Statistics;

        void CombinedDesignPointModel::addDesignPointModel(std::shared_ptr<DesignPoint> designPoint, bool ignore, bool invert)
        {
            std::shared_ptr<DesignPointModel> designPointModel = std::make_shared<DesignPointModel>();
            designPointModel->designPoint = designPoint;
            designPointModel->ignored = ignore;
            designPointModel->inverted = invert;

            this->designPointModels.push_back(designPointModel);
        }

        void CombinedDesignPointModel::addStochasts(std::vector<std::shared_ptr<Stochast>> stochasts, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix)
        {
            this->stochasts.clear();
            this->standardNormalStochasts.clear();
            this->stochastsMap.clear();
            this->designPointsMap.clear();

            // copy the stochasts
            for (size_t i = 0; i < stochasts.size(); i++)
            {
                this->stochasts.push_back(stochasts[i]);
            }

            // create a standard normal or deterministic stochast for each stochast
            for (size_t i = 0; i < stochasts.size(); i++)
            {
                if (this->isVarying(stochasts[i]))
                {
                    if (selfCorrelationMatrix != nullptr && selfCorrelationMatrix->isSelfCorrelated(stochasts[i]))
                    {
                        for (size_t j = 0; j < designPointModels.size(); j++)
                        {
                            std::shared_ptr<Stochast> normalizedStochast = std::make_shared<Stochast>();
                            normalizedStochast->setDistributionType(DistributionType::Normal);
                            normalizedStochast->getProperties()->Location = 0;
                            normalizedStochast->getProperties()->Scale = 1;
                            this->standardNormalStochasts.push_back(normalizedStochast);

                            stochastsMap.insert({ normalizedStochast, stochasts[i] });
                            designPointsMap.insert({ normalizedStochast, designPointModels[j]->designPoint });
                        }
                    }
                    else
                    {
                        std::shared_ptr<Stochast> normalizedStochast = std::make_shared<Stochast>();
                        normalizedStochast->setDistributionType(DistributionType::Normal);
                        normalizedStochast->getProperties()->Location = 0;
                        normalizedStochast->getProperties()->Scale = 1;
                        this->standardNormalStochasts.push_back(normalizedStochast);

                        stochastsMap.insert({ normalizedStochast, stochasts[i] });
                    }
                }
                else
                {
                    std::shared_ptr<Stochast> normalizedStochast = std::make_shared<Stochast>();
                    normalizedStochast->setDistributionType(DistributionType::Deterministic);
                    normalizedStochast->getProperties()->Location = 0;
                    this->standardNormalStochasts.push_back(normalizedStochast);

                    stochastsMap.insert({ normalizedStochast, stochasts[i] });
                }
            }

            // prepare the design point models
            for (size_t i = 0; i < designPointModels.size(); i++)
            {
                designPointModels[i]->setParameters(standardNormalStochasts, stochastsMap, designPointsMap);
            }
        }

        bool CombinedDesignPointModel::isVarying(std::shared_ptr<Stochast> stochast)
        {
            for (size_t j = 0; j < this->designPointModels.size(); j++)
            {
                if (this->designPointModels[j]->isVarying(stochast))
                {
                    return true;
                }
            }

            return false;
        }

        std::shared_ptr<CorrelationMatrix> CombinedDesignPointModel::getCorrelationMatrix(std::shared_ptr<SelfCorrelationMatrix> selfCorrelationMatrix)
        {
            std::shared_ptr<CorrelationMatrix> correlationMatrix = std::make_shared<CorrelationMatrix>();

            // initialize the correlation matrix
            correlationMatrix->init(this->standardNormalStochasts);

            if (selfCorrelationMatrix != nullptr)
            {
                for (size_t i = 0; i < standardNormalStochasts.size(); i++)
                {
                    for (size_t j = 0; j < i; j++)
                    {
                        if (stochastsMap[standardNormalStochasts[i]] == stochastsMap[standardNormalStochasts[j]])
                        {
                            const std::shared_ptr<Stochast> stochast = stochastsMap[standardNormalStochasts[i]];
                            const std::shared_ptr<DesignPoint> designPoint1 = designPointsMap[standardNormalStochasts[i]];
                            const std::shared_ptr<DesignPoint> designPoint2 = designPointsMap[standardNormalStochasts[j]];

                            // ensure the correlation value is applied to stochasts from different design points
                            if (designPoint1 != nullptr && designPoint2 != nullptr && designPoint1 != designPoint2)
                            {
                                const double correlationValue = selfCorrelationMatrix->getSelfCorrelation(stochast, designPoint1, designPoint2);
                                correlationMatrix->SetCorrelation(standardNormalStochasts[i], standardNormalStochasts[j], correlationValue);
                            }
                        }
                    }
                }
            }

            return correlationMatrix;
        }

        void CombinedDesignPointModel::calculate(std::shared_ptr<ModelSample> sample)
        {
            double result = this->combineType == combineAndOr::combOr ? std::numeric_limits<double>::max() : - std::numeric_limits<double>::max();

            for (std::shared_ptr<DesignPointModel> designPointModel : designPointModels)
            {
                designPointModel->calculate(sample);
                if (std::isnan(sample->Z))
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

        double CombinedDesignPointModel::getStartValue(std::shared_ptr<Statistics::Stochast> stochast)
        {
            double startLoad = 0;
            double startResistance = 0;

            for (std::shared_ptr<DesignPointModel> model : designPointModels)
            {
                std::shared_ptr<StochastPointAlpha> alpha = nullptr;

                for (size_t i = 0; i < model->designPoint->Alphas.size(); i++)
                {
                    if (model->designPoint->Alphas[i]->Stochast == stochast)
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
            std::vector<std::shared_ptr<StochastPointAlpha>> normalizedAlphas;
            for (size_t i = 0; i < designPoint->Alphas.size(); i++)
            {
                normalizedAlphas.push_back(designPoint->Alphas[i]);
            }

            designPoint->Alphas.clear();
            std::shared_ptr<Stochast> prevStochast = nullptr;
            std::shared_ptr<DesignPoint> prevDesignPoint = nullptr;

            std::shared_ptr<StochastPointAlpha> alpha = nullptr;

            for (size_t i = 0; i < normalizedAlphas.size(); i++)
            {
                std::shared_ptr<Stochast> stochast = this->stochastsMap[normalizedAlphas[i]->Stochast];
                std::shared_ptr<DesignPoint> stochastDesignPoint = this->designPointsMap[normalizedAlphas[i]->Stochast];

                if (stochast != prevStochast || stochastDesignPoint == prevDesignPoint)
                {
                    alpha = std::make_shared<StochastPointAlpha>();

                    alpha->Stochast = stochast;
                    alpha->Alpha = normalizedAlphas[i]->Alpha;
                    alpha->AlphaCorrelated = normalizedAlphas[i]->AlphaCorrelated;
                    alpha->U = normalizedAlphas[i]->U;
                    alpha->X = stochast->getXFromU(alpha->U);

                    designPoint->Alphas.push_back(alpha);
                }
                else
                {
                    alpha->Alpha = getCombinedAlpha(alpha->Alpha, normalizedAlphas[i]->Alpha);
                    alpha->AlphaCorrelated = getCombinedAlpha(alpha->AlphaCorrelated, normalizedAlphas[i]->AlphaCorrelated);
                    alpha->U = -designPoint->Beta * alpha->Alpha;
                    alpha->X = stochast->getXFromU(alpha->U);
                }

                prevStochast = stochast;
                prevDesignPoint = stochastDesignPoint;
            }
        }

        double CombinedDesignPointModel::getCombinedAlpha(double alpha1, double alpha2)
        {
            const double sign = std::abs(alpha1) > std::abs(alpha2) ? Numeric::NumericSupport::GetSign(alpha1) : Numeric::NumericSupport::GetSign(alpha2);
            return sign * std::sqrt(alpha1 * alpha1 + alpha2 * alpha2);
        }
    }
}

