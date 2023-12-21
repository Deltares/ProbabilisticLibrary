#include <memory>
#include <math.h>
#include <vector>
#include "HRcorrelation.h"
#include "../probFuncs/conversions.h"
#include "../utils/probLibException.h"
#include "../utils/basic_math.h"
#include "../distributions/parseDistribution.h"
#include "../distributions/distrib.h"
#include "trapeziumRule.h"

namespace Deltares {
    namespace ProbLibCore {

        // method for calculating the correlation model
        void HRcorrelation::calculateCorrelation(const double u1, double& u2, const corrParams& correlationParameters, const bool typeUCorrelation)
        {
            // u1                     !< Independent variable
            // u2                     !< Dependent variable
            // correlationParameters  !< Parameters for the correlation
            // typeUCorrelation       !< u-space (true) or x-space (false)

            bool typeUCorr = typeUCorrelation;

            switch (correlationParameters.correlationId)
            {
            case (corrIds::correlationComplete):
                u2 = u1;
                break;
            case (corrIds::correlationNLmodel):
                if (typeUCorrelation)
                {
                    calculateCorrelationNLmodel(u1, u2, correlationParameters);
                }
                break;
            case (corrIds::correlationPCRModel):
                if (typeUCorr)
                {
                    calculateCorrelationPCRmodel(u1, u2, correlationParameters);
                }
                break;
            case (corrIds::correlationGaussianModel):
                if (typeUCorr)
                {
                    calculateCorrelationGaussianModel(u1, u2, correlationParameters);
                }
                break;
            case (corrIds::correlationVolker):
                calculateCorrelationVolker(u1, u2, correlationParameters);
                break;

            default:
                throw probLibException("Unknown correlation model - ID.");
            }

        }

        // Method for calculating the dependent variable according to the "NL" model with constant standard deviation (CS-model)
        void HRcorrelation::calculateCorrelationNLmodel(const double u1, double& u2, const corrParams& correlationParameters)
        {
            // u1                      !< Independent variable u1
            // u2                      !< Dependent variable u2
            // correlationParameters   !< Distribution function

            // stdev                      ! Standard deviation
            // delta                      ! Delta parameter
            // p                          ! Probability of non-exceedance
            // q                          ! Probability of exceedance
            // v                          ! Standard exponential distributed variable
            // w                          ! dependent variable
            // iv                         ! counter of the integration steps
            // variable(:)                ! vector with variables for the integration
            // integrand(:)               ! vector with integrand values
            // Fw                         ! Probability of non-exceedance for variable w
            // equalSteps                 ! Flag for using equal stepsize
            // factorStapSize             ! factor for the step size: the next step is this factor larger
            // step                       ! Start value for the variable for the integration

            double stdev = correlationParameters.distParameter2;
            double delta = correlationParameters.shift;

            // tests for on parameters per specific distribution

            if (stdev <= 0.0) throw probLibException("standard deviation normal distribution <= 0; ", stdev);

            // Compute the standard exponential distributed variable

            double p; double q;
            conversions::PQfromBeta(u1, p, q);
            double v = -log(q);

            // Numerical integration using trapezium rule
            // number of integration steps
            auto xy = std::vector<xyPair>(nv);

            // With the code below it is possible that variable(nx) is not equal to x_upperbound,
            // It differs perhaps a little bit. That doesn't matter. The upper bound is high enough.
            xy[0].x = v_lowerbound;

            const bool equalSteps = false;
            if (equalSteps)
            {
                // equal steps
                double step = (v_upperbound - v_lowerbound) / (double)(nv - 1);
                for (int iv = 1; iv < nv; iv++)
                {
                    xy[iv].x = xy[iv - 1].x + step;
                }
            }
            else
            {
                // factor for the step size is chosen in such a way that the last step is ten times the first step
                double factorStapSize = pow(10.0, (1.0 / (double)(nv - 2)));
                double step = v_upperbound * (1.0 - factorStapSize) / (1.0 - pow(factorStapSize, nv - 1));

                xy[1].x = step;
                for (int iv = 2; iv < nv; iv++)
                {
                    step = step * factorStapSize;
                    xy[iv].x = xy[iv - 1].x + step;
                }
            }

            // Constant standard deviation in the Hydra-NL correlation model (CS-model)
            double w = v + delta + u2 * stdev;
            // Fill the integrand
            for (int iv = 0; iv < nv; iv++)
            {
                // Constant standard deviation in the Hydra-NL correlation model (CS-model)
                conversions::PQfromBeta((w - xy[iv].x - delta) / stdev, p, q);
                if (w - delta <= v)
                {
                    xy[iv].y = exp(-xy[iv].x) * p;
                }
                else
                {
                    xy[iv].y = exp(-xy[iv].x) * q;
                }
            }

            // Call Trapezium rule
            double Fw = trapeziumRule::calc(xy);

            if (w - delta <= v)
            {
                Fw = 1.0 - Fw;
            }

            // Compute dependant variable u2
            u2 = conversions::betaFromQ(Fw);
        }

