#include <iostream>
#include <pybind11/embed.h>
#include "runPy2Problib.h"

using namespace::Deltares::ProbLibCore;

namespace py = pybind11;

void usage()
{
    std::cout << "usage:" << std::endl;
    std::cout << "py2problib [--seq] pythonBasename [outputFile]" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        usage();
        return -1;
    }

    int posIn = 1;
    int posOut = 2;

#ifdef _MSC_VER
    runType type = runType::sequential;
#else
    runType type = runType::fork;
#endif

    std::string optArg = argv[1];
    if (optArg == "--seq")
    {
        posIn++;
        posOut++;
        type = runType::sequential;
        if (argc == 2)
        {
            usage();
            return -1;
        }
    }

    std::string out = "-";
    if (argc > posOut)
    {
        out = argv[posOut];
    }

    try
    {
        // start the interpreter and keep it alive
        py::scoped_interpreter guard{};

        auto runner = runPy2Problib();
        runner.run(argv[posIn], out, type);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
