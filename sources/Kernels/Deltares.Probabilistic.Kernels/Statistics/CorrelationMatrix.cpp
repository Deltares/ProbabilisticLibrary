#include "CorrelationMatrix.h"

using namespace Deltares::ProbLibCore;

double* CorrelationMatrix::Cholesky(double* uValues, int count)
{
    size_t m1; size_t m2;
    choleskyMatrix.get_dims(m1, m2);
    if (m1 == 0) CholeskyDecomposition();

    auto u = vector1D(count);
    for (size_t i = 0; i < count; i++)
    {
        u(i) = uValues[i];
    }

    auto uNew = choleskyMatrix.matvec(u);

    double* correlatedValues = new double[count];

    for (int i = 0; i < count; i++)
    {
        correlatedValues[i] = uNew(i);
    }

    return correlatedValues;
}

void CorrelationMatrix::init(const int maxStochasts)
{
    matrix = Matrix(maxStochasts, maxStochasts);
}

void CorrelationMatrix::SetCorrelation(const int i, const int j, const double value)
{
    size_t m1; size_t m2;
    matrix.get_dims(m1, m2);
    if (std::max(i, j) >= m1)
    {
        throw probLibException("dimension mismatch in addCorrelation");
    }
    matrix(i, j) = value;
    matrix(j, i) = value;
}

void CorrelationMatrix::CholeskyDecomposition()
{
    choleskyMatrix = matrix.CholeskyDecomposition();
}

