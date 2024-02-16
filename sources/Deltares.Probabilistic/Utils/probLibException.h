#pragma once
#include <exception>
#include <string>
#include <cstddef>
#include "probLibString.h"

namespace Deltares {
    namespace ProbLibCore {

        class probLibException : public std::exception
        {
        public:
            probLibException(const std::string& msg) : message(msg) {}
            probLibException(const std::string& msg, const double r) : message(msg + tos(r)) {}
            probLibException(const std::string& msg, const int i) : message(msg + std::to_string(i)) {}
            probLibException(const std::string& msg, const size_t i) : message(msg + std::to_string(i)) {}
            virtual const char* what() const throw() { return message.c_str(); };
        private:
            const std::string message;
            std::string tos(const double r) { auto pls = probLibString(); return pls.double2str(r); }
        };
    }
}
