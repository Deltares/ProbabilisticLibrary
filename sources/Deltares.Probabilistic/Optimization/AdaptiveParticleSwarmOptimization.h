#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <vector>

class APSO
{
public:
    APSOOptions Options;

    OptimizationSample GetCalibrationPoint(
        SearchArea& searchArea,
        IOptimizationModel& model)
    {
        std::mt19937 rng(Options.Seed);
        std::uniform_real_distribution<double> uniform(0.0, 1.0);

        std::vector<OptimizationSample> population;

        OptimizationSample bestParticle =
            InitializePopulation(
                searchArea,
                model,
                population,
                rng);

        const int particleSize =
            static_cast<int>(
                searchArea.Dimensions.size());

        int generationIndex = 0;
        int lastImprovingGeneration = 0;

        while (generationIndex < Options.GenerationCount)
        {
            std::vector<OptimizationSample> elite =
                population;

            std::sort(
                elite.begin(),
                elite.end(),
                const OptimizationSample& a,
                   const OptimizationSample& b
                {
                    return a.ModelValue <
                           b.ModelValue;
                });

            if (elite.size() >
                static_cast<size_t>(
                    Options.EliteCount))
            {
                elite.resize(
                    Options.EliteCount);
            }

            double algorithmDivider =
                std::max(
                    0.1,
                    0.9 -
                    generationIndex /
                    (0.9 *
                     static_cast<double>(
                         Options.GenerationCount)));

            for (int m = 0;
                 m < Options.PopulationCount;
                 ++m)
            {
                //
                // APSO branch
                //
                if (uniform(rng) <
                    algorithmDivider)
                {
                    double alpha =
                        std::pow(
                            Options.Delta,
                            generationIndex);

                    for (int n = 0;
                         n < particleSize;
                         ++n)
                    {
                        if (uniform(rng) <
                            Options.CrossOver)
                        {
                            double randomApso =
                                alpha *
                                ((uniform(rng) * 2.0)
                                 - 1.0);

                            double prevRatio =
                                searchArea
                                    .Dimensions[n]
                                    .GetRelativeValue(
                                        population[m]
                                            .Input[n]);

                            double bestRatio =
                                searchArea
                                    .Dimensions[n]
                                    .GetRelativeValue(
                                        bestParticle
                                            .Input[n]);

                            double newRatio =
                                (1.0 -
                                 Options.Beta) *
                                    prevRatio +
                                Options.Beta *
                                    bestRatio +
                                randomApso;

                            if (searchArea
                                    .Dimensions[n]
                                    .Move ||
                                (newRatio >= 0.0 &&
                                 newRatio <= 1.0))
                            {
                                population[m]
                                    .Input[n] =
                                    searchArea
                                        .Dimensions[n]
                                        .GetAbsoluteValue(
                                            newRatio);
                            }
                            else
                            {
                                population[m]
                                    .Input[n] =
                                    bestParticle
                                        .Input[n];
                            }
                        }
                    }
                }
                //
                // Differential Evolution branch
                //
                else
                {
                    std::uniform_int_distribution<int>
                        eliteDist(
                            0,
                            static_cast<int>(
                                elite.size()) -
                                1);

                    OptimizationSample de0 =
                        elite[eliteDist(rng)];

                    OptimizationSample de1 =
                        elite[eliteDist(rng)];

                    OptimizationSample de2 =
                        elite[eliteDist(rng)];

                    OptimizationSample de3 =
                        elite[eliteDist(rng)];

                    for (int q = 0;
                         q < particleSize;
                         ++q)
                    {
                        if (uniform(rng) <
                            Options.CrossOver)
                        {
                            double bestRatio =
                                searchArea
                                    .Dimensions[q]
                                    .GetRelativeValue(
                                        bestParticle
                                            .Input[q]);

                            double r0 =
                                searchArea
                                    .Dimensions[q]
                                    .GetRelativeValue(
                                        de0.Input[q]);

                            double r1 =
                                searchArea
                                    .Dimensions[q]
                                    .GetRelativeValue(
                                        de1.Input[q]);

                            double r2 =
                                searchArea
                                    .Dimensions[q]
                                    .GetRelativeValue(
                                        de2.Input[q]);

                            double r3 =
                                searchArea
                                    .Dimensions[q]
                                    .GetRelativeValue(
                                        de3.Input[q]);

                            double newRatio =
                                bestRatio +
                                Options
                                    .DifferentialWeight *
                                    (r0 - r1 +
                                     r2 - r3);

                            if (searchArea
                                    .Dimensions[q]
                                    .Move ||
                                (newRatio >= 0.0 &&
                                 newRatio <= 1.0))
                            {
                                population[m]
                                    .Input[q] =
                                    searchArea
                                        .Dimensions[q]
                                        .GetAbsoluteValue(
                                            newRatio);
                            }
                            else
                            {
                                population[m]
                                    .Input[q] =
                                    bestParticle
                                        .Input[q];
                            }
                        }
                    }
                }

                population[m].ModelValue =
                    model.GetZValue(
                        population[m].Input,
                        generationIndex);

                if (population[m].ModelValue <
                    bestParticle.ModelValue)
                {
                    bestParticle =
                        population[m];

                    lastImprovingGeneration =
                        generationIndex;
                }
            }

            model.ReportResult(
                bestParticle.ModelValue,
                generationIndex + 1,
                Options.GenerationCount);

            if (generationIndex -
                    lastImprovingGeneration >=
                Options
                    .StopAfterNonImprovingGenerations)
            {
                break;
            }

            ++generationIndex;
        }

        return bestParticle;
    }

private:
    OptimizationSample InitializePopulation(
        SearchArea& searchArea,
        IOptimizationModel& model,
        std::vector<OptimizationSample>& population,
        std::mt19937& rng)
    {
        std::uniform_real_distribution<double>
            uniform(0.0, 1.0);

        const int particleSize =
            static_cast<int>(
                searchArea.Dimensions.size());

        OptimizationSample bestParticle;
        bool first = true;

        for (int k = 0;
             k < Options.PopulationCount;
             ++k)
        {
            OptimizationSample sample(
                particleSize);

            double b = uniform(rng);
            double c = uniform(rng);

            c = (c < 0.5)
                    ? (1.0 - c)
                    : c;

            double a =
                (uniform(rng) - b) /
                (c * c);

            for (int genomeIndex = 0;
                 genomeIndex < particleSize;
                 ++genomeIndex)
            {
                double genomeRatio =
                    particleSize > 1
                        ? static_cast<double>(
                              genomeIndex) /
                              (particleSize - 1)
                        : 0.0;

                double ratio =
                    a *
                        (genomeRatio - c) *
                        (genomeRatio - c) +
                    b;

                sample.Input[genomeIndex] =
                    searchArea
                        .Dimensions[genomeIndex]
                        .GetAbsoluteValue(
                            ratio);
            }

            sample.ModelValue =
                model.GetZValue(
                    sample.Input,
                    -1);

            population.push_back(sample);

            if (first ||
                sample.ModelValue <
                    bestParticle.ModelValue)
            {
                bestParticle = sample;
                first = false;
            }

            model.ReportResult(
                bestParticle.ModelValue,
                0,
                Options.GenerationCount);
        }

        return bestParticle;
    }
};

