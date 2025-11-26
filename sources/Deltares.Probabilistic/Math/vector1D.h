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
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Numeric
    {
        using Deltares::Reliability::probLibException;

        class vector1D
        {
        public:
            vector1D();
            vector1D(size_t rows);
            vector1D(const vector1D& m);
            vector1D(const std::initializer_list<double>& m);
            vector1D(vector1D&& m) noexcept;

            vector1D& operator=(const vector1D& m);
            vector1D& operator=(vector1D&& m) noexcept;

            vector1D operator+(const vector1D& m) const;
            vector1D operator*(double d) const;
            void operator*=(double d);
            void operator+=(const vector1D& m);

            friend vector1D operator*(double d, const vector1D& m);
            friend std::ostream& operator<<(std::ostream& os, const vector1D& m);

            double& operator()(size_t row)
            {
                if (row >= m_rows) throw probLibException("array bound error.");
                return m_data[row];
            }
            const double& operator()(size_t row) const
            {
                if (row >= m_rows) throw probLibException("array bound error.");
                return m_data[row];
            }

            size_t size() const;
            double sumOfSquares() const;
            double sumOfInner(const vector1D& m) const;
            double sumOfInners(const vector1D& m, const vector1D& n) const;
            void assign(const double x);
            double norm();
            void normalize();
            double minval() const;
            double maxval() const;
            void dumpResult(std::ofstream& o) const;
            static vector1D readDumpFile(std::fstream& o);

        private:
            std::vector<double> m_data;
            size_t m_rows;
        };
    }
}
