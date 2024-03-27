#include "funcWrapper.h"
#include "../Deltares.Probabilistic/Utils/probLibException.h"

namespace Deltares
{
	namespace Models
	{
		class ModelSample;
	}
}

using namespace Deltares::ProbLibCore;

void funcWrapper::FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s)
{
    auto xx = new double[allStoch];
    for (size_t i = 0; i < allStoch; i++)
    {
        xx[i] = xRef[i];
    }
    for (size_t i = 0; i < s->Values.size(); i++)
    {
        xx[iPointer[i]] = s->Values[i];
    }
    int i[sizeIntArray];
    i[0] = compIds[0];
    tError e = tError();
    double result = zfunc(xx, i, &e);
    delete[] xx;
    if (e.errorCode != 0) throw probLibException(e.errorMessage);
    s->Z = result;
}

void funcWrapper::updateXinDesignPoint(double x[])
{
    int i[sizeIntArray];
    i[0] = 0;            // set design output on
    tError e = tError();
    zfunc(x, i, &e);
}

