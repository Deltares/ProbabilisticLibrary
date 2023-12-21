#pragma once

namespace Deltares {
    namespace ProbLibCore {

        enum class corrIds
        {
            correlationComplete = 1,
            correlationNLmodel,
            correlationPCRModel = 4,
            correlationVolker,
            correlationGaussianModel
        };

        class corrParams
        {
        public:
            int independentId;
            corrIds correlationId;
            int correlationParameterId;
            int distribution;
            double distParameter1;
            double distParameter2;
            double distParameter3;
            double distParameter4;
            double correlationParameter1;
            double correlationParameter2;
            double correlationParameter3;
            double correlationParameter4;
            double correlationParameter5;
            double correlationParameter6;
            double correlationParameter7;
            double correlationParameter8;
            double shift;
        };

        /*
         * Implementation of the correlation models. The probabilisticDataStructure method
         * calculateCorrelation merely dispatches the actual calculatation
         * to the various specific methods.
         */
        class HRcorrelation
        {
        public:
            void calculateCorrelation(const double u1, double& u2, const corrParams& correlationParameters, const bool typeUCorrelation);
            void calculateCorrelationNLmodel(const double u1, double& u2, const corrParams& correlationParameters);
            void calculateCorrelationPCRmodel(const double u1, double& u2, const corrParams& correlationParameters);
            void calculateCorrelationGaussianModel(const double u1, double& u2, const corrParams& correlationParameters);
            void calculateCorrelationVolker(const double u1, double& u2, const corrParams& correlationParameters);
        private:
            // v_lowerbound is the lower - bound for the variable for the integration
            const double v_lowerbound = 0.0;

            // v_upperbound is the upper - bound for the variable for the integration
            const double v_upperbound = 16.0;

            // number of steps for the variable for the integration
            const int nv = 801;

            // threshold for using p instead of q or vice versa
            //TODO this is also defined in distributionFunctions
            const double  uLimit = 5.6;
        };
    }
}
