module interface_probCalc
  use, intrinsic :: iso_c_binding, only: c_double, c_bool
  use interface_gen
  use interface_distributions
  use interface_correlation
  use precision
  use m_realloc_check
  use f2c_tools
  implicit none

  integer, parameter :: sizeSmallStr = 32
  integer, parameter :: maxActiveStochast = 32

  integer, parameter :: stochastNotActive                             = -1
  integer, parameter :: stochastDeterminist                           =  0
  integer, parameter :: stochastActive                                =  1

  integer, parameter :: methodFORM                                       =  1
  integer, parameter :: methodCrudeMonteCarlo                            =  3
  integer, parameter :: methodDirectionalSampling                        =  4
  integer, parameter :: methodNumericalIntegration                       =  5
  integer, parameter :: methodImportanceSampling                         =  6
  integer, parameter :: methodAdaptiveImportanceSampling                 =  7
  integer, parameter :: methodFORMandDirSampling                         = 11
  integer, parameter :: methodDirSamplingWithFORMiterations              = 12
  integer, parameter :: methodCrudeMonteCarloWithFORMiterations          = 13
  integer, parameter :: methodImportanceSamplingWithFORMiterations       = 14
  integer, parameter :: methodFORMandDirSamplingWithFORMiterations       = 15
  integer, parameter :: methodDirSamplingWithFORMiterationsStartU        = 16
  integer, parameter :: methodFORMandDirSamplingWithFORMiterationsStartU = 17
  integer, parameter :: methodFORMstart                                  = 1234

  integer, parameter :: GeorgeMarsaglia = 1
  integer, parameter :: MersenneTwister = 2

  type, public, bind(c) :: tdistrib
    character(len=1)    :: name(sizeSmallStr)
    integer             :: distributionId
    real(kind=c_double) :: params(4)
  end type tdistrib

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
  end type tMethod

  type, public, bind(c) :: tResult
    real(kind=c_double)  :: beta
    real(kind=c_double)  :: alpha(maxActiveStochast)
    integer              :: iPoint(maxActiveStochast)
    integer              :: stepsNeeded
    integer              :: samplesNeeded
    logical(kind=c_bool) :: convergence
  end type tResult

  integer, parameter :: designPointNone                               =  0
  integer, parameter :: designPointXfromU                             =  1
  integer, parameter :: designPointXCorrelatedFromU                   =  2
  integer, parameter :: designPointRMin                               =  3
  integer, parameter :: designPointRMinZFunc                          =  4
  integer, parameter :: designPointXCorrelatedFromUCompatible         =  5
  integer, parameter :: designPointRMinZFuncCompatible                =  6
  integer, parameter :: designPointMultiplyInvCholesky                =  7

  integer, parameter :: fORMStartZero                                 =  1
  integer, parameter :: fORMStartOne                                  =  2
  integer, parameter :: fORMStartGivenVector                          =  3
  integer, parameter :: fORMStartRaySearch                            =  4
  integer, parameter :: fORMStartSphereSearch                         =  5
  integer, parameter :: fORMStartRaySearchNewDeprecated               =  6
  integer, parameter :: fORMStartRaySearchWind                        =  7
  integer, parameter :: fORMStartRaySearchVector                      =  8
  integer, parameter :: fORMStartRaySearchVectorScaled                =  9
  integer, parameter :: fORMStartSphereSearchKW                       =  10

  integer, parameter :: DirSamplingIterMethodRobust                   =  1

  integer, parameter :: designPointOutputFALSE                        =  0
  integer, parameter :: designPointOutputTRUE                         =  1
  integer, parameter :: designPointOutputPrintAll                     =  3

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
      integer                     :: minimumSamples       !< Minimum number samples (>1)
      integer                     :: maximumSamples       !< Maximum number samples
      real(kind=wp)               :: varCoeffFailure      !< Required variation coefficient Failure
      real(kind=wp)               :: varCoeffNoFailure    !< Required variation coefficient Non failure
  end type tpCMC

  type, public :: tpIS
      integer                     :: seedPRNG             !< Start value random generator
      integer                     :: minimumSamples       !< Minimum number samples (>1)
      integer                     :: maximumSamples       !< Maximum number samples
      real(kind=wp)               :: varCoeffFailure      !< Required variation coefficient Failure
      real(kind=wp)               :: varCoeffNoFailure    !< Required variation coefficient Non failure
      real(kind=wp)               :: varianceFactor       !< Multiplicative factor to increase the standard deviation of the standard normal distribution in the u-space
  end type tpIS

  type, public :: tpAdaptiveIS
      integer                     :: seedPRNG             !< Start value random generator
      integer                     :: minimumSamples       !< Minimum number samples (>1)
      integer                     :: maximumSamples       !< Maximum number samples
      real(kind=wp)               :: varCoeffFailure      !< Required variation coefficient Failure
      real(kind=wp)               :: varCoeffNoFailure    !< Required variation coefficient Non failure
      integer                     :: Nadp                 !< Maximum number of loops
      integer                     :: startMethod          !< Method for startvector
      real(kind=wp), allocatable  :: varianceFactor(:)    !< Multiplicative factor to increase the standard deviation of the standard normal distribution in the u-space
      real(kind=wp), allocatable  :: translation(:)       !< Translatory term for the u vector
      real(kind=wp), allocatable  :: startValue(:)        !< Start values u for Adaptive MCIS computation
      real(kind=wp)               :: epsFailed            !< epsilon failed (test on convergence)
      integer                     :: maxSamplesDef        !< Maximum numbers samples in Recalculate
      integer                     :: minFailed            !< check whether variance must be increased
      real(kind=wp)               :: increaseVariance     !< additional value when updating variance
      integer                     :: globalModelOption = 0 !< global model option (works identical to Fortran or not)
  end type tpAdaptiveIS

  type, public :: tpMeth
    type(tpFORM)        :: FORM               !< Data for FORM method
    type(tpCMC)         :: CMC                !< Data for CMC method
    type(tpDS)          :: DS                 !< Data for DS method
    type(tpIS)          :: IS                 !< Data for IS method
    type(tpAdaptiveIS)  :: AdaptiveIS         !< Data for AdaptiveMCIS method
    type(tpNI)     :: NI                      !< Data for NI method
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
    !type(interpolationTable), allocatable         :: table(:)                   !< Interpolation table for the stochastic variables
    !type(correlationModelParameters), allocatable :: correlationParameters(:)   !< Data on correlation with other variables
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
      type(convDataSamplingMethods)   :: cnvg_data_ds
      type(storedConvergenceDataFORM) :: cnvg_data_form
    end type storedConvergenceData

    type, public, bind(c) :: computationSetting
        integer :: designPointSetting
        integer :: computationId
        integer :: threadId
    end type computationSetting

  interface
    function zfunc(x, compSetting, e) result(z) bind(c)
      use, intrinsic :: iso_c_binding, only: c_double
      import tError, computationSetting
      real(kind=c_double) :: z
      real(kind=c_double),      intent(inout) :: x(*)
      type(computationSetting), intent(in)    :: compSetting
      type(tError),             intent(inout) :: e
    end function zfunc
  end interface

  interface
    function progressCancel(i, s) result(cancel) bind(c)
      integer,          intent(in), value :: i
      character(len=1), intent(in)        :: s(*)
      logical(kind=1)                        :: cancel
    end function progressCancel
  end interface

  interface
    subroutine probCalcF2C(method, distribs, nstoch, vectorSize, correlations, nrCorrelations, fx, pc, &
        compIds, iPoint, x, rn, ierr) bind(C)
      use, intrinsic :: iso_c_binding, only: c_double
