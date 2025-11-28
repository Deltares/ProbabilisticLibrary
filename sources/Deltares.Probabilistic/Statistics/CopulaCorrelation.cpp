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

#include "CopulaCorrelation.h"

#include "ClaytonCopula.h"
#include "FrankCopula.h"
#include "GumbelCopula.h"
#include "DiagonalBandCopula.h"
#include "GaussianCopula.h"

namespace Deltares::Statistics
{
    using enum correlationType;

    void CopulaCorrelation::SetCorrelation(const int i, const int j, double value, correlationType type)
    {
        auto pair = copulaPair();
        pair.i = i;
        pair.j = j;
        switch (type)
        {
        case Clayton:
            pair.copula = std::make_shared<ClaytonCopula>(value);
            break;
        case Frank:
            pair.copula = std::make_shared<FrankCopula>(value);
            break;
        case Gumbel:
            pair.copula = std::make_shared<GumbelCopula>(value);
            break;
        case DiagonalBand:
            pair.copula = std::make_shared<DiagonalBandCopula>(value);
            break;
        case UnknownYet:
            lastValue = value;
            return;
        case Gaussian:
            pair.copula = std::make_shared<GaussianCopula>(value);
        }
        copulas.push_back(pair);
    }

    bool CopulaCorrelation::isValid() const
    {
        auto report = Logging::ValidationReport();
        validate(report);
        return report.messages.empty();
    }

    void CopulaCorrelation::validate(Logging::ValidationReport& report) const
    {
        for (size_t i = 0; i < copulas.size(); i++)
        {
            auto& c = copulas[i];
            c.copula->validate(report);
            if (c.i == c.j)
            {
                auto msg = std::make_shared<Logging::Message>();
                msg->Text = "Self correlation not allowed for copulas, found for stochast " + std::to_string(c.i);
                msg->Type = Logging::MessageType::Error;
                report.messages.push_back(msg);
            }
            for (size_t j = 0; j < i; j++)
            {
                auto& other = copulas[j];
                if (c.i == other.i || c.i == other.j || c.j == other.i || c.j == other.j)
                {
                    auto msg = std::make_shared<Logging::Message>();
                    msg->Text = "Multiple correlations not allowed for copulas, found for correlations " + std::to_string(i) + " and " + std::to_string(j);
                    msg->Type = Logging::MessageType::Error;
                    report.messages.push_back(msg);
                }
            }
        }
    }

    void CopulaCorrelation::SetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2 , double value, correlationType type)
    {
        size_t index1 = std::numeric_limits<size_t>::max();
        size_t index2 = std::numeric_limits<size_t>::max();
        for (size_t i = 0; i < stochasts.size(); i++)
        {
            if (stochasts[i] == stochast1) index1 = i;
            if (stochasts[i] == stochast2) index2 = i;
        }

        if (index1 < stochasts.size() && index2 < stochasts.size())
        {
            SetCorrelation(static_cast<int>(index1), static_cast<int>(index2), value, type);
        }
    }


    void CopulaCorrelation::filter(const std::shared_ptr<BaseCorrelation> m, const std::vector<int>& index)
    {
        auto src = std::dynamic_pointer_cast<CopulaCorrelation> (m);
        for (auto copula : src->copulas)
        {
            auto ii = findNewIndex(index, copula.i);
            auto jj = findNewIndex(index, copula.j);
            if (ii >= 0 && jj >= 0)
            {
                copula.i = ii;
                copula.j = jj;
                copulas.push_back(copula);
            }
        }
    }

    std::vector<double> CopulaCorrelation::ApplyCorrelation(const std::vector<double>& uValues)
    {
        auto newUvalues = uValues;
        for (const auto& copula : copulas)
        {
            copula.copula->update_uspace(newUvalues[copula.i], newUvalues[copula.j]);
        }
        return newUvalues;
    }

    correlationValueAndType CopulaCorrelation::GetCorrelation(const int i, const int j) const
    {
        for(const auto& p : copulas)
        {
            if (p.i == i && p.j == j) return { p.copula->getCorrelation() };
            if (p.i == j && p.j == i) return { p.copula->getCorrelation() };
        }
        return { lastValue, correlationType::Gaussian };
    }

    correlationValueAndType CopulaCorrelation::GetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2)
    {
        size_t index1 = std::numeric_limits<size_t>::max();
        size_t index2 = std::numeric_limits<size_t>::max();
        for (size_t i = 0; i < stochasts.size(); i++)
        {
            if (stochasts[i] == stochast1) index1 = i;
            if (stochasts[i] == stochast2) index2 = i;
        }

        if (index1 < stochasts.size() && index2 < stochasts.size())
        {
            return GetCorrelation(static_cast<int>(index1), static_cast<int>(index2));
        }
        else
        {
            return { 0, correlationType::Gaussian };
        }

    }

}


