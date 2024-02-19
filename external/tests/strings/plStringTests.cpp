#include "gtest/gtest.h"
#include "plStringTests.h"
#include "../../src/utils/probLibString.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        void plStringTests::allPlStringTests()
        {
            plStringTest1();
            plStringTest2();
            plStringTest3();
            plStringTest4();
            plDouble2StringTest();
            plStringTrimTest();
        }

        void plStringTests::plStringTest1()
        {
            auto pls = probLibString();
            std::string s1 = "abc";
            std::string s2 = "ABC";
            ASSERT_TRUE(pls.iStrcmp(s1, s2));
        }

        void plStringTests::plStringTest2()
        {
            auto pls = probLibString();
            std::string s1 = "abc789";
            std::string s2 = "C78";
            ASSERT_TRUE(pls.iFind(s1, s2));
        }

        void plStringTests::plStringTest3()
        {
            auto pls = probLibString();
            std::string s1 = "abc";
            std::string s2 = "ABCD";
            ASSERT_FALSE(pls.iStrcmp(s1, s2));
        }

        void plStringTests::plStringTest4()
        {
            auto pls = probLibString();
            std::string s1 = "abc789";
            std::string s2 = "C89";
            ASSERT_FALSE(pls.iFind(s1, s2));
        }

        void plStringTests::plDouble2StringTest()
        {
            auto pls = probLibString();
            double d = 3.14;
            auto s = pls.double2str(d);
            EXPECT_EQ(s, "       3.140000");
            d = 5e14;
            s = pls.double2str(d);
            EXPECT_EQ(s, "500000000000000.000000");
            d = -5e14;
            s = pls.double2str(d);
            EXPECT_EQ(s, "-500000000000000.000000");
        }

        void plStringTests::plStringTrimTest()
        {
            auto pls = probLibString();
            double d = 3.14;
            auto s = pls.double2str(d);
            s = pls.trim(s, " ");
            EXPECT_EQ(s, "3.140000");

            s = "  abc    ";
            s = pls.trim(s, " ");
            EXPECT_EQ(s, "abc");

            s = "**abc++";
            s = pls.trim(s, "*+");
            EXPECT_EQ(s, "abc");
        }
    }
}