#ifdef _MSC_VER
      import tMethod, tDistrib, basicCorrelation, tError, tResult, zfunc, progressCancel
#else
      import tMethod, tDistrib, basicCorrelation, tError, tResult
#endif
      type(tMethod),  intent(in)    :: method
      type(tDistrib), intent(in)    :: distribs(*)
      integer, value, intent(in)    :: nstoch
      integer, value, intent(in)    :: vectorSize
      type(basicCorrelation), intent(in)  :: correlations(*)
      integer, value, intent(in)    :: nrCorrelations
      integer,        intent(in)    :: compIds(*)
      integer,        intent(in)    :: iPoint(*)
      procedure(zfunc)              :: fx
      procedure(progressCancel)     :: pc
      real(kind=c_double), intent(inout) :: x(*)
      type(tError),   intent(  out) :: ierr
      type(tResult),  intent(  out) :: rn
    end subroutine probCalcF2C
  end interface

contains

function textualProgress(progress, str) result(cancel) bind(c)
    integer, intent(in), value :: progress
    character(len=1), intent(in) :: str(*)
    logical(kind=1)              :: cancel

    continue
    cancel = .false.
end function textualProgress

!>
!! Subroutine for the calculation of a limit state function
subroutine calculateLimitStateFunction(probDb, fx, alfaN, beta, x, conv, convCriterium, convergenceData, name, id, pc)
    use feedback
    type(probabilisticDataStructure_data), intent(in) :: probDb    !< Probabilistic data module
    procedure(zfunc)                           :: fx               !< Function implementing the z-function of the failure mechanism
    real(kind=wp), intent(out)                 :: alfaN(:)         !< Alpha values
    real(kind=wp), intent(out)                 :: beta             !< Reliability index
    real(kind=wp), intent(inout)               :: x(:)             !< X values of design point
    logical,       intent(out)                 :: conv             !< Convergence indicator
    logical,       intent(out)                 :: convCriterium    !< Convergence criterium indicator
    type(storedConvergenceData), intent(inout) :: convergenceData  !< struct holding all convergence data
    character(len=*), intent(in), optional     :: name             !< Name of mechanism (for use in error message)
    integer         , intent(in), optional     :: id               !< Id of mechanism (for use in error message)
    procedure(progressCancel),    optional     :: pc               !< progress function

    integer, allocatable        :: iPointMax(:), iPointCpp(:)    ! Temporary max length Pointer to active variables used in the limit state function

    type(tMethod)               :: method
    type(tDistrib)              :: distribs(probDb%stoVar%maxStochasts)
    integer                     :: i, nStochActive, k, nstoch
    integer                     :: compIds(1) = designPointOutputFALSE
    type(tError)                :: ierr
    type(tResult)               :: rn
    character(len=ErrMsgLength) :: msg

    call realloc_check (iPointMax, probDb%stoVar%maxStochasts, "ipointMax" )

    method%methodId   = probDb%method%calcMethod
    method%tolA       = probDb%method%FORM%epsilonBeta
    method%trialLoops = probDb%method%FORM%trialLoops
    method%numExtraInt = probDb%method%FORM%maxIterations
    select case (method%methodId)
    case(methodDirectionalSampling, methodFORMandDirSampling, methodFORMandDirSamplingWithFORMiterations, &
         methodDirSamplingWithFORMiterationsStartU, methodDirSamplingWithFORMiterations)
        method%tolB       = probDb%method%DS%varcoefffailure
        method%minSamples = probDb%method%DS%minimumsamples
        method%maxSamples = probDb%method%DS%maximumsamples
        method%seed1      = probDb%method%DS%seedPRNG
        method%seed2      = probDb%method%DS%seedPRNG
    case(methodCrudeMonteCarlo, methodCrudeMonteCarloWithFORMiterations)
        method%tolB       = probDb%method%CMC%varcoefffailure
        method%minSamples = probDb%method%CMC%minimumsamples
        method%maxSamples = probDb%method%CMC%maximumsamples
        method%seed1      = probDb%method%CMC%seedPRNG
        method%seed2      = probDb%method%CMC%seedPRNG
    case(methodImportanceSampling, methodImportanceSamplingWithFORMiterations)
        method%tolB           = probDb%method%IS%varcoefffailure
        method%minSamples     = probDb%method%IS%minimumsamples
        method%maxSamples     = probDb%method%IS%maximumsamples
        method%varianceFactor = probDb%method%IS%varianceFactor
        method%seed1          = probDb%method%IS%seedPRNG
        method%seed2          = probDb%method%IS%seedPRNG
    case(methodAdaptiveImportanceSampling)
        method%tolB            = probDb%method%adaptiveIS%varcoefffailure
        method%minSamples      = probDb%method%adaptiveIS%minimumsamples
        method%maxSamples      = probDb%method%adaptiveIS%maximumsamples
        do i = 1, size(probDb%method%adaptiveIS%varianceFactor)
            method%offsets(i)         = probDb%method%adaptiveIS%translation(i)
            method%varianceFactors(i) = probDb%method%adaptiveIS%varianceFactor(i)
        end do
        method%seed1           = probDb%method%adaptiveIS%seedPRNG
        method%seed2           = probDb%method%adaptiveIS%seedPRNG
        method%trialLoops      = probDb%method%adaptiveIS%nAdp
        method%numExtraInt     = probDb%method%adaptiveIS%maxSamplesDef
        method%numExtraInt2    = probDb%method%adaptiveIS%globalModelOption
        method%numExtraReal1   = probDb%method%adaptiveIS%epsFailed
        method%numExtraReal2   = probDb%method%adaptiveIS%increaseVariance
        method%iterationMethod = probDb%method%adaptiveIS%minFailed
    case (methodNumericalIntegration)
        method%numExtraInt     = probDb%method%NI%numberIntervals
        method%numExtraReal1   = probDb%method%NI%minimumUvalue
        method%numExtraReal2   = probDb%method%NI%maximumUvalue
    end select
    method%du               = probDb%method%FORM%du
    method%relaxationFactor = probDb%method%FORM%relaxationFactor
    method%iterationMethod  = probDb%method%DS%iterationMethod
    method%du1              = probDb%method%DS%du1
    method%tolC             = probDb%method%DS%epsilonDu
    method%chunkSize        = probDb%method%chunkSizeOpenMP

    nstoch = probDb%stoVar%maxStochasts
    nStochActive = 0
    do i = 1, nstoch
        if (probDb%stovar%disttypex(i) == distributionDeterministic) then
            x(i) = probDb%stovar%distparameterx(i,1)
        else if (probDb%stovar%activex(i) == stochastActive) then
            nStochActive = nStochActive + 1
            call copystr("var1", distribs(i)%name)
            distribs(nStochActive)%distributionId = probDb%stovar%disttypex(i)
            distribs(nStochActive)%params = probDb%stovar%distparameterx(i,:)
            iPointMax(nStochActive) = i
        end if
    end do

    method%designPointOption = probDb%method%dpOption
    method%numThreads        = probDb%method%maxParallelThreads
    method%rnd = GeorgeMarsaglia

    call convertStartMethod(probDb, method, iPointMax(1:nStochActive))
    allocate(iPointCpp(nStochActive))
    do i = 1, nStochActive
        iPointCpp(i) = iPointMax(i) - 1  ! Fortran->C
    end do

    if (method%iterationMethod <= 0 .or. method%iterationMethod > 9 .or. method%iterationMethod == 7) then
        call fatalError("Unknown method in subroutine IterationDS: ", method%iterationMethod)
    else if (nstoch > 0) then
        method%progressInterval = 1
        if (present(pc)) then
            call probCalcF2C(method, distribs, nStochActive, nStoch, probDb%basic_correlation, &
                probDb%number_correlations, fx, pc, compIds, iPointCpp, x, rn, ierr)
        else
            call probCalcF2C(method, distribs, nStochActive, nStoch, probDb%basic_correlation, &
                probDb%number_correlations, fx, textualProgress, compIds, iPointCpp, x, rn, ierr)
        end if

        if (ierr%iCode /= 0) then
            call copystrback(msg, ierr%message)
            call fatalError(msg)
            x = 0.0_wp
        else
            beta = rn%beta
            alfaN = 0.0_wp
            do k = 1, nStochActive
                alfaN(iPointMax(k)) = rn%alpha(k)
            end do
            if (method%methodId == methodFORMandDirSampling .and. rn%samplesNeeded > 0) then
                ! to get logging in output.txt right; as we have samples, Form did not succeed (no convergence or beta out of range)
                conv = .false.
            else
                conv = rn%convergence
            end if
            convCriterium = rn%convergence
            convergenceData%cnvg_data_ds%numberSamples = rn%samplesNeeded
            convergenceData%cnvg_data_form%numberiterations = rn%stepsNeeded
        end if
    else
        call fatalError("No stochastic parameters found")
    end if

