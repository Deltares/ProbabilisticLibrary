#pragma once
#include <iostream>

namespace Deltares {
    namespace ProbLibCore {

        class progress
        {
        public:
            progress() : os(std::cout) {};
            progress(const int i) : interval(i), os(std::cout) {};
            progress(const int i, std::ostream& f) : interval(i), os(f) {};
            virtual bool printProgressAndCancel(int nTimes, double result, double conv) const;
            virtual void printInfo(const std::string& s) const { os << s << std::endl; };
        private:
            int interval = -1;
            std::ostream& os;
        };

        class externalProgressCancel : public progress
        {
        public:
            externalProgressCancel(const bool(*p)(int, double, double)) : pc(p) {};
            virtual bool printProgressAndCancel(int nTimes, double result, double conv) const
            {
                return pc(nTimes, result, conv);
            };
        private:
            const bool(*pc)(int, double, double);
        };
    }
}
