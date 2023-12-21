#include "probDraw.h"
#include "../correlation/identity.h"
#include "../correlation/gaussianCorrelation.h"
#include "../utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

        probDraw::probDraw(const size_t n, const double x, const stochSettings& s) : u(n), stochParams(s)
        {
            if (s.getNrStochasts() != n)
            {
                throw probLibException("sizes for nrStochasts and size stochParams must be the same.");
            }
            init_corr(s);
        }

        probDraw::probDraw(const size_t n, const vector1D& x, const stochSettings& s) : u(x), stochParams(s)
        {
            if (s.getNrStochasts() != n)
            {
                throw probLibException("sizes for nrStochasts and size stochParams must be the same.");
            }
            init_corr(s);
        }

        void probDraw::init_corr(const stochSettings& s)
        {
            s.updateU(u);
        }

        void probDraw::setu(const vector1D& uu)
        {
            u = uu;
            stochParams.updateU(u);
        }

        double probDraw::getx(size_t i) const
        {
            const auto p = stochParams.getParamI(i);
            if (p->Type() == distEnum::combined)
            {
                double x1 = getx(p->indexIndependent);
                double x2 = getx(p->indexDependent);
                return p->getx(x1, x2);
            }

            auto id = stochParams.getStochId(i);
            double x;
            if (id.second)
            {
                x = p->getValue(u(id.first));
            }
            else
            {
                x = p->getValue(0.0);
            }
            return x;
        }
    }
}
