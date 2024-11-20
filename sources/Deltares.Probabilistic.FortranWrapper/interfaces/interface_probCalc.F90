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
module interface_probCalc
  use, intrinsic :: iso_c_binding, only: c_double, c_bool
  use interface_gen
  use interface_distributions
  use precision
  use m_realloc_check
  use f2c_tools
  use mTpXData
  use interface_enums
  use interface_typedefs
  implicit none

  logical :: userAborted = .false.

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
    subroutine probCalcF2C(method, distribs, correlations, fx, pc, &
        compIds, x, rn) bind(C)
      use, intrinsic :: iso_c_binding, only: c_double
#ifdef _MSC_VER
      import tMethod, tDistrib, basicCorrelation, tResult, tCompIds, zfunc, progressCancel
#else
      import tMethod, tDistrib, basicCorrelation, tResult, tCompIds
#endif
      type(tMethod),  intent(in)    :: method
      type(tDistrib), intent(in)    :: distribs(*)
      type(basicCorrelation), intent(in)  :: correlations(*)
      type(tCompIds), intent(in)    :: compIds
      procedure(zfunc)              :: fx
      procedure(progressCancel)     :: pc
      real(kind=c_double), intent(inout) :: x(*)
      type(tResult),  intent(  out) :: rn
    end subroutine probCalcF2C
  end interface

contains

subroutine stopSampling()
    userAborted = .true.
end subroutine stopSampling

function textualProgress(progress, str) result(cancel) bind(c)
    integer, intent(in), value :: progress
    character(len=1), intent(in) :: str(*)
    logical(kind=1)              :: cancel

    cancel = userAborted
end function textualProgress

