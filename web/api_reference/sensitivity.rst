Sensitivity
===========

This module contains all sensitivity related functionality.

The entry point for performing a sensitivity analysis is probabilistic_library.project.SensitivityProject.
A model can be attached to a sensitivity project, then stochastic variables, correlation matrix and settings are provided.
When the sensitivity project is run, a sensitivity result is generated.


.. mermaid::

    classDiagram
        class ModelProject{
            +model ZModel
            +variables list[Stochast]
            +correlation_matrix CorrelationMatrix
        }
        class SensitivityProject{
            +parameter ModelParameter
            +settings SensitivitySettings
            +results list[SensitivityResult]
            +result SensitivityResult
            +run()
        }
        class Stochast{}
        class CorrelationMatrix{}
        class SensitivitySettings{
            +method SensitivityMethod
        }
        class StochastSettings{
            +variable Stochast
        }
        class SensitivityResult{
            +parameter ModelParameter
            +realizations list[Evaluation]
        }
        class Evaluation{}

        SensitivityProject <|-- ModelProject
        Stochast "*" <-- ModelProject
        CorrelationMatrix <-- ModelProject
        SensitivitySettings <-- SensitivityProject
        StochastSettings "*" <-- SensitivitySettings
        Stochast <-- StochastSettings
        SensitivityResult "*" <-- SensitivityProject
        Evaluation "*" <-- SensitivityResult

.. automodule:: probabilistic_library.sensitivity
   :members:
   :undoc-members:
   :show-inheritance:
