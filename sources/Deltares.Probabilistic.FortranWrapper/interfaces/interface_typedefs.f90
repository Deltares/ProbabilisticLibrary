! Copyright (C) Stichting Deltares. All rights reserved.
!
! This file is part of Streams.
!
! Streams is free software: you can redistribute it and/or modify
! it under the terms of the GNU Affero General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
!
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
! GNU Affero General Public License for more details.
!
! You should have received a copy of the GNU Affero General Public License
! along with this program. If not, see <http://www.gnu.org/licenses/>.
!
! All names, logos, and references to "Deltares" are registered trademarks of
! Stichting Deltares and remain full property of Stichting Deltares at all times.
! All rights reserved.
!
module interface_typedefs
  use, intrinsic :: iso_c_binding, only: c_double, c_bool
  use precision, only : wp
  use interface_enums, only : GeorgeMarsaglia
  use interface_gen, only : tError
  private :: wp, GeorgeMarsaglia, tError

  integer, parameter :: maxActiveStochast = 32

  type, public, bind(c) :: basicCorrelation
    integer             :: first                !< Index of the first stochastic variable
    integer             :: second               !< Index of the second stochastic variable
    real(kind=c_double) :: correlation          !< Correlation coefficient
  end type basicCorrelation

  type, public, bind(c) :: tdistrib
    integer             :: distributionId
    real(kind=c_double) :: params(4)
  end type tdistrib

  type, public, bind(c) :: tpNumericalBisectionSettings
    integer             :: minimumIterations = 8
    integer             :: maximumIterations = 50
    real(kind=c_double) :: epsilonBeta = 0.01_wp
  end type tpNumericalBisectionSettings

  type, public, bind(c) :: tpLatinHypercubeSettings
      integer           :: MinimumSamples = 1000
  end type tpLatinHypercubeSettings
  type, public, bind(c) :: tpCobylaReliability
    real(kind=c_double) :: EpsilonBeta = 1e-3_c_double
    integer             :: MaximumIterations = 150
  end type tpCobylaReliability

  type, public, bind(c) :: tMethod
    integer             :: methodId
    real(kind=c_double) :: tolA       = 0.001_c_double
    real(kind=c_double) :: tolB       = 0.1_c_double
    real(kind=c_double) :: tolC       = 0.001_c_double
    integer             :: numThreads = 1
    integer             :: chunkSize  = 200
    integer             :: minSamples = 10000
    integer             :: maxSamples = 99000
    integer             :: seed1      = 2
    integer             :: seed2      = 1
    integer             :: isRepeatableRandom = 1
    integer             :: rnd        = GeorgeMarsaglia
    integer             :: startMethod
    integer             :: progressInterval = -1
    real(kind=c_double) :: varianceFactor = 1.5
    real(kind=c_double) :: epsilonDu      = 0.001_c_double
    integer             :: designPointOption = 3
    real(kind=c_double) :: du
    real(kind=c_double) :: du1
    real(kind=c_double) :: relaxationFactor
    integer             :: trialLoops
    integer             :: iterationMethod      !< iterationMethod or minFailed
    integer             :: numExtraInt          !< Number intervals or maxSamplesDef
    integer             :: numExtraInt2 = 0     !< global model option
    real(kind=c_double) :: numExtraReal1        !< Minimum U value or epsFailed
    real(kind=c_double) :: numExtraReal2        !< Maximum U value or increaseVariance
    real(kind=c_double) :: startVector(maxActiveStochast)
    real(kind=c_double) :: offsets(maxActiveStochast)
    real(kind=c_double) :: varianceFactors(maxActiveStochast)
    type(tpNumericalBisectionSettings) :: numericalBisectionSettings
    type(tpLatinHypercubeSettings)     :: latinHypercubeSettings
    type(tpCobylaReliability) :: cobylaReliability
  end type tMethod

  type, public, bind(c) :: tResult
    type(tError)         :: error
    real(kind=c_double)  :: beta
    real(kind=c_double)  :: alpha(maxActiveStochast)
    integer              :: stepsNeeded
    integer              :: samplesNeeded
    logical(kind=c_bool) :: convergence
  end type tResult

  type, public, bind(c) :: tCompIds
    integer :: id
    integer ::  nrStochasts
    integer :: nrCorrelations
  end type tCompIds

  type, public :: tpFORM
      integer                     :: startMethod          !< Method for startvector
      integer                     :: maxIterations        !< Maximum number of iterations
      integer                     :: trialLoops           !< Number of trial loops
      real(kind=wp)               :: epsilonBeta          !< Epsilon Beta (relative accuracy)
      real(kind=wp)               :: epsilonZvalue        !< Epsilon Zfunction (relative accuracy)
      real(kind=wp)               :: relaxationFactor     !< Relaxation factor
      real(kind=wp)               :: dU                   !< Interval size for derivatives
      real(kind=wp), allocatable  :: startValue(:)        !< Start values u for FORM computation
  end type tpFORM

  type, public :: tpNI
      integer                     :: numberIntervals      !< Number intervals
      real(kind=wp)               :: minimumUvalue        !< Minimum U value
      real(kind=wp)               :: maximumUvalue        !< Maximum U value
  end type tpNI

  type, public :: tpDS
      integer                     :: seedPRNG             !< Start value random generator
      logical                     :: isRepeatableRandom   !< true: sampling is reproducable
      integer                     :: minimumSamples       !< Minimum number samples (>1)
      integer                     :: maximumSamples       !< Maximum number samples
      real(kind=wp)               :: varCoeffFailure      !< Required variation coefficient Failure
      real(kind=wp)               :: varCoeffNoFailure    !< Required variation coefficient Non Failure
      integer                     :: iterationMethod      !< Iteration method
      integer                     :: samplingMethod       !< Sampling method
      integer                     :: iterations1          !< Number iterations method 1
      real(kind=wp)               :: du1                  !< Step size methode 1
      integer                     :: maximumIterations2   !< Maximum number iterations method 2
      real(kind=wp)               :: initialDu2           !< Initial step size du method 2
      real(kind=wp)               :: maximumDu2           !< Maximum step size du method 2
      real(kind=wp)               :: epsilonDu            !< Epsilon step size du
      real(kind=wp)               :: maximumLengthU       !< Maximum length u-vector
  end type tpDS

  type, public :: tpCMC
      integer                     :: seedPRNG             !< Start value random generator
      logical                     :: isRepeatableRandom   !< true: sampling is reproducable
      integer                     :: minimumSamples       !< Minimum number samples (>1)
      integer                     :: maximumSamples       !< Maximum number samples
      real(kind=wp)               :: varCoeffFailure      !< Required variation coefficient Failure
      real(kind=wp)               :: varCoeffNoFailure    !< Required variation coefficient Non failure
  end type tpCMC

  type, public :: tpIS
      integer                     :: seedPRNG             !< Start value random generator
      logical                     :: isRepeatableRandom   !< true: sampling is reproducable
      integer                     :: minimumSamples       !< Minimum number samples (>1)
      integer                     :: maximumSamples       !< Maximum number samples
      real(kind=wp)               :: varCoeffFailure      !< Required variation coefficient Failure
      real(kind=wp)               :: varCoeffNoFailure    !< Required variation coefficient Non failure
      real(kind=wp)               :: varianceFactor       !< Multiplicative factor to increase the standard deviation of the standard normal distribution in the u-space
  end type tpIS

  type, public :: tpAdaptiveIS
      integer                     :: seedPRNG                    !< Start value random generator
      logical                     :: isRepeatableRandom          !< true: sampling is reproducable
      integer                     :: minimumSamples              !< Minimum number samples (>1)
      integer                     :: maximumSamples              !< Maximum number samples
      real(kind=wp)               :: varCoeffFailure             !< Required variation coefficient
      integer                     :: Nadp                        !< Maximum number of loops
      integer                     :: startMethod                 !< Method for startvector
      real(kind=wp), allocatable  :: varianceFactor(:)           !< Multiplicative factor to increase the standard deviation of the standard normal distribution in the u-space
      real(kind=wp), allocatable  :: translation(:)              !< Translatory term for the u vector
      real(kind=wp), allocatable  :: startValue(:)               !< Start values u for Adaptive MCIS computation
      real(kind=wp)               :: epsFailed                   !< epsilon failed (test on convergence)
      integer                     :: minFailed                   !< check whether variance must be increased
      real(kind=wp)               :: increaseVariance            !< additional value when updating variance
      logical                     :: AutoMaximumSamples = .true. !< flag for maximum number of samples in final step
  end type tpAdaptiveIS

  type, public :: tpMeth
    type(tpFORM)        :: FORM               !< Data for FORM method
    type(tpCMC)         :: CMC                !< Data for CMC method
    type(tpDS)          :: DS                 !< Data for DS method
    type(tpIS)          :: IS                 !< Data for IS method
    type(tpAdaptiveIS)  :: AdaptiveIS         !< Data for AdaptiveMCIS method
    type(tpNI)          :: NI                 !< Data for NI method
    type(tpNumericalBisectionSettings) :: &
        numericalBisectionSettings            !< data for numerical bisection method
    type(tpLatinHypercubeSettings)     :: &
        latinHypercubeSettings                !< data for Latin hyper cube method
    type(tpCobylaReliability) :: &
                      CobylaReliability       !< Data for Cobyla Reliability
    integer        :: DPoption                !< Options for calculation of the design point
    integer        :: maxParallelThreads = 1  !< Maximum number of OpenMP threads
    integer        :: chunkSizeOpenMP = 200   !< Number of samples to handle simultaneously, when using OpenMP
    integer        :: calcMethod              !< calculation method (used for combination of methods)
  end type tpMeth

  type, public :: tpStoch
    integer                                       :: maxStochasts               !< Maximum number stochastic variables
    integer                                       :: maxExternalStochasts       !< Maximum number external variables
    integer, allocatable                          :: activeX(:)                 !< Used for X stochast in limit state function or not
    integer, allocatable                          :: distTypeX(:)               !< Distribution type of X stochast
    real(kind=wp), allocatable                    :: distParameterX(:,:)        !< Parameters of distribution of X stochast
  end type tpStoch

  type, public :: probabilisticDataStructure_data
    type(tpStoch) :: stoVar
    type(tpMeth)  :: method
    integer       :: number_correlations    = 0
    integer       :: metaId(10)
    logical       :: databaseConvergence
    logical       :: databaseConvergenceDetailed
    type(basicCorrelation), dimension(:), allocatable :: basic_correlation
  end type probabilisticDataStructure_data

    type storedConvergenceDataFORM
        integer                    :: numberIterations
        real(kind=wp), allocatable :: beta(:), scaledz(:), z(:), deviationBeta(:)
        character(len=64)          :: errormsgs
    end type storedConvergenceDataFORM

    type convDataSamplingMethods
        integer                    :: numberSamples
        integer                    :: convergenceType
        character(len=64)          :: errorMessage
        real(kind=wp), allocatable :: rbeta(:), pf(:), pfsamp(:), covar(:)
    end type convDataSamplingMethods

    type, public :: storedConvergenceData
      type(convDataSamplingMethods)   :: cnvg_data_ds     !< Convergence data directional sampling
      type(storedConvergenceDataFORM) :: cnvg_data_form   !< Convergence data FORM
      logical                         :: conv             !< Convergence indicator
      logical                         :: convCriterium    !< Convergence criterium indicator
    end type storedConvergenceData

    type, public, bind(c) :: computationSetting
        integer :: designPointSetting
        integer :: computationId
        integer :: threadId
        integer :: reliabilityMethodSubStepsCounter
    end type computationSetting

end module interface_typedefs
