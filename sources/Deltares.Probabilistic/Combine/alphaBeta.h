// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once
#include "../Math/vector1D.h"
#include "../Statistics/StandardNormal.h"

namespace Deltares
{
    namespace Reliability
    {
        class alphaBeta
        {
        public:
            alphaBeta() : beta(0), alpha(0) {};
            alphaBeta(const double b, const std::initializer_list<double>& a) : beta(b), alpha(a) {};
            alphaBeta(const double b, const Numeric::vector1D& a) : beta(b), alpha(a) {};
            size_t size() const { return alpha.size(); }
            void setAlpha(const Numeric::vector1D& a) { alpha = a; }
            double sumOfInners(const alphaBeta& e, const Numeric::vector1D& r) const { return alpha.sumOfInners(e.alpha, r); }
            double getQ() const { return Deltares::Statistics::StandardNormal::getQFromU(beta); }
            double getP() const { return Deltares::Statistics::StandardNormal::getPFromU(beta); }
            double getBeta() const { return beta; }
            void setBeta(const double b) { beta = b; }
            double getAlphaI(const size_t i) const { return alpha(i); }
            Numeric::vector1D getAlpha() const { return alpha; }
            void normalize() { alpha.normalize(); }
            void setAlpha(const size_t i, const double a) { alpha(i) = a; }
            void assign(const double x) { alpha.assign(x); }
        private:
            double beta;
            Numeric::vector1D alpha;
        };
    }
}
