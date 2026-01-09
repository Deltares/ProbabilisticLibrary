API Reference
=============

The Probabilistic Library provides functionality to perform reliability, uncertainty and sensitivity analyses via a probabilistic_library.project.ReliabilityProject, probabilistic_library.project.UncertaintyProject or probabilistic_library.project.SensitivityProject.

Using an externally defined model as a python script, the Probabilistic Library derives input and output values. They will be provided to the user as probabilistic_library.statistic.Stochasts, so that the user can provide uncertainty to them. Correlations are provided via a probabilistic_library.statistic.CorrelationMatrix.

When running an analysis, the result is a probabilistic_library.reliability.DesignPoint, probabilistic_library.uncertainty.UncertaintyResult or probabilistic_library.sensitivity.SensitivityResult.
s


.. toctree::
   :maxdepth: 2

   project
   statistics
   reliability
   uncertainty
   sensitivity
   utils