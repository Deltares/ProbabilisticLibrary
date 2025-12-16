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
#include <string>
#include <vector>
#include "../../Deltares.Probabilistic/Combine/alphaBeta.h"
#include "../../Deltares.Probabilistic/Math/matrix.h"

using namespace Deltares::Reliability;

namespace Deltares::Probabilistic::Test
{
    class testutils
    {
    public:
        bool compareFiles(const std::string& refFile, const std::string& newFile) const;
        std::string refFileWithPath(const std::string& sourceFile, const std::string& relativePath);
        void checkAlphaBeta(const alphaBeta& computed, const alphaBeta& ref, const double margin);
        void checkAlphaBeta(const alphaBeta& computed, const alphaBeta& ref, const double margin1, const double margin2);
        static Numeric::Matrix convert1dmatrix(const std::initializer_list<double>& m);
        static Numeric::Matrix convert1dmatrix(const std::initializer_list<double>& m, size_t rows, size_t columns, bool mirrored);
    private:
        std::vector<std::string> readWholeFile(const std::string& file) const;
        bool compareLine(const std::string& ref, const std::string& nw) const;
        std::vector<std::string> mySplit(const std::string& str) const;
        bool looksLikeAnumber(const std::string& s) const;
        const double margin = 1e-5;
    };
}

