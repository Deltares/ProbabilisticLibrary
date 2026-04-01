:html_theme.sidebar_secondary.remove:

.. _case_studies:

Case Studies
============

.. raw:: html

   <p class="psl-page-intro">
     End-to-end worked examples on realistic engineering problems, authored and
     validated by the Deltares team. Each case study demonstrates a complete
     probabilistic workflow: from problem definition and uncertainty
     characterisation to reliability results and interpretation.
   </p>

These examples are more detailed than the :doc:`tutorials <../tutorials/tutorials>`. They use realistic model parameters, discuss engineering context, and show
how to interpret results in a professional setting.

----

.. raw:: html

   <h2 class="psl-category-heading">Geotechnical</h2>

.. grid:: 1 1 2 2
   :gutter: 3

   .. grid-item-card:: Reliability of a Retaining Wall
      :class-card: psl-start-card psl-case-card psl-case-card--geo
      :class-header: psl-case-header psl-case-header--geo

      Assess the probability of failure of a gravity-based retaining wall
      under uncertain soil parameters. Combines
      FORM analysis with Monte Carlo.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">FORM</span>
             <span class="psl-tag">Reliability</span>
             <span class="psl-tag">Geotechnical</span>
           </div>
           <span class="psl-difficulty psl-difficulty--beginner">&#9632;&#9633;&#9633; Beginner</span>
         </div>

      +++
      .. button-link:: ../_examples/Case_1_retaining_wall.html
         :color: primary
         :outline:
         :expand:

         Open example →

   .. grid-item-card:: Connect D-Stability model (GEOLIB)
      :class-card: psl-start-card psl-case-card psl-case-card--geo
      :class-header: psl-case-header psl-case-header--geo

      How to connect D-Stability to a probabilistic workflow using the GEOLIB Python library. Demonstrates the effect of load variability and
      correlated soil parameters.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">D-Stability</span>
             <span class="psl-tag">Stochastic loads</span>
             <span class="psl-tag">Slope stability</span>
             <span class="psl-tag">Geotechnical</span>

           </div>
           <span class="psl-difficulty psl-difficulty--beginner">&#9632;&#9633;&#9633; Beginner</span>
         </div>

      +++
      .. button-link:: slope_stability.html
         :color: primary
         :outline:
         :expand:

         Open example →  *(coming soon)*

   .. grid-item-card:: Connect D-Settlement model (GEOLIB)
      :class-card: psl-start-card psl-case-card psl-case-card--geo
      :class-header: psl-case-header psl-case-header--geo

      Probabilistic settlement analysis by coupling D-Settlement to the
      probabilistic engine via a Python wrapper. Propagate soil parameter
      uncertainty through a full settlement model.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">D-Settlement</span>
             <span class="psl-tag">External model</span>
             <span class="psl-tag">Uncertainty propagation</span>
           </div>
           <span class="psl-difficulty psl-difficulty--beginner">&#9632;&#9633;&#9633; Beginner</span>
         </div>

      +++
      .. button-link:: d_settlement.html
         :color: primary
         :outline:
         :expand:

         Open example →  *(coming soon)*

   .. grid-item-card:: Connect D-SheetPiling model (GEOLIB) — FORM analysis
      :class-card: psl-start-card psl-case-card psl-case-card--geo
      :class-header: psl-case-header psl-case-header--geo

      Connects a D-SheetPiling cantilever sheet pile model via the D-GEOLib
      Python library. Wraps the model as a limit state function and runs a FORM
      analysis with three stochastic soil and hydraulic parameters.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">D-SheetPiling</span>
             <span class="psl-tag">D-GEOLib</span>
             <span class="psl-tag">External model</span>
           </div>
           <span class="psl-difficulty psl-difficulty--beginner">&#9632;&#9633;&#9633; Beginner</span>
         </div>

      +++
      .. button-link:: ../_examples/Case_2_DSheetPiling_Tutorial.html
         :color: primary
         :outline:
         :expand:

         Open example →

   .. grid-item-card:: Fragility Curve for a Sheet Pile Wall — Corrosion. Time dependent reliability analysis.
      :class-card: psl-start-card psl-case-card psl-case-card--geo
      :class-header: psl-case-header psl-case-header--geo

      Builds a fragility curve for a D-SheetPiling wall as a function
      of a corrosion rate parameter. Integration of time-dependent reliability methods to show how failure 
      probability evolves over the structure's lifetime.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">D-SheetPiling</span>
             <span class="psl-tag">Fragility curves</span>
             <span class="psl-tag">Corrosion</span>
             <span class="psl-tag">Time-dependent reliability</span>
           </div>
           <span class="psl-difficulty psl-difficulty--intermediate">&#9632;&#9632;&#9633; Intermediate</span>
         </div>

      +++
      .. button-link:: ../_examples/Case_3_DSheetPiling_FragilityCurve.html
         :color: primary
         :outline:
         :expand:

         Open example →

   .. grid-item-card:: Connect D-Stability model(PTK Wheel)
      :class-card: psl-start-card psl-case-card psl-case-card--geo
      :class-header: psl-case-header psl-case-header--geo

      Slope stability reliability analysis driven by D-Stability. Uses the
      PTK Python wheel integration to evaluate the limit state function, enabling
      FORM and Monte Carlo runs directly against the D-Stability engine.

      .. raw:: html

         <div class="psl-card-meta">
           <div class="psl-tag-row">
             <span class="psl-tag">D-Stability</span>
             <span class="psl-tag">PTK wheel</span>
             <span class="psl-tag">FORM</span>
           </div>
           <span class="psl-difficulty psl-difficulty--intermediate">&#9632;&#9633;&#9633; Intermediate</span>
         </div>

      +++
      .. button-link:: d_stability.html
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
           <span class="psl-difficulty psl-difficulty--beginner">&#9632;&#9633;&#9633; Beginner</span>
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
           <span class="psl-difficulty psl-difficulty--beginner">&#9632;&#9633;&#9633; Beginner</span>
         </div>

      +++
      .. button-link:: fragility_barrier.html
         :color: primary
         :outline:
         :expand:

         Open example →  *(coming soon)*

----

.. note::

   Examples marked *coming soon* are planned for upcoming releases.
   Want to suggest a case study or contribute one?
   Open a discussion on `GitHub <https://github.com/Deltares/ProbabilisticLibrary/discussions>`_.
