#pragma once
#include "../Math/vector1D.h"
#include "../Statistics/StandardNormal.h"

namespace Deltares
{
    namespace Reliability
    {
        using Deltares::Numeric::vector1D;

        class alphaBeta
        {
        public:
            alphaBeta() : beta(0), alpha(0) {};
            alphaBeta(const double b, const std::initializer_list<double>& a) : beta(b), alpha(a) {};
            alphaBeta(const double b, const vector1D& a) : beta(b), alpha(a) {};
            size_t size() const { return alpha.size(); }
            void setAlpha(const vector1D& a) { alpha = a; }
            double sumOfInners(alphaBeta e, vector1D r) const { return alpha.sumOfInners(e.alpha, r); }
            double getQ() const { return Deltares::Statistics::StandardNormal::getQFromU(beta); }
            double getP() const { return Deltares::Statistics::StandardNormal::getPFromU(beta); }
            double getBeta() const { return beta; }
            void setBeta(const double b) { beta = b; }
            double getAlphaI(const size_t i) const { return alpha(i); }
            const vector1D getAlpha() const { return alpha; }
            void normalize() { alpha.normalize(); }
            void setAlpha(const size_t i, const double a) { alpha(i) = a; }
            void assign(const double x) { alpha.assign(x); }
        private:
            double beta;
            vector1D alpha;
        };
    }
}
