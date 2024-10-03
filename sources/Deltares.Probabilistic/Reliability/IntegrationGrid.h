#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "ReliabilityMethod.h"
#include "NumericalBisectionSettings.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Reliability
    {
        // forward declarations:
        class IntegrationPoint;
        class IntegrationCell;
        class IntegrationLine;

        class IntegrationDomain
        {
        public:
            IntegrationDomain(const int n);
            void SetOrigin(const std::vector<double>& origin)
            {
                this->origin = origin;
            }
            std::vector<std::shared_ptr<IntegrationPoint>> Points;
            std::unordered_map<std::string,std::shared_ptr<IntegrationPoint>> pointsSet;
            std::vector<std::shared_ptr<IntegrationCell>> Cells;
            std::vector <std::vector<std::shared_ptr<IntegrationLine>>> Lines;
            std::vector<std::unordered_map<std::string,std::shared_ptr<IntegrationLine>>> LinesSet;
            std::shared_ptr<IntegrationPoint> GetIntegrationPoint(std::vector<double>& coordinates);
            void AddPoint(std::shared_ptr<IntegrationPoint> point);
            int getDimension() const { return Dimension; }
            std::vector<std::vector<int>> getSplit() const { return split; }
        private:
            std::vector<std::vector<int>> split;
            int Dimension;
            std::vector<double> origin;
        };

        class IntegrationPoint
        {
        public:
            IntegrationPoint(std::vector<double> coord) : Coordinates (coord) {}
            double ZValue = std::nan("");
            double ProbabilityDensity() const;
            void deriveByExtrapolation();
            void derive();
            std::vector<double> Coordinates;
            Numeric::DoubleType getResult() const { return Numeric::NumericSupport::getDoubleType(ZValue); };
            std::vector<std::shared_ptr<IntegrationLine>> Lines;
            void setKnown(const bool b) { Known = b; }
            bool getKnown() const { return Known; }
            void setZValue(const double z) { ZValue = z; }
        private:
            bool Known = false;
        };

        class IntegrationCell
        {
        public:
            IntegrationCell(std::shared_ptr<IntegrationDomain> domain, std::vector<double> lowerBoundaries, std::vector<double> upperBoundaries);
            bool Determined = false;
            bool Known = false;
            double getProbability() const;
            void split();
            void split(const std::vector<double>& origin);
            void update();
            Numeric::DoubleType Result = Numeric::NaN;
            std::vector<std::shared_ptr<IntegrationPoint>> CornerPoints;
            std::shared_ptr<IntegrationPoint> getCenterPoint() const;
        private:
            size_t get_index() const;
            std::vector<double> lowerBoundaries;
            std::vector<double> upperBoundaries;
            std::vector<double> lowerProbabilities;
            std::vector<double> upperProbabilities;
            std::shared_ptr<IntegrationDomain> domain;
            void UpdateCornerPoints();
        };

        class IntegrationLine
        {
        public:
            IntegrationLine(int dimension, double origin) : Dimension(dimension), Origin(origin) {};
            std::unordered_map<IntegrationPoint*, int> pointSides = std::unordered_map<IntegrationPoint*, int>();
            std::vector<std::shared_ptr<IntegrationPoint>> Points;
            std::shared_ptr<IntegrationPoint> GetPreviousKnownPoint(IntegrationPoint* p);
            std::shared_ptr<IntegrationPoint> GetNextKnownPoint(IntegrationPoint* p);
            bool OnSameSide(IntegrationPoint& p1, IntegrationPoint& p2);
            bool HasNextKnownPoint(std::shared_ptr<IntegrationPoint> point, const Numeric::DoubleType r);
            bool HasPreviousKnownPoint(std::shared_ptr<IntegrationPoint> point, const Numeric::DoubleType r);
            size_t get_index(std::shared_ptr<IntegrationPoint> point);
            size_t get_index(IntegrationPoint* point);
            int getDimension() const { return Dimension; }
        private:
            int Dimension;
            double Origin;
        };

    }
}