        /* Volker Correlation Model: The Volker model determines the wind speed as a function of the water level, and is designed specifically for use in the Netherlands.
        * The correlation model is set up in the real space (x-space). \n
        * The input into the model are standard normally distributed u1 and u2 values associated with the water level and wind speed, respectively. Further, correlation parameters are
        * required as input. There are 8 such parameters: \n
        * * A \n
        * * B \n
        * * M \n
        * * rho \n
        * * a \n
        * * b \n
        * * c \n
        * * d \n
        * The placement of these parameters in the model is shown in the text that follows. \n
        * The first step in the correlation model is to determine the value of the water level (x1) associated with u1: \n \n
        * \f$
        * x_1  = F_{x_1 }^{ - 1} \left( {\Phi \left( {u_1 } \right)} \right)
        * \f$ where \n \n
        * \f$
        * F_{x_1 }^{ - 1}
        * \f$
        * is the inverse distribution function of x1 (water level). The next step is to determine the wind speed <EM> given the water level </EM>.
        * This conditional relationship is in fact the correlation model: \n \n
        *\f$
        * F\left( {x_2 |x_1 ,r} \right) = F_{TG} \left( {\frac{{ - K_r (x_2 ) + \rho {{\left( {x_1  - A_r } \right)} \mathord{\left/ {\vphantom {{\left( {x_1  - A_r } \right)} {B_r }}} \right. } {B_r }}}}{{M_r }}} \right)
        * \f$ where \n \n
        * \f$
        * F_{TG}
        * \f$
        * is the truncated gumbel distribution function (see scientific documentation) and
        * \f$
        * { - K_r (x_2 )}
        * \f$
        * is described as: \n \n
        *\f$
        * K_r \left( {x_2 } \right) = ax_2 ^2  + bx_2  + c
        * \f$ \n \n
        * In Hydra-Ring we need the inverse - that is, we need the value of x2.
        * Solving for x2 gives: \n \n
        * \f$
        * x_2  = \left\{ \begin{array}{l}
        * \frac{{ - b + \sqrt {b^2  - 4ac'} }}{{2a}},\,\,\,\,\left( {b^2  - 4ac'} \right) > 0 \\
        * \frac{{ - b}}{{2a}},\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\,\left( {b^2  - 4ac'} \right) < 0\,\, \\
        * \end{array} \right.
        * \f$ \n \n
        * The description of this solution is given in the scientific documentation. The value c' is given as follows: \n \n
        *\f$
        *c' = c - \rho  \cdot {{\left( {x_1  - A} \right)} \mathord{\left/
        * {\vphantom {{\left( {x_1  - A} \right)} B}} \right.
        * } B} + M\ln \left[ { - \ln \left[ {p \cdot \left( {1 - d} \right)} \right]} \right]
        *\f$ \n \n
        * Note that the values of the parameters that are equivalent to "uncorrelated" are as follows:
        * A    = 0 \n
        * B    = 1 \n
        * M    = 1 \n
        * rho  = 0 \n
        * d    = 0 \n
        * When the wind is uncorrelated with the water level, the distribution of the wind speed reduces to a modified Gumbel distribution
        * (see scientific documentation), with parameters a, b, and c.
        * (Private to this module)
        */
        void HRcorrelation::calculateCorrelationVolker(const double u1, double& u2, const corrParams& correlationParameters)
        {
            // u1                      !< Independent std normal variable u1
            // u2                      !< Dependent std normal variable u2
            // correlationParameters   !< Structure with necessary information for the correlation model


            // x1                   ! Independent variable - real space
            // x2                   ! Dependent variable - real space
            // distType             ! Type of distribution for independent variable
            // distPar1             ! parameter 1 of the independent variable's distribution function
            // distPar2             ! parameter 2 of the independent variable's distribution function
            // distPar3             ! parameter 3 of the independent variable's distribution function
            // distPar4             ! parameter 4 of the independent variable's distribution function
            // A                    ! correlation model parameter
            // B                    ! correlation model parameter
            // M                    ! correlation model parameter
            // rho                  ! correlation model parameter
            // aK                   ! parameter "a" of the quadratic function Kr(u)
            // bK                   ! parameter "b" of the quadratic function Kr(u)
            // cK                   ! parameter "c" of the quadratic function Kr(u)
            // d                    ! truncation factor
            // p,q                  ! non-exceedance (p) and exceedance (q) probabilities of u2
            // cprime               ! c' in formula 3.66 in the documentation (to do: re-write here above for easier reference)
            // determinant          ! criteria for which solution for x2 to use (depends if the square root term is >= zero or not)

            double A = correlationParameters.correlationParameter1;
            double B = correlationParameters.correlationParameter2;
            double rho = correlationParameters.correlationParameter3;
            double M = correlationParameters.correlationParameter4;
            double aK = correlationParameters.correlationParameter5;
            double bK = correlationParameters.correlationParameter6;
            double cK = correlationParameters.correlationParameter7;
            double d = correlationParameters.correlationParameter8;

            int distType = correlationParameters.distribution;
            double distPar[4];
            distPar[0] = correlationParameters.distParameter1;
            distPar[1] = correlationParameters.distParameter2;
            distPar[2] = correlationParameters.distParameter3;
            distPar[3] = correlationParameters.distParameter4;

            // Step 1: convert u1 to x1 using the inverse distribution function of x1 (u1 is input, x1 is output of calculateDistribution)
            double x1;
            if (distType == 17)
            {
                x1 = distPar[0];
            }
            else
            {
                std::string name = "name";
                auto pd = parseDistribution();
                auto type = (EnumDistributions)distType;
                auto nrActive = pd.activeParameters(type);
                std::vector<double> p4;
                for (size_t i = 0; i < nrActive; i++)
                {
                    p4.push_back(distPar[i]);
                }

                std::unique_ptr<distrib> d(pd.parse(type, name, p4));
                x1 = d->getValue(u1);
            }

            // Step 2: Compute c'
            double p; double q;
            conversions::PQfromBeta(u2, p, q);

            double cprime;
            if (u2 > uLimit)
            {
                // this uses q = -log(p) for large u2 as in LogQFromBeta
                // and log(a*b) = log(a) + log(b)
                const double tiny = 2.225073858507201e-308;
                cprime = cK - rho * (x1 - A) / B + M * log(max(q, tiny) - log(1.0 - d));
            }
            else
            {
                cprime = cK - rho * (x1 - A) / B + M * log(-log(p * (1.0 - d)));
            }

            // Step 3: compute criteria for solution (depending if crit is pos or negative, different forms of the solution are used)
            // (actually the determinant of the quadratic equation - truncate if negative)
            double determinant = max(pow(bK, 2) - 4.0 * aK * cprime, 0.0);

            // Step 4: compute x2 with inverse solution
            double x2 = (-bK + sqrt(determinant)) / (2.0 * aK);

            // Step 5: "Convert" x2 back to the u-space
            u2 = x2;
        }

