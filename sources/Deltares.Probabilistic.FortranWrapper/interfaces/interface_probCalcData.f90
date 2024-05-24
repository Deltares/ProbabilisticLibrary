module interface_probCalcData
use interface_probCalc
use interface_distributions
use feedback
use m_realloc_check
implicit none
private

integer :: curParallelThreads = 1

public :: initProbabilisticCalculation, &
          finalizeProbabilisticCalculation, &
          setStochasticDistrib, &
          curParallelThreads, &
          initDeterministicValue, &
          initPointerToStochast

    interface setParametersProbabilisticIS
        module procedure setParametersProbabilisticISsimple
        module procedure setParametersProbabilisticISdetail
    end interface

    public :: setParametersProbabilisticForm, &
              setParametersProbabilisticNI, &
              setParametersProbabilisticCMC, &
              setParametersProbabilisticDS, &
              setParametersProbabilisticIS, &
              setParametersProbabilisticAdpMCIS, &
              setParametersProbabilisticCalculation

contains

subroutine initProbabilisticCalculation( probDb, maxStochasts, databaseFlag, expertFlag )

    type(probabilisticDataStructure_data)   :: probDb         !< Probabilistic data module
    integer, intent(in)                     :: maxStochasts   !< Maximum number of stochasts
    logical, intent(in)                     :: databaseFlag   !< Flag for storing detailed convergence in the database
    logical, intent(in)                     :: expertFlag     !< Flag for storing expert-only details

    integer :: i

    ! Allocate memory for stochasts
    probDb%stoVar%maxStochasts = maxStochasts
    call realloc_check( probDb%stoVar%activeX, maxStochasts, "activeX" )
    call realloc_check( probDb%stoVar%distTypeX, maxStochasts, "distTypeX")
    call realloc_check( probDb%stoVar%distParameterX, maxStochasts, 4, "distParameterX")
    if (.not. allocated(probDb%basic_correlation)) then
        allocate(probDb%basic_correlation(0))
        probDb%number_correlations = 0
    end if

    probDb%stoVar%activeX   = stochastNotActive
    probDb%stoVar%distTypeX = distributionUnknown
    probDb%stoVar%distParameterX = -999.0_wp

    ! Default values iteration proces
    ! FORM
    probDb%method%FORM%startMethod          = fORMStartZero
    probDb%method%FORM%maxIterations        = 50
    probDb%method%FORM%trialLoops           = 1
    probDb%method%FORM%epsilonBeta          = 0.01D0
    probDb%method%FORM%epsilonZvalue        = 0.01D0
    probDb%method%FORM%relaxationFactor     = 0.75D0
    probDb%method%FORM%dU                   = 0.3D0
    !probDb%method%FORM%startValue           = 0.0D0
    !
    ! Crude Monte Carlo
    probDb%method%CMC%seedPRNG              = 0
    probDb%method%CMC%minimumSamples        = 1000
    probDb%method%CMC%maximumSamples        = 100000
    probDb%method%CMC%varCoeffFailure       = 0.10D0
    probDb%method%CMC%varCoeffNoFailure     = 0.10D0
    !
    ! Directional Sampling
    probDb%method%DS%seedPRNG               = 0
    probDb%method%DS%minimumSamples         = 10
    probDb%method%DS%maximumSamples         = 10000
    probDb%method%DS%varCoeffFailure        = 0.10D0
    probDb%method%DS%varCoeffNoFailure      = 0.10D0
    probDb%method%DS%iterationMethod        = 1
    probDb%method%DS%iterations1            = 20
    probDb%method%DS%du1                    = 1.D0
    probDb%method%DS%maximumIterations2     = 20
    probDb%method%DS%initialDu2             = 3.D0
    probDb%method%DS%maximumDu2             = 3.D0
    probDb%method%DS%epsilonDu              = 0.001D0
    probDb%method%DS%maximumLengthU         = 20.D0
    !
    ! Importance Sampling
    probDb%method%IS%seedPRNG               = 0
    probDb%method%IS%minimumSamples         = 1000
    probDb%method%IS%maximumSamples         = 100000
    probDb%method%IS%varCoeffFailure        = 0.10D0
    probDb%method%IS%varCoeffNoFailure      = 0.10D0
    probDb%method%IS%varianceFactor         = 1.5D0

    ! Adative Importance Sampling
    probDb%method%AdaptiveIS%seedPRNG               = 0
    probDb%method%AdaptiveIS%minimumSamples         = 1000
    probDb%method%AdaptiveIS%maximumSamples         = 100000
    probDb%method%AdaptiveIS%varCoeffFailure        = 0.10D0
    probDb%method%AdaptiveIS%varianceFactor         = [ (1.5D0, i = 1,maxStochasts) ]
    probDb%method%AdaptiveIS%translation            = [ (0.0D0, i = 1,maxStochasts) ]
    probDb%method%AdaptiveIS%startMethod            = fORMStartZero
    probDb%method%AdaptiveIS%Nadp                   = 5
    probDb%method%AdaptiveIS%startValue             = [ (0.0D0, i = 1,maxStochasts) ]
    probDb%method%AdaptiveIS%epsFailed              = 0.10D0
    probDb%method%AdaptiveIS%minFailed              = 0
    probDb%method%AdaptiveIS%increaseVariance       = 0.5
    !
    ! Numerical integration
    probDb%method%NI%numberIntervals        = 200
    probDb%method%NI%minimumUvalue          = -10.D0
    probDb%method%NI%maximumUvalue          = 10.D0
    !
    ! Form / Directional Sampling
    !probDb%method%FDthreshold               = -2.D0
    !
    ! Design point
