#include <math.h>
#include "mtable.h"
#include "../utils/basic_math.h"
#include "../utils/probLibException.h"
#include "../utils/probLibString.h"

namespace Deltares {
    namespace ProbLibCore {

        mtable::mtable(const Matrix& t, const extrapolation xp, const size_t N) :tvalues(t), xPol(xp), MaxYvalues(N)
        {
            size_t rows; size_t cols;
            tvalues.get_dims(rows, cols);

            if (xp == extrapolation::basicLinear)
            {
                indexFirst = 1;
                indexLast = rows - 2;
            }
            else
            {
                indexFirst = rows - 1;
                indexLast = 0;
            }

            for (size_t i = 1; i < rows; i++)
            {
                if (tvalues(i - 1, 0) >= tvalues(i, 0))
                {
                    throw probLibException("u-values in table are not increasing.");
                }
            }

        }

        std::vector<double> mtable::getValue(const double x) const
        {
            size_t rows; size_t cols;
            tvalues.get_dims(rows, cols);
            size_t i0;
            size_t i1;
            bool justOne = false;

            if (x < tvalues(0, 0))
            {
                if (xPol == extrapolation::constant)
                {
                    justOne = true;
                    i0 = 0;
                }
                else
                {
                    i0 = 0;
                    i1 = indexFirst;
                }
            }
            else if (x > tvalues(rows - 1, 0))
            {
                if (xPol == extrapolation::constant)
                {
                    justOne = true;
                    i0 = rows - 1;
                }
                else
                {
                    i0 = indexLast;
                    i1 = rows - 1;
                }
            }
            else
            {
                i0 = getValueInBetween(x);
                i1 = i0 + 1;
            }

            auto y = std::vector<double>(MaxYvalues);
            for (size_t j = 1; j <= MaxYvalues; j++)
            {
                if (justOne)
                {
                    y[j - 1] = tvalues(i0, j);
                }
                else
                {
                    y[j - 1] = interpLine(x, tvalues(i0, 0), tvalues(i1, 0),
                        tvalues(i0, j), tvalues(i1, j));
                }
            }
            return y;
        }

        size_t mtable::getValueInBetween(const double x) const
        {
            size_t rows; size_t cols;
            tvalues.get_dims(rows, cols);
            for (size_t i = 1; i < rows; i++)
            {
                if (x <= tvalues(i, 0))
                {
                    return i - 1;
                }
            }

            throw probLibException("invalid case for mtable::getValueInBetween");
        }

        // wrapper around mtable to use it as an distribution
        dtable::dtable(const Matrix& t, const extrapolation xp, const size_t N) : cp(Matrix(t))
        {
            table = new mtable(cp, xp, N);
        };

        dtable::dtable(const std::string& s, const Matrix& t, const extrapolation xp, const size_t N)
            : distrib(s, 0), cp(Matrix(t))
        {
            table = new mtable(cp, xp, N);
        };

        dtable::~dtable()
        {
            delete table;
        }

        double dtable::getValue(const double u) const
        {
            auto x = table->getValue(u);
            return x[0];
        }

        std::string mtable::getRange(const size_t row) const
        {
            size_t rows; size_t cols;
            tvalues.get_dims(rows, cols);
            auto pls = probLibString();

            auto s = pls.trim(pls.double2str(tvalues(0, row)), " ");
            s += " ... ";
            s += pls.trim(pls.double2str(tvalues(rows - 1, row)), " ");
            return s;
        }

        std::string mtable::getSummary() const
        {
            size_t rows; size_t cols;
            tvalues.get_dims(rows, cols);

            std::string s = "u: ";
            s += getRange(0);
            s += "; x: ";
            s += getRange(cols - 1);

            return s;
        }

        std::string dtable::getSummary(const std::vector<double>& p4) const
        {
            return table->getSummary();
        }
    }
}
