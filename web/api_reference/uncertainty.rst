Uncertainty
===========

This module contains all uncertainty related functionality.

The entry point for performing an uncertainty analysis is probabilistic_library.project.UncertaintyProject.
A model can be attached to an uncertainty project, then stochastic variables, correlation matrix and settings are provided.
When the uncertainty project is run, an uncertainty result is generated, which contains a stochastic variable, which resembles the variation of the output of the model.


.. mermaid::

    classDiagram
        class ModelProject{
            +model ZModel
            +variables list[Stochast]
            +correlation_matrix CorrelationMatrix
        }
        class UncertaintyProject{
            +parameter ModelParameter
            +settings UncertaintySettings
            +results list[UncertaintyResult]
            +result UncertaintyResult
            +run()
        }
        class Stochast{}
        class CorrelationMatrix{}
        class ProbabilityValue{}
        class UncertaintySettings{
            +method UncertaintyMethod
            +quantiles list[ProbabilityValue]
        }
        class StochastSettings{
            +variable Stochast
        }
        class UncertaintyResult{
            +parameter ModelParameter
            +variable Stochast
            +realizations list[Evaluation]
        }
        class Evaluation{}

        UncertaintyProject <|-- ModelProject
        Stochast "*" <-- ModelProject
        CorrelationMatrix <-- ModelProject
        UncertaintySettings <-- UncertaintyProject
        StochastSettings "*" <-- UncertaintySettings
        ProbabilityValue "*" <-- UncertaintySettings
        Stochast <-- StochastSettings
        UncertaintyResult "*" <-- UncertaintyProject
        Stochast <-- UncertaintyResult
        Evaluation "*" <-- UncertaintyResult

.. automodule:: probabilistic_library.uncertainty
   :members:
   :undoc-members:
   :show-inheritance:
