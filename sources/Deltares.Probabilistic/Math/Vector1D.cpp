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
#include <cmath>
#include "Vector1D.h"
#include <fstream>

namespace Deltares::Numeric
{
    Vector1D::Vector1D() : m_data(0), m_rows(0) {}

    Vector1D::Vector1D(size_t rows) : m_data(rows), m_rows(rows)
    {
        for (size_t pos = 0; pos < m_rows; pos++)
        {
            m_data[pos] = 0.0;
        }
    }

    Vector1D::Vector1D(const Vector1D& m) : m_data(m.m_rows), m_rows(m.m_rows)
    {
        for (size_t pos = 0; pos < m_rows; pos++)
        {
            m_data[pos] = m.m_data[pos];
        }
    }

    Vector1D::Vector1D(const std::initializer_list<double>& m) : m_data(m.size()), m_rows(m.size())
    {
        size_t pos = 0;
        for (auto x : m)
        {
            m_data[pos] = x;
            pos++;
        }
    }

    Vector1D::Vector1D(Vector1D&& m) noexcept : m_data(m.m_data), m_rows(m.m_rows)
    {
        m.m_data = std::vector<double>(0);
        m.m_rows = 0;
    }

    Vector1D& Vector1D::operator=(const Vector1D& m)
    {
        if (this != &m)
        {
            m_rows = m.m_rows;

            m_data = std::vector<double>(m_rows);

            for (size_t pos = 0; pos < m_rows; pos++)
            {
                m_data[pos] = m.m_data[pos];
            }
        }

        return *this;
    }

    Vector1D& Vector1D::operator=(Vector1D&& m) noexcept
    {
        std::swap(m_data, m.m_data);
        std::swap(m_rows, m.m_rows);
        return *this;
    }

    Vector1D Vector1D::operator+(const Vector1D& m) const
    {
        if (m_rows != m.m_rows)
        {
            throw Reliability::ProbabilisticLibraryException("number of rows differ");
        }

        Vector1D result(m_rows);

        for (size_t pos = 0; pos < m_rows; pos++)
        {
            result.m_data[pos] = m_data[pos] + m.m_data[pos];
        }

        return result;
    }

    Vector1D Vector1D::operator*(double d) const
    {
        Vector1D result(m_rows);

        for (size_t pos = 0; pos < m_rows; pos++)
        {
            result.m_data[pos] = m_data[pos] * d;
        }

        return result;
    }

    Vector1D operator*(double d, const Vector1D& m)
    {
        return m * d;
    }

    std::ostream& operator<<(std::ostream& os, const Vector1D& m)
    {
        for (size_t row = 0; row < m.m_rows; row++)
        {
            os << m(row) << ", ";

        }
        os << std::endl;

        return os;
    }

    void Vector1D::operator*=(double d)
    {
        for (size_t row = 0; row < m_rows; row++)
        {
            m_data[row] *= d;
        }
    }

    void Vector1D::operator+=(const Vector1D& m)
    {
        if (m_rows != m.m_rows)
        {
            throw Reliability::ProbabilisticLibraryException("number of rows differ");
        }
        for (size_t row = 0; row < m_rows; row++)
        {
            m_data[row] += m.m_data[row];
        }
    }

    size_t Vector1D::size() const
    {
        return m_rows;
    }

    double Vector1D::sumOfSquares() const
    {
        auto sum = 0.0;
        for (size_t k = 0; k < m_rows; k++)
        {
            sum += (m_data[k] * m_data[k]);
        }
        return sum;
    }

    double Vector1D::sumOfInner(const Vector1D& m) const
    {
        if (m_rows != m.m_rows)
        {
            throw Reliability::ProbabilisticLibraryException("number of rows differ");
        }
        auto sum = 0.0;
        for (size_t k = 0; k < m_rows; k++)
        {
            sum += (m_data[k] * m.m_data[k]);
        }
        return sum;
    }

    double Vector1D::sumOfInners(const Vector1D& m, const Vector1D& n) const
    {
        if (m_rows != m.m_rows || m_rows != n.m_rows)
        {
            throw Reliability::ProbabilisticLibraryException("number of rows differ");
        }
        auto sum = 0.0;
        for (size_t k = 0; k < m_rows; k++)
        {
            sum += (m_data[k] * m.m_data[k] * n.m_data[k]);
        }
        return sum;
    }

    void Vector1D::assign(const double x)
    {
        for (size_t k = 0; k < m_rows; k++)
        {
            m_data[k] = x;
        }
    }

    double Vector1D::norm() const
    {
        return sqrt(sumOfSquares());
    }

    void Vector1D::normalize()
    {
        double s = norm();
        if (s > 0.0)
        {
            double r = 1.0 / s;
            for (size_t k = 0; k < m_rows; k++)
            {
                m_data[k] *= r;
            }
        }
        else
        {
            double r = 1.0 / sqrt((double)m_rows);
            for (size_t k = 0; k < m_rows; k++)
            {
                m_data[k] = r;
            }
        }
    }

    double Vector1D::minval() const
    {
        if (m_rows == 0) return nan("");

        double m = m_data[0];
        for (size_t k = 1; k < m_rows; k++)
        {
            m = std::min(m, m_data[k]);
        }
        return m;
    }

    double Vector1D::maxval() const
    {
        if (m_rows == 0) return nan("");

        double m = m_data[0];

        for (size_t k = 1; k < m_rows; k++)
        {
            m = std::max(m, m_data[k]);
        }
        return m;
    }

    void Vector1D::dumpResult(std::ofstream& o) const
    {
        o << m_rows << std::endl;
        for (size_t i = 0; i < m_rows; i++)
        {
            o << m_data[i] << std::endl;
        }
    }

    Vector1D Vector1D::readDumpFile(std::fstream& o)
    {
        size_t n;
        o >> n;
        Vector1D x(n);
        for (size_t i = 0; i < n; i++)
        {
            o >> x(i);
        }
        return x;
    }
}
