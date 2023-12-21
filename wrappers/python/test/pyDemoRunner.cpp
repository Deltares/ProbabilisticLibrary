#include <gtest/gtest.h>
#include "../src/runPy2Problib.h"
#include "../../../tests/utils/testutils.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "pydemotst.test4";

    // start the interpreter and keep it alive
    py::scoped_interpreter guard{};
    return RUN_ALL_TESTS();
}

void runADemo(const std::string & pyfile, const std::string & outfile, const std::string & reffile)
{
    auto runner = runPy2Problib();
    auto utils = Deltares::ProbLibTests::testutils();
    runner.run(pyfile.c_str(), outfile, runType::sequential);
    auto result = utils.comparefiles(outfile, "../refData/" + reffile);
    ASSERT_TRUE(result);
    remove(outfile.c_str());
}

void runADemo(const int i)
{
    std::string ii = std::to_string(i);
    auto pyfile = "zfunc"+ii;
    auto outfile = "out"+ii;
    auto reffile = "pydemo" + ii + ".out";
    runADemo(pyfile, outfile, reffile);
}

TEST (pydemotst, test1)
{
    runADemo(1);
}

TEST (pydemotst, test2)
{
    runADemo(2);
}

TEST (pydemotst, test3)
{
    runADemo(3);
}

TEST (pydemotst, test4)
{
    runADemo(4);
}

TEST (pydemotst, test5)
{
    runADemo(5);
}

TEST (pydemotst, test6)
{
    runADemo(6);
}

TEST (pydemotst, test7)
{
    runADemo(7);
}

TEST (pydemotst, test8)
{
    runADemo("zfunc_dist_table", "out8", "pydemo8.out");
}

TEST (pydemotst, test9)
{
    runADemo("zfunc_quiz_a", "out9", "pydemo9.out");
}

TEST (pydemotst, test10)
{
    runADemo("zfunc_quiz_b", "out10", "pydemo10.out");
}

TEST (pydemotst, test11)
{
    runADemo(11);
}

TEST (pydemotst, test12)
{
    runADemo(12);
}
