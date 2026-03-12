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
         <strong>Deltares Probabilistic Suite</strong> — a framework for
         uncertainty, reliability, and risk-based engineering analysis.
       </p>
       <div class="psl-hero-ctas">
         <a href="installation.html" class="psl-btn psl-btn--light">
           ▶ &nbsp;Get Started
         </a>
         <a href="https://github.com/Deltares/ProbabilisticLibrary" class="psl-btn psl-btn--outline">
           ⌥ &nbsp;GitHub
         </a>
       </div>
       <div class="psl-hero-stats">
         <div class="psl-stat"><span class="psl-stat-num">6+</span><span class="psl-stat-label">Reliability Methods</span></div>
         <div class="psl-stat"><span class="psl-stat-num">Python</span><span class="psl-stat-label">First-class Interface</span></div>
         <div class="psl-stat"><span class="psl-stat-num">C++</span><span class="psl-stat-label">High-perf Kernel</span></div>
         <div class="psl-stat"><span class="psl-stat-num">Open</span><span class="psl-stat-label">Community-driven</span></div>
       </div>
     </div>
   </div>


Suite Components
----------------

.. grid:: 1 1 3 3
   :gutter: 3
   :class-container: psl-section psl-cards-section

   .. grid-item-card:: ⚡ Probabilistic Library (C++)
      :link: api_reference/index
      :link-type: doc
      :class-card: psl-card
      :class-header: psl-card-header psl-card-header--teal

      High-performance routines for reliability, uncertainty, and sensitivity
      analyses. The computational core powering the entire suite.

      +++
      .. button-ref:: api_reference/index
         :color: primary
         :outline:
         :expand:

         Explore API →

   .. grid-item-card:: 🐍 Python Library
      :link: installation
      :link-type: doc
      :class-card: psl-card
      :class-header: psl-card-header psl-card-header--blue

      A user-friendly Python interface to the probabilistic kernel. Enables
      scripting, automation, and integration with your toolchain.

      +++
      .. button-ref:: installation
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
      probabilistic analyses — no coding required.

      +++
      .. button-link:: https://download.deltares.nl/en/download/probabilistic-toolkit-ptk/
         :color: primary
         :outline:
         :expand:

         Download PTK →


What You'll Find Here
---------------------

.. raw:: html

   <p class="psl-section-sub">
     This GitHub Pages site is maintained by Deltares and the Probabilistic Suite community.
   </p>

.. grid:: 1 2 2 4
   :gutter: 2
   :class-container: psl-find-grid

   .. grid-item-card::
      :class-card: psl-find-card
      :shadow: sm

      **Getting Started Guides**

      Step-by-step instructions to install the Python libraries and run your
      first analyses.

      .. button-ref:: installation
         :color: primary
         :outline:

         Open →

   .. grid-item-card::
      :class-card: psl-find-card
      :shadow: sm

      **Tutorials**

      Usage of probabilistic methods, workflows, and best practices for
      real-world problems.

      .. button-ref:: tutorials/tutorials
         :color: primary
         :outline:

         Open →

   .. grid-item-card::
      :class-card: psl-find-card
      :shadow: sm

      **Gallery of Examples**

      Ready-to-run Jupyter notebooks showcasing the full range of available
      methods.

      .. button-ref:: tutorials/tutorials
         :color: primary
         :outline:

         Open →

   .. grid-item-card::
      :class-card: psl-find-card
      :shadow: sm

      **API Reference**

      Complete auto-generated documentation for every class and function in
      the Python library.

      .. button-ref:: api_reference/index
         :color: primary
         :outline:

         Open →


.. raw:: html

   <div class="psl-community-banner">
     <div class="psl-community-text">
       <h2>Join the Community</h2>
       <p>
         The Probabilistic Suite Community is open and evolving.
         Contributions are welcome — share examples, improve docs, or discuss best practices.
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
   api_reference/index