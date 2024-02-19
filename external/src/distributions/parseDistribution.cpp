#include "parseDistribution.h"
#include "logNormalDist.h"
#include "normalDist.h"
#include "truncatedNormal.h"
#include "uniformDist.h"
#include "exponentialDist.h"
#include "gumble.h"
#include "weibull.h"
#include "rayleighDist.h"
#include "paretoDist.h"
#include "triangleDist.h"
#include "distBeta.h"
#include "mtable.h"
#include "../utils/probLibException.h"
#include "../utils/probLibString.h"

namespace Deltares {
    namespace ProbLibCore {

        EnumDistributions parseDistribution::parse(const std::string& s) const
        {
            auto pls = probLibString();
            if (pls.iStrcmp(s, "deterministic")) // id=0
            {
                return EnumDistributions::deterministic;
            }
            else if (pls.iStrcmp(s, "uniform")) // id=1
            {
                return EnumDistributions::uniform;
            }
            else if (pls.iStrcmp(s, "normal")) // id=2
            {
                return EnumDistributions::normal;
            }
            else if (pls.iStrcmp(s, "lognormal")) // id=3
            {
                return EnumDistributions::lognormal;
            }
            else if (pls.iStrcmp(s, "lognormal2")) // id=4
            {
                return EnumDistributions::lognormal2;
            }
            else if (pls.iStrcmp(s, "shiftedExponential")) // id=5
            {
                return EnumDistributions::shiftedExponential;
            }
            else if (pls.iStrcmp(s, "gumbel")) // id=6
            {
                return EnumDistributions::gumbel;
            }
            else if (pls.iStrcmp(s, "gumbel2")) // id=7
            {
                return EnumDistributions::gumbel2;
            }
            else if (pls.iStrcmp(s, "weibull")) // id=8
            {
                return EnumDistributions::weibull;
            }
            else if (pls.iStrcmp(s, "rayleigh")) // id=9
            {
                return EnumDistributions::rayleigh;
            }
            else if (pls.iStrcmp(s, "pareto")) // id=10
            {
                return EnumDistributions::pareto;
            }
            else if (pls.iStrcmp(s, "triangular")) // id=11
            {
                return EnumDistributions::triangular;
            }
            else if (pls.iStrcmp(s, "table")) // id=12
            {
                return EnumDistributions::table;
            }
            else if (pls.iStrcmp(s, "tablet")) // id=12
            {
                return EnumDistributions::tableT;
            }
            else if (pls.iStrcmp(s, "conditionalWeibull")) // id=14
            {
                return EnumDistributions::conditionalWeibull;
            }
            else if (pls.iStrcmp(s, "modifiedGumbel")) // id=15
            {
                return EnumDistributions::modifiedGumbel;
            }
            else if (pls.iStrcmp(s, "truncatedModifiedGumbel")) // id=16
            {
                return EnumDistributions::truncatedModifiedGumbel;
            }
            else if (pls.iStrcmp(s, "RayleighN")) // id=18
            {
                return EnumDistributions::RayleighN;
            }
            else if (pls.iStrcmp(s, "truncatedNormal")) // id=19
            {
                return EnumDistributions::truncatedNormal;
            }
            else if (pls.iStrcmp(s, "beta")) // id=20
            {
                return EnumDistributions::beta;
            }
            else if (pls.iStrcmp(s, "uspace")) // id=21
            {
                return EnumDistributions::uspace;
            }
            else
            {
                throw probLibException("Unknown type " + s);
            }
        }

        distrib* parseDistribution::parse(const std::vector<std::string>& s, const std::vector<double>& p4) const
        {
            auto distType = s[1];
            auto name = s[0];

            auto d = parse(distType);
            return parse(d, name, p4);
        }

        distrib* parseDistribution::parse(const EnumDistributions distType, const std::string& name, const std::vector<double>& p4) const
        {
            switch (distType)
            {
            case EnumDistributions::deterministic:
                return new deterministicDist(name, p4[0]);
            case EnumDistributions::uniform:
                return new uniformDist(name, p4[0], p4[1]);
            case EnumDistributions::normal:
                return new normalDist(name, p4[0], p4[1]);
            case EnumDistributions::lognormal:
                return new logNormalDist(name, p4[0], p4[1], p4[2]);
            case EnumDistributions::lognormal2:
                return new logNormalIIDist(name, p4[0], p4[1], p4[2]);
            case EnumDistributions::shiftedExponential:
                return new exponentialDist(name, p4[0], p4[1]);
            case EnumDistributions::gumbel:
                return new gumbel(name, p4[0], p4[1]);
            case EnumDistributions::gumbel2:
                return new gumbelII(name, p4[0], p4[1]);
            case EnumDistributions::weibull:
                return new weibull(name, p4[0], p4[1], p4[2]);
            case EnumDistributions::rayleigh:
                return new rayleighDist(name, p4[0], p4[1]);
            case EnumDistributions::pareto:
                return new paretoDist(name, p4[0], p4[1], p4[2]);
            case EnumDistributions::triangular:
                return new triangleDist(name, p4[0], p4[1], p4[2]);
            case EnumDistributions::table: {
                size_t dim = p4.size() / 2;
                auto d = Matrix(dim, 2);
                for (size_t i = 0; i < dim; i++)
                {
                    d(i, 0) = p4[2 * i];
                    d(i, 1) = p4[2 * i + 1];
                }
                return new dtable(name, d, extrapolation::basicLinear, 1);
            }
            case EnumDistributions::tableT: {
                size_t dim = p4.size() / 2;
                auto d = Matrix(dim, 2);
                for (size_t i = 0; i < dim; i++)
                {
                    d(i, 0) = p4[i];
                    d(i, 1) = p4[i + dim];
                }
                return new dtable(name, d, extrapolation::basicLinear, 1);
            }
            case EnumDistributions::conditionalWeibull:
                return new conditionalWeibull(name, p4[0], p4[1], p4[2], p4[3]);
            case EnumDistributions::modifiedGumbel:
                return new gumbelModified(name, p4[0], p4[1], p4[2]);
            case EnumDistributions::truncatedModifiedGumbel:
                return new gumbelTruncatedModified(name, p4[0], p4[1], p4[2], p4[3]);
            case EnumDistributions::RayleighN:
                return new rayleighNDist(name, p4[0], p4[1]);
            case EnumDistributions::truncatedNormal:
                return new truncatedNormal(name, p4[0], p4[1], p4[2], p4[3]);
            case EnumDistributions::beta:
                return new distBeta(name, p4[0], p4[1], p4[2], p4[3]);
            case EnumDistributions::uspace:
                return new uSpace(name);
            default:
                throw probLibException("Unknown distribution function - code: ", (int)distType);
            }
        }

        size_t parseDistribution::activeParameters(const EnumDistributions distType) const
        {
            switch (distType)
            {
            case EnumDistributions::deterministic:
                return 1;
            case EnumDistributions::uniform:
            case EnumDistributions::normal:
            case EnumDistributions::shiftedExponential:
            case EnumDistributions::gumbel:
            case EnumDistributions::gumbel2:
            case EnumDistributions::rayleigh:
            case EnumDistributions::RayleighN:
                return 2;
            case EnumDistributions::lognormal:
            case EnumDistributions::lognormal2:
            case EnumDistributions::weibull:
            case EnumDistributions::pareto:
            case EnumDistributions::triangular:
            case EnumDistributions::modifiedGumbel:
                return 3;
            case EnumDistributions::uspace:
                return 0;
            default:
                return 4;
            }

        }
    }
}
