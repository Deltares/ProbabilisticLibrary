#include "ImportanceSamplingCombiner.h"

#include <algorithm>
#if __has_include(<format>)
#include <format>
#else
#include "../Utils/probLibString.h"
#endif
#include <unordered_set>

#include "../Math/NumericSupport.h"
#include "../Reliability/DesignPoint.h"
#include "../Reliability/DesignPointBuilder.h"
#include "../Reliability/ImportanceSampling.h"
#include "../Model/ProgressIndicator.h"
#include "../Model/Project.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Statistics/StandardNormal.h"
#include "combiner.h"
#include "CombinedDesignPointModel.h"

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Statistics;
        using namespace Deltares::Reliability;

        std::shared_ptr<DesignPoint> ImportanceSamplingCombiner::combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress)
        {
            constexpr double invertProbability = 0.1;

            double approximatedProbability = this->getApproximatedProbability(combineMethodType, designPoints);

            if (combineMethodType == combineAndOr::combOr && approximatedProbability > invertProbability)
            {
                combineMethodType = combineAndOr::combAnd;

                for (std::shared_ptr<DesignPoint> designPoint : designPoints)
                {
                    invert(designPoint);
                }

                std::shared_ptr<DesignPoint> invertedDesignPoint = combineDesignPointsAdjusted(combineMethodType, selfCorrelationMatrix, progress, designPoints);

                invert(invertedDesignPoint);

                for (std::shared_ptr<DesignPoint> designPoint : designPoints)
                {
                    invert(designPoint);
                    invertedDesignPoint->ContributingDesignPoints.push_back(designPoint);
                }

                return invertedDesignPoint;
            }
            else
            {
                std::shared_ptr<DesignPoint> designPoint =  combineDesignPointsAdjusted(combineMethodType, selfCorrelationMatrix, progress, designPoints);

                for (size_t i = 0; i < designPoints.size(); i++)
                {
                    designPoint->ContributingDesignPoints.push_back(designPoints[i]);
                }

                return designPoint;
            }
        }

        double ImportanceSamplingCombiner::getApproximatedProbability(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints)
        {
            if (combineMethodType == combineAndOr::combAnd)
            {
                double prob = 1.0;
                for (const std::shared_ptr<DesignPoint>& designPoint : designPoints)
                {
                    prob *= Statistics::StandardNormal::getQFromU(designPoint->Beta);
                }

                return prob;
            }
            else
            {
                double prob = 1.0;
                for (const std::shared_ptr<DesignPoint>& designPoint : designPoints)
                {
                    prob *= Statistics::StandardNormal::getPFromU(designPoint->Beta);
                }

                return 1.0 - prob;
            }
        }

        void ImportanceSamplingCombiner::invert(std::shared_ptr<DesignPoint>& designPoint)
        {
            designPoint->Beta = -designPoint->Beta;
            for (std::shared_ptr<StochastPointAlpha>& alpha : designPoint->Alphas)
            {
                alpha->Alpha = -alpha->Alpha;
                alpha->AlphaCorrelated = -alpha->AlphaCorrelated;
            }

            for (std::shared_ptr<ReliabilityResult>& report : designPoint->ReliabililityResults)
            {
                report->Reliability = -report->Reliability;
            }
        }

        std::shared_ptr<DesignPoint> ImportanceSamplingCombiner::combineDesignPointsAdjusted(combineAndOr combineMethodType, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress, std::vector<std::shared_ptr<DesignPoint>>& designPoints)
        {
            constexpr double deltaReliabilityIndex = 0.01;

            std::vector<std::shared_ptr<Stochast>> stochasts = getUniqueStochasts(designPoints);

            if (combineMethodType == combineAndOr::combOr)
            {
                std::sort(designPoints.begin(), designPoints.end(), [](std::shared_ptr<DesignPoint> val1, std::shared_ptr<DesignPoint> val2) {return val1->Beta < val2->Beta; });

                // administration for design point
                std::shared_ptr<DesignPointBuilder> builder = std::make_shared<DesignPointBuilder>(stochasts.size(), DesignPointMethod::CenterOfGravity);

                // add first realization to design point
                const std::shared_ptr<Sample> firstDesignPointSample = designPoints[0]->getSampleForStochasts(stochasts);
                firstDesignPointSample->Weight = designPoints[0]->getFailureProbability();
                builder->addSample(firstDesignPointSample);

                double probability = StandardNormal::getQFromU(designPoints[0]->Beta);
                double reliabilityIndex = designPoints[0]->Beta;

                if (progress != nullptr)
                {
                    progress->doProgress(0);
#ifdef __cpp_lib_format
                    progress->doTextualProgress(ProgressType::Detailed, std::format("Combining design points {0:}/{1:}, Reliability index = {2:.2F}", 0, designPoints.size(), reliabilityIndex));
#else
                    auto pl = Deltares::Reliability::probLibString();
                    progress->doTextualProgress(ProgressType::Detailed, "Combining design points 0/" + std::to_string(designPoints.size()) +
                        ", Reliability index = " + pl.double2str(reliabilityIndex));
#endif
                }

                std::vector<std::shared_ptr<DesignPoint>> previousRealizations;

                for (size_t i = 1; i < designPoints.size(); i++)
                {
                    previousRealizations.push_back(designPoints[i - 1]);
                    const std::shared_ptr<DesignPoint> currentDesignPoint = designPoints[i];

                    // add to design point
                    const std::shared_ptr<Sample> designPointSample = currentDesignPoint->getSampleForStochasts(stochasts);
                    designPointSample->Weight = currentDesignPoint->getFailureProbability();
                    builder->addSample(designPointSample);

                    // calculate contributing probability for the stochast
                    const std::shared_ptr<DesignPoint> contributingRealization = getSeriesProbability(selfCorrelationMatrix, currentDesignPoint, previousRealizations, currentDesignPoint, stochasts, progress);

                    // add contributing probability of total probability
                    probability += currentDesignPoint->getFailureProbability() * contributingRealization->getNonFailureProbability();
                    reliabilityIndex = StandardNormal::getUFromQ(probability);

                    double maxRemainingProbability = 0;
                    for (size_t j = i + 1; j < designPoints.size(); j++)
                    {
                        maxRemainingProbability += StandardNormal::getQFromU(designPoints[i]->Beta);
                    }

                    const double maxProbability = probability + maxRemainingProbability;
                    const double maxReliability = Statistics::StandardNormal::getUFromQ(maxProbability);
                    double diffReliability = std::abs(reliabilityIndex - maxReliability);

                    if (progress != nullptr)
                    {
                        progress->doProgress(Numeric::NumericSupport::Divide(i, designPoints.size()));
#ifdef __cpp_lib_format
                        progress->doTextualProgress(ProgressType::Detailed, std::format("{0:}/{1:}, Reliability index = {2:.2F}, Δ Reliability index = {3:.3F}", i, designPoints.size(), reliabilityIndex, diffReliability));
#else
                        auto pl = Deltares::Reliability::probLibString();
                        progress->doTextualProgress(ProgressType::Detailed, std::to_string(i) + "/" + std::to_string(designPoints.size()) + ", Reliability index = " +
                            pl.double2str(reliabilityIndex) + " Δ Reliability index = " + pl.double2str(diffReliability));
#endif
                    }

                    if (diffReliability < deltaReliabilityIndex)
                    {
                        break;
                    }
                }

                // create final design point
                std::shared_ptr<DesignPoint> combinedRealization = std::make_shared<DesignPoint>();
                combinedRealization->Beta = Statistics::StandardNormal::getUFromQ(probability);

                // create alpha values for final design point
                const std::shared_ptr<Sample> combinedSample = builder->getSample();

                for (size_t i = 0; i < stochasts.size(); i++)
                {
                    const double alphaValue = -combinedSample->Values[i] / combinedRealization->Beta;
                    std::shared_ptr<StochastPointAlpha> alpha = std::make_shared<StochastPointAlpha>();
                    alpha->Stochast = stochasts[i];
                    alpha->Alpha = alphaValue;
                    alpha->AlphaCorrelated = alphaValue;
                    alpha->U = - combinedRealization->Beta * alphaValue;
                    alpha->X = stochasts[i]->getXFromU(alpha->U);
                    combinedRealization->Alphas.push_back(alpha);
                }

                if (progress != nullptr)
                {
                    progress->complete();
                }

                return combinedRealization;
            }
            else
            {
                return getParallelProbability(selfCorrelationMatrix, designPoints, stochasts, progress);
            }
        }


        std::shared_ptr<DesignPoint> ImportanceSamplingCombiner::getSeriesProbability(std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<DesignPoint> currentDesignPoint, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::shared_ptr<DesignPoint> startPoint, std::vector<std::shared_ptr<Stochast>>& stochasts, std::shared_ptr<ProgressIndicator> progress)
        {
            // create the model from design points
            const std::shared_ptr<CombinedDesignPointModel> model = getModel(combineAndOr::combOr, currentDesignPoint, previousDesignPoints, stochasts, selfCorrelationMatrix);

            const std::shared_ptr<Project> project = getProject(model, selfCorrelationMatrix);

            const std::shared_ptr<ImportanceSampling> importanceSampling = std::make_shared<ImportanceSampling>();
            fillSettingsSeries(startPoint, model, importanceSampling->Settings);

            project->reliabilityMethod = importanceSampling;

            std::shared_ptr<DesignPoint> combinedDesignPoint = project->getDesignPoint();

            // convert the stochasts back
            model->replaceStandardNormalStochasts(combinedDesignPoint);

            return combinedDesignPoint;
        }

        void ImportanceSamplingCombiner::fillSettingsSeries(std::shared_ptr<DesignPoint> startPoint, std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<ImportanceSamplingSettings> settings)
        {
            settings->MinimumSamples = 2000;
            settings->MaximumSamples = 10000;
            settings->VariationCoefficient = 0.1;
            settings->designPointMethod = DesignPointMethod::NearestToMean; // result will be ignored, fastest option
            settings->randomSettings->SkipUnvaryingParameters = false;
            settings->randomSettings->RandomGeneratorType = randomGeneratorType;

            for (size_t i = 0; i < model->stochasts.size(); i++)
            {
                std::shared_ptr<StochastSettings> stochastSetting = std::make_shared<StochastSettings>();
                stochastSetting->stochast = model->standardNormalStochasts[i];
                stochastSetting->VarianceFactor = 1;
                stochastSetting->StartValue = 0;

                std::shared_ptr<StochastPointAlpha> alpha = startPoint->getAlpha(model->stochasts[i]);
                if (alpha != nullptr)
                {
                    stochastSetting->StartValue = alpha->U;
                }

                settings->StochastSet->stochastSettings.push_back(stochastSetting);
            }
        }

        std::shared_ptr<DesignPoint> ImportanceSamplingCombiner::getParallelProbability(std::shared_ptr<SelfCorrelationMatrix> selfCorrelationMatrix, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::vector<std::shared_ptr<Stochast>>& stochasts, std::shared_ptr<ProgressIndicator> progress)
        {
            constexpr int maxIterations = 4;
            constexpr double requiredFailures = 0.1;
            double upscaleFactor = 1.25;

            // create the model from design points
            const std::shared_ptr<CombinedDesignPointModel> model = getModel(combineAndOr::combAnd, nullptr, previousDesignPoints, stochasts, selfCorrelationMatrix);

            const std::shared_ptr<Project> project = getProject(model, selfCorrelationMatrix);

            int iteration = 0;
            double factor = 1;

            std::shared_ptr<DesignPoint> bestDesignPoint = nullptr;
            bool revertedDirection = false;

            while (true)
            {
                const std::shared_ptr<ImportanceSampling> importanceSampling = std::make_shared<ImportanceSampling>();
                fillSettingsParallel(model, importanceSampling->Settings, factor);

                project->reliabilityMethod = importanceSampling;

                bool improved = false;

                std::shared_ptr<DesignPoint> designPoint = project->getDesignPoint();

                // convert the stochasts back
                model->replaceStandardNormalStochasts(designPoint);

                if (bestDesignPoint == nullptr || designPoint->convergenceReport->getSmallestFraction() > bestDesignPoint->convergenceReport->getSmallestFraction())
                {
                    bestDesignPoint = designPoint;
                    improved = true;
                }

                // if the first iteration did not improve the results, search in the other direction
                if (!improved && iteration == 1 && !revertedDirection)
                {
                    revertedDirection = true;
                    factor = 1;
                    improved = true;
                    upscaleFactor = 0.8;
                }

                iteration++;

                if (!improved)
                {
                    if (progress != nullptr)
                    {
                        progress->complete();
                    }

                    return bestDesignPoint;
                }
                else if (iteration >= maxIterations || designPoint->convergenceReport->getSmallestFraction() >= requiredFailures)
                {
                    if (progress != nullptr)
                    {
                        progress->complete();
                    }

                    return designPoint;
                }
                else
                {
                    if (progress != nullptr)
                    {
                        progress->doProgress(Numeric::NumericSupport::Divide(iteration, maxIterations));
#ifdef __cpp_lib_format
                        progress->doTextualProgress(ProgressType::Detailed, std::format("{0:}/{1:}, Reliability index = {2:.2F}", iteration, maxIterations, designPoint->Beta));
#else
                        auto pl = Deltares::Reliability::probLibString();
                        progress->doTextualProgress(ProgressType::Detailed, std::to_string(iteration) + "/" +
                            std::to_string(maxIterations) + ", Reliability index = " + pl.double2str(designPoint->Beta));
#endif
                    }

                    if (designPoint->convergenceReport->FailFraction < requiredFailures)
                    {
                        factor *= upscaleFactor;
                    }
                    else
                    {
                        factor /= upscaleFactor;
                    }
                }
            }
        }

        void ImportanceSamplingCombiner::fillSettingsParallel(std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<ImportanceSamplingSettings> settings, double factor)
        {
            settings->MinimumSamples = 20000;
            settings->MaximumSamples = 100000;
            settings->VariationCoefficient = 0.1;
            settings->designPointMethod = DesignPointMethod::CenterOfGravity; // result will be ignored, fastest option
            settings->randomSettings->SkipUnvaryingParameters = false;
            settings->randomSettings->RandomGeneratorType = randomGeneratorType;

            for (size_t i = 0; i < model->stochasts.size(); i++)
            {
                std::shared_ptr<StochastSettings> stochastSetting = std::make_shared<StochastSettings>();
                stochastSetting->stochast = model->standardNormalStochasts[i];
                stochastSetting->VarianceFactor = 1;
                stochastSetting->StartValue = factor * model->getStartValue(model->stochasts[i]);

                settings->StochastSet->stochastSettings.push_back(stochastSetting);
            }
        }
    }
}

