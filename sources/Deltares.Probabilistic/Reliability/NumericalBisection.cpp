// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "NumericalBisection.h"

#include <algorithm>
#include <cmath>
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

            auto domain = IntegrationDomain(getStartPoint(nStochasts));

            auto lowerBoundaries = std::vector<double>(nStochasts, -StandardNormal::UMax);
            auto upperBoundaries = std::vector<double>(nStochasts, StandardNormal::UMax);

            domain.Cells.push_back(std::make_shared<IntegrationCell>(domain, lowerBoundaries, upperBoundaries));

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
                for (const auto& cell : domain.Cells)
                {
                    if (!cell->Determined) unknownCells.push_back(cell);
                }

                std::sort(unknownCells.begin(), unknownCells.end(), [](const auto& lhs, const auto& rhs)
                    {
                        return lhs->getProbability() > rhs->getProbability();
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
                for(auto& points : domain.Points)
                {
                    if (!points->isKnown()) unknownPoints.push_back(points);
                }

                std::vector<std::shared_ptr<Sample>> upar;
                for (auto& points : unknownPoints)
                {
                    auto sample = std::make_shared<Sample>(points->Coordinates);
                    sample->IterationIndex = step-1;
                    upar.push_back(sample);
                }

                std::vector<double> zValues = modelRunner->getZValues(upar);

                for (size_t i = 0; i < unknownPoints.size(); i++)
                {
                    unknownPoints[i]->setKnown(true);
                    unknownPoints[i]->ZValue = zValues[i];
                }

                // update cells
                for (auto& cell : domain.Cells)
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

                double pLower = probFail / (1.0 - probExcluded);
                double pUpper = (probFail + probUnknown) / (1.0 - probExcluded);

                double betaLower = -StandardNormal::getUFromP(pLower);
                double betaUpper = -StandardNormal::getUFromP(pUpper);

                beta = 0.5 * (betaLower + betaUpper);

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
            auto uMin = getMostProbableFailingPoint(beta, domain);
            auto designPoint = modelRunner->getDesignPoint(uMin, beta, convergenceReport, "Numerical Bisection");
            return designPoint;
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
                chunkSize /= 2;
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

        void NumericalBisection::derivePoints(IntegrationDomain& domain)
        {
            bool changed = true;
            while (changed)
            {
                changed = false;
                for (auto& point : domain.Points)
                {
                    if (!point->isKnown())
                    {
                        point->derive();
                        changed |= point->isKnown();
                    }
                }
            }

            changed = true;
            while (changed)
            {
                changed = false;
                for (auto& point : domain.Points)
                {
                    if (!point->isKnown())
                    {
                        point->deriveByExtrapolation();
                        changed |= point->isKnown();
                    }
                }
            }
        }

        void NumericalBisection::updateProbabilities(IntegrationDomain& domain, double& probUnknown, double& probExcluded, double& probFail)
        {
            for (int i = static_cast<int>(domain.Cells.size()) - 1; i >= 0; i--)
            {
                std::shared_ptr<IntegrationCell> cell = domain.Cells[i];
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

        std::shared_ptr<Sample> NumericalBisection::getMostProbableFailingPoint(double beta, IntegrationDomain& domain) const
        {
            auto method = Settings->designPointMethod;
            auto designPoint = DesignPointBuilder(static_cast<int>(domain.getDimension()), method);

            const auto compResult = (beta >= 0.0 ? DoubleType::Negative : DoubleType::Positive);
            for (const auto& point : domain.Points)
            {
                if (point->getResult() == compResult || point->getResult() == DoubleType::Zero)
                {
                    auto sample = std::make_shared<Sample>(point->Coordinates);
                    sample->Weight = point->ProbabilityDensity();
                    designPoint.addSample(sample);
                }
            }
            auto uMin = designPoint.getSample();
            return uMin;
        }

    }

}