end subroutine calculateLimitStateFunction

subroutine convertStartMethod(probDb, method, iPoint)
    use feedback
    type(probabilisticDataStructure_data), intent(in)    :: probDb
    type(tMethod),                         intent(inout) :: method
    integer,                               intent(in)    :: iPoint(:)

    integer :: nstoch, i

    nstoch = size(iPoint)

    select case (probDb%method%FORM%startMethod)
    case (fORMStartOne)
        method%startmethod = probDb%method%FORM%startMethod
    case (fORMStartRaySearch, fORMStartRaySearchWind)
        method%startmethod = fORMStartRaySearch
        if (allocated(probDb%method%FORM%startValue)) then
            do i = 1, nstoch
                method%startVector(i) = probDb%method%FORM%startValue(iPoint(i))
            end do
        else
            method%startVector(1:nstoch) = 1.0_wp
        end if
    case (fORMStartSphereSearch)
        method%startmethod = probDb%method%FORM%startMethod
        if (allocated(probDb%method%FORM%startValue)) then
            do i = 1, nstoch
                method%startVector(i) = probDb%method%FORM%startValue(iPoint(i))
            end do
        else
            method%startVector(1:nstoch) = 1.0_wp
        end if
    case (fORMStartGivenVector)
        method%startmethod = probDb%method%FORM%startMethod
        if (allocated(probDb%method%FORM%startValue)) then
            do i = 1, nstoch
                method%startVector(i) = probDb%method%FORM%startValue(iPoint(i))
            end do
        else
            call fatalError("start value must be given for fORMStartGivenVector")
        end if
    case (fORMStartRaySearchVector)
        method%startmethod = probDb%method%FORM%startMethod
        if (allocated(probDb%method%FORM%startValue)) then
            do i = 1, nstoch
                method%startVector(i) = probDb%method%FORM%startValue(iPoint(i))
            end do
        else
            call fatalError("start value must be given for fORMStartRaySearchVector")
        end if
    case (fORMStartRaySearchVectorScaled)
        method%startmethod = probDb%method%FORM%startMethod
        if (allocated(probDb%method%FORM%startValue)) then
            do i = 1, nstoch
                method%startVector(i) = probDb%method%FORM%startValue(iPoint(i))
            end do
        else
            call fatalError("start value must be given for fORMStartRaySearchVectorScaled")
        end if
    case (fORMStartZero)
        method%startmethod = probDb%method%FORM%startMethod
    case default
        call fatalError("start method not implemented: ", probDb%method%FORM%startMethod)
    end select
