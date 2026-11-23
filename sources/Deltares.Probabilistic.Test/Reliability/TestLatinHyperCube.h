#pragma once

namespace Deltares::Probabilistic::Test
{
    class TestLatinHyperCube
    {
    public:
        static void testDifferentChunkSizes();
        static void testBoundedProbability();
        static void testFailedEvaluations();
        static void testModelReturnTypes();
    };
}