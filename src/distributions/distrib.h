#pragma once
#include <cstddef>
#include <string>
#include <vector>

namespace Deltares {
    namespace ProbLibCore {

        enum class distEnum {
            combined,
            general
        };

        class distrib
        {
        public:
            distrib(const size_t i, const size_t j) :
                indexIndependent(i), indexDependent(j), type(distEnum::combined), name("") {};
            distrib() : name("") {};
            distrib(const std::string& s, const int nrp) : name(s), nrParams(nrp) {};
            distrib(const distEnum e) : type(e), name("") {};
            virtual ~distrib() {};
            virtual double getValue(const double u) const { return -999.0; };
            virtual bool isDeterminist() const { return false; }
            bool isDeterministOrCombined() const { return isDeterminist() || type == distEnum::combined; }
            virtual double getx(const double x1, const double x2) const { return -999.0; };
            distEnum Type() const { return type; }
            const std::string getName() const { return name; }
            const int getNrParams() const { return nrParams; }
            virtual std::string getSummary(const std::vector<double>& p4) const;
            const size_t indexIndependent = 1;
            const size_t indexDependent = 1;
        protected:
            const double qMin = 1.0e-300;
            const double upperLog = 700.0;
            const double uLimit = 5.6;
            const double EMconstant = 0.5772156649015328606;  // Euler Mascheroni constant
            const double tiny = 1e-300;
            const distEnum type = distEnum::general;
            const std::string name;
            const int nrParams = 4;
        };

        class distrib4p
        {
        public:
            virtual double getValue(const double u, const double p1, const double p2,
                const double p3, const double p4) const = 0;
        };
    }
}
