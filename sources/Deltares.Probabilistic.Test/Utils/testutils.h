#pragma once
#include <string>
#include <vector>
#include "../../Deltares.Probabilistic/Combine/alphaBeta.h"

using namespace Deltares::Reliability;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class testutils
            {
            public:
                bool comparefiles(const std::string& refFile, const std::string& newFile) const;
                void checkAlphaBeta(alphaBeta& computed, alphaBeta& ref, const double margin);
                void checkAlphaBeta(alphaBeta& computed, alphaBeta& ref, const double margin1, const double margin2);
                size_t readNumThreadsFromEnv(const bool debug);
            private:
                std::vector<std::string> readWholeFile(const std::string& file) const;
                bool compareLine(const std::string& ref, const std::string& nw) const;
                std::vector<std::string> mySplit(const std::string& str) const;
                bool lookAnumber(std::string s) const;
                const double margin = 2e-6;
            };
        }
    }
}