!    probDb%method%DPoption                  = designPointXCorrelatedFromU
    probDb%method%DPoption                  = designPointRMinZFunc
    probDb%method%maxParallelThreads        = curParallelThreads

end subroutine initProbabilisticCalculation

subroutine finalizeProbabilisticCalculation(probDb, convergenceData)

    type(probabilisticDataStructure_data), intent(inout) :: probDb          !< Probabilistic data module
    type(storedConvergenceData), optional, intent(inout) :: convergenceData !< convergenceData for FORM/DS

    if (allocated(probDb%Method%form%startValue)) then
        deallocate(probDb%Method%form%startValue)
    end if
end subroutine finalizeProbabilisticCalculation

!>
!! Subroutine to define and fill a structure containing information about the probability distribution of a stochastic variable
!!   @ingroup Probabilistic
subroutine setStochasticDistrib( probDb, stochastNr, distType, &
                distParameter1, distParameter2, distParameter3, distParameter4 )

    type(probabilisticDataStructure_data)   :: probDb            !< Probabilistic data module
    integer,       intent(in)               :: stochastNr        !< Index of stochastic variable (input)
    integer,       intent(in)               :: distType          !< Distribution type (input), see distributionEnumerations
    real(kind=wp), intent(in)               :: distParameter1    !< Parameter 1 of distribution type (input)
    real(kind=wp), intent(in)               :: distParameter2    !< Parameter 2 of distribution type (input)
    real(kind=wp), intent(in)               :: distParameter3    !< Parameter 3 of distribution type (input)
    real(kind=wp), intent(in)               :: distParameter4    !< Parameter 4 of distribution type (input)
    !
    ! Check the index of the stochastic variable
    !
    if ( stochastNr > probDb%stoVar%maxStochasts ) then
        call fatalError( "Index of stochastic parameter exceeds maximum - index:", stochastNr )
    endif
    !
    ! Put data in structure
    !
    if ( distType > 0 ) then
        probDb%stoVar%activeX(stochastNr) = stochastActive
    else
        probDb%stoVar%activeX(stochastNr) = stochastDeterminist
    end if
    probDb%stoVar%distTypeX(stochastNr)         = distType
    probDb%stoVar%distParameterX(stochastNr, 1) = distParameter1
    probDb%stoVar%distParameterX(stochastNr, 2) = distParameter2
    probDb%stoVar%distParameterX(stochastNr, 3) = distParameter3
    probDb%stoVar%distParameterX(stochastNr, 4) = distParameter4

    ! validation for some distributions
    if (distType == distributionTruncatedNormal) then
        if (distParameter2 <= 0.0_wp) then
            call fatalError("deviation must be > 0 for truncated normal; stochastnr = ", stochastNr)
        end if
        if (distParameter3 >= distParameter4) then
            call fatalError("minimum must be smaller than maximum for truncated normal; stochastnr = ", stochastNr)
        end if
    endif

