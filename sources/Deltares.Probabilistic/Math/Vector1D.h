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
#pragma once
#include <iosfwd>
#include <vector>
#include <initializer_list>
#include "../Utils/ProbabilisticLibraryException.h"

namespace Deltares::Numeric
{
    class Vector1D
    {
    public:
        Vector1D();
        Vector1D(size_t rows);
        Vector1D(const Vector1D& m);
        Vector1D(const std::initializer_list<double>& m);
        Vector1D(Vector1D&& m) noexcept;

        Vector1D& operator=(const Vector1D& m);
        Vector1D& operator=(Vector1D&& m) noexcept;

        Vector1D operator+(const Vector1D& m) const;
        Vector1D operator*(double d) const;
        void operator*=(double d);
        void operator+=(const Vector1D& m);

        friend Vector1D operator*(double d, const Vector1D& m);
        friend std::ostream& operator<<(std::ostream& os, const Vector1D& m);

        double& operator()(size_t row)
        {
            if (row >= m_rows) throw Reliability::ProbabilisticLibraryException("array bound error.");
            return m_data[row];
        }
        const double& operator()(size_t row) const
        {
            if (row >= m_rows) throw Reliability::ProbabilisticLibraryException("array bound error.");
            return m_data[row];
        }

        size_t size() const;
        double sumOfSquares() const;
        double sumOfInner(const Vector1D& m) const;
        double sumOfInners(const Vector1D& m, const Vector1D& n) const;
        void assign(const double x);
        double norm() const;
        void normalize();
        double minval() const;
        double maxval() const;
        void dumpResult(std::ofstream& o) const;
        static Vector1D readDumpFile(std::fstream& o);

    private:
        std::vector<double> m_data;
        size_t m_rows;
    };
}
