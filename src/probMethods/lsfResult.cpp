#include <iostream>
#include <iomanip>
#include "lsfResult.h"
#include "../probFuncs/probDraw.h"
#include "../utils/probLibString.h"
#include "../utils/basic_math.h"

namespace Deltares {
    namespace ProbLibCore {

        lsfResult::lsfResult(const size_t n) : result(-999.0, n), x(0), u(0) {}
        lsfResult::lsfResult(const size_t n, const DPoptions d) : result(-999.0, n), x(0), u(0), designPointOption(d) {}
        lsfResult::~lsfResult() {};

        void lsfResult::calcXinDesignPoint(const stochSettings& s)
        {
            switch (designPointOption) {
            case DPoptions::XfromU:
            case DPoptions::XCorrelatedFromU:
            case DPoptions::XCorrelatedFromUCompatible:
                calcXfromAlpha(s);
                break;
            case DPoptions::None: {
                auto nrStochasts = s.getNrAllStochasts();
                x = vector1D(nrStochasts);
            }
                                break;
            case DPoptions::RMin: {
                auto useU = s.getNrStochasts() == u.size();
                if (useU)
                {
                    calcXfromU(s);
                }
                else
                {
                    calcXfromAlpha(s);
                }}
                                break;
            case DPoptions::MultiplyInvCholesky: {
                calcXfromAlpha(s);
                alpha_u = result.getAlpha();
                auto newAlpha = s.applyCholesky(result.getAlpha());
                result.setAlpha(newAlpha);
            }
                                               break;
            case DPoptions::RMinZFunc:
            case DPoptions::RMinZFuncCompatible:
                // only implemented below, where zmodel is also given
                throw probLibException("Internal error: zfunc not available in designpoint calculation.");
                break;
            default:
                throw probLibException("Unknown design point option: ", (int)designPointOption);
                break;
            }
        }

        void lsfResult::calcXinDesignPoint(const zmodel& m, const stochSettings& s)
        {
            switch (designPointOption) {
            case DPoptions::RMinZFunc:
            case DPoptions::RMinZFuncCompatible: {
                auto nrAllStochasts = s.getVectorSize();
                x = vector1D(nrAllStochasts);
                auto nrStochasts = s.getNrStochasts();
                if (u.size() == 0)
                {
                    auto drwu = result.getAlpha();
                    drwu *= -result.getBeta();
                    auto drw = probDraw(nrStochasts, drwu, s);
                    m.zfunc(drw, OutputFlags::designPointOutputTRUE, x);
                }
                else
                {
                    auto drw = probDraw(nrStochasts, u, s);
                    m.zfunc(drw, OutputFlags::designPointOutputTRUE, x);
                }}
                                               break;
            default:
                calcXinDesignPoint(s);
                break;
            }
        }

        void lsfResult::calcXfromAlpha(const stochSettings& s)
        {
            auto nrStochasts = s.getNrAllStochasts();
            x = vector1D(nrStochasts);
            auto drwu = result.getAlpha();
            drwu *= -result.getBeta();
            auto p = probDraw(s.getNrStochasts(), drwu, s);
            for (size_t i = 0; i < nrStochasts; i++)
            {
                x(i) = p.getx(i);
            }
        }

        void lsfResult::calcXfromU(const stochSettings& s)
        {
            auto nrStochasts = s.getNrAllStochasts();
            x = vector1D(nrStochasts);
            auto p = probDraw(s.getNrStochasts(), u, s);
            for (size_t i = 0; i < nrStochasts; i++)
            {
                x(i) = p.getx(i);
            }
        }

        void lsfResult::printResult(std::ostream& o, const stochSettings& s)
        {
            if (convergence == ConvergenceStatus::success)
            {
                o << method << " did converge.";
            }
            else
            {
                o << method << " did NOT converge.";
            }

            auto steps = max(stepsNeeded, samplesNeeded);
            o << " Needed " << steps << " step(s)." << std::endl;

            auto pls = probLibString();
            o << "beta = " << pls.double2str(result.getBeta()) << "; q = " << result.getQ() << std::endl;
            o << "alpha and x values: " << std::endl;
            auto nVar = s.getNrAllStochasts();
            size_t ii = 0;
            for (size_t i = 0; i < nVar; i++)
            {
                if (s.getParamI(i)->isDeterministOrCombined())
                {
                    o << i << pls.double2str(0.0);
                }
                else
                {
                    o << i << pls.double2str(result.getAlphaI(ii));
                    ii++;
                }
                if (x.size() == nVar) { o << pls.double2str(x(i)); }
                o << " " << s.getParamI(i)->getName();
                o << std::endl;
            }
        }

        void lsfResult::dumpResult(std::ofstream& o)
        {
            o << std::scientific;
            o << std::setprecision(12);
            o << result.getBeta() << std::endl;
            result.getAlpha().dumpResult(o);
            x.dumpResult(o);
            o << method << std::endl;
            o << (int)convergence << std::endl;
            o << stepsNeeded << std::endl;
            o << std::fixed;
        }

        void lsfResult::readDumpFile(std::fstream& o)
        {
            double beta;
            o >> beta;
            result.setBeta(beta);
            result.setAlpha(vector1D::readDumpFile(o));
            x = vector1D::readDumpFile(o);
            o >> method;
            int conv;
            o >> conv;
            convergence = (ConvergenceStatus)conv;
            o >> stepsNeeded;
        }
    }
}