end subroutine setStochasticDistrib

!>
!! Subroutine which initializes the deterministic values
subroutine initDeterministicValue( probDb, x )
!
!   INPUT/OUTPUT VARIABLES
!
    type(probabilisticDataStructure_data), intent(in)   :: probDb   !< Probabilistic data module
    real(kind=wp), intent(inout)                        :: x(:)     !< Vector with expected values of the deterministic parameters
!
    integer                                             :: i        ! do-loop counter
!
    do i = 1, probDb%stoVar%maxStochasts
        if (probDb%stoVar%distTypeX(i) == distributionDeterministic) then
            x(i) = probDb%stoVar%distParameterX(i, 1)
        endif
    enddo

end subroutine initDeterministicValue

!> Subroutine which initializes the pointer to a stochast
subroutine initPointerToStochast( probDb, nStochasts, iPoint )
!
!   INPUT/OUTPUT VARIABLES
!
    type(probabilisticDataStructure_data)   :: probDb       !< Probabilistic data module
    integer, intent(out)                    :: nStochasts   !< Number of stochasts
    integer, intent(out)                    :: iPoint(:)    !< Pointer to stochastic variables
!
!   LOCAL VARIABLES
!
    integer                                 :: i            ! do-loop counter
!
!   Compute the number of stochastic parameters and fill the matrix
!
    nStochasts = 0
    iPoint = 0
    do i = 1, probDb%stoVar%maxStochasts
        if (probDb%stoVar%activeX(i) == stochastActive) then
            nStochasts = nStochasts + 1
            iPoint(nStochasts) = i
        endif
    enddo

end subroutine initPointerToStochast


!>
!! Subroutine for assignment of numeric control data FORM
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticForm(probDb, FstartMethod, FmaxIterations, &
               FtrialLoops, FepsilonBeta, FepsilonZvalue, FrelaxationFactor, &
               FdU, FstartValue, maxParThreads)

    type(probabilisticDataStructure_data)   :: probDb               !< Probabilistic data module
    integer, intent(in)                     :: FstartMethod         !< Method for startvector
    integer, intent(in)                     :: FmaxIterations       !< Maximum number of iterations
    integer, intent(in)                     :: FtrialLoops          !< Number of trial loops
    integer, intent(in)                     :: maxParThreads        !< maximum number of parallel threads
    real(kind=wp), intent(in)               :: FepsilonBeta         !< Epsilon Beta (relative accuracy)
    real(kind=wp), intent(in)               :: FepsilonZvalue       !< Epsilon Zfunction (relative accuracy)
    real(kind=wp), intent(in)               :: FrelaxationFactor    !< Relaxation factor
    real(kind=wp), intent(in)               :: FdU                  !< Interval size for derivatives
    real(kind=wp), intent(in)               :: FstartValue(:)       !< Startvector
    !
    ! Put data in structure
    !
    associate ( method => probDb%method, FORM => probDb%method%FORM )
        FORM%startMethod          = FstartMethod
        FORM%maxIterations        = FmaxIterations
        FORM%trialLoops           = FtrialLoops
        FORM%epsilonBeta          = FepsilonBeta
        FORM%epsilonZvalue        = FepsilonZvalue
        FORM%relaxationFactor     = FrelaxationFactor
        FORM%dU                   = FdU
        FORM%startValue           = FstartValue
        method%maxParallelThreads = maxParThreads
    end associate

end subroutine setParametersProbabilisticForm

