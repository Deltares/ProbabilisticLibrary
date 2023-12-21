#pragma once
#include "../utils/vector1D.h"
#include "../probFuncs/alphaBeta.h"
#include "Hohenbichler.h"

namespace Deltares {
    namespace ProbLibCore {

        //
        // Class to combine failure probabilities for identical time elements (identical = same reliability index and alpha values)
        //
        class upscaling
        {
        public:
            int upscaleInTime(const double nrTimes, alphaBeta& element, const vector1D& inRhoT);
            std::pair<alphaBeta, int> upscaleLength(alphaBeta& crossSectionElement, const vector1D& rhoXK,
                const vector1D& dXK, const double sectionLength, double breachL);
            void upscaleToLargestBlock(const alphaBeta& smallBlock,
                const vector1D& rhoTSmallBlock, const vector1D& blockDurations, const double largestBlockDuration,
                alphaBeta& largestBlock, vector1D& durationsLargestBlock);
            // TODO: ComputeBetaSection is public for testing
            std::pair<double, ConvergenceStatus> ComputeBetaSection(const double betaCrossSection, const double sectionLength,
                const double breachL, const double rhoZ, const double dz, const double deltaL);
        private:
            const double rhoLimit = 0.99999; // Limit value for the correlation coefficient
            const double rhoLowLim = 0.001;          // Lower limit of correlation coefficient before Hohenbichler method must be called
            progress p = progress();
            Hohenbichler hhb = Hohenbichler(p);
            probNumber upscaleBeta(probNumber elm, const double rhoT, const double nrTimes, int& failures);
        };
    }
}
