#include <omp.h>
#include <cmath>
#include <iostream>
#include "zmodel.h"

namespace Deltares {
    namespace ProbLibCore {

        // basic implementation for getZandDerivatives
        // could be overwritten by methods that:
        // - only computes single sided derivatives
        void zmodel::getZandDerivatives(const vector1D& drwu, const stochSettings& s,
            double& z, vector1D& dzdu, const double du) const
        {
            if (numcores > 1)
            {
                getZandDerivatives_omp(drwu, s, z, dzdu, du);
            }
            else
            {
                getZandDerivatives_seq(drwu, s, z, dzdu, du);
            }
        }

        inline
            double derivative(const double z, const double zplus, const double zmin, const double du)
        {
            if (!std::isfinite(z))
                throw probLibException("z = NaN");

            if (std::isfinite(zplus) && std::isfinite(zmin))
                return (zplus - zmin) / du;

            const double halfDu = du * 0.5;
            if (std::isfinite(zplus))
                return (zplus - z) / halfDu;

            if (std::isfinite(zmin))
                return (z - zmin) / halfDu;

            throw probLibException("NaN in derivate");
        }

        // sequential implementation
        void zmodel::getZandDerivatives_seq(const vector1D& drwZ, const stochSettings& s, double& z, vector1D& dzdu, const double du) const
        {
            auto nrStochasts = dzdu.size();
            auto drw = probDraw(nrStochasts, drwZ, s);
            z = zfunc(drw);
            auto drwu = drwZ;
            for (size_t k = 0; k < nrStochasts; k++)
            {
                if (s.isFullyCorrelated(k))
                {
                    dzdu(k) = 0.0;
                }
                else
                {
                    auto uorig = drwZ(k);
                    drwu(k) = uorig - 0.5 * du;
                    auto drwplus = probDraw(nrStochasts, drwu, s);
                    auto zmin = zfunc(drwplus);
                    drwu(k) = uorig + 0.5 * du;
                    auto drwmin = probDraw(nrStochasts, drwu, s);
                    auto zplus = zfunc(drwmin);
                    dzdu(k) = derivative(z, zplus, zmin, du);
                    drwu(k) = uorig;
                }
            }
        }

        // OMP implementation for getZandDerivatives
        void zmodel::getZandDerivatives_omp(const vector1D& drwZ, const stochSettings& s, double& z, vector1D& dzdu, const double du) const
        {
            int nrStochasts = (int)dzdu.size();
            auto drwu = std::vector<vector1D>(2 * nrStochasts + 1);
            auto zValues = vector1D(2 * nrStochasts + 1);
            auto notNeeded = vector1D(2 * nrStochasts + 1);
            size_t ii = 0;
            for (int k = 0; k < nrStochasts; k++)
            {
                notNeeded(ii) = (s.isFullyCorrelated(k));
                drwu[ii] = drwZ;
                auto uorig = drwZ(k);
                drwu[ii](k) = uorig - 0.5 * du;
                ii++;
                notNeeded(ii) = (s.isFullyCorrelated(k));
                drwu[ii] = drwZ;
                drwu[ii](k) = uorig + 0.5 * du;
                ii++;
            }
            drwu[ii] = drwZ;
            notNeeded(ii) = false;

#pragma omp parallel for num_threads(numcores)
            for (int k = 0; k < 2 * nrStochasts + 1; k++)
            {
                int thread = omp_get_thread_num();
                //std::cout << "thread: " << thread << std::endl;
#ifndef __cplusplus_cli
                try
                {
#endif
                    if (notNeeded(k))
                    {
                        zValues(k) = 0.0;
                    }
                    else
                    {
                        auto drw = probDraw(nrStochasts, drwu[k], s);
                        zValues(k) = zfunc(drw, thread);
                    }
#ifndef __cplusplus_cli
                }
                catch (const std::exception& e)
                {
                    std::cout << e.what() << std::endl;
                    zValues(k) = nan("");
                }
#endif
            }

            z = zValues(2 * nrStochasts);
            for (int k = 0; k < nrStochasts; k++)
            {
                dzdu(k) = derivative(z, zValues(2 * k + 1), zValues(2 * k), du);
            }
        }
    }
}
