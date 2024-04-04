#pragma once
#include <iosfwd>
#include <vector>
#include <initializer_list>
#include "../Utils/probLibException.h"

namespace Deltares {
    namespace Reliability {

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
