// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include <set>
#include <numbers>
#include <cmath>
#include "IntegrationGrid.h"
#include "../Math/BinarySupport.h"
#include "../Utils/probLibString.h"

using namespace Deltares::Numeric;

namespace Deltares
{
    namespace Reliability
    {
        void IntegrationPoint::derive()
        {
            for (auto& line : Lines)
            {
                std::shared_ptr<IntegrationPoint> previousPoint = line->GetPreviousKnownPoint(this);
                std::shared_ptr<IntegrationPoint> nextPoint = line->GetNextKnownPoint(this);

                if (previousPoint != nullptr && nextPoint != nullptr && line->OnSameSide(*previousPoint, *nextPoint))
                {
                    if (previousPoint->getResult() == nextPoint->getResult())
                    {
                        setKnown(true);
                    }
                    else if (previousPoint->getResult() == DoubleType::Zero && (nextPoint->getResult() == DoubleType::Negative || nextPoint->getResult() == DoubleType::Positive))
                    {
                        setKnown(true);
                    }
                    else if (nextPoint->getResult() == DoubleType::Zero && (previousPoint->getResult() == DoubleType::Negative || previousPoint->getResult() == DoubleType::Positive))
                    {
                        setKnown(true);
                    }

                    if (Known)
                    {
                        double xPrev = previousPoint->Coordinates[line->getDimension()];
                        double xNext = nextPoint->Coordinates[line->getDimension()];
                        double x = this->Coordinates[line->getDimension()];

                        double fraction = (x - xPrev) / (xNext - xPrev);

                        setZValue(fraction * nextPoint->ZValue + (1 - fraction) * previousPoint->ZValue);

                        //zValue = (previousPoint.ZValue + nextPoint.ZValue) / 2;
                    }
                }

                if (Known)
                {
                    return;
                }
            }
        }

        void IntegrationPoint::deriveByExtrapolation()
        {
            for (auto& line : Lines)
            {
                std::shared_ptr<IntegrationPoint> previousPoint = line->GetPreviousKnownPoint(this);
                std::shared_ptr<IntegrationPoint> nextPoint = line->GetNextKnownPoint(this);

                if (previousPoint == nullptr && nextPoint != nullptr && line->OnSameSide(*this, *nextPoint))
                {
                    if (nextPoint->getResult() == DoubleType::Negative)
                    {
                        if (line->HasNextKnownPoint(nextPoint, DoubleType::Positive))
                        {
                            setKnown(true);
                            setZValue(nextPoint->ZValue);
                        }
                    }
                    else if (nextPoint->getResult() == DoubleType::Positive)
                    {
                        if (line->HasNextKnownPoint(nextPoint, DoubleType::Negative))
                        {
                            setKnown(true);
                            setZValue(nextPoint->ZValue);
                        }
                    }
                }
                else if (previousPoint != nullptr && nextPoint == nullptr && line->OnSameSide(*this, *previousPoint))
                {
                    if (previousPoint->getResult() == DoubleType::Negative)
                    {
                        if (line->HasPreviousKnownPoint(previousPoint, DoubleType::Positive))
                        {
                            setKnown(true);
                            setZValue(previousPoint->ZValue);
                        }
                    }
                    else if (previousPoint->getResult() == DoubleType::Positive)
                    {
                        if (line->HasPreviousKnownPoint(previousPoint, DoubleType::Negative))
                        {
                            setKnown(true);
                            setZValue(previousPoint->ZValue);
                        }
                    }
                }

                if (Known)
                {
                    return;
                }
            }
        }

        double GetPDF(double x)
        {   // todo use pdf from normal distribution
            double normalFactor = 1.0 / (sqrt(2.0 * std::numbers::pi));
            double distance = -x * x / 2.0;

            return normalFactor * exp(distance);
        }

        double IntegrationPoint::ProbabilityDensity() const
        {
            double probability = 1.0;
            for(double coordinate : Coordinates)
            {
                probability *= GetPDF(coordinate);
            }
            return probability;
        }


        bool IntegrationLine::OnSameSide(IntegrationPoint& previousPoint, IntegrationPoint& nextPoint)
        {
            return pointSides[&previousPoint] == pointSides[&nextPoint] || pointSides[&previousPoint] == 0 || pointSides[&nextPoint] == 0;
        }

        size_t IntegrationLine::get_index(std::shared_ptr<IntegrationPoint> point)
        {
            for (size_t i = 0; i < Points.size(); i++)
            {
                if (Points[i] == point)
                {
                    return i;
                }
            }
            throw probLibException("index not found in integrationLine");
        }

