#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <omp.h>
#include "parseAndRunMethod.h"
#include "fdir.h"
#include "dsfi.h"
#include "formDsfi.h"
#include "formWithTrialLoops.h"
#include "crudeMonteCarlo.h"
#include "crudeMonteCarloForm.h"
#include "adaptiveImpSampling.h"
#include "importanceSamplingForm.h"
#include "numint.h"
#include "progress.h"
#include "../rnd/george_marsaglia.h"
#include "../rnd/mersenne_twister.h"
#include "../combine/combineElements.h"
#include "fillStartVector.h"
#ifndef _MSC_VER
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace Deltares {
    namespace ProbLibCore {

        lsfResult parseAndRunMethod::parseAndRun(const ProbMethod method, const zmodel& w, const stochSettings& s, const size_t numCores) const
        {
            if (s.getNrStochasts() == 0) throw probLibException("No stochastic parameters found.");

            auto FormFlags = FormSettings();
            FormFlags.epsilonBeta = settings.tolA;
            FormFlags.epsilonZvalue = settings.tolA;
            FormFlags.designPointOption = settings.designPointOptions;
            FormFlags.trialLoops = settings.trialLoops;
            FormFlags.du = settings.du;
            FormFlags.relaxationFactor = settings.relaxationFactor;
            FormFlags.maxIter = settings.numExtraInt;

            auto DSFlags = DSsettings();
            DSFlags.varCoeffFailure = settings.tolB;
            DSFlags.varCoeffNoFailure = settings.tolB;
            DSFlags.minSamples = settings.minSamples;
            DSFlags.maxSamples = settings.maxSamples;
            DSFlags.designPointOption = settings.designPointOptions;
            DSFlags.chunkSize = settings.chunkSize;

            auto fillsv = FillStartVector(settings.startMethod, settings.startVector);

            std::unique_ptr<randomForSampling> rnd(settings.rnd == rndTypes::GeorgeMarsaglia ?
                (randomForSampling*) new george_marsaglia(settings.seed1, settings.seed2) :
                (randomForSampling*) new mersenne_twister(settings.seed1));

            lsfResult calcResult;
            switch (method)
            {
            case ProbMethod::FORM: {
                auto u = fillsv.GetU(w, s, (int)numCores);
                auto Form = formWithTrialLoops(FormFlags, u, pc);
                calcResult = Form.calc(w, s); }
                                 break;
            case ProbMethod::FDIR: {
                auto u = fillsv.GetU(w, s, (int)numCores);
                std::unique_ptr<dsRootFinder> rtf(getRootFinder(settings));
                auto fd = fdir(DSFlags, FormFlags, *rnd, *rtf, u, numCores, pc);
                calcResult = fd.calc(w, s); }
                                 break;
            case ProbMethod::DSFIHR: {
                std::unique_ptr<dsRootFinder> rtf(getRootFinder(settings));
                auto mdsfi = dsfiHR(DSFlags, FormFlags, *rnd, *rtf, numCores, pc);
                calcResult = mdsfi.calc(w, s); }
                                   break;
            case ProbMethod::DSFI: {
                std::unique_ptr<dsRootFinder> rtf(getRootFinder(settings));
                auto mdsfi = dsfi(DSFlags, FormFlags, *rnd, *rtf, numCores, pc);
                calcResult = mdsfi.calc(w, s); }
                                 break;
            case ProbMethod::DS: {
                std::unique_ptr<dsRootFinder> rtf(getRootFinder(settings));
                auto ds = directionalSampling(DSFlags, *rnd, *rtf, numCores, pc);
                calcResult = ds.calc(w, s); }
                               break;
            case ProbMethod::CM: {
                auto flgs = CMsettings();
                flgs.minimumSamples = settings.minSamples;
                flgs.maximumSamples = settings.maxSamples;
                flgs.varCoeffFailure = settings.tolB;
                flgs.varCoeffNoFailure = settings.tolB;
                auto cm = crudeMonteCarlo(flgs, *rnd, pc);
                calcResult = cm.calc(w, s); }
                               break;
            case ProbMethod::IM: {
                auto flgs = IMsettings();
                flgs.maximumSamples = settings.maxSamples;
                flgs.minimumSamples = settings.minSamples;
                flgs.varCoeffFailure = settings.tolB;
                flgs.varCoeffNoFailure = settings.tolB;
                flgs.varianceFactor = settings.varianceFactor;
                if (settings.numThreads > 1)
                {
                    flgs.chunkSize = settings.chunkSize;
                    auto bf = IMbuffer(flgs.chunkSize, s.getNrStochasts(), true);
                    auto im = ImportanceSampling(flgs, *rnd, pc, bf);
                    calcResult = im.calc(w, s);
                }
                else
                {
                    auto im = ImportanceSamplingSeq(flgs, *rnd, pc);
                    calcResult = im.calc(w, s);
                }}
                               break;
            case ProbMethod::AdaptiveIM: {
                auto flgs = AIMsettings();
                flgs.maximumSamples = settings.maxSamples;
                flgs.minimumSamples = settings.minSamples;
                flgs.varCoeffFailure = settings.tolB;
                flgs.varCoeffNoFailure = settings.tolB;
                auto nrStoch = s.getNrStochasts();
                flgs.varianceFactors = vector1D(nrStoch);
                flgs.offsets = vector1D(nrStoch);
                flgs.startVector = vector1D(nrStoch);
                flgs.chunkSize = settings.chunkSize;
                for (size_t i = 0; i < nrStoch; i++)
                {
                    flgs.varianceFactors(i) = settings.varianceFactors[i];
                    flgs.offsets(i) = settings.offsets[i];
                    flgs.startVector(i) = settings.startVector[i];
                }
                flgs.startMethod = settings.startMethod;
                flgs.nAdp = settings.trialLoops;
                flgs.maxSamplesDef = settings.numExtraInt;
                flgs.epsFailed = settings.numExtraReal1;
                flgs.increaseVariance = settings.numExtraReal2;
                flgs.minFailed = (int)settings.iterationMethod;
                auto aim = adaptiveImpSampling(flgs, *rnd, pc, settings.numExtraInt2 == 0);
                calcResult = aim.calc(w, s); }
                                       break;
            case ProbMethod::NI: {
                auto flgs = NIsettings();
                flgs.numExtraInt = settings.numExtraInt;
                flgs.minimumUvalue = settings.numExtraReal1;
                flgs.maximumUvalue = settings.numExtraReal2;
                flgs.designPointOption = settings.designPointOptions;
                auto ni = numint(flgs, numCores, pc);
                calcResult = ni.calc(w, s); }
                               break;
            case ProbMethod::CMFORM: {
                auto flgs = CMsettings();
                flgs.minimumSamples = settings.minSamples;
                flgs.maximumSamples = settings.maxSamples;
                flgs.varCoeffFailure = settings.tolB;
                flgs.varCoeffNoFailure = settings.tolB;
                auto cmform = crudeMonteCarloForm(flgs, FormFlags, *rnd, pc);
                calcResult = cmform.calc(w, s); }
                                   break;
            case ProbMethod::CMIMFORM: {
                auto flgs = IMsettings();
                flgs.minimumSamples = settings.minSamples;
                flgs.maximumSamples = settings.maxSamples;
                flgs.varCoeffFailure = settings.tolB;
                flgs.varCoeffNoFailure = settings.tolB;
                flgs.varianceFactor = settings.varianceFactor;
                auto cmimform = importanceSamplingForm(flgs, FormFlags, *rnd, pc);
                calcResult = cmimform.calc(w, s); }
                                     break;
            case ProbMethod::FORMDSFIHR: {
                std::unique_ptr<dsRootFinder> rtf(getRootFinder(settings));
                auto mdsfi = formDsfiHR(DSFlags, FormFlags, *rnd, *rtf, numCores, pc);
                calcResult = mdsfi.calc(w, s); }
                                       break;
            case ProbMethod::FORMDSFI: {
                std::unique_ptr<dsRootFinder> rtf(getRootFinder(settings));
                auto mdsfi = formDsfi(DSFlags, FormFlags, *rnd, *rtf, numCores, pc);
                calcResult = mdsfi.calc(w, s); }
                                     break;
            case ProbMethod::FORMSTART:
                calcResult.x = fillsv.GetU(w, s, (int)numCores);
                break;
            default:
                auto m = (int)method;
                throw probLibException("Unknown method: ", m);
                break;
            }
            return calcResult;
        }

        dsRootFinder* parseAndRunMethod::getRootFinder(const basicSettings& settings) const
        {
            auto DSrtFlags = DSrootFinderSettings();
            DSrtFlags.epsilonDu = settings.epsilonDu;
            DSrtFlags.iterationMethod = settings.iterationMethod;
            DSrtFlags.du1 = settings.du1;
            DSrtFlags.epsilonDu = settings.tolC;

            if (DSrtFlags.iterationMethod == DSiterationMethods::DirSamplingIterMethodRobust ||
                DSrtFlags.iterationMethod == DSiterationMethods::DirSamplingIterMethodRobustBisection)
            {
                return new dsRootFinderRobust(DSrtFlags);
            }
            else
            {
                return new dsRootFinder(DSrtFlags);
            }
        }

        lsfResult parseAndRunMethod::parseRunAndPrint(const ProbMethod method, const zmodel& w,
            const stochSettings& s, std::ostream& f, const size_t numCores) const
        {
            auto calcResult = parseAndRun(method, w, s, numCores);
            calcResult.printResult(f, s);
            return calcResult;
        }

        std::vector<lsfResult> parseAndRunMethod::parseAndRunTree(const ProbMethod method, const std::vector<std::unique_ptr<zmodel>>& z,
            const stochSettings& s, const std::string& tree, const runType type, std::ostream& f) const
        {
            auto nrStoch = s.getNrStochasts();
            auto lsfs = std::vector<lsfResult>(3);

            combineAndOr AndOr;
            auto pls = probLibString();
            if (pls.iFind(tree, " or "))
            {
                AndOr = combineAndOr::combOr;
            }
            else if (pls.iFind(tree, " and "))
            {
                AndOr = combineAndOr::combAnd;
            }
            else
            {
                throw probLibException("and / or not found in: " + tree);
            }

            if (type == runType::OMP)
            {
#pragma omp parallel for
                for (int i = 0; i < 2; i++)
                {
                    lsfs[i] = parseAndRun(method, *z[i], s);
                }
            }
#ifndef _MSC_VER
            else if (type == runType::fork)
            {
                const char dumpFile[32] = "lsf.dump";
                pid_t id = fork();
                if (id > 0)
                {
                    lsfs[0] = parseAndRun(method, *z[0], s);
                    wait(NULL);
                }
                else
                {
                    lsfs[1] = parseAndRun(method, *z[1], s);
                    std::ofstream outfile;
                    outfile.open(dumpFile, std::ios::out);
                    lsfs[1].dumpResult(outfile);
                    outfile.close();
                    exit(0);
                }
                std::fstream infile;
                infile.open(dumpFile);
                lsfs[1].readDumpFile(infile);
                infile.close();
                std::filesystem::remove(dumpFile);
            }
#endif
            else
            {
                for (size_t i = 0; i < 2; i++)
                {
                    lsfs[i] = parseAndRun(method, *z[i], s);
                }
            }

            auto cmb = combineElements();
            auto rho = vector1D(nrStoch);
            rho.assign(1.0);

            auto combined = cmb.combineTwoElementsPartialCorrelation(lsfs[0].result, lsfs[1].result, rho, AndOr);
            lsfs[2].result = combined.ab;
            lsfs[2].method = "combin";
            lsfs[2].convergence = combined.n == 0 ? ConvergenceStatus::success : ConvergenceStatus::noConvergence;
            lsfs[2].stepsNeeded = lsfs[0].stepsNeeded + lsfs[1].stepsNeeded;
            lsfs[2].calcXinDesignPoint(s);
            f << "z1:" << std::endl;
            lsfs[0].printResult(f, s);
            f << "z2:" << std::endl;
            lsfs[1].printResult(f, s);
            f << tree << std::endl;
            lsfs[2].printResult(f, s);

            return lsfs;
        }
    }
}
