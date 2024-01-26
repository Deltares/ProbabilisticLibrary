#include "funcWrapper.h"

double funcWrapper::FDelegate(Sample* s)
{
    auto xx = new double[allStoch];
    for (size_t i = 0; i < allStoch; i++)
    {
        xx[i] = xRef[i];
    }
    for (size_t i = 0; i < s->getSize(); i++)
    {
        xx[iPointer[i]] = s->Values[i];
    }
    auto i = new int[4];
    tError e = tError();
    double result = staticF(xx, i, &e);
    delete[] xx;
    delete[] i;
    s->Z = result;
    return result;
}