!>
!! Subroutine for assignment of numeric control data Numeric Integration
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticNI(probDb, numExtraInt, NInumExtraReal1, NImaximumUvalue)

    type(probabilisticDataStructure_data)   :: probDb               !< Probabilistic data module
    integer, intent(in)                     :: numExtraInt          !< Number intervals
    real(kind=wp), intent(in)               :: NInumExtraReal1      !< Minimum U value
    real(kind=wp), intent(in)               :: NImaximumUvalue      !< Maximum U value
    !
    ! Put data in structure
    !
    probDb%method%NI%numberIntervals    = numExtraInt
    probDb%method%NI%minimumUvalue      = NInumExtraReal1
    probDb%method%NI%maximumUvalue      = NImaximumUvalue

end subroutine setParametersProbabilisticNI

!>
!! Subroutine for assignment of numeric control data Crude Monte Carlo
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticCMC(probDb, CMCseedPRNG, CMCminimumSamples, &
               CMCmaximumSamples, CMCvarCoeffFailure, CMCvarCoeffNoFailure)

    type(probabilisticDataStructure_data)   :: probDb                !< Probabilistic data module
    integer, intent(in)                     :: CMCseedPRNG           !< Start value random generator
    integer, intent(in)                     :: CMCminimumSamples     !< Minimum number samples (>1)
    integer, intent(in)                     :: CMCmaximumSamples     !< Maximum number samples
    real(kind=wp), intent(in)               :: CMCvarCoeffFailure    !< Required variation coefficient Failure
    real(kind=wp), intent(in)               :: CMCvarCoeffNoFailure  !< Required variation coefficient Non Failure
    !
    ! Put data in structure
    !
    probDb%method%CMC%seedPRNG           = CMCseedPRNG
    probDb%method%CMC%minimumSamples     = CMCminimumSamples
    probDb%method%CMC%maximumSamples     = CMCmaximumSamples
    probDb%method%CMC%varCoeffFailure    = CMCvarCoeffFailure
    probDb%method%CMC%varCoeffNoFailure  = CMCvarCoeffNoFailure

end subroutine setParametersProbabilisticCMC

!>
!! Subroutine for assignment of numeric control data Directional Sampling
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticDS(probDb, DSseedPRNG, DSminimumSamples, &
               DSmaximumSamples, DSvarCoeffFailure, DSvarCoeffNoFailure, &
               DSiterationMethod, DSiterations1, DSdu1, DSmaximumIterations2, &
               DSinitialDu2, DSmaximumDu2, DSepsilonDu, DSmaximumLengthU, DSmaxParThreads)

    type(probabilisticDataStructure_data)   :: probDb               !< Probabilistic data module
    integer, intent(in)                     :: DSseedPRNG           !< Start value random generator
    integer, intent(in)                     :: DSminimumSamples     !< Minimum number samples (>1)
    integer, intent(in)                     :: DSmaximumSamples     !< Maximum number samples
    real(kind=wp), intent(in)               :: DSvarCoeffFailure    !< Required variation coefficient Failure
    real(kind=wp), intent(in)               :: DSvarCoeffNoFailure  !< Required variation coefficient Non Failure
    integer, intent(in)                     :: DSiterationMethod    !< Iteration method
    integer, intent(in)                     :: DSiterations1        !< Number iterations method 1
    real(kind=wp), intent(in)               :: DSdu1                !< Step size methode 1
    integer, intent(in)                     :: DSmaximumIterations2 !< Maximum number iterations method 2
    real(kind=wp), intent(in)               :: DSinitialDu2         !< Initial step size du method 2
    real(kind=wp), intent(in)               :: DSmaximumDu2         !< Maximum step size du method 2
    real(kind=wp), intent(in)               :: DSepsilonDu          !< Epsilon  step size du methods 1, 2
    real(kind=wp), intent(in)               :: DSmaximumLengthU     !< Maximum length u-vector
    integer, intent(in)                     :: DSmaxParThreads      !< Maximum OpenMP threads
    !
    ! Put data in structure
    !
    associate ( method => probDb%method, DS => probDb%method%DS )
        DS%seedPRNG               = DSseedPRNG
        DS%minimumSamples         = DSminimumSamples
        DS%maximumSamples         = DSmaximumSamples
        DS%varCoeffFailure        = DSvarCoeffFailure
        DS%varCoeffNoFailure      = DSvarCoeffNoFailure
        DS%iterationMethod        = DSiterationMethod
        DS%iterations1            = DSiterations1
        DS%du1                    = DSdu1
        DS%maximumIterations2     = DSmaximumIterations2
        DS%initialDu2             = DSinitialDu2
        DS%maximumDu2             = DSmaximumDu2
        DS%epsilonDu              = DSepsilonDu
        DS%maximumLengthU         = DSmaximumLengthU
        method%maxParallelThreads = DSmaxParThreads
    end associate

