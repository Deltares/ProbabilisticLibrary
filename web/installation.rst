Getting Started
===============



Installation
------------

The Probabilistic Suite Python libraries can be installed using ``pip``.

It is strongly recommended to install the package in a **virtual environment**.

Create and activate a virtual environment
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

On Windows:

.. code-block:: console

   python -m venv pl-env
   pl-env\Scripts\activate

On Linux / macOS:

.. code-block:: console

   python -m venv pl-env
   source pl-env/bin/activate

Install using pip
^^^^^^^^^^^^^^^^^

Once the environment is activated, install the package:

.. code-block:: console

   pip install probabilistic-library

If you are installing from a local repository:

.. code-block:: console

   pip install .

----

Requirements
------------

- Python 3.11 or newer
- ``pip`` (latest version recommended)


----

Verify the installation
-----------------------

To verify that the installation was successful, start Python and try importing the library:

.. code-block:: python

   import probabilistic_library
   print(probabilistic_library.__version__)

If no error occurs, the installation was successful.

----

Your first example
------------------

Below is a minimal example showing how to perform a statistical conversion in Python.

.. code-block:: python

   from probabilistic_library import StandardNormal
   beta = StandardNormal.get_u_from_p(0.95)
   print(beta)

For more realistic and complete examples, see the `tutorials <./gallery/tutorials.rst>`_ section.

----

Using the Probabilistic Toolkit (PTK)
-------------------------------------

If you prefer a graphical user interface, you can use the **Probabilistic Toolkit (PTK)**.

PTK uses the same probabilistic core as the Python library and allows you to:

- Define uncertain parameters
- Configure analyses
- Visualize results interactively

PTK can be downloaded from the `official Deltares website <https://www.deltares.nl/en/software-and-data/products/probabilistic-toolkit-ptk>`_.

----

Next steps
----------

Now that you are set up, you can:

- Explore the `Tutorials <./gallery/tutorials.rst>`_ to learn about probabilistic methods and workflows
- Read the `API Reference <./api_reference/index.rst>`_ for detailed class and function documentation

----

Need help?
----------

If you encounter issues or have questions:

- Check the `tutorials <./gallery/tutorials.rst>`_ and `documentation <./api_reference/index.rst>`_
- Ask questions or report issues via the `community forum <https://github.com/Deltares/ProbabilisticLibrary/discussions>`_
