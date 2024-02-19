#include "formWithTrialLoops.h"
#include <iostream>

namespace Deltares {
    namespace ProbLibCore {

        formWithTrialLoops::formWithTrialLoops(const progress& pg)
            : startVector(0), p(pg) {}

        formWithTrialLoops::formWithTrialLoops(const FormSettings& s, const progress& pg)
            : startVector(0), settings(s), p(pg) {}

        formWithTrialLoops::formWithTrialLoops(const FormSettings& s, vector1D start, const progress& pg)
            : startVector(start), settings(s), p(pg) {}

        lsfResult formWithTrialLoops::calc(const zmodel& m, const stochSettings& s)
        {
            auto calcResult = lsfResult(0);
            auto settingsAdjust = settings;

            for (int i = 0; i < settingsAdjust.trialLoops; i++)
            {
                if (i > 0)
                {
                    settingsAdjust.relaxationFactor *= 0.5;
                    settingsAdjust.maxIter *= 2;
                }
                auto Form = form(settingsAdjust, startVector, p);
                calcResult.result.setBeta(Form.betaMax);
                try
                {
                    calcResult = Form.calc(m, s);
                }
                catch (const std::exception& e)
                {
                    std::cout << e.what() << std::endl;
                    calcResult.convergence = ConvergenceStatus::noConvergence;
                }

                if (calcResult.convergence == ConvergenceStatus::success) break;
                if (calcResult.convergence == ConvergenceStatus::abortedByUser) break;
            }
            return calcResult;
        }
    }
}
