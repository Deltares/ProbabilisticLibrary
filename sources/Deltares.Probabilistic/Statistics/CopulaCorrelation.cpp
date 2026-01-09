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
#include <format>

namespace Deltares::Statistics
{
    using enum CorrelationType;

    void CopulaCorrelation::SetCorrelation(const int i, const int j, double value, CorrelationType type)
    {
        if (i >= max_stochasts)
        {
            throw Reliability::probLibException("Invalid index in SetCorrelation: ", i);
        }
        else if (j >= max_stochasts)
        {
            throw Reliability::probLibException("Invalid index in SetCorrelation: ", j);
        }

        auto pair = CopulaPair();
        pair.index1 = i;
        pair.index2 = j;
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
        case Gaussian:
            pair.copula = std::make_shared<GaussianCopula>(value);
            if (value == 0.0)
            {
                RemovePair(pair);
                return;
            }
        }

        auto index = FindPair(pair);
        if (index < copulas.size())
        {
            copulas[index] = pair;
        }
        else
        {
            copulas.push_back(pair);
        }

    }

    size_t CopulaCorrelation::FindPair(const CopulaPair& pair) const
    {
        for (size_t i = 0; i < copulas.size(); i++)
        {
            if (copulas[i].index1 == pair.index1 && copulas[i].index2 == pair.index2) return i;
            if (copulas[i].index1 == pair.index2 && copulas[i].index2 == pair.index1) return i;
        }
        return std::numeric_limits<size_t>::max();
    }

    void CopulaCorrelation::RemovePair(const CopulaPair& pair)
    {
        const auto index = FindPair(pair);
        if (index < copulas.size())
        {
            copulas[index] = copulas.back();
            copulas.pop_back();
        }
    }

    bool CopulaCorrelation::IsValid()
    {
        auto report = Logging::ValidationReport();
        Validate(report);
        return report.messages.empty();
    }

    void CopulaCorrelation::Validate(Logging::ValidationReport& report)
    {
        for (size_t i = 0; i < copulas.size(); i++)
        {
            const auto& c = copulas[i];
            c.copula->validate(report);
            if (c.index1 == c.index2)
            {
                auto msg = std::make_shared<Logging::Message>();
                msg->Text = "Self correlation not allowed for copulas, found for stochast ";
                if (stochasts.empty())
                {
                    msg->Text += std::to_string(c.index1);
                }
                else
                {
                    msg->Text += stochasts[c.index1]->name;
                }
                msg->Type = Logging::MessageType::Error;
                report.messages.push_back(msg);
            }
            for (size_t j = 0; j < i; j++)
            {
                const auto& other = copulas[j];
                if (!c.AreLinked(other)) continue;
                auto msg = std::make_shared<Logging::Message>();
                constexpr auto fmt = "Multiple correlations not allowed for copulas, found for correlations {:} and {:}";
                if (stochasts.empty())
                {
                    msg->Text = std::format(fmt, i, j);
                }
                else
                {
                    const auto pair1 = stochasts[c.index1]->name + "-" + stochasts[c.index2]->name;
                    const auto pair2 = stochasts[other.index1]->name + "-" + stochasts[other.index2]->name;
                    msg->Text = std::format(fmt, pair1, pair2);
                }
                msg->Type = Logging::MessageType::Error;
                msg->Subject = "Copula correlation";
                report.messages.push_back(msg);
            }
        }
    }

    void CopulaCorrelation::SetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2 , double value, CorrelationType type)
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

    void CopulaCorrelation::Filter(const std::shared_ptr<BaseCorrelation> source, const std::vector<int>& index)
    {
        auto src = std::dynamic_pointer_cast<CopulaCorrelation> (source);
        if (src == nullptr) throw Reliability::probLibException("error casting a correlation source in filter method.");
        for (auto copula : src->copulas)
        {
            auto ii = FindNewIndex(index, copula.index1);
            auto jj = FindNewIndex(index, copula.index2);
            if (ii >= 0 && jj >= 0)
            {
                copula.index1 = ii;
                copula.index2 = jj;
                copulas.push_back(copula);
            }
        }
    }

    std::vector<double> CopulaCorrelation::ApplyCorrelation(const std::vector<double>& u_values)
    {
        auto new_u_values = u_values;
        for (const auto& copula : copulas)
        {
            copula.copula->update_uspace(new_u_values[copula.index1], new_u_values[copula.index2]);
        }
        return new_u_values;
    }

    CorrelationValueAndType CopulaCorrelation::GetCorrelation(const int i, const int j) const
    {
        for(const auto& p : copulas)
        {
            if (p.index1 == i && p.index2 == j) return { p.copula->getCorrelation() };
            if (p.index1 == j && p.index2 == i) return { p.copula->getCorrelation() };
        }
        return { 0.0, CorrelationType::Gaussian };
    }

    CorrelationValueAndType CopulaCorrelation::GetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2)
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
            return { 0, CorrelationType::Gaussian };
        }

    }

}


