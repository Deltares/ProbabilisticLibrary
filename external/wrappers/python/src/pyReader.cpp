#include <iostream>
#include "../../../src/correlation/identity.h"
#include "../../../src/correlation/gaussianCorrelationRobust.h"
#include "../../../src/distributions/parseDistribution.h"
#include "pyReader.h"
#include "../../../src/utils/probLibString.h"
#include "../../../src/probMethods/fillStartVector.h"
#include "../../../src/rnd/rndDefs.h"

using namespace::Deltares::ProbLibCore;

namespace py = pybind11;

pyReader::pyReader(std::ostream & f, py::module & zfunc, const int nrSt)
: o(f), myzfunc(zfunc), nrStoch(nrSt) {}

tPyCalc pyReader::readBasics()
{
    auto calc = tPyCalc();

    auto pm = parseMethod();
    auto r = myzfunc.attr("method")();
    calc.method = pm.str2enum(r.cast<std::string>());

    calc.settings = read_settings(calc.method);
    calc.design = read_design();
    calc.tree = read_tree();

    return calc;
}

void pyReader::read_params(listDistribs & params)
{
    auto pd = parseDistribution();
    for(int i=0; i<nrStoch; i++)
    {
        auto s = std::vector<std::string>();
        auto p4 = std::vector<double>();
        py::object l = myzfunc.attr("stochprop")(i);
        for(auto it = l.begin(); it != l.end(); ++it)
        {
            if (s.size() < 2) // list starts with two strings
            {
                s.push_back(it->cast<std::string>());
            }
            else
            {
                p4.push_back(it->cast<double>());
            }
        }

        o << "   stochast " << i << ": "  << s[0] << " ; " << s[1] << ": ";
        params.addDistrib(pd.parse(s, p4));
        o << params.getDistrib(i)->getSummary(p4) << std::endl;
    }
}

basicSettings pyReader::read_settings(const ProbMethod m)
{
    auto pm = parseMethod();
    auto bs = basicSettings();
    bool hasDS = false;
    std::string startMethod = "zero";
    std::string rnd = "gm";
    try
    {
        auto r1 = myzfunc.attr("settings")("tol");
        bs.tolA = r1.cast<double>();
        bs.tolB = bs.tolA;
        if (pm.useSampling(m))
        {
            auto r2 = myzfunc.attr("settings")("minSamples");
            bs.minSamples = r2.cast<int>();
            auto r3 = myzfunc.attr("settings")("maxSamples");
            bs.maxSamples = r3.cast<int>();
            auto r4 = myzfunc.attr("settings")("rnd");
            rnd = r4.cast<std::string>();
            bs.rnd = rndDefs::convert(rnd);
            if (bs.rnd == rndTypes::GeorgeMarsaglia)
            {
                auto r6 = myzfunc.attr("settings")("seed1");
                bs.seed1 = r6.cast<int>();
                auto r7 = myzfunc.attr("settings")("seed2");
                bs.seed2 = r7.cast<int>();
            }
            else
            {
                auto r5 = myzfunc.attr("settings")("seed");
                bs.seed1 = r5.cast<int>();
            }

            switch (m)
            {
            case ProbMethod::IM: case ProbMethod::AdaptiveIM:
            {
                auto r = myzfunc.attr("settings")("varianceFactor");
                bs.varianceFactor = r.cast<double>();
                for (int i = 0; i < nrStoch; i++)
                {
                    bs.varianceFactors[i] = bs.varianceFactor;
                }
                bs.numExtraReal1 = 0.333;
                bs.numExtraReal2 = 1.0;
                if (m == ProbMethod::AdaptiveIM)
                {
                    auto r2 = myzfunc.attr("settings")("maxSamplesDef");
                    bs.numExtraInt = r2.cast<double>();
                    if (bs.numExtraInt < 0) bs.numExtraInt = 10 * bs.maxSamples;
                    auto r3 = myzfunc.attr("settings")("nAdp");
                    auto nAdp = r3.cast<int>();
                    bs.trialLoops = (nAdp > 0 ? nAdp : 3);
                    auto r4 = myzfunc.attr("settings")("globalModelOption");
                    auto option = r4.cast<int>();
                    bs.numExtraInt2 = option;
                }
            }
            break;
            case ProbMethod::DS : case ProbMethod::DSFI : case ProbMethod::FDIR:
            {
                auto r = myzfunc.attr("settings")("epsilonDu");
                bs.epsilonDu = r.cast<double>();
                if (bs.epsilonDu < 0) bs.epsilonDu = 0.001;
                hasDS = true;
            }
            break;
            default:
                break;
            }
        }
        if (pm.useForm(m))
        {
            bs.numExtraInt = 50;
            auto r6 = myzfunc.attr("settings")("startmethod");
            startMethod = r6.cast<std::string>();
            bs.startMethod = FillStartVector::convert(startMethod);
            for (size_t i = 0; i < maxActiveStochast; i++)
            {
                bs.startVector[i] = 1.0;
            }
        }
        auto r7 = myzfunc.attr("settings")("progressInterval");
        bs.progressInterval = r7.cast<int>();
    }
    catch(const std::exception& e)
    {
        std::cerr << "error while getting settings from python." << std::endl;
        std::cerr << e.what() << std::endl;
    }

    o << "settings:" << std::endl;
    o << "   methode        : " << pm.enum2str(m) << std::endl;
    o << "   tol            : " << bs.tolA << std::endl;
    if (pm.useSampling(m))
    {
        o << "   min/max samples: " << bs.minSamples << ", " << bs.maxSamples;
        if (m == ProbMethod::AdaptiveIM) o << ", " << bs.numExtraInt;
        o << std::endl;
        o << "   seed           : " << rnd << ", " << bs.seed1 << ", " << bs.seed2 << std::endl;
    }
    if (m == ProbMethod::IM || m == ProbMethod::AdaptiveIM)
    {
        o << "   varianceFactor : " << bs.varianceFactor << std::endl;
        if (m == ProbMethod::AdaptiveIM)
        {
            o << "   nAdp           : " << bs.trialLoops << std::endl;
            o << "   global option  : " << bs.numExtraInt2 << std::endl;
        } 
    }
    if (hasDS)
    {
        o << "   epsilonDu      : " << bs.epsilonDu << std::endl;
    }
    if (pm.useForm(m))
    {
        o << "   startmethod    : " << startMethod << std::endl;
    }
    if (bs.progressInterval > 0)
    {
        o << "   progress each  : " << bs.progressInterval << " steps." << std::endl;
    }

    return bs;
}

