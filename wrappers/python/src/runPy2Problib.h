#pragma once
#include <string>
#include "pyReader.h"

class runPy2Problib
{
public:
    void run(const char* pyFile, const std::string & outFile, const runType type);
};
