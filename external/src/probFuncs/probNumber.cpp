#include "probNumber.h"
#include "conversions.h"
#include "../utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

        probNumber::probNumber()
        {
            isdirtyPQ = false;
            isdirtyU = false;
            u = 0.0;
            p = 0.5;
            q = 0.5;
            i = 'u';
        }

        probNumber::probNumber(const double beta)
        {
            u = beta;
            isdirtyPQ = true;
            isdirtyU = false;
            i = 'u';
        }

        probNumber::probNumber(const double pq, const char f)
        {
            if (f == 'p')
            {
                p = pq;
                q = 1.0 - pq; // losing digits if p near 0
            }
            else if (f == 'q')
            {
                q = pq;
                p = 1.0 - pq; // losing digits if q near 0
            }
            else { throw probLibException("invalid value for f in constructor probNumber"); }
            if (pq < 0.0 || pq > 1.0)
            {
                throw probLibException("pq must be between 0 and 1 in constructor probNumber");
            }

            isdirtyPQ = false;
            isdirtyU = true;
            i = f;
        }

        double probNumber::getu()
        {
            if (isdirtyU)
            {
                // use the value that is given with constructor;
                // 1 - pq is always less accurate as pq
                u = (i == 'q' ? conversions::betaFromQ(q) : conversions::betaFromP(p));
                isdirtyU = false;
            }
            return u;
        }

        double probNumber::getp()
        {
            if (isdirtyPQ)
            {
                conversions::PQfromBeta(u, p, q);
                isdirtyPQ = false;
            }
            return p;
        }

        double probNumber::getq()
        {
            if (isdirtyPQ)
            {
                conversions::PQfromBeta(u, p, q);
                isdirtyPQ = false;
            }
            return q;
        }
    }
}
