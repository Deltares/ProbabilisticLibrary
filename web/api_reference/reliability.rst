Reliability module
==================



This module contains all reliability related functionality.

The entry point for performing a reliability analysis is probabilistic_library.project.ReliabilityProject.
A model can be attached to a reliability project, then stochastic variables, correlation matrix and settings are provided.
When the reliability project is run, a DesignPoint is generated, which contains the results of the reliability analysis.


.. mermaid::

    classDiagram
        class ModelProject{
            +model ZModel
            +variables list[Stochast]
            +correlation_matrix CorrelationMatrix
        }
        class ReliabilityProject{
            +settings Settings
            +design_point DesignPoint
            +run()
        }
        class Stochast{}

        class CorrelationMatrix{}

        class LimitStateFunction{
            +parameter ModelParameter,
            +critical_value float
        }

        class Settings{
            +method ReliabilityMethod
            +limit_state_function LimitStateFunction
        }
        class StochastSettings{
            +variable Stochast
        }

        class DesignPoint{
            +reliability_index float
            +alphas List[Alpha]
            +contributing_design_points List[DesignPoint]
            +realizations List[Evaluation]
        }
        class Alpha{
            +variable Stochast
            +alpha float
        }

        class ReliabilityResult{}

        ReliabilityProject <|-- ModelProject
        Stochast "*" <-- ModelProject
        CorrelationMatrix <-- ModelProject
        Settings <-- ReliabilityProject
        LimitStateFunction <-- Settings
        ModelParameter "parameter, compare" <-- LimitStateFunction
        StochastSettings "*" <-- Settings
        Stochast <-- StochastSettings
        DesignPoint <-- ReliabilityProject
        Alpha "*" <-- DesignPoint
        DesignPoint "*, contributing" <-- DesignPoint
        ReliabilityResult "*" <-- DesignPoint

.. automodule:: probabilistic_library.reliability
   :members:
   :undoc-members:
   :show-inheritance:
