#pragma once
#include <vector>
#include "../utils/matrix.h"
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        enum class extrapolation
        {
            constant,
            basicLinear,
            smartLinear,
        };

        class mtable
        {
        public:
            mtable(const Matrix& t, const extrapolation xp, const size_t N);
            std::vector<double> getValue(const double x) const;
            std::string getSummary() const;
        private:
            size_t getValueInBetween(const double x) const;
            const Matrix& tvalues;
            const extrapolation xPol;
            size_t indexFirst;
            size_t indexLast;
            const size_t MaxYvalues;
            std::string getRange(const size_t row) const;
        };

        // wrapper around mtable to use it as an distribution
        class dtable : public distrib
        {
        public:
            dtable(const Matrix& t, const extrapolation xp, const size_t N);
            dtable(const std::string& name, const Matrix& t, const extrapolation xp, const size_t N);
            ~dtable();
            virtual double getValue(const double u) const;
            virtual std::string getSummary(const std::vector<double>& p4) const;
        private:
            const mtable* table;
            const Matrix cp;
        };
    }
}
