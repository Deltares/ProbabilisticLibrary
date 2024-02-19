#pragma once
#include "../utils/vector1D.h"
#include "probNumber.h"

namespace Deltares {
    namespace ProbLibCore {

        class alphaBeta
        {
        public:
            alphaBeta() {};
            alphaBeta(const double b, const std::initializer_list<double>& a) : beta(b), alpha(a) {};
            alphaBeta(const double pq, const char f, const std::initializer_list<double>& a) : beta(pq, f), alpha(a) {};
            alphaBeta(const double b, const vector1D& a) : beta(b), alpha(a) {};
            alphaBeta(const double pq, const char f, const vector1D& a) : beta(pq, f), alpha(a) {};
            double getBeta() { return beta.getu(); };
            double getQ() { return beta.getq(); }
            double getP() { return beta.getp(); }
            double getAlphaI(const size_t i) const { return alpha(i); }
            const vector1D getAlpha() const { return alpha; }
            probNumber getU() { return beta; }
            size_t size() const { return alpha.size(); }
            void setBeta(const double b) { beta = b; }
            void setBeta(const double b, const char f) { beta = probNumber(b, f); }
            void setAlpha(const vector1D& a) { alpha = a; }
            void setAlpha(const size_t i, const double a) { alpha(i) = a; }
            void operator*=(double d) { alpha *= d; }
            void assign(const double x) { alpha.assign(x); }
            void assignU(const probNumber u) { beta = u; }
            void normalize() { alpha.normalize(); }
            double sumOfInners(alphaBeta e, vector1D r) const { return alpha.sumOfInners(e.alpha, r); }
        private:
            probNumber beta;
            vector1D alpha;
        };
    }
}