correlation* pyReader::read_correlations()
{
    auto r1 = myzfunc.attr("ncorr")();
    auto ncorr = r1.cast<int>();
    if (ncorr == 0)
    {
        return new identity(nrStoch);
    }
    else
    {
        auto vecc = std::vector<corrStruct>();
        for (int i = 0; i < ncorr; i++)
        {
            auto ij = std::vector<int>();
            auto l = myzfunc.attr("corrpair")(i);
            corrStruct c;
            for(auto it = l.begin(); it != l.end(); ++it)
            {
                if (ij.size() < 2)
                {
                    ij.push_back(it->cast<int>());
                }
                else
                {
                    c.correlation = it->cast<double>();
                }
            }
            c.idx1 = ij[0];
            c.idx2 = ij[1];
            
            vecc.push_back(c);
        }
        o << "found " << ncorr << " correlated stochastic parameter(s)" << std::endl;
        return new gaussianCorrelationRobust(nrStoch, vecc);
    }
}

std::string pyReader::read_tree()
{
    std::string tree = "";
    try
    {
        auto r = myzfunc.attr("tree")();
        tree = r.cast<std::string>();
    }
    catch(const std::exception&)
    {
        ;
    }
    return tree;
}

designCalcType str2enum(const std::string & s)
{
    auto pls = probLibString();
    if (pls.iStrcmp(s, "table"))
    {
        return designCalcType::table;
    }
    else if (pls.iStrcmp(s, "design"))
    {
        return designCalcType::design;
    }
    else
    {
        return designCalcType::normal;
    }
}

tDesign pyReader::read_design()
{
    auto d = tDesign();

    try
    {
        auto r1 = myzfunc.attr("design")("type");
        auto typeStr = r1.cast<std::string>();
        d.type = str2enum(typeStr);

        auto r2 = myzfunc.attr("design")("startvalue");
        d.startValue = r2.cast<double>();
        auto r3 = myzfunc.attr("design")("stopvalue");
        d.stopValue = r3.cast<double>();
        if (d.type == designCalcType::design)
        {
            auto r4 = myzfunc.attr("design")("beta");
            d.beta = r4.cast<double>();
        }
        else if (d.type == designCalcType::table)
        {
            auto r4 = myzfunc.attr("design")("step");
            d.step = r4.cast<double>();
        }
        auto r5 = myzfunc.attr("design")("varid");
        d.varid = r5.cast<size_t>();
    }
    catch(const std::exception&)
    {
        o << "no table/design" << std::endl;
    }

    if ((int) d.varid >= nrStoch)
    {
        throw probLibException("Invalid varid in design/table run");
    }

    return d;
}