end subroutine setParametersProbabilisticDS

!>
!! Subroutine for assignment of numeric control data Importance Sampling
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticISsimple(probDb, ISseedPRNG, ISminimumSamples, &
               ISmaximumSamples, ISvarCoeffFailure, ISvarCoeffNoFailure, ISvarianceFactor)

    type(probabilisticDataStructure_data)   :: probDb                !< Probabilistic data module
    integer, intent(in)                     :: ISseedPRNG           !< Start value random generator
    integer, intent(in)                     :: ISminimumSamples     !< Minimum number samples (>1)
    integer, intent(in)                     :: ISmaximumSamples     !< Maximum number samples
    real(kind=wp), intent(in)               :: ISvarCoeffFailure    !< Required variation coefficient Failure
    real(kind=wp), intent(in)               :: ISvarCoeffNoFailure  !< Required variation coefficient Non Failure
    real(kind=wp), intent(in)               :: ISvarianceFactor      !< Multiplicative factor to increase the standard deviation

    integer                                 :: i
    !
    ! Put data in structure
    !
    probDb%method%IS%seedPRNG           = ISseedPRNG
    probDb%method%IS%minimumSamples     = ISminimumSamples
    probDb%method%IS%maximumSamples     = ISmaximumSamples
    probDb%method%IS%varCoeffFailure    = ISvarCoeffFailure
    probDb%method%IS%varCoeffNoFailure  = ISvarCoeffNoFailure
    probDb%method%IS%varianceFactor     = ISvarianceFactor
!    probDb%method%IS%translation        = [ (0.0_wp, i = 1,probDb%stoVar%maxStochasts) ]

end subroutine setParametersProbabilisticISsimple

subroutine setParametersProbabilisticISdetail(probDb, ISseedPRNG, ISminimumSamples, &
               ISmaximumSamples, ISvarCoeffFailure, ISvarCoeffNoFailure, ISvarianceFactor, IStranslation)

    type(probabilisticDataStructure_data)   :: probDb                !< Probabilistic data module
    integer, intent(in)                     :: ISseedPRNG           !< Start value random generator
    integer, intent(in)                     :: ISminimumSamples     !< Minimum number samples (>1)
    integer, intent(in)                     :: ISmaximumSamples     !< Maximum number samples
    real(kind=wp), intent(in)               :: ISvarCoeffFailure    !< Required variation coefficient Failure
    real(kind=wp), intent(in)               :: ISvarCoeffNoFailure  !< Required variation coefficient Non Failure
    real(kind=wp), intent(in)               :: ISvarianceFactor(:)  !< Multiplicative factor to increase the standard deviation
    real(kind=wp), intent(in)               :: IStranslation(:)     !< Translatory term for u vector

    if ( size(ISvarianceFactor) /= probDb%stoVar%maxstochasts ) then
        call fatalError( "Array of variance factors should be equal to the maximum number of stochasts:", &
            size(ISvarianceFactor) )
    endif
    if ( size(IStranslation) /= probDb%stoVar%maxstochasts ) then
        call fatalError( "Array of translation terms for u vector should be equal to the maximum number of stochasts:", &
            size(ISvarianceFactor) )
    endif

    !
    ! Put data in structure
    !
    probDb%method%IS%seedPRNG           = ISseedPRNG
    probDb%method%IS%minimumSamples     = ISminimumSamples
    probDb%method%IS%maximumSamples     = ISmaximumSamples
    probDb%method%IS%varCoeffFailure    = ISvarCoeffFailure
    probDb%method%IS%varCoeffNoFailure  = ISvarCoeffNoFailure
