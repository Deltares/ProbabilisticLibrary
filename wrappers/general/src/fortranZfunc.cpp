#include "fortranZfunc.h"
#include "../../../src/utils/probLibException.h"

using namespace Deltares::ProbLibCore;

fortranZfunc::fortranZfunc(const double(*f)(double[], int[], tError*), const size_t n, const size_t vectorSize,
  int compIds[], int iPoint[], double xin[], const size_t maxThreads) : zmodel(maxThreads), fx(f), nrStoch(n), ipoint(iPoint)
{
    for (size_t i = 0; i < numcores; i++)
    {
        auto y = new double[vectorSize];
        for (size_t j = 0; j < vectorSize; j++)
        {
            y[j] = xin[j];
        }
        x.push_back(y);
    }
    ids = compIds;
};

fortranZfunc::~fortranZfunc()
{
    for (size_t i = 0; i < numcores; i++)
    {
        delete[] x[i];
    }
}

double fortranZfunc::zfunc(const probDraw& d) const
{
    return zfunc(d, 0);
}

double fortranZfunc::zfunc(const probDraw & d, const int iThread) const
{
    for (size_t i = 0; i < nrStoch; i++)
    {
        x[iThread][ipoint[i]] = d.getx(i);
    }
    auto e = tError();
    int copyIds[3];
    copyIds[0] = ids[0];
    copyIds[1] = ids[1];
    copyIds[2] = iThread;
    double y = fx(x[iThread], copyIds, &e);
    if (e.errorCode != 0)
    {
        std::string s = e.errorMessage;
        throw probLibException(s);
    }
    return y;
};

double fortranZfunc::zfunc(const probDraw& d, const OutputFlags option, vector1D& xout) const
{
    auto iThread = 0;
    auto vectorSize = xout.size();
    for (size_t i = 0; i < nrStoch; i++)
    {
        x[iThread][ipoint[i]] = d.getx(i);
    }
    auto e = tError();
    auto backupIds0 = ids[0];
    ids[0] = (int)option;
    double y = fx(x[iThread], ids, &e);
    for (size_t i = 0; i < vectorSize; i++)
    {
        xout(i) = x[iThread][i];
    }
    ids[0] = backupIds0;
    return y;
}

double fortranZfunc::zfunc(const vector1D & v) const
{
    auto iThread = 0;
    for (size_t i = 0; i < v.size(); i++)
    {
        x[iThread][ipoint[i]] = v(i);
    }
    auto e = tError();
    double y = fx(x[iThread], ids, &e);
    if (e.errorCode != 0)
    {
        std::string s = e.errorMessage;
        throw probLibException(s);
    }
    return y;
}
