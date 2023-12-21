#include "stochSettings.h"

namespace Deltares {
    namespace ProbLibCore {

        stochSettings::stochSettings(const correlation& c, listDistribs& p) : corr(c), params(p)
        {
            nrStochasts = calcNrStochasts();
        }

        stochSettings::stochSettings(const correlation& c, listDistribs& p, const bool ip) :
            corr(c), useIpoint(ip), params(p)
        {
            nrStochasts = calcNrStochasts();
        }

        size_t stochSettings::getNrStochasts() const
        {
            return nrStochasts;
        }

        size_t stochSettings::getNrAllStochasts() const
        {
            return params.size();
        }

        size_t stochSettings::calcNrStochasts()
        {
            if (useIpoint)
            {
                int j = 0;
                for (size_t i = 0; i < params.size(); i++)
                {
                    auto p = params.getDistrib(i);
                    if (p->Type() == distEnum::combined) continue;
                    if (p->isDeterminist()) continue;
                    j++;
                }
                return j;
            }
            else
            {
                return params.size();
            }
        }

        std::pair<size_t, bool> stochSettings::getStochId(const size_t i) const
        {
            std::pair<size_t, bool> retVal;
            if (useIpoint)
            {
                size_t j = 0;
                for (size_t k = 0; k < params.size(); k++)
                {
                    auto d = params.getDistrib(k);
                    if (d->Type() == distEnum::combined) continue;
                    if (d->isDeterminist()) continue;
                    if (i == k)
                    {
                        retVal.first = j;
                        retVal.second = true;
                        return retVal;
                    }
                    j++;
                }
                retVal.second = false;
            }
            else
            {
                retVal.first = i;
                retVal.second = true;
            }
            return retVal;
        }

        void stochSettings::updateU(vector1D& u) const
        {
            corr.updateU(u);
        }

        const distrib* stochSettings::getParamI(const size_t i) const
        {
            return params.getDistrib(i);
        }
    }
}
