#include "funcWrapper.h"
#include "../Deltares.Probabilistic.Kernels/Utils/probLibException.h"

using namespace Deltares::ProbLibCore;

void funcWrapper::FDelegate(std::shared_ptr<Deltares::Models::Sample> s)
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
    i[0] = compIds[0];
    tError e = tError();
    double result = zfunc(xx, i, &e);
    delete[] xx;
    delete[] i;
    if (e.errorCode != 0) throw probLibException(e.errorMessage);
    s->Z = result;
}