end subroutine convertStartMethod

!> Subroutine registers the correlation between two stochasts in the x vector
subroutine registerCorrelation( probDb, idx1, idx2, correlation )
    implicit none
    type(probabilisticDataStructure_data)             :: probDb               !< Probabilistic data module
    integer, intent(in)                               :: idx1                 !< Index of the first stochast in the x vector
    integer, intent(in)                               :: idx2                 !< Index of the second stochast in the x vector
    real(kind=wp), intent(in)                         :: correlation          !< Correlation coefficient

    integer                                           :: idx
    integer                                           :: ierr
    integer                                           :: old_size
    integer                                           :: new_size
    type(basicCorrelation), dimension(:), allocatable :: basic_correlation

    ! No "self-correlation"
    if ( idx1 == idx2 ) then
        return
    endif

    !
    ! Ensure the array is large enough
    !
    if ( .not. allocated( probDb%basic_correlation ) ) then
        probDb%number_correlations = 0
        allocate( probDb%basic_correlation(100), stat = ierr )
        call check_alloc( ierr, 100, "basic_correlation" )
    endif

    if ( probDb%number_correlations == size(probDb%basic_correlation) ) then
        old_size = size(probDb%basic_correlation)
        new_size = size(probDb%basic_correlation) + 100
        allocate( basic_correlation(new_size), stat = ierr )
        call check_alloc( ierr, new_size, "basic_correlation" )
        basic_correlation(1:old_size) = probDb%basic_correlation(1:old_size)

        deallocate( probDb%basic_correlation )
        call move_alloc( basic_correlation, probDb%basic_correlation )
    endif

    !
    ! Store the new correlation
    !
    idx = probDb%number_correlations + 1
    probDb%number_correlations = idx
    probDb%basic_correlation(idx)%first  = idx1 - 1 ! one based => zero based
    probDb%basic_correlation(idx)%second = idx2 - 1
    probDb%basic_correlation(idx)%correlation = min( 1.0_wp, max( -1.0_wp, correlation ) )

end subroutine registerCorrelation

end module interface_probCalc