        size_t IntegrationLine::get_index(IntegrationPoint* point)
        {
            for (size_t i = 0; i < Points.size(); i++)
            {
                if (Points[i].get() == point)
                {
                    return i;
                }
            }
            throw probLibException("index not found in integrationLine");
        }

        bool IntegrationLine::HasNextKnownPoint(std::shared_ptr<IntegrationPoint> point, const DoubleType result)
        {
            size_t index = get_index(point);

            if (index > 0 && index < this->Points.size() - 1)
            {
                for (size_t i = index + 1; i < this->Points.size(); i++)
                {
                    if (this->Points[i]->getKnown() && (this->Points[i]->getResult() == result || this->Points[i]->getResult() == DoubleType::Zero))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        bool IntegrationLine::HasPreviousKnownPoint(std::shared_ptr<IntegrationPoint> point, DoubleType result)
        {
            size_t index = get_index(point);
            if (index > 0 && index < this->Points.size() - 1)
            {
                for (int i = index - 1; i >= 0; i--)
                {
                    if (this->Points[i]->getKnown() && (this->Points[i]->getResult() == result || this->Points[i]->getResult() == DoubleType::Zero))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        IntegrationCell::IntegrationCell(std::shared_ptr<IntegrationDomain> domain, std::vector<double> lowerBoundaries, std::vector<double> upperBoundaries)
        {
            this->domain = domain;
            this->lowerBoundaries = lowerBoundaries;
            this->upperBoundaries = upperBoundaries;
            for(const auto& bnd: lowerBoundaries)
            {
                lowerProbabilities.push_back(Statistics::StandardNormal::getPFromU(bnd));
            }
            for (const auto& bnd : upperBoundaries)
            {
                upperProbabilities.push_back(Statistics::StandardNormal::getPFromU(bnd));
            }
            UpdateCornerPoints();
        }

        void IntegrationCell::UpdateCornerPoints()
        {
            CornerPoints = std::vector<std::shared_ptr<IntegrationPoint>>();
            std::vector<std::vector<double>> allBoundaries = { lowerBoundaries, upperBoundaries };

            for (auto& side : domain->getSplit())
            {
                std::vector<double> coordinates(domain->getDimension());
                for (int i = 0; i < domain->getDimension(); i++)
                {
                    coordinates[i] = allBoundaries[side[i]][i];
                }

                CornerPoints.push_back(domain->GetIntegrationPoint(coordinates));
            }
        }

        double IntegrationCell::getProbability() const
        {
            double probability = 1.0;
            for (size_t i = 0; i < upperProbabilities.size(); i++)
            {
                probability *= std::abs(upperProbabilities[i] - lowerProbabilities[i]);
            }

            return probability;
        }

        size_t IntegrationCell::get_index() const
        {
            for (size_t i = 0; i < domain->Cells.size(); i++)
            {
                if (domain->Cells[i].get() == this) return i;
            }
            throw probLibException("index not found in integration cell");
        }

        void IntegrationCell::split()
        {
            auto index = get_index();

            domain->Cells.erase(domain->Cells.begin()+index);

            for (auto& cornerPoint : this->CornerPoints)
            {
                std::shared_ptr<IntegrationCell> cell = std::make_shared<IntegrationCell>(domain, cornerPoint->Coordinates, getCenterPoint()->Coordinates);
                domain->Cells.push_back(cell);
            }
        }

        void IntegrationCell::split(const std::vector<double>& coordinates)
        {
            auto index = get_index();

            domain->Cells.erase(domain->Cells.begin() + index);

            for (auto cornerPoint : this->CornerPoints)
            {
                std::shared_ptr<IntegrationCell> cell = std::make_shared<IntegrationCell>(domain, cornerPoint->Coordinates, coordinates);
                domain->Cells.push_back(cell);
            }
        }

        void IntegrationCell::update()
        {
            bool allKnown = true;
            for(const auto& p : CornerPoints)
            {
                if ( ! p->getKnown())
                {
                    allKnown = false;
                    break;
                }
            }
            if ( ! allKnown)
            {
                Known = false;
                Result = DoubleType::NaN;
            }
            else
            {
                Known = true;
                std::set<DoubleType> resultsSet;
                for(const auto& p : CornerPoints)
                {
                    resultsSet.insert(p->getResult());
                }

                std::vector<DoubleType> results(resultsSet.begin(), resultsSet.end());
                if (resultsSet.size() == 1)
                {
                    Result = results[0];
                    Determined = true;
                }
                else if (resultsSet.size() == 2)
                {
                    if (results[0] == DoubleType::Zero && (results[1] == DoubleType::Negative || results[1] == DoubleType::Positive))
                    {
                        Result = results[1];
                        Determined = true;
                    }
                    else if (results[1] == DoubleType::Zero && (results[0] == DoubleType::Negative || results[0] == DoubleType::Positive))
                    {
                        Result = results[0];
                        Determined = true;
                    }
                    else
                    {
                        Determined = false;
                    }
                }
                else
                {
                    Determined = false;
                }
            }
        }

        IntegrationDomain::IntegrationDomain(const int n) : Dimension(n)
        {
            split = Mathematics::BinarySupport::GetBinaryList(Dimension, 0, 1);
            for (int i = 0; i < Dimension; i++)
            {
                Lines.push_back(std::vector<std::shared_ptr<IntegrationLine>>());
                LinesSet.push_back(std::unordered_map<std::string, std::shared_ptr<IntegrationLine>>());
            }
        };

        std::shared_ptr<IntegrationPoint> IntegrationDomain::GetIntegrationPoint(std::vector<double>& coordinates)
        {
            std::shared_ptr<IntegrationPoint> integrationPoint;
            for(const auto& point : Points)
            {
                // TODO compare with tolerance
                if (point->Coordinates[0] == coordinates[0] && point->Coordinates[1] == coordinates[1])
                {
                    integrationPoint = point;
                    break;
                }
            }

            if (integrationPoint == nullptr)
            {
                integrationPoint = std::make_shared<IntegrationPoint>(coordinates);
                AddPoint(integrationPoint);
            }

            return integrationPoint;
        }

        void IntegrationDomain::AddPoint(std::shared_ptr<IntegrationPoint> point)
        {
            Points.push_back(point);
            pointsSet.insert({ probLibString::doubles2str(point->Coordinates), point});

            for (int i = 0; i < Dimension; i++)
            {
                auto lineCoordinates = point->Coordinates;
                lineCoordinates.erase(lineCoordinates.begin()+i);

                auto line = LinesSet[i][probLibString::doubles2str(lineCoordinates)];
                if (line == nullptr)
                {
                    line = std::make_shared<IntegrationLine>(i, origin[i]);
                    Lines[i].push_back(line);
                    LinesSet[i][probLibString::doubles2str(lineCoordinates)] = line;
                }

                line->Add(point);
                point->Lines.push_back(line);
            }
        }

        void IntegrationLine::Add(std::shared_ptr<IntegrationPoint> point)
        {
            pointSides[point.get()] = GetSide(point);

            for (int i = 0; i < Points.size(); i++)
            {
                if (Points[i]->Coordinates[getDimension()] > point->Coordinates[getDimension()])
                {
                    Points.insert(Points.begin()+ i, point);
                    return;
                }
            }

            Points.push_back(point);
        }

        int IntegrationLine::GetSide(std::shared_ptr<IntegrationPoint> point) const
        {
            constexpr double tolerance = 1E-10;

            if (NumericSupport::areEqual(point->Coordinates[getDimension()], Origin, tolerance))
            {
                return 0;
            }
            else
            {
                auto diffCoordinates = (point->Coordinates[getDimension()] - Origin);
                if (diffCoordinates > 0) return 1;
                if (diffCoordinates < 0) return -1;
                return 0;
            }
        }

        std::shared_ptr<IntegrationPoint> IntegrationCell::getCenterPoint() const
        {
            std::vector<double> coordinates(domain->getDimension());
            for (int i = 0; i < domain->getDimension(); i++)
            {
                coordinates[i] = (lowerBoundaries[i] + upperBoundaries[i]) / 2;
            }

            auto centerPoint = domain->GetIntegrationPoint(coordinates);

            return centerPoint;
        }

        std::shared_ptr<IntegrationPoint> IntegrationLine::GetPreviousKnownPoint(IntegrationPoint* point)
        {
            auto index = get_index(point);
            if (index > 0 && index < Points.size() - 1)
            {
                for (int i = index - 1; i >= 0; i--)
                {
                    if (Points[i]->getKnown())
                    {
                        return Points[i];
                    }
                }
            }

            return nullptr;
        }

        std::shared_ptr<IntegrationPoint> IntegrationLine::GetNextKnownPoint(IntegrationPoint* point)
        {
            auto index = get_index(point);
            if (index > 0 && index < Points.size() - 1)
            {
                for (int i = index + 1; i < Points.size(); i++)
                {
                    if (Points[i]->getKnown())
                    {
                        return Points[i];
                    }
                }
            }

            return nullptr;
        }

    }
}