!    probDb%method%IS%varianceFactor     = ISvarianceFactor
!    probDb%method%IS%translation        = IStranslation

end subroutine setParametersProbabilisticISdetail

!>
!! Subroutine for assignment of numeric control data Adaptive Importance Sampling
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticAdpMCIS(probDb, adaptiveIS)

    type(probabilisticDataStructure_data), intent(inout) :: probDb                !< Probabilistic data module
    type(tpAdaptiveIS)                   , intent(in   ) :: adaptiveIS

    !
    ! Put data in structure
    !
    probDb%method%AdaptiveIS%seedPRNG           = AdaptiveIS%seedPRNG
    probDb%method%AdaptiveIS%minimumSamples     = AdaptiveIS%minimumSamples
    probDb%method%AdaptiveIS%maximumSamples     = AdaptiveIS%maximumSamples
    probDb%method%AdaptiveIS%Nadp               = AdaptiveIS%Nadp
    probDb%method%AdaptiveIS%varCoeffFailure    = AdaptiveIS%varCoeffFailure
    probDb%method%AdaptiveIS%epsFailed          = AdaptiveIS%epsFailed
    probDb%method%AdaptiveIS%minFailed          = AdaptiveIS%minFailed
    probDb%method%AdaptiveIS%increaseVariance   = AdaptiveIS%increaseVariance
    probDb%method%AdaptiveIS%varianceFactor     = AdaptiveIS%varianceFactor

end subroutine setParametersProbabilisticAdpMCIS

!>
!! Subroutine for assignment of numeric control data (small subset)
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticCalculation ( probDb, FstartMethod, FmaxIterations, &
               FtrialLoops, FepsilonBeta, FepsilonZvalue, FrelaxationFactor, Fdu, &
               MCseedPRNG, MCmaximumSamples, numExtraInt, NInumExtraReal1, &
               NImaximumUvalue)

    type(probabilisticDataStructure_data)   :: probDb               !< Probabilistic data module
    integer,       intent(in)               :: FstartMethod         !< Method for startvector
    integer,       intent(in)               :: FmaxIterations       !< Maximum number of iterations
    integer,       intent(in)               :: FtrialLoops          !< Number of trial loops
    real(kind=wp), intent(in)               :: FepsilonBeta         !< Epsilon Beta (relative accuracy)
    real(kind=wp), intent(in)               :: FepsilonZvalue       !< Epsilon Zfunction (relative accuracy)
    real(kind=wp), intent(in)               :: FrelaxationFactor    !< Relaxation factor
    real(kind=wp), intent(in)               :: FdU                  !< Interval size for derivatives
    integer,       intent(in)               :: MCseedPRNG           !< Start value random generator
    integer,       intent(in)               :: MCmaximumSamples     !< Maximum number samples
    integer,       intent(in)               :: numExtraInt          !< Number intervals
    real(kind=wp), intent(in)               :: NInumExtraReal1      !< Minimum U value
    real(kind=wp), intent(in)               :: NImaximumUvalue      !< Maximum U value
    !
    ! Put data in structure
    !
    probDb%method%FORM%startMethod      = FstartMethod
    probDb%method%FORM%maxIterations    = FmaxIterations
    probDb%method%FORM%trialLoops       = FtrialLoops
    probDb%method%FORM%epsilonBeta      = FepsilonBeta
    probDb%method%FORM%epsilonZvalue    = FepsilonZvalue
    probDb%method%FORM%relaxationFactor = FrelaxationFactor
    probDb%method%FORM%dU               = FdU
    probDb%method%CMC%seedPRNG          = MCseedPRNG
    probDb%method%CMC%maximumSamples    = MCmaximumSamples
    probDb%method%NI%numberIntervals    = numExtraInt
    probDb%method%NI%minimumUvalue      = NInumExtraReal1
    probDb%method%NI%maximumUvalue      = NImaximumUvalue

end subroutine setParametersProbabilisticCalculation

end module interface_probCalcData
