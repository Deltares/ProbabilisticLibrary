#pragma once
#include "lsf.h"
#include "form.h"
#include "../utils/vector1D.h"

namespace Deltares {
    namespace ProbLibCore {

        class formWithTrialLoops : public lsf
        {
        public:
            formWithTrialLoops(const progress& p);
            formWithTrialLoops(const FormSettings& s, const progress& p);
            formWithTrialLoops(const FormSettings& s, vector1D start, const progress& p);
            virtual ~formWithTrialLoops() {};
            lsfResult calc(const zmodel& m, const stochSettings& s);
        private:
            vector1D startVector;
            FormSettings settings;
            const progress& p;
        };
    }
}
