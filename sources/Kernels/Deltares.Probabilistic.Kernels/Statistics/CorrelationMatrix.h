#pragma once
#include "../Math/matrix.h"

class CorrelationMatrix
{
public:
    void init(const int maxStochasts);
    double* Cholesky(double* uValues, int count);
    void SetCorrelation(const int i, const int j, const double value);
    void CholeskyDecomposition();
private:
    Deltares::ProbLibCore::Matrix matrix = Deltares::ProbLibCore::Matrix(0,0);
    Deltares::ProbLibCore::Matrix choleskyMatrix = Deltares::ProbLibCore::Matrix(0, 0);
};

