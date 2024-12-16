! Copyright (C) Stichting Deltares. All rights reserved.
!
! This file is part of the Probabilistic Library.
!
! The Probabilistic Library is free software: you can redistribute it and/or modify
! it under the terms of the GNU Lesser General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
!
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
! GNU Lesser General Public License for more details.
!
! You should have received a copy of the GNU Lesser General Public License
! along with this program. If not, see <http://www.gnu.org/licenses/>.
!
! All names, logos, and references to "Deltares" are registered trademarks of
! Stichting Deltares and remain full property of Stichting Deltares at all times.
! All rights reserved.
!
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
          initPointerToStochast, &
          setParametersProbabilisticForm, &
          setParametersProbabilisticNI, &
          setParametersProbabilisticCMC, &
          setParametersProbabilisticDS, &
          setParametersProbabilisticIS, &
          setParametersProbabilisticAdpMCIS

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
    probDb%method%CMC%isRepeatableRandom    = .TRUE.
    probDb%method%CMC%minimumSamples        = 1000
    probDb%method%CMC%maximumSamples        = 100000
    probDb%method%CMC%varCoeffFailure       = 0.10D0
    probDb%method%CMC%varCoeffNoFailure     = 0.10D0
    !
    ! Directional Sampling
    probDb%method%DS%seedPRNG               = 0
    probDb%method%DS%isRepeatableRandom     = .TRUE.
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
    probDb%method%IS%isRepeatableRandom     = .TRUE.
    probDb%method%IS%minimumSamples         = 1000
    probDb%method%IS%maximumSamples         = 100000
    probDb%method%IS%varCoeffFailure        = 0.10D0
    probDb%method%IS%varCoeffNoFailure      = 0.10D0
    probDb%method%IS%varianceFactor         = 1.5D0

    ! Adaptive Importance Sampling
    probDb%method%AdaptiveIS%seedPRNG               = 0
    probDb%method%AdaptiveIS%isRepeatableRandom     = .TRUE.
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
subroutine setParametersProbabilisticForm(probDb, FORMsettings, maxParThreads)

    type(probabilisticDataStructure_data), intent(inout) :: probDb        !< Probabilistic data module
    type(tpFORM),                          intent(in   ) :: FORMsettings  !< setting for FORM
    integer,                               intent(in   ) :: maxParThreads !< maximum number of parallel threads
    !
    ! Put data in structure
    !
    associate ( method => probDb%method, FORM => probDb%method%FORM )
        FORM%startMethod          = FORMsettings%startMethod
        FORM%maxIterations        = FORMsettings%maxIterations
        FORM%trialLoops           = FORMsettings%trialLoops
        FORM%epsilonBeta          = FORMsettings%epsilonBeta
        FORM%epsilonZvalue        = FORMsettings%epsilonZvalue
        FORM%relaxationFactor     = FORMsettings%relaxationFactor
        FORM%dU                   = FORMsettings%dU
        FORM%startValue           = FORMsettings%startValue
        method%maxParallelThreads = maxParThreads
    end associate

end subroutine setParametersProbabilisticForm

!>
!! Subroutine for assignment of numeric control data Numeric Integration
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticNI(probDb, NIsettings)

    type(probabilisticDataStructure_data), intent(inout) :: probDb      !< Probabilistic data module
    type(tpNI),                            intent(in   ) :: NIsettings  !< settings for numerical integration
    !
    ! Put data in structure
    !
    probDb%method%NI%numberIntervals    = NIsettings%numberIntervals
    probDb%method%NI%minimumUvalue      = NIsettings%minimumUvalue
    probDb%method%NI%maximumUvalue      = NIsettings%maximumUvalue

end subroutine setParametersProbabilisticNI

