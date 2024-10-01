#include "NumericalBisection.h"

#include <algorithm>

#include "../Statistics/StandardNormal.h"

using namespace Deltares::Models;
using namespace Deltares::Statistics;
using namespace Deltares::Numeric;

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> NumericalBisection::getDesignPoint(std::shared_ptr<ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            int nStochasts = modelRunner->getVaryingStochastCount();

            std::shared_ptr<IntegrationDomain> domain = std::make_shared<IntegrationDomain>(nStochasts);

            domain->SetOrigin(getStartPoint(nStochasts));

            auto lowerBoundaries = std::vector<double>(nStochasts, -8.0);
            auto upperBoundaries = std::vector<double>(nStochasts, 8.0);

            domain->Cells.push_back(std::make_shared<IntegrationCell>(domain, lowerBoundaries, upperBoundaries));

            bool ready = false;

            double beta = 0;
            std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

            auto chunkSize = getChunkSize(nStochasts);

            int step = 0;

            while (!ready)
            {
                step++;

                // determine the biggest unknown cells

                std::vector<std::shared_ptr<IntegrationCell>> unknownCells;
                for (const auto& cell : domain->Cells)
                {
                    if (!cell->Determined) unknownCells.push_back(cell);
                }

                std::sort(unknownCells.begin(), unknownCells.end(), [](const auto& lhs, const auto& rhs)
                    {
                        return lhs->getProbability() > rhs->getProbability(); // TODO check < or >
                    });

                if (unknownCells.size() > chunkSize)
                {
                    unknownCells.resize(chunkSize);
                }

                // split the unknown cells
                splitUnknownCells(modelRunner, step, unknownCells);

                // derive the values of the new points
                derivePoints(domain);

                // calculate the unknown points
                std::vector<std::shared_ptr<IntegrationPoint>> unknownPoints;
                for(auto& points : domain->Points)
                {
                    if (!points->Known) unknownPoints.push_back(points);
                }

                std::vector<std::shared_ptr<Sample>> upar;
                for (auto& points : unknownPoints)
                {
                    upar.push_back(std::make_shared<Sample>(points->Coordinates));  // TODO add step-1 to iteration
                }

                std::vector<double> zValues = modelRunner->getZValues(upar);

                for (size_t i = 0; i < unknownPoints.size(); i++)
                {
                    unknownPoints[i]->Known = true;
                    unknownPoints[i]->ZValue = zValues[i];
                }

                // update cells
                for (auto& cell : domain->Cells)
                {
                    if (!cell->Known)
                    {
                        cell->update();
                    }
                }

                // recalculate probabilities
                double probFail = 0.0;
                double probExcluded = 0.0;
                double probUnknown = 0.0;

                updateProbabilities(domain, probUnknown, probExcluded, probFail);

                double pLower = probFail / (1 - probExcluded);
                double pUpper = (probFail + probUnknown) / (1 - probExcluded);

                double betaLower = -StandardNormal::getUFromP(pLower);
                double betaUpper = -StandardNormal::getUFromP(pUpper);

                beta = 0.5 * (betaLower + betaUpper);

                //double convLower = Math.Abs(betaLower - beta) / settings.EpsilonBeta;
                //double convUpper = Math.Abs(betaUpper - beta) / settings.EpsilonBeta;
                //double convergence = Math.Max(convLower, convUpper);

                double diffLower = std::abs(betaLower - beta);
                double diffUpper = std::abs(betaUpper - beta);
                double diff = std::max(diffLower, diffUpper);

                auto report = std::make_shared<ReliabilityReport>();
                report->Step = step;
                report->MaxSteps = Settings->MaximumIterations;
                report->Reliability = beta;
                report->ConvBeta = diff;

                modelRunner->reportResult(report);

                convergenceReport->Convergence = diff;

                ready = isReady(diff, step);
            }

            // find the design point

            auto designPoint = getMostProbableFailingPoint(beta, domain);

            auto uMin = std::make_shared<Sample>(designPoint->Coordinates);
            std::shared_ptr<DesignPoint> dp = modelRunner->getDesignPoint(uMin, beta, convergenceReport);
            return dp;
        }

        std::vector<double> NumericalBisection::getStartPoint(const int nStochasts) const
        {
            auto startPoint = std::vector<double>();
            for (int i = 0; i < nStochasts; i++)
            {
                startPoint.push_back(Settings->StochastSet->VaryingStochastSettings[i]->StartValue);
            }
            return startPoint;
        }

        size_t NumericalBisection::getChunkSize(const int nStochasts)
        {
            size_t chunkSize = 8192;
            for (int i = 0; i < std::min(10, nStochasts); i++)
            {
                chunkSize = chunkSize / 2;
            }

            return chunkSize;
        }

        void NumericalBisection::splitUnknownCells(std::shared_ptr<ModelRunner> model, int step, std::vector<std::shared_ptr<IntegrationCell>>& unknownCells) const
        {
            int nStochasts = model->getVaryingStochastCount();
            if (step == 1)
            {
                // split according to initial values
                for (auto& cell : unknownCells)
                {
                    cell->split(getStartPoint(nStochasts));
                }
            }
            else
            {
                for (auto& cell : unknownCells)
                {
                    cell->split();
                }
            }
        }

        void NumericalBisection::derivePoints(std::shared_ptr<IntegrationDomain> domain)
        {
            bool changed = true;
            while (changed)
            {
                changed = false;
                for (auto& point : domain->Points)
                {
                    if (!point->Known)
                    {
                        point->derive();
                        changed |= point->Known;
                    }
                }
            }

            changed = true;
            while (changed)
            {
                changed = false;
                for (auto& point : domain->Points)
                {
                    if (!point->Known)
                    {
                        point->deriveByExtrapolation();
                        changed |= point->Known;
                    }
                }
            }
        }

        void NumericalBisection::updateProbabilities(std::shared_ptr<IntegrationDomain> domain, double& probUnknown, double& probExcluded, double& probFail)
        {
            for (int i = domain->Cells.size() - 1; i >= 0; i--)
            {
                std::shared_ptr<IntegrationCell> cell = domain->Cells[i];
                if (!cell->Known || !cell->Determined)
                {
                    probUnknown += cell->getProbability();
                }
                else
                {
                    switch (cell->Result)
                    {
                    case DoubleType::NaN:
                        probExcluded += cell->getProbability();
                        break;
                    case DoubleType::Positive:
                        break;
                    case DoubleType::Negative:
                        probFail += cell->getProbability();
                        break;
                    case DoubleType::Zero:
                        break;
                    default:
                        throw probLibException("invalid result in updateProbabilities");
                    }
                }
            }
        }

        bool NumericalBisection::isReady(double diff, int step) const
        {
            return (diff < Settings->EpsilonBeta && step >= Settings->MinimumIterations) || step >= Settings->MaximumIterations;
        }

        std::shared_ptr<IntegrationPoint> NumericalBisection::getMostProbableFailingPoint(double beta, std::shared_ptr<IntegrationDomain> domain)
        {
            double minProbability = 0;
            std::shared_ptr<IntegrationPoint> designPoint = nullptr;

            if (beta >= 0)
            {
                for (auto& point : domain->Points)
                {
                    if (point->Result == DoubleType::Negative || point->Result == DoubleType::Zero)
                    {
                        if (point->ProbabilityDensity > minProbability)
                        {
                            designPoint = point;
                            minProbability = designPoint->ProbabilityDensity;
                        }
                    }
                }
            }
            else
            {
                for (auto& point : domain->Points)
                {
                    if (point->Result == DoubleType::Positive || point->Result == DoubleType::Zero)
                    {
                        if (point->ProbabilityDensity > minProbability)
                        {
                            designPoint = point;
                            minProbability = designPoint->ProbabilityDensity;
                        }
                    }
                }
            }

            return designPoint;
        }

    }

}