!>
!! Subroutine for the calculation of a limit state function
subroutine calculateLimitStateFunction(probDb, fx, alfaN, beta, x, convergenceData, CpData, pc)
    use feedback
    type(probabilisticDataStructure_data), intent(in) :: probDb    !< Probabilistic data module
    procedure(zfunc)                           :: fx               !< Function implementing the z-function of the failure mechanism
    real(kind=wp), intent(out)                 :: alfaN(:)         !< Alpha values
    real(kind=wp), intent(out)                 :: beta             !< Reliability index
    real(kind=wp), intent(inout)               :: x(:)             !< X values of design point
    type(storedConvergenceData), intent(inout) :: convergenceData  !< struct holding all convergence data
    type(tCpData), intent(inout)               :: CpData           !< class for copying the x-vector
    procedure(progressCancel),    optional     :: pc               !< progress function

    type(tMethod)               :: method
    type(tDistrib)              :: distribs(probDb%stoVar%maxStochasts)
    integer                     :: i, k, nstoch, nStochActive
    type(tCompIds)              :: compIds
    type(tResult)               :: rn
    character(len=ErrMsgLength) :: msg
    integer, allocatable        :: iPoint(:)
    real(kind=wp), allocatable  :: xDense(:)

    call realloc_check (iPoint, probDb%stoVar%maxStochasts, "ipoint local" )
    call realloc_check (CpData%xHR, probDb%stoVar%maxStochasts, "xHR" )

    method%methodId   = probDb%method%calcMethod
    method%tolA       = probDb%method%FORM%epsilonBeta
    method%trialLoops = probDb%method%FORM%trialLoops
    method%numExtraInt = probDb%method%FORM%maxIterations
    select case (method%methodId)
    case(methodDirectionalSampling, methodFORMandDirSampling, methodFORMandDirSamplingWithFORMiterations, &
         methodDirSamplingWithFORMiterationsStartU, methodDirSamplingWithFORMiterations)
        method%tolB          = probDb%method%DS%varcoefffailure
        method%minSamples    = probDb%method%DS%minimumsamples
        method%maxSamples    = probDb%method%DS%maximumsamples
        method%seed1         = probDb%method%DS%seedPRNG
        method%seed2         = probDb%method%DS%seedPRNG
        method%isRepeatableRandom = merge(1, 0, probDb%method%DS%isRepeatableRandom)
        method%numExtraReal1 = -2.0_wp ! FDthreshold
    case(methodCrudeMonteCarlo, methodCrudeMonteCarloWithFORMiterations)
        method%tolB       = probDb%method%CMC%varcoefffailure
        method%minSamples = probDb%method%CMC%minimumsamples
        method%maxSamples = probDb%method%CMC%maximumsamples
        method%seed1      = probDb%method%CMC%seedPRNG
        method%seed2      = probDb%method%CMC%seedPRNG
        method%isRepeatableRandom = merge(1, 0, probDb%method%CMC%isRepeatableRandom)
    case(methodImportanceSampling, methodImportanceSamplingWithFORMiterations)
        method%tolB           = probDb%method%IS%varcoefffailure
        method%minSamples     = probDb%method%IS%minimumsamples
        method%maxSamples     = probDb%method%IS%maximumsamples
        method%varianceFactor = probDb%method%IS%varianceFactor
        method%seed1          = probDb%method%IS%seedPRNG
        method%seed2          = probDb%method%IS%seedPRNG
        method%isRepeatableRandom = merge(1, 0, probDb%method%IS%isRepeatableRandom)
    case(methodAdaptiveImportanceSampling)
        method%tolB            = probDb%method%adaptiveIS%varcoefffailure
        method%minSamples      = probDb%method%adaptiveIS%minimumsamples
        method%maxSamples      = probDb%method%adaptiveIS%maximumsamples
        method%seed1           = probDb%method%adaptiveIS%seedPRNG
        method%seed2           = probDb%method%adaptiveIS%seedPRNG
        method%isRepeatableRandom = merge(1, 0, probDb%method%adaptiveIS%isRepeatableRandom)
        method%trialLoops      = probDb%method%adaptiveIS%nAdp
        method%numExtraInt     = merge(1, 0, probDb%method%adaptiveIS%AutoMaximumSamples)
        method%numExtraReal1   = probDb%method%adaptiveIS%epsFailed
        method%numExtraReal2   = probDb%method%adaptiveIS%increaseVariance
        method%numExtraInt2    = probDb%method%adaptiveIS%minFailed
    case (methodNumericalIntegration)
        method%numExtraInt     = probDb%method%NI%numberIntervals
        method%numExtraReal1   = probDb%method%NI%minimumUvalue
        method%numExtraReal2   = probDb%method%NI%maximumUvalue
    case (methodNumericalBisection)
        method%numericalBisectionSettings = probDb%method%numericalBisectionSettings
    case (methodLatinHyperCube)
        method%latinHypercubeSettings = probDb%method%latinHyperCubeSettings
    case (methodCobylaReliability)
        method%cobylaReliability = probDb%method%cobylaReliability
    case (methodSubSetSimulationReliability)
        method%subSetSimulationReliability = probDb%method%subSetSimulationReliability
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
            distribs(nStochActive)%distributionId = probDb%stovar%disttypex(i)
            distribs(nStochActive)%params = probDb%stovar%distparameterx(i,:)
            iPoint(nStochActive) = i
        end if
    end do

    if (method%methodId == methodAdaptiveImportanceSampling) then
        do i = 1, nStochActive
            method%offsets(i)         = probDb%method%adaptiveIS%translation(iPoint(i))
            method%varianceFactors(i) = probDb%method%adaptiveIS%varianceFactor(iPoint(i))
        end do
    end if

    CpData%xHR = x
    CpData%nStochActive = nStochActive

    method%designPointOption = probDb%method%dpOption
    method%numThreads        = probDb%method%maxParallelThreads
    method%rnd = GeorgeMarsaglia
    call realloc_check(xDense, nStochActive, "xDense")
    call realloc_check(cpData%iPoint, nStochActive, "ipoint in cpData" )
    cpData%iPoint = iPoint(1:nStochActive)

    call convertStartMethod(probDb, method, CpData%iPoint(1:nStochActive))

    if (method%iterationMethod <= 0 .or. method%iterationMethod > 9 .or. method%iterationMethod == 7) then
        call fatalError("Unknown method in subroutine IterationDS: ", method%iterationMethod)
    else if (nstoch > 0) then
        method%progressInterval = 1
        compIds%id             = designPointOutputFALSE
        compIds%nrStochasts    = nStochActive
        compIds%nrCorrelations = probDb%number_correlations
        if (present(pc)) then
            call probCalcF2C(method, distribs, probDb%basic_correlation, fx, pc, compIds, xDense, rn)
        else
            call probCalcF2C(method, distribs, probDb%basic_correlation, fx, textualProgress, compIds, xDense, rn)
        end if

        call cpData%copyDense2Full(xDense, x)
        beta = rn%beta
        alfaN = 0.0_wp
        do k = 1, nStochActive
            alfaN(CpData%iPoint(k)) = rn%alpha(k)
        end do
        if (method%methodId == methodFORMandDirSampling .and. rn%samplesNeeded > 0) then
            ! to get logging in output.txt right; as we have samples, Form did not succeed (no convergence or beta out of range)
            convergenceData%conv = .false.
        else
            convergenceData%conv = rn%convergence
        end if
        convergenceData%convCriterium = rn%convergence
        convergenceData%cnvg_data_ds%numberSamples = rn%samplesNeeded
        convergenceData%cnvg_data_form%numberiterations = rn%stepsNeeded
        if (rn%error%iCode /= 0) then
            call copystrback(msg, rn%error%message)
            if (rn%error%iCode > 0) then
                call warningMessage(msg)
            else
                call fatalError(msg)
            end if
        end if
    else
        call fatalError("No stochastic parameters found")
    end if

