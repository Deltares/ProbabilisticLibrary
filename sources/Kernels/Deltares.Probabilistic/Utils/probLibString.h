#pragma once
#include <string>

namespace Deltares {
    namespace ProbLibCore {

        class probLibString
        {
        public:
            bool iStrcmp(const std::string& s1, const std::string& s2);
            bool iFind(const std::string& s1, const std::string& s2);
            std::string double2str(const double x) const;

            // trim from both ends of string (right then left)
            std::string trim(const std::string& s, const char* t);

        private:
            std::string strToLower(const std::string& data);

            // trim from end of string (right)
            std::string rtrim(const std::string& s, const char* t);

            // trim from beginning of string (left)
            std::string ltrim(const std::string& s, const char* t);
        };
    }
}
