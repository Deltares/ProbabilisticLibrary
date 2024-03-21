#include <fstream>
#include <stdio.h>
#include "gtest/gtest.h"
#include "hohenbichler_tests.h"
#include "../Utils/testutils.h"
#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"
#include "../../Deltares.Probabilistic/Combine/Hohenbichler.h"

using namespace Deltares::Reliability;
using namespace Deltares::Statistics;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void HohenbichlerTest::allHohenbichlerTests()
            {
                NoCorrelation();
                AlmostFullCorrelation();
                FullCorrelation();
                PartialCorrelation();
                negativeCorrelation();
                negativeCorrelationNegativeBeta();
                negativeCorrelationZeroBeta();
            }

            void HohenbichlerTest::NoCorrelation()
            {
                const double beta2 = 3.0;
                const double pf1 = 1.0e-4;
                const double rho = 0.0;
                double expectedpf2pf1 = StandardNormal::getQFromU(beta2);

                auto h = Hohenbichler();
                auto result = h.PerformHohenbichler(beta2, pf1, rho);
                double pf2pf1 = result.first;

                EXPECT_NEAR(pf2pf1, expectedpf2pf1, margin);
                EXPECT_EQ(result.second, 0);
            }

            void HohenbichlerTest::AlmostFullCorrelation()
            {
                const double beta2 = 3.0;
                const double pf1 = 1.0e-4;
                const double rho = 0.998;

                const double expectedpf2pf1 = 0.999983622739076;

                auto h = Hohenbichler();
                auto result = h.PerformHohenbichler(beta2, pf1, rho);
                double pf2pf1 = result.first;

                EXPECT_NEAR(pf2pf1, expectedpf2pf1, margin);
                EXPECT_EQ(result.second, 0);
            }

            void HohenbichlerTest::FullCorrelation()
            {
                const double beta2 = 3.0;
                const double pf1 = 1.0e-4;
                const double rho = 1.0;
                const double expectedpf2pf1 = 1.0;

                auto h = Hohenbichler();
                auto result = h.PerformHohenbichler(beta2, pf1, rho);
                double pf2pf1 = result.first;

                EXPECT_NEAR(pf2pf1, expectedpf2pf1, margin);
                EXPECT_EQ(result.second, 0);
            }

            void HohenbichlerTest::PartialCorrelation()
            {
                double beta2 = 3.0;
                double pf1 = 1.0e-4;
                double rho = 0.5;
                double expectedpf2pf1 = 0.11323892;

                auto h = Hohenbichler();
                double pf2pf1 = h.PerformHohenbichler(beta2, pf1, rho).first;

                EXPECT_NEAR(pf2pf1, expectedpf2pf1, 1e-8);

                //alternative values:
                beta2 = 2.5;
                pf1 = 6.209679853494862e-3;
                rho = 0.95;
                expectedpf2pf1 = 0.600507845;

                auto result = h.PerformHohenbichler(beta2, pf1, rho);
                pf2pf1 = result.first;

                EXPECT_NEAR(pf2pf1, expectedpf2pf1, 1e-8);
                EXPECT_EQ(result.second, 0);
            }

            void HohenbichlerTest::negativeCorrelation()
            {
                const double beta2 = 3.0;
                const double pf1 = 1.0e-4;
                const double rho = -1.0;
                const double expectedpf2pf1 = 0.0;

                auto h = Hohenbichler();
                auto result = h.PerformHohenbichler(beta2, pf1, rho);
                double pf2pf1 = result.first;

                EXPECT_NEAR(pf2pf1, expectedpf2pf1, 2e-7);
                EXPECT_EQ(result.second, 1);
            }

            void HohenbichlerTest::negativeCorrelationNegativeBeta()
            {
                const double beta2 = -3.0;
                const double pf1 = 0.8;
                const double rho = -1.0;
                const double expectedpf2pf1 = 0.998312630153354;

                auto h = Hohenbichler();
                auto result = h.PerformHohenbichler(beta2, pf1, rho);
                double pf2pf1 = result.first;

                EXPECT_NEAR(pf2pf1, expectedpf2pf1, 1e-8);
                EXPECT_EQ(result.second, 0);
            }

            // Test on the margin of what can be handled (hence a fairly large margin):
            // The two stochasts are perfectly negatively correlated so they fail in
            // separate half-planes, the area of common failure is actually the _line_
            // u1 = 0 (since both probabilities of failure are 0.5).
            void HohenbichlerTest::negativeCorrelationZeroBeta()
            {
                const double beta2 = 0.0;
                const double pf1 = 0.5;
                const double rho = -1.0;
                const double expectedpf2pf1 = 1.052261624672e-9;

                auto h = Hohenbichler();
                auto result = h.PerformHohenbichler(beta2, pf1, rho);
                double pf2pf1 = result.first;

                EXPECT_NEAR(pf2pf1, expectedpf2pf1, 1e-8);
                EXPECT_EQ(result.second, 1);
            }

            // Test for verifying effects of the setting of the parameter rhoLimitHohenbichler in subroutine Hohenbichler( betaV, pfU, rhoInput, pfVpfU )  \n
            // NOTE: Formally this is not a unit test is strict sense since no computational results are compared to expected results \n
            // \n
            // In this test the output parameter pfVpfU of Hohenbichler() is computed for variations of its three input parameters {betaV, pfU, rhoInput} \n
            // rhoInput is varied on a dense partition of [0,1] \n
            // The betaV variations consist of the following four values {-3, 0, 3, 6, 9} \n
            // Similarly three variations are applied for pfU \n
            // For all rhoInput * betaV * pfU combinations subroutine Hohenbichler( ) is called to obtain the corresponding pfVpfU \n
            // The results are stored in a plain ascii file ..\test\unitTests\Test_RhoLimit.tek \n
            // On the basis of this file plots can be made of pfVpfU as function of rhoInput (for fixed betaV and pfU) \n
            // In particular (possibly undesired) effects of the restriction rho = max( min( rhoInput, rhoLimitHohenbichler ) can be verified. \n
            // At this moment rhoLimitHohenbichler=0.99 as set in subroutine Hohenbichler() \n
            void  HohenbichlerTest::RhoLimit()
            {
                const int npfUvar = 3;               // total number of variations of the failure probability pfU
                const int lWork = 2 + 2 * npfUvar; // dimension of locally used work space array work()
                std::vector<double> work(lWork + 1);  // locally defined and used array for work space within the computations

                //   Range and increment for the rhoInput variations
                const double rhoInput_Min = 0.0;
                double rhoInput_Max = 1.0;
                //  rhoInput_Delta= 0.00001   // Reference setting for high resolution of rho-variations rhoLimitHohenbichler. Requires much computation time
                //  rhoInput_Delta= 0.0001    // for less dense variations of rhoLimit
                const double rhoInput_Delta = 0.001;   // for less dense variations of rhoLimit

                //  Number of gridpoints for equidistant rhoInput variations:
                const int nRhoInput = int(1.5 + (rhoInput_Max - rhoInput_Min) / rhoInput_Delta);
                rhoInput_Max = rhoInput_Min + rhoInput_Delta * double(nRhoInput - 1);

                //  Range and increment for the Beta variations
                const double betaMin = -3.0;
                const double betaMax = 9.0;
                const double dBeta = 3.0;
                //  Number of gridpoints for equidistant Beta variations:
                const int nBeta = int(1.5 + (betaMax - betaMin) / dBeta);

                //  Initialisation of the pfU factors
                std::vector<double> pfUfactor = { 1.0, 1.0e-3, 1.0e-5 };

                //  Open a data file for saving the result of the (beta, pfU, rhoInput)-variations:
                auto lunOut = std::ofstream("Test_RhoLimit.tek", std::ios::out);
                //  Visualisation of the results (pfVpfU as function of rhoInput) must be done with some plotting program

                //  Generate header/legends in the output file for the applied variations:
                lunOut << "* Listing/Legends for the " << nBeta << " data blocks in present file:" << std::endl;
                lunOut << "*" << std::endl;

                for (int iBeta = 1; iBeta <= nBeta; iBeta++)
                {
                    const double betaV = betaMin + dBeta * double(iBeta - 1);
                    double p; double pfU;
                    p = StandardNormal::getPFromU(betaV);
                    pfU = StandardNormal::getQFromU(betaV);
                    lunOut <<
                        "* ---------------------------------------------" << std::endl <<
                        "* Block of (beta, pfU, rhoInput) variations Nr. " << iBeta << "/" << nBeta << std::endl <<
                        "* ---------------------------------------------" << std::endl <<
                        "* Beta             = " << betaV << std::endl <<
                        "* ==> p(Beta)      = " << p << std::endl <<
                        "* ==> pfU(Beta)    = " << pfU << std::endl << "*" << std::endl;
                    for (int ipfUvar = 1; ipfUvar <= npfUvar; ipfUvar++)
                    {
                        const double pfUvar = pfU * pfUfactor[ipfUvar - 1];
                        lunOut << "* Variation pfU(" << ipfUvar << ") = " << pfUvar << std::endl;
                    }
                    lunOut << "*" << std::endl <<
                        "* rhoInput_Min     = " << rhoInput_Min << std::endl <<
                        "* rhoInput_Max     = " << rhoInput_Max << std::endl <<
                        "* rhoInput_Delta   = " << rhoInput_Delta << std::endl <<
                        "* nRhoInput        = " << nRhoInput << std::endl << "*" << std::endl;
                }

                // Entry to the computation and saving results for plotting pfVpfU as function of rhoInput,
                // for given variations of beta, and pfU:
                auto h = Hohenbichler();
                for (int iBeta = 1; iBeta <= nBeta; iBeta++)
                {
                    const double betaV = betaMin + dBeta * double(iBeta - 1);
                    double p; double pfU;
                    p = StandardNormal::getPFromU(betaV);
                    pfU = StandardNormal::getQFromU(betaV);
                    work[1] = betaV;
                    lunOut <<
                        "* ---------------------------------------" << std::endl <<
                        "* Entry to (pfU, rhoInput) variations Nr. " << iBeta << "/" << nBeta << std::endl <<
                        "* ---------------------------------------" << std::endl <<
                        "* Beta             = " << betaV << std::endl <<
                        "* ==> p(Beta)      = " << p << std::endl <<
                        "* ==> pfU(Beta)    = " << pfU << std::endl << "*" << std::endl;
                    for (int ipfUvar = 1; ipfUvar <= npfUvar; ipfUvar++)
                    {
                        const double pfUvar = pfU * pfUfactor[ipfUvar - 1];
                        lunOut << "* Variation pfU(" << ipfUvar << ") = " << pfUvar << std::endl;
                    }
                    lunOut << "*" << std::endl <<
                        "* rhoInput_Min     = " << rhoInput_Min << std::endl <<
                        "* rhoInput_Max     = " << rhoInput_Max << std::endl <<
                        "* rhoInput_Delta   = " << rhoInput_Delta << std::endl <<
                        "* nRhoInput        = " << nRhoInput << std::endl << "*" << std::endl;
                    lunOut << "*  1       2          3               4               5             6               7             8" << std::endl;
                    lunOut << "*Beta   rhoInput    pfU(1)        pfVpfU(1)         pfU(2)      pfVpfU(2)         pfU(3)      pfVpfU(3)" << std::endl;
                    lunOut << "BL" << iBeta << std::endl << nRhoInput << " " << lWork << std::endl;
                    for (int iRhoInput = 1; iRhoInput <= nRhoInput; iRhoInput++)
                    {
                        const double rhoInput = rhoInput_Min + rhoInput_Delta * double(iRhoInput - 1);
                        work[2] = rhoInput;
                        int iWork = 2;
                        for (int ipfUvar = 1; ipfUvar <= npfUvar; ipfUvar++)
                        {
                            const double pfUvar = pfU * pfUfactor[ipfUvar - 1];
                            const double pfVpfU = h.PerformHohenbichler(betaV, pfUvar, rhoInput).first;

                            iWork = iWork + 1;
                            work[iWork] = pfUvar;
                            iWork = iWork + 1;
                            work[iWork] = pfVpfU;
                        }
                        for (int iWork = 1; iWork <= lWork; iWork++)
                        {
                            lunOut << work[iWork] << " ";
                        }
                        lunOut << std::endl;
                    }
                    lunOut << "*" << std::endl;
                }

                lunOut.close();

                auto tester = testutils();
                auto result = tester.comparefiles("tests/Test_RhoLimit.tek.ref", "refData/Test_RhoLimit.tek");
                ASSERT_TRUE(result);
                remove("Test_RhoLimit.tek");
            }

        }
    }
}
