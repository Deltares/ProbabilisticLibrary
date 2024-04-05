#include "testutils.h"
#include "gtest/gtest.h"
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>
#include "../../Deltares.Probabilistic/Utils/probLibString.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            std::string testutils::refFileWithPath(const std::string& sourceFile, const std::string& relativePath)
            {
                auto found = sourceFile.find_last_of("/\\");
                auto base = sourceFile.substr(0, found+1);
                return base + relativePath;
            }

            bool testutils::comparefiles(const std::string& refFile, const std::string& newFile) const
            {
                auto sref = readWholeFile(refFile);
                auto snew = readWholeFile(newFile);

                if (sref.size() != snew.size()) return false;

                for (size_t i = 0; i < sref.size(); i++)
                {
                    if (!compareLine(sref[i], snew[i])) return false;
                }

                return true;
            }

            std::vector<std::string> testutils::readWholeFile(const std::string& file) const
            {
                auto in = std::ifstream(file, std::ios::in);

                auto values = std::vector<std::string>();

                std::string line;
                auto pls = probLibString();
                while (std::getline(in, line))
                {
                    line = pls.trim(line, "\r");
                    values.push_back(line);
                }
                return values;
            }

            // returns true if the provided string looks like a string
            // only checks the first character.
            // used to avoid exceptions later on
            bool testutils::looksLikeAnumber(const std::string& s) const
            {
                if (std::isdigit(s.at(0))) return true;
                if (s.at(0) == '-') return true;
                if (s.at(0) == '+') return true;
                if (s.at(0) == '.') return true;
                return false;
            }

            // returns true if both strings are equal by a word by word comparison
            // if that word is a number, allow small differences
            bool testutils::compareLine(const std::string& ref, const std::string& nw) const
            {
                if (ref == nw) return true;
                auto r = mySplit(ref);
                auto n = mySplit(nw);
                if (r.size() != n.size()) return false;
                for (size_t i = 0; i < r.size(); i++)
                {
                    if (r[i] != n[i])
                    {
                        if (looksLikeAnumber(r[i]) && looksLikeAnumber(n[i]))
                        {
                            double numr = stof(r[i]);
                            double numn = stof(n[i]);
                            double tol = margin * std::abs(numr);
                            EXPECT_NEAR(numr, numn, tol);
                            if (std::abs(numr - numn) > tol) return false;
                        }
                        else
                        {
                            return false;
                        }
                    }
                }

                return true;
            }

            std::vector<std::string> testutils::mySplit(const std::string& str) const
            {
                auto s = std::vector<std::string>();
                size_t i = 0;
                for (size_t j = 0; j < str.size(); j++)
                {
                    if (str.at(i) == ' ')
                    {
                        i++;
                    }
                    else if (str.at(j) == ' ')
                    {
                        auto r = str.substr(i, j - i);
                        s.push_back(r);
                        i = j;
                    }
                }
                return s;
            }

            void testutils::checkAlphaBeta(alphaBeta& computed, alphaBeta& ref, const double margin)
            {
                checkAlphaBeta(computed, ref, margin, margin);
            }

            void testutils::checkAlphaBeta(alphaBeta& computed, alphaBeta& ref, const double m1, const double m2)
            {
                EXPECT_NEAR(computed.getBeta(), ref.getBeta(), m1);
                EXPECT_EQ(computed.size(), ref.size());
                for (size_t iStochast = 0; iStochast < ref.size(); iStochast++)
                {
                    EXPECT_NEAR(computed.getAlphaI(iStochast), ref.getAlphaI(iStochast), m2);
                }
            }

            size_t testutils::readNumThreadsFromEnv(const bool debug)
            {
                auto env = getenv("OMP_NUM_THREADS");
                size_t numcores = 1;
                if (env != NULL)
                {
                    std::stringstream envStream(env);
                    envStream >> numcores;
                }

                if (debug) std::cout << "using " << numcores << " threads" << std::endl;
                return numcores;
            }

        }
    }
}
