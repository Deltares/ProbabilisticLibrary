Welcome to the Community Page of the Deltares Probabilistic Suite
=================================================================

This is the documentation of RA2CE (*just say race!*) - the Resilience Assessment and
Adaptation for Critical infrastructurE Toolkit Python Package developed by Deltares.
RA2CE helps to quantify resilience of critical infrastructure networks, prioritize
interventions and adaptation measures and select the most appropriate action perspective
to increase resilience considering future conditions.

Or navigate directly to another section:

Model Project Diagram
====================

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



.. toctree::
   :caption: Table of Contents
   :maxdepth: 1
   :titlesonly:


   api_reference/index.rst



Lost..? Try the :ref:`search`.