end subroutine calculateLimitStateFunction

subroutine copyStartVector(probDb, method, iPoint, methodVectorMandatory)
    use feedback
    type(probabilisticDataStructure_data), intent(in)    :: probDb
    type(tMethod),                         intent(inout) :: method
    integer,                               intent(in)    :: iPoint(:)
    character(len=*),            optional, intent(in)    :: methodVectorMandatory

    integer :: nstoch, i

    nstoch = size(iPoint)

    if (allocated(probDb%method%FORM%startValue)) then
        do i = 1, nstoch
            method%startVector(i) = probDb%method%FORM%startValue(iPoint(i))
        end do
    else if (allocated(probDb%method%AdaptiveIS%startValue)) then
        do i = 1, nstoch
            method%startVector(i) = probDb%method%AdaptiveIS%startValue(iPoint(i))
        end do
    else if (present(methodVectorMandatory)) then
        call fatalError("start value must be given for " // methodVectorMandatory)
    else
        method%startVector(1:nstoch) = 1.0_wp
    end if
end subroutine copyStartVector

subroutine convertStartMethod(probDb, method, iPoint)
    use feedback
    type(probabilisticDataStructure_data), intent(in)    :: probDb
    type(tMethod),                         intent(inout) :: method
    integer,                               intent(in)    :: iPoint(:)

    integer :: nstoch, startMethod

    nstoch = size(iPoint)

    startMethod = probDb%method%FORM%startMethod
    if (method%methodId == methodAdaptiveImportanceSampling) startMethod = probDb%method%adaptiveIS%startMethod
    select case (startMethod)
    case (fORMStartOne)
        method%startmethod = startMethod
    case (fORMStartRaySearch, fORMStartRaySearchWind)
        method%startmethod = fORMStartRaySearch
        call copyStartVector(probDb, method, iPoint)
    case (fORMStartSphereSearch)
        method%startmethod = startMethod
        method%allQuadrants = probDb%method%FORM%allQuadrants
        method%maxStepsSphereSearch = probDb%method%FORM%maxStepsSphereSearch
        call copyStartVector(probDb, method, iPoint)
    case (fORMStartGivenVector)
        method%startmethod = startMethod
        call copyStartVector(probDb, method, iPoint, "fORMStartGivenVector")
    case (fORMStartRaySearchVector)
        method%startmethod = startMethod
        call copyStartVector(probDb, method, iPoint, "fORMStartRaySearchVector")
    case (fORMStartRaySearchVectorScaled)
        method%startmethod = startMethod
        call copyStartVector(probDb, method, iPoint, "fORMStartRaySearchVectorScaled")
    case (fORMStartZero, fORMStartSensitivity)
        method%startmethod = startMethod
    case default
        call fatalError("start method not implemented: ", startMethod)
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
