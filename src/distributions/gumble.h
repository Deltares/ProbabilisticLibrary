#pragma once
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class gumbel : public distrib
        {
        public:
            gumbel(const double p1, const double p2) : a(p1), b(p2) {};
            gumbel(const std::string& s, const double p1, const double p2) : distrib(s, 2), a(p1), b(p2) {};
            virtual double getValue(const double u) const;
        private:
            const double a;
            const double b;
        };

        class gumbelII : public distrib
        {
        public:
            gumbelII(const double p1, const double p2) : a(p1), b(p2) {};
            gumbelII(const std::string& s, const double p1, const double p2) : distrib(s, 2), a(p1), b(p2) {};
            virtual double getValue(const double u) const;
        private:
            const double a;
            const double b;
        };

        class gumbelModified : public distrib
        {
        public:
            gumbelModified(const double p1, const double p2, const double p3) : a(p1), b(p2), c(p3) {};
            gumbelModified(const std::string& s, const double p1, const double p2, const double p3) :
                distrib(s, 3), a(p1), b(p2), c(p3) {};
            virtual double getValue(const double u) const;
        private:
            const double a;
            const double b;
            const double c;
        };

        class gumbelTruncatedModified : public distrib
        {
        public:
            gumbelTruncatedModified(const double p1, const double p2, const double p3, const double p4);
            gumbelTruncatedModified(const std::string& s, const double p1, const double p2, const double p3, const double p4);
            virtual double getValue(const double u) const;
        private:
            const gumbelModified gm;
            const double d;
        };
    }
}
