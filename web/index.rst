:html_theme.sidebar_secondary.remove:
:html_theme.sidebar_primary.remove:

.. raw:: html

   <div class="psl-hero">
     <div class="psl-hero-blobs">
       <div class="psl-blob psl-blob--1"></div>
       <div class="psl-blob psl-blob--2"></div>
       <div class="psl-blob psl-blob--3"></div>
     </div>
     <div class="psl-hero-inner">
       <div class="psl-hero-badge">Deltares · Open Community</div>
       <h1 class="psl-hero-title">Probabilistic Suite<br><em>Community Hub</em></h1>
       <p class="psl-hero-sub">
         The central resource for users and developers of the
         <strong>Deltares Probabilistic Suite</strong>: a framework for
         uncertainty, reliability, and risk-based engineering analysis.
       </p>
       <div class="psl-hero-ctas">
         <a href="installation.html" class="psl-btn psl-btn--light">
           ▶ &nbsp;Get Started
         </a>
         <a href="https://github.com/Deltares/ProbabilisticLibrary/discussions" class="psl-btn psl-btn--light">
           ⌥ &nbsp;Forum
         </a>
         <a href="https://pypi.org/project/probabilistic-library/" class="psl-btn psl-btn--light">
           &#x1F4E6; &nbsp;PyPI
         </a>
       </div>
       <p class="psl-hero-proof">
         &#9679;&nbsp; Used in national flood-safety assessments in the Netherlands
         and applied to geotechnical projects
       </p>
     </div>
   </div>

The Probabilistic Suite enables quantitative analysis of uncertainty, reliability, and sensitivity in engineering and scientific models.

Three tightly integrated components: choose your entry point or use all three together.

.. grid:: 1 1 3 3
   :gutter: 3
   :class-container: psl-section psl-cards-section

   .. grid-item-card:: ⚡ Probabilistic Library (C++)
      :link: https://github.com/Deltares/ProbabilisticLibrary
      :link-type: url
      :class-card: psl-card
      :class-header: psl-card-header psl-card-header--teal

      High-performance routines for reliability, uncertainty, and sensitivity
      analyses. The computational core powering the entire suite.

      +++
      .. button-link:: https://github.com/Deltares/ProbabilisticLibrary
         :color: primary
         :outline:
         :expand:

         Explore API →

   .. grid-item-card:: 🐍 Python Library
      :link: api_reference/index
      :link-type: doc
      :class-card: psl-card
      :class-header: psl-card-header psl-card-header--blue

      A user-friendly Python interface to the probabilistic kernel. Enables
      scripting, automation, and integration with your toolchain.

      +++
      .. button-ref:: api_reference/index
         :color: primary
         :outline:
         :expand:

         View Docs →

   .. grid-item-card:: 🖥️ Probabilistic Toolkit (PTK)
      :link: https://download.deltares.nl/en/download/probabilistic-toolkit-ptk/
      :link-type: url
      :class-card: psl-card
      :class-header: psl-card-header psl-card-header--green

      A graphical user interface for setting up, executing, and visualizing
      probabilistic analyses. No coding required!

      +++
      .. button-link:: https://download.deltares.nl/en/download/probabilistic-toolkit-ptk/
         :color: primary
         :outline:
         :expand:

         Download PTK →

.. raw:: html

   <div class="psl-apart">
     <div class="psl-apart-header">
       <h2 class="psl-apart-title">Why engineers use the suite</h2>
     </div>
     <div class="psl-apart-grid">

       <div class="psl-apart-item">
         <div class="psl-apart-icon">&#9881;</div>
         <div class="psl-apart-body">
           <p class="psl-apart-item-title">Multi-interface, single engine</p>
           <p class="psl-apart-item-desc">
             A graphical interface for practitioners, a clean Python API for scripting and
             automation, and a .NET layer for application builders. All running on one
             consistent, high-performance C++ engine underneath.
           </p>
         </div>
       </div>

       <div class="psl-apart-item">
         <div class="psl-apart-icon">&#10003;</div>
         <div class="psl-apart-body">
           <p class="psl-apart-item-title">Institutional trust</p>
           <p class="psl-apart-item-desc">
             Part of the Deltares software product line. Every component is thoroughly
             reviewed, tested, validated, documented, and actively maintained in stable release. Trusted for the flood safety assessment of the Netherlands.
           </p>
         </div>
       </div>

       <div class="psl-apart-item">
         <div class="psl-apart-icon">&#128279;</div>
         <div class="psl-apart-body">
           <p class="psl-apart-item-title">Plug in your own model</p>
           <p class="psl-apart-item-desc">
             Proven integration with D-Series, FEM solvers, and more. If your model can
             be driven by a Python wrapper, it connects directly to the probabilistic engine.
           </p>
         </div>
       </div>

       <div class="psl-apart-item">
         <div class="psl-apart-icon">&#128202;</div>
         <div class="psl-apart-body">
           <p class="psl-apart-item-title">One library, complete workflow</p>
           <p class="psl-apart-item-desc">
             From uncertainty definition to sensitivity analysis, reliability methods, and
             system assemblage: the full probabilistic workflow in a single framework,
             with methods that chain into end-to-end analysis pipelines.
           </p>
         </div>
       </div>

     </div>
   </div>



.. raw:: html

   <div class="psl-community-banner">
     <div class="psl-community-text">
       <h2>Join the Community</h2>
       <p>
         The Probabilistic Suite Community is open and evolving.
         Join Dutch water engineers, researchers, and consultants building the probabilistic and reliability community together. Share examples, ask questions and contribute 😊.
       </p>
     </div>
     <div class="psl-community-ctas">
       <a href="https://github.com/Deltares/ProbabilisticLibrary" class="psl-btn psl-btn--light">Open GitHub</a>
       <a href="mailto:d-prob-community@deltares.nl" class="psl-btn psl-btn--outline">d-prob-community@deltares.nl</a>
     </div>
   </div>


.. toctree::
   :hidden:
   :maxdepth: 2

   installation
   tutorials/tutorials
   examples_page/examples_index
   api_reference/index
