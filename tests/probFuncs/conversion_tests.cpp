#include <gtest/gtest.h>
#include <vector>
#include "conversion_tests.h"
#include "../../src/probFuncs/conversions.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void conversion_tests::all_conversion_tests()
        {
            returnTimeFromBetaTests();
            freqFromBetaTests();
            logqFromBetaTests();
        }

        void conversion_tests::returnTimeFromBetaTests()
        {
            std::vector<double> expected =
            { 1.44269504088896, 5.78858538234500, 43.4538712904684, 740.296582122419,
             31573.8855320086, 3488555.28669823, 1013594691.25190, 781364428834.289 };

            for (size_t i = 1; i <= expected.size(); i++)
            {
                double beta = i - 1.0;
                double returnTime = conversions::ReturnTimeFromBeta(beta);
                ASSERT_NEAR(returnTime, expected[i - 1], returnTime * 1e-8);
            }
        }

        void conversion_tests::freqFromBetaTests()
        {
            std::vector<double> freq_expected =
            { 0.693147180559945, 0.172753779023450, 0.0230129093289635, 0.00135080996474820,
             3.16717433774893e-05, 2.86651613008105e-07, 9.86587645037701e-10, 1.27981254388584e-12 };

            for (size_t i = 1; i <= freq_expected.size(); i++)
            {
                double beta = i - 1.0;
                double freq = conversions::FreqFromBeta(beta);
                ASSERT_NEAR(freq, freq_expected[i - 1], freq * 1e-8);
            }
        }

        void conversion_tests::logqFromBetaTests()
        {
            std::vector<double> logQ_expected =
            { 0.366512920581664, 1.75588794089519, 3.77169994533666, 6.60705089255662,
             10.3600856506974, 15.0649982505082, 20.7367689499747, 27.3843074988111 };

            for (size_t i = 1; i <= logQ_expected.size(); i++)
            {
                double beta = i - 1.0;
                double logq = conversions::LogQFromBeta(beta);
                ASSERT_NEAR(logq, logQ_expected[i - 1], logq * 1e-8);
            }
        }
    }
}
