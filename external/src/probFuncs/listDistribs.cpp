#include "listDistribs.h"

namespace Deltares {
    namespace ProbLibCore {

        listDistribs::~listDistribs()
        {
            for (size_t i = 0; i < params.size(); i++)
            {
                delete params[i];
            }
        }

        void listDistribs::addDistrib(distrib* d)
        {
            params.push_back(d);
        }

        distrib* listDistribs::getDistrib(const size_t i)
        {
            return params[i];
        }

        void listDistribs::updateDistrib(const size_t i, distrib* d)
        {
            delete params[i];
            params[i] = d;
        }
    }
}