!>
!! Subroutine for assignment of numeric control data Crude Monte Carlo
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticCMC(probDb, CMCsettings)

    type(probabilisticDataStructure_data), intent(inout) :: probDb       !< Probabilistic data module
    type(tpCMC),                           intent(in   ) :: CMCsettings  !< settings for Crude Monte Carlo
    !
    ! Put data in structure
    !
    probDb%method%CMC%seedPRNG           = CMCsettings%seedPRNG
    probDb%method%CMC%isRepeatableRandom = CMCsettings%isRepeatableRandom
    probDb%method%CMC%minimumSamples     = CMCsettings%minimumSamples
    probDb%method%CMC%maximumSamples     = CMCsettings%maximumSamples
    probDb%method%CMC%varCoeffFailure    = CMCsettings%varCoeffFailure
    probDb%method%CMC%varCoeffNoFailure  = CMCsettings%varCoeffNoFailure

end subroutine setParametersProbabilisticCMC

!>
!! Subroutine for assignment of numeric control data Directional Sampling
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticDS(probDb, DSsettings, maxParThreads)

    type(probabilisticDataStructure_data), intent(inout) :: probDb        !< Probabilistic data module
    type(tpDS),                            intent(in   ) :: DSsettings    !< settings for Directional Sampling
    integer,                               intent(in   ) :: maxParThreads !< Maximum OpenMP threads
    !
    ! Put data in structure
    !
    associate ( method => probDb%method, DS => probDb%method%DS )
        DS%seedPRNG               = DSsettings%seedPRNG
        DS%isRepeatableRandom     = DSsettings%isRepeatableRandom
        DS%minimumSamples         = DSsettings%minimumSamples
        DS%maximumSamples         = DSsettings%maximumSamples
        DS%varCoeffFailure        = DSsettings%varCoeffFailure
        DS%varCoeffNoFailure      = DSsettings%varCoeffNoFailure
        DS%iterationMethod        = DSsettings%iterationMethod
        DS%iterations1            = DSsettings%iterations1
        DS%du1                    = DSsettings%du1
        DS%maximumIterations2     = DSsettings%maximumIterations2
        DS%initialDu2             = DSsettings%initialDu2
        DS%maximumDu2             = DSsettings%maximumDu2
        DS%epsilonDu              = DSsettings%epsilonDu
        DS%maximumLengthU         = DSsettings%maximumLengthU
        method%maxParallelThreads = maxParThreads
    end associate

end subroutine setParametersProbabilisticDS

!>
!! Subroutine for assignment of numeric control data Importance Sampling
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticIS(probDb, ISsettings)
    type(probabilisticDataStructure_data), intent(inout) :: probDb      !< Probabilistic data module
    type(tpIS)                           , intent(in   ) :: ISsettings  !< settings for importance sampling

    !
    ! Put data in structure
    !
    probDb%method%IS%seedPRNG           = ISsettings%seedPRNG
    probDb%method%IS%isRepeatableRandom = ISsettings%isRepeatableRandom
    probDb%method%IS%minimumSamples     = ISsettings%minimumSamples
    probDb%method%IS%maximumSamples     = ISsettings%maximumSamples
    probDb%method%IS%varCoeffFailure    = ISsettings%varCoeffFailure
    probDb%method%IS%varCoeffNoFailure  = ISsettings%varCoeffNoFailure
    probDb%method%IS%varianceFactor     = ISsettings%varianceFactor
!    probDb%method%IS%translation        = [ (0.0_wp, i = 1,probDb%stoVar%maxStochasts) ]

end subroutine setParametersProbabilisticIS

!>
!! Subroutine for assignment of numeric control data Adaptive Importance Sampling
!!   @ingroup Probabilistic
subroutine setParametersProbabilisticAdpMCIS(probDb, adaptiveIS)
    type(probabilisticDataStructure_data), intent(inout) :: probDb     !< Probabilistic data module
    type(tpAdaptiveIS)                   , intent(in   ) :: adaptiveIS !< settings for adaptive importance sampling

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

end module interface_probCalcData