        // Method for calculating the dependent variable according to the "PCR" model
        void HRcorrelation::calculateCorrelationPCRmodel(const double u1, double& u2, const corrParams& correlationParameters)
        {
            // u1                      !< Independent variable u1
            // u2                      !< Dependent variable u2
            // correlationParameters   !< Distribution function

            // s : correlation model parameter
            // p : Probability of non-exceedance
            // q : Probability of exceedance
            // v : Standard exponential distributed variable
            // w : dependent variable

            double s = correlationParameters.correlationParameter1;

            // Compute the standard exponential distributed variable
            double p; double q;
            conversions::PQfromBeta(u1, p, q);
            double v = -log(q);

            //Computing variable w dependent on v
            double w = v + (u2 * s) - pow(s, 2) / 2.0;

            if (w < 1e-10) w = 1e-10;

            // Compute dependant variable u2
            q = exp(-w);
            u2 = conversions::betaFromQ(q);
        }

        // Method for calculating the dependent variable according to the Gaussian model
        void HRcorrelation::calculateCorrelationGaussianModel(const double u1, double& u2, const corrParams& correlationParameters)
        {
            // u1                      !< Independent variable u1
            // u2                      !< Dependent variable u2
            // correlationParameters   !< Distribution function

            // rho : Correlation coefficient

            double rho = correlationParameters.correlationParameter1;

            if (rho < -1.0 || rho > 1.0)
            {
                throw probLibException("Correlation coefficient of Gaussian correlation model outside valid range [-1.0, 1.0] ", rho);
            }

            u2 = u1 * rho + u2 * sqrt(1.0 - pow(rho, 2));
        }
    }
}
