#include "probLibString.h"
#include <string.h> //For strcasecmp(). Also could be found in <mem.h>
#include <algorithm>
#include <cctype>

namespace Deltares {
    namespace ProbLibCore {

#ifdef _MSC_VER
        //not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

        bool probLibString::iStrcmp(const std::string& s1, const std::string& s2)
        {
            if (s1.length() != s2.length())
                return false;  // optimization since std::string holds length in variable.
            return strcasecmp(s1.c_str(), s2.c_str()) == 0;
        }

        bool probLibString::iFind(const std::string& s1, const std::string& s2)
        {
            return (strToLower(s1).find(strToLower(s2)) < s1.length());
        }

        std::string probLibString::strToLower(const std::string& data)
        {
            std::string s = data;
            std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
            return s;
        }

        std::string probLibString::double2str(const double x) const
        {
            char buffer[32];
#ifdef _WIN32
            sprintf_s(buffer, "%15.6f", x);
#else
            snprintf(buffer, 32, "%15.6f", x);
#endif // _WIN32
            std::string retval = buffer;
            return retval;
        }

        // trim from end of string (right)
        std::string probLibString::rtrim(const std::string& s, const char* t)
        {
            std::string r = s;
            r.erase(r.find_last_not_of(t) + 1);
            return r;
        }

        // trim from beginning of string (left)
        std::string probLibString::ltrim(const std::string& s, const char* t)
        {
            std::string r = s;
            r.erase(0, r.find_first_not_of(t));
            return r;
        }

        // trim from both ends of string (right then left)
        std::string probLibString::trim(const std::string& s, const char* t)
        {
            auto trimmed = rtrim(s, t);
            return ltrim(trimmed, t);
        }
    }
}
