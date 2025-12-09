# Probabilistic Library

This is the repository of the Probabilistic Library of Deltares.

The library provides a set of routines that enable reliability analysis of a component or a system of components.

## Scientific background

The scientific background document is part of the release,
and can be found here:
[scientific_background.pdf](https://github.com/Deltares/ProbabilisticLibrary/releases/download/25.3.1/scientific_background.pdf)

## Python wrapper

You can access the routines of the library using a Python wrapper. 

### Installation

Request the wheel of the Probabilistic Library from Deltares.

Install the wheel as follows:
```
pip install probabilistic_library-25.3.1-py3-none-any.whl --force-reinstall
```

### Jupyter notebooks
Several Jupyter notebooks are available to demonstrate how to use the Probabilistic Library through the Python wrapper:

See the [Gallery](/sources/Deltares.Probabilistic.PWrapper.Notebooks/gallery/gallery.md) for several demonstrations and instructions how to use the Probabilistic Library.

1. Distribution functions:
    * [Overview of the available distribution functions](sources/Deltares.Probabilistic.PWrapper.Notebooks/overview_distribution_functions.ipynb)

    * [Operations on distribution functions](sources/Deltares.Probabilistic.PWrapper.Notebooks/operations_on_distribution_functions.ipynb)

    * [Composite distributions](sources/Deltares.Probabilistic.PWrapper.Notebooks/composite_distribution.ipynb)

    * [Fitting with prior distribution](sources/Deltares.Probabilistic.PWrapper.Notebooks/fitting_with_prior_distribution.ipynb)

    * [Including statistical uncertainty](sources/Deltares.Probabilistic.PWrapper.Notebooks/statistical_uncertainty.ipynb)

2. [Conversion functions (reliability index to failure probability)](sources/Deltares.Probabilistic.PWrapper.Notebooks/conversion_functions.ipynb)


3. Reliability analysis of a single component:

    * [Simple calculations with FORM and correlation case](sources/Deltares.Probabilistic.PWrapper.Notebooks/FORM_simple_correlations.ipynb)
    
    * [FORM, Crude Monte Carlo and Numerical Integration](sources/Deltares.Probabilistic.PWrapper.Notebooks/reliability_methods_bligh.ipynb)

    * [Directional sampling, Importance sampling, Adaptive importance sampling and Subset simulation](sources/Deltares.Probabilistic.PWrapper.Notebooks/reliability_methods_hunt.ipynb)

    * [Numerical bisection, Latin hypercube and COBYLA](sources/Deltares.Probabilistic.PWrapper.Notebooks/reliability_methods_overtopping.ipynb)

    * [Hybrid reliability methods (FDIR, DSFI)](sources/Deltares.Probabilistic.PWrapper.Notebooks/hybrid_reliability_methods.ipynb)

    * [Reliability calculations with a model](sources/Deltares.Probabilistic.PWrapper.Notebooks/reliability_with_piping_model.ipynb)

    * [Reliability calculations with arrays as arguments in the model](sources/Deltares.Probabilistic.PWrapper.Notebooks/limit_state_function_with_arrays.ipynb)

4. Reliability analysis of a system of components:

    * [Reliability analysis of a system of components](sources/Deltares.Probabilistic.PWrapper.Notebooks/reliability_analysis_system.ipynb)

5. Working with conditional variables:

    * [Conditional variable](sources/Deltares.Probabilistic.PWrapper.Notebooks/conditional_variable.ipynb)

    * [Integration of a fragility curve](sources/Deltares.Probabilistic.PWrapper.Notebooks/failure_probability_fragility_curve.ipynb)

6. Uncertainty analysis:

    * [Simple example](sources/Deltares.Probabilistic.PWrapper.Notebooks/uncertainty_analysis.ipynb)

    * [Safety levels](sources/Deltares.Probabilistic.PWrapper.Notebooks/safety_levels.ipynb)

7. [Sensitivity analysis](sources/Deltares.Probabilistic.PWrapper.Notebooks/sensitivity_analysis.ipynb)

8. [Including spatial correlations (length-effect)](sources/Deltares.Probabilistic.PWrapper.Notebooks/length_effect.ipynb)

9. [Parallel computing and reusing realizations](sources/Deltares.Probabilistic.PWrapper.Notebooks/parallel_overtopping.ipynb)

10. [Visualization of results](sources/Deltares.Probabilistic.PWrapper.Notebooks/visualize_results.ipynb)

11. [Single model run](sources/Deltares.Probabilistic.PWrapper.Notebooks/run_model.ipynb)

12. [Validation](sources/Deltares.Probabilistic.PWrapper.Notebooks/validation.ipynb)

## License

This library is released under the LGPL license. For full text of this license, refer to the [LICENSE](LICENSE) file.

## Developers section
Please check our [Wiki](https://github.com/Deltares/ProbabilisticLibrary/wiki) for a more details.

## Contact
For the wheel or additional information, contact:
* Edwin Spee
* Rob Brinkman
* Karolina Wojciechowska
