Statistics
==========

This module contains all statistics related functionality.

The most important class in this module is Stochast. It defines the stochastic properties of a stochastic variable,
which is part of a project (which is the main entry point for performing a reliability, uncertainty or sensitivity analysis).


.. mermaid::

    classDiagram
        class Stochast{
            +distribution : DistributionType
            +"defining properties" : float
            +"derived properties" : float
            +is_conditional : bool
            +conditional_source : Stochast
        }
        class HistogramValue{
            +lower_bound: float
            +upper_bound: float
            +amount: float
            +reliability_index: float
        }
        class DiscreteValue{
            +x: float
            +amount: float
        }
        class FragilityValue{
            +x: float
            +reliability_index: float
            +design_point : DesignPoint
        }
        class DesignPoint{
        }
        class ContributingStochast{
            +probability float
            +variable Stochast
        }
        class ConditionalValue{
            +x: float
            +"defining properties"
        }
        HistogramValue "*" <-- Stochast
        DiscreteValue "*" <-- Stochast
        FragilityValue "*" <-- Stochast
        Stochast "conditional source" <-- Stochast
        Stochast "conditional source" <-- Stochast
        ConditionalValue "*" <-- Stochast
        Stochast <-- ContributingStochast
        ContributingStochast "*" <-- Stochast
        DesignPoint <-- FragilityValue

.. automodule:: probabilistic_library.statistic
   :members:
   :undoc-members:
   :show-inheritance:
