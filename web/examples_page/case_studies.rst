:html_theme.sidebar_secondary.remove:

.. _case_studies:

Case Studies
============

.. raw:: html

   <p class="psl-page-intro">
     End-to-end worked examples on realistic engineering problems, authored and
     validated by the Deltares team. Each case study demonstrates a complete
     probabilistic workflow — from problem definition and uncertainty
     characterisation to reliability results and interpretation.
   </p>

These examples are more detailed than the :doc:`tutorials <../tutorials/tutorials>`
— they use realistic model parameters, discuss engineering context, and show
how to interpret results in a professional setting.

----

.. raw:: html

   <h2 class="psl-category-heading">Geotechnical</h2>

.. grid:: 1 1 2 2
   :gutter: 3

   .. grid-item-card:: Reliability of a Retaining Wall
      :class-card: psl-start-card psl-case-card psl-case-card--geo
      :class-header: psl-case-header psl-case-header--geo

      Assess the probability of failure of a sheet pile retaining wall
      under uncertain soil parameters and water levels. Combines
      FORM analysis with sensitivity ranking.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">FORM</span>
             <span class="psl-tag">Sensitivity</span>
             <span class="psl-tag">Geotechnical</span>
           </div>
           <span class="psl-difficulty psl-difficulty--intermediate">&#9632;&#9632;&#9633; Intermediate</span>
         </div>

      +++
      .. button-link:: ../_examples/Case_1_retaining_wall.html
         :color: primary
         :outline:
         :expand:

         Open example →

   .. grid-item-card:: Slope Stability under Uncertain Soil Properties
      :class-card: psl-start-card psl-case-card psl-case-card--geo
      :class-header: psl-case-header psl-case-header--geo

      Probabilistic slope stability analysis using a Bishop-simplified
      model. Demonstrates the effect of spatial variability and
      correlated soil parameters.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">Monte Carlo</span>
             <span class="psl-tag">Correlation</span>
             <span class="psl-tag">Spatial variability</span>
           </div>
           <span class="psl-difficulty psl-difficulty--advanced">&#9632;&#9632;&#9632; Advanced</span>
         </div>

      +++
      .. button-link:: slope_stability.html
         :color: primary
         :outline:
         :expand:

         Open example →  *(coming soon)*

.. raw:: html

   <h2 class="psl-category-heading">Flood Defences</h2>

.. grid:: 1 1 2 2
   :gutter: 3

   .. grid-item-card:: Dike Safety Assessment (Overtopping + Piping)
      :class-card: psl-start-card psl-case-card psl-case-card--flood
      :class-header: psl-case-header psl-case-header--flood

      Full assessment of a primary flood defence against two failure
      mechanisms: wave overtopping and internal erosion (piping).
      Shows how to combine failure probabilities into a system result
      aligned with the Dutch WBI assessment framework.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">System reliability</span>
             <span class="psl-tag">FORM</span>
             <span class="psl-tag">WBI</span>
           </div>
           <span class="psl-difficulty psl-difficulty--advanced">&#9632;&#9632;&#9632; Advanced</span>
         </div>

      +++
      .. button-link:: dike_assessment.html
         :color: primary
         :outline:
         :expand:

         Open example →  *(coming soon)*

   .. grid-item-card:: Fragility Curve for a Storm Surge Barrier
      :class-card: psl-start-card psl-case-card psl-case-card--flood
      :class-header: psl-case-header psl-case-header--flood

      Compute a fragility curve for a hydraulic structure as a function
      of water level, accounting for uncertainty in structural resistance
      and load parameters.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">Fragility curves</span>
             <span class="psl-tag">Uncertainty analysis</span>
             <span class="psl-tag">Hydraulics</span>
           </div>
           <span class="psl-difficulty psl-difficulty--intermediate">&#9632;&#9632;&#9633; Intermediate</span>
         </div>

      +++
      .. button-link:: fragility_barrier.html
         :color: primary
         :outline:
         :expand:

         Open example →  *(coming soon)*

.. raw:: html

   <h2 class="psl-category-heading">Hydraulics &amp; Other</h2>

.. grid:: 1 1 2 2
   :gutter: 3

   .. grid-item-card:: Uncertainty in a Hydrodynamic Model
      :class-card: psl-start-card psl-case-card psl-case-card--hydraulics
      :class-header: psl-case-header psl-case-header--hydraulics

      Propagate parametric uncertainty through an external hydrodynamic
      simulation (D-Flow FM wrapper). Demonstrates how to connect any
      Python-scriptable model to the probabilistic engine.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">Uncertainty propagation</span>
             <span class="psl-tag">External model</span>
             <span class="psl-tag">D-Flow</span>
           </div>
           <span class="psl-difficulty psl-difficulty--intermediate">&#9632;&#9632;&#9633; Intermediate</span>
         </div>

      +++
      .. button-link:: hydrodynamic_uncertainty.html
         :color: primary
         :outline:
         :expand:

         Open example →  *(coming soon)*

   .. grid-item-card:: Response Surface for an Expensive Model
      :class-card: psl-start-card psl-case-card psl-case-card--hydraulics
      :class-header: psl-case-header psl-case-header--hydraulics

      Speed up reliability analysis by replacing a computationally
      expensive FEM model with a surrogate (response surface). Compares
      Monte Carlo results with and without the surrogate.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">Surrogate model</span>
             <span class="psl-tag">Response surface</span>
             <span class="psl-tag">FEM coupling</span>
           </div>
           <span class="psl-difficulty psl-difficulty--advanced">&#9632;&#9632;&#9632; Advanced</span>
         </div>

      +++
      .. button-link:: surrogate_fem.html
         :color: primary
         :outline:
         :expand:

         Open example →  *(coming soon)*

----

.. note::

   Examples marked *coming soon* are planned for upcoming releases.
   Want to suggest a case study or contribute one?
   Open a discussion on `GitHub <https://github.com/Deltares/ProbabilisticLibrary/discussions>`_.
