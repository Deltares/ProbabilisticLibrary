#pragma once
#include "../Math/matrix.h"
#include <vector>

public class CorrelationMatrix
{
public:
    void init(const int maxStochasts);
    std::vector<double> Cholesky(const std::vector<double> & uValues);
    void SetCorrelation(const int i, const int j, const double value);
    void CholeskyDecomposition();
    bool checkFullyCorrelated(const int i);
    void filter(const CorrelationMatrix* m, const std::vector<int> & index);
    std::pair<int, double> findDependent(const int i);
private:
    Deltares::ProbLibCore::Matrix matrix = Deltares::ProbLibCore::Matrix(0,0);
    Deltares::ProbLibCore::Matrix choleskyMatrix = Deltares::ProbLibCore::Matrix(0, 0);
    int findNewIndex(const std::vector<int> index, const size_t i);
};

