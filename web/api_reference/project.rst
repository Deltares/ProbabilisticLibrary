Project
=======

This module contains all the basic functionality for a project.

A project forms the foundation for performing analyses, such as reliability, uncertainty or sensitivity analyses.
In addition to reliability analyses, additional functionalities are available to combine or upscale reliability results.

A model can be assigned to a project, which can be either a Python script or a model run by the `PTK <https://www.deltares.nl/en/software-and-data/products/probabilistic-toolkit-ptk>`_
(the ptk wheel is required for this). When a model is assigned, stochastic variables and a correlation matrix are automatically generated.


.. mermaid::

    classDiagram
        class ModelProject{
            +model ZModel
            +variables list[Stochast]
            +correlation_matrix CorrelationMatrix
        }
        class Stochast{}
        class CorrelationMatrix{}
        class ZModel{
            +name : string
            +input_parameters : list[ModelParameter]
            +output_parameters : list[ModelParameter]
            ~run()
            ~run_multiple()
        }
        class ZModelContainer{
            +get_model() : ZModel
        }
        class ModelParameter{
            +name : string
        }

        ZModel <-- ModelProject
        ZModel "get_model" <-- ZModelContainer
        ModelParameter "*, input, output " <-- ModelProject
        Stochast <-- ModelProject
        CorrelationMatrix <-- ModelProject

.. automodule:: probabilistic_library.project
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.ZModel
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.ZModelContainer
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.ModelParameter
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.ModelProject
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.RunValuesType
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.RunProjectSettings
   :members:
   :undoc-members:
   :show-inheritance:

.. mermaid::

    classDiagram
        class ModelProject{
            +model ZModel
            +variables list[Stochast]
            +correlation_matrix CorrelationMatrix
        }
        class RunProject{
            +settings RunProjectSettings
            +realization Evalution
            +run()
        }
        class RunProjectSettings{}
        class Stochast{}
        class CorrelationMatrix{}
        class Evaluation{}
        RunProject <|-- ModelProject
        Stochast <-- ModelProject
        CorrelationMatrix <-- ModelProject
        RunProjectSettings <-- RunProject
        Evaluation <-- RunProject


.. autoclass:::: probabilistic_library.project.RunProject
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.SensitivityProject
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.UncertaintyProject
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.ReliabilityProject
   :members:
   :undoc-members:
   :show-inheritance:


.. mermaid::

    classDiagram
        class CombineProject{
            +design_points list[DesignPoint]
            +design_point DesignPoint
            +settings CombineSettings
            +run()
        }

        class Stochast{}
        class CombineSettings{
            +combine_type CombineType
            +combine_method CombineMethod
        }
        class CorrelationMatrix{}
        class SelfCorrelationMatrix{}
        class Stochast{}

        DesignPoint "*, input" <-- CombineProject
        DesignPoint "result" <-- CombineProject
        CombineSettings <-- CombineProject
        CorrelationMatrix <-- CombineProject
        SelfCorrelationMatrix <-- CombineProject
        Stochast "*" <-- CorrelationMatrix
        Stochast "*" <-- SelfCorrelationMatrix


.. autoclass:::: probabilistic_library.project.CombineProject
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.ExcludingCombineProject
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:::: probabilistic_library.project.LengthEffectProject
   :members:
   :undoc-members:
   :show-inheritance:

.. mermaid::

    classDiagram
        class ExcludingCombineProject{
            +design_points list[DesignPoint]
            +scenarios list[Scenario]
            +settings ExcludingCombineSettings
            +design_point DesignPoint
            +run()
        }

        class DesignPoint{}
        class Scenario{}
        class ExcludingCombineSettings{}

        Scenario "*" <-- ExcludingCombineProject
        DesignPoint "*, input" <-- ExcludingCombineProject
        DesignPoint "result" <-- ExcludingCombineProject
        ExcludingCombineSettings <-- ExcludingCombineProject