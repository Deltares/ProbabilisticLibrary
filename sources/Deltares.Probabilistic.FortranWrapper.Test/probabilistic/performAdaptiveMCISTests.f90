! Copyright (C) Stichting Deltares and State of the Netherlands 2023. All rights reserved.
!
! This file is part of the Hydra Ring Application.
!
! The Hydra Ring Application is free software: you can redistribute it and/or modify
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

module performAdaptiveMCIStests
    !
    use ftnunit
    use precision
    use interface_probCalc
    use interface_probCalcData
    use interface_distributions
    use class_probcalc
    use sparseWaartsTestFunctions
    use toolkitTestFunctions
    use sparseWaartsTestFunctions

    implicit none

    private

    public :: allPerformAdaptiveMCIStests

contains
!> This subroutine contains the calls for all the tests of the adaptive importance sampling technique.

subroutine allPerformAdaptiveMCIStests

    integer, parameter :: level = 1

    call testWithLevel( AdapMCIStest1, "Test 1 for adaptive importance sampling routine", level)
    call testWithLevel( AdaptiveImportanceSamplingTestLinear, &
        "Test adaptive importance sampling routine: linear function of two stochasts", level)
    call testWithLevel( AdaptiveImportanceSamplingTestNonlinear, &
        "Test adaptive importance sampling routine: nonlinear function of two stochasts", level)
    call testWithLevel( AdaptiveImportanceSamplingTestMany, &
        "Test adaptive importance sampling routine: nonlinear function of many stochasts", level)
    call testWithLevel( AdaptiveImportanceSamplingTestBligh, &
        "Test adaptive importance sampling routine (start vector): Bligh function", level)
    call testWithLevel( AdaptiveImportanceSamplingTestBligh2, &
        "Test adaptive importance sampling routine (high eps failure): Bligh function", level)
    call testWithLevel( AdaptiveImportanceSamplingTestBligh3, &
        "Test adaptive importance sampling routine (low eps failure): Bligh function", level)

end subroutine allPerformAdaptiveMCIStests

subroutine AdapMCIStest1
    implicit none

    type(probabilisticDataStructure_data)          :: probDb

    real (kind = wp), pointer   :: alfa(:)
    real (kind = wp), pointer   :: x(:)
    integer, pointer            :: iPoint(:)

    real (kind = wp)            :: beta
    real (kind = wp), parameter :: betaKnown = 2.85d0
    real (kind = wp), parameter :: margin = 1d-2

    integer, parameter            :: nStochasts = 2
    logical                       :: conv

    allocate( alfa(nStochasts), x(3), iPoint(nStochasts) )
    iPoint     = (/ 1, 2 /)
    call initProbabilisticCalculation ( probDb, 3, .false., .false. )

    probDb%method%AdaptiveIS%startmethod = fORMStartOne

    ! Initialize stochast data
    call setStochasticDistrib( probDb, 1, distributionNormal       , 0.0d0, 1.0D0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 2, distributionNormal       , 0.0d0, 1.0d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 3, distributionDeterministic, 4.0d0, 0.0d0, 0.0d0, 0.0d0)

    ! Initialise deterministic parameters
    call initDeterministicValue( probDb, x )

    probDb%method%AdaptiveIS%seedPRNG = 1

    ! Perform computation to determine alpha and beta
    call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
    call performAdpMCIS( probDb, simpleZ, x, alfa, beta, conv )
    call cleanUpWaartsTestsFunctions

    call assert_comparable(beta, betaKnown, margin, "The computed beta deviates from the analytically computed value")
    call finalizeProbabilisticCalculation(probDb)

end subroutine AdapMCIStest1

!> Tests for adaptive importance sampling: compare with test cases from the probabilistic toolkit.
!> Linear function of two uniformly distributed stochasts

subroutine AdaptiveImportanceSamplingTestLinear

    !
    ! Test performAdaptiveImportanceSampling
    !
    use distributionFunctions

    implicit none

    type(probabilisticDataStructure_data)          :: probDb

    real (kind = wp), allocatable :: alpha(:)
    real (kind = wp), allocatable :: x(:)
    integer, allocatable          :: iPoint(:)

    real (kind = wp)              :: beta = 0.0d0
    real (kind = wp)              :: betaKnown = 3.91_wp
    real (kind = wp)              :: margin = 1.0d-2

    integer                       :: i
    integer                       :: nStochasts
    logical                       :: conv
    type(tpAdaptiveIS)            :: AdaptiveIS

    nStochasts = 2
    allocate( alpha(nStochasts), x(nStochasts), iPoint(nStochasts) )
    iPoint     = (/ 1, 2 /)
    call initProbabilisticCalculation ( probDb, size(alpha), .false., .false. )

    probDb%method%AdaptiveIS%startmethod = fORMStartOne

    ! Initialize stochast data
    do i = 1,nStochasts
        call setStochasticDistrib( probDb, i, distributionUniform, -1.0d0, 1.0D0, 0.0d0, 0.0d0)
    end do

    ! Initialise deterministic parameters
    call initDeterministicValue( probDb, x )

    ! Set details:
    ! - 1000, 2000 and 10000 samples (min, max, final)
    ! - coefficient for failure 0.01 (choose the same for non-failure)
    ! - scale factor for variance 2.0
    AdaptiveIS%seedPRNG           = 1
    AdaptiveIS%minimumSamples     = 1000
    AdaptiveIS%maximumSamples     = 2000
    AdaptiveIS%Nadp               = 5
    AdaptiveIS%varCoeffFailure    = 0.01_wp
    AdaptiveIS%epsFailed          = 0.1_wp
    AdaptiveIS%minFailed          = 0
    AdaptiveIS%increaseVariance   = 1.0_wp
    AdaptiveIS%varianceFactor     = [ (2.0_wp, i = 1,nStochasts) ]

    call setParametersProbabilisticAdpMCIS( probDb, AdaptiveIS)

    ! Perform computation to determine alpha and beta
    call performAdpMCIS( probDb, linearIsShiftZ, x, alpha, beta, conv )

    call assert_comparable(beta, betaKnown, margin, "The computed beta deviates from the analytically computed value")
    call finalizeProbabilisticCalculation(probDb)

end subroutine AdaptiveImportanceSamplingTestLinear

!> Tests for adaptive importance sampling: compare with test cases from the probabilistic toolkit.
!> Non-linear function of two normally distributed stochasts

subroutine AdaptiveImportanceSamplingTestNonLinear

    !
    ! Test performAdaptiveImportanceSampling
    !
    use distributionFunctions

    implicit none

    type(probabilisticDataStructure_data)          :: probDb

    real (kind = wp), allocatable :: alpha(:)
    real (kind = wp), allocatable :: x(:)
    integer, allocatable          :: iPoint(:)

    real (kind = wp)              :: beta = 0.0d0
    real (kind = wp)              :: betaReference = 3.5241_wp
    real (kind = wp)              :: margin = 1.0d-4

    integer                       :: i
    integer                       :: nStochasts
    logical                       :: conv

    type(tpAdaptiveIS)            :: AdaptiveIS

    nStochasts = 2
    allocate( alpha(nStochasts), x(nStochasts), iPoint(nStochasts) )
    iPoint = [ (i, i = 1,nStochasts) ]
    call initProbabilisticCalculation ( probDb, size(alpha), .false., .false. )

    probDb%method%AdaptiveIS%startmethod = fORMStartOne

    ! Initialize stochast data
    do i = 1,nStochasts
        call setStochasticDistrib( probDb, i, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)
    end do

    ! Initialise deterministic parameters
    call initDeterministicValue( probDb, x )

    ! Set details:
    ! - 1000, 2000 and 5000 samples (min, max, final)
    ! - coefficient for failure 0.01 (choose the same for non-failure)
    ! - scale factor for variance 1.5
    AdaptiveIS%seedPRNG           = 1
    AdaptiveIS%minimumSamples     = 1000
    AdaptiveIS%maximumSamples     = 2000
    AdaptiveIS%Nadp               = 5
    AdaptiveIS%varCoeffFailure    = 0.01_wp
    AdaptiveIS%epsFailed          = 0.25_wp
    AdaptiveIS%minFailed          = 0
    AdaptiveIS%increaseVariance   = 1.0_wp
    AdaptiveIS%varianceFactor     = [ (1.5_wp, i = 1,nStochasts) ]
    call setParametersProbabilisticAdpMCIS( probDb, AdaptiveIS )

    ! Perform computation to determine alpha and beta
    call performAdpMCIS( probDb, nonlinearIsZ, x, alpha, beta, conv )

    call assert_comparable(beta, betaReference, margin, "The computed beta deviates from the analytically computed value")
    call finalizeProbabilisticCalculation(probDb)

end subroutine AdaptiveImportanceSamplingTestNonLinear

!> Tests for adaptive importance sampling: compare with test cases from the probabilistic toolkit.
!> Non-linear function of many uniformly distributed stochasts

subroutine AdaptiveImportanceSamplingTestMany

    !
    ! Test performAdaptiveImportanceSampling
    !
    use distributionFunctions

    implicit none

    type(probabilisticDataStructure_data)          :: probDb

    real (kind = wp), allocatable :: alpha(:)
    real (kind = wp), allocatable :: x(:)
    integer, allocatable          :: iPoint(:)

    real (kind = wp)              :: beta
    real (kind = wp), parameter   :: betaKnown = 3.265_wp
    real (kind = wp), parameter   :: margin = 1.0d-2

    integer                       :: i
    integer, parameter            :: nStochasts = 13
    logical                       :: conv
    type(tpAdaptiveIS)            :: AdaptiveIS

    allocate( alpha(nStochasts), x(nStochasts), iPoint(nStochasts) )
    iPoint = [ (i, i = 1,nStochasts) ]
    call initProbabilisticCalculation ( probDb, size(alpha), .false., .false. )

    ! Initialize stochast data
    do i = 1,nStochasts
        call setStochasticDistrib( probDb, i, distributionUniform, 0.0d0, 1.0D0, 0.0d0, 0.0d0)
    enddo

    ! Initialise deterministic parameters
    call initDeterministicValue( probDb, x )

    probDb%method%AdaptiveIS%startmethod = fORMStartOne

    ! Set details:
    ! - 1000, 2000 and 5000 samples (min, max, final)
    ! - coefficient for failure 0.01 (choose the same for non-failure)
    ! - scale factor for variance 1.0
    AdaptiveIS%seedPRNG           = 1
    AdaptiveIS%minimumSamples     = 1000
    AdaptiveIS%maximumSamples     = 2000
    AdaptiveIS%Nadp               = 5
    AdaptiveIS%varCoeffFailure    = 0.01_wp
    AdaptiveIS%epsFailed          = 0.25_wp
    AdaptiveIS%minFailed          = 10
    AdaptiveIS%increaseVariance   = 1.0_wp
    AdaptiveIS%varianceFactor     = [ (1.0_wp, i = 1,nStochasts) ]
    call setParametersProbabilisticAdpMCIS( probDb, adaptiveIS)

    ! Perform computation to determine alpha and beta
    call performAdpMCIS( probDb, manyIsZ, x, alpha, beta, conv )

    call assert_comparable(beta, betaKnown, margin, "The computed beta deviates from the analytically computed value")
    call finalizeProbabilisticCalculation(probDb)

end subroutine AdaptiveImportanceSamplingTestMany

!> Tests for adaptive importance sampling: compare with test cases from the probabilistic toolkit.
!> Z-function with piping model conform Bligh

subroutine AdaptiveImportanceSamplingTestBligh

    !
    ! Test performAdaptiveImportanceSampling
    !
    use distributionFunctions

    implicit none

    type(probabilisticDataStructure_data)          :: probDb

    real (kind = wp), allocatable :: alpha(:)
    real (kind = wp), allocatable :: x(:)
    integer, allocatable          :: iPoint(:)
    integer                       :: i

    real (kind = wp)              :: beta
    real (kind = wp), parameter   :: betaKnown = 5.56312640962081_wp
    real (kind = wp), parameter   :: margin = 1d-2 ! TODO 1.0d-5

    integer, parameter            :: nStochasts = 4
    logical                       :: conv
    type(tpAdaptiveIS)            :: AdaptiveIS

    allocate( alpha(nStochasts), x(nStochasts), iPoint(nStochasts) )
    iPoint = [ (i, i = 1,nStochasts) ]
    call initProbabilisticCalculation ( probDb, size(alpha), .false., .false. )

    ! Initialize stochast data
    call setStochasticDistrib( probDb, 1, distributionShiftedLognormal2  ,  2.0d0,  0.4d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 2, distributionNormal             , 60.0d0, 10.0d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 3, distributionDeterministic      , 15.0d0,  0.0d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 4, distributionGumbelDistribution2,  0.5d0,  0.1d0, 0.0d0, 0.0d0)

    ! Initialise deterministic parameters
    call initDeterministicValue( probDb, x )

    probDb%method%AdaptiveIS%startMethod = fORMStartGivenVector

    ! Set details:
    ! - 10000 samples
    ! - coefficient for failure 0.01 (choose the same for non-failure)
    ! - different values of scale factor for variance
    ! - different values of shift
    AdaptiveIS%seedPRNG           = 1
    AdaptiveIS%minimumSamples     = 1000
    AdaptiveIS%maximumSamples     = 2000
    AdaptiveIS%Nadp               = 5
    AdaptiveIS%varCoeffFailure    = 0.01_wp
    AdaptiveIS%epsFailed          = 0.5_wp
    AdaptiveIS%minFailed          = 0
    AdaptiveIS%increaseVariance   = 1.0_wp
    AdaptiveIS%varianceFactor     = [ (1.5_wp, i = 1,nStochasts) ]
    call setParametersProbabilisticAdpMCIS( probDb, adaptiveIS)

    probDb%method%AdaptiveIS%startValue = [-0.97798_wp, -5.0_wp, 0.0_wp, 0.021763_wp]
    probDb%method%AdaptiveIS%varianceFactor = [1.0_wp, 2.0_wp, 1.0_wp, 2.0_wp]

    ! Perform computation to determine alpha and beta
    call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
    call performAdpMCIS( probDb, blighZ, x, alpha, beta, conv )
    call cleanUpWaartsTestsFunctions

    call assert_comparable(beta, betaKnown, margin, "The computed beta deviates from the analytically computed value")
    call finalizeProbabilisticCalculation(probDb)

end subroutine AdaptiveImportanceSamplingTestBligh

!> Tests for adaptive importance sampling: compare with test cases from the probabilistic toolkit.
!> Z-function with piping model conform Bligh

subroutine AdaptiveImportanceSamplingTestBligh2

    !
    ! Test performAdaptiveImportanceSampling
    !
    use distributionFunctions

    implicit none

    type(probabilisticDataStructure_data)          :: probDb

    real (kind = wp), allocatable :: alpha(:)
    real (kind = wp), allocatable :: x(:)
    integer, allocatable          :: iPoint(:)
    integer                       :: i

    real (kind = wp)              :: beta
    real (kind = wp), parameter   :: betaReference = 5.493762_wp
    real (kind = wp), parameter   :: margin = 1.0d-5

    integer, parameter            :: nStochasts = 4
    logical                       :: conv
    type(tpAdaptiveIS)            :: AdaptiveIS

    allocate( alpha(nStochasts), x(nStochasts), iPoint(nStochasts) )
    iPoint = [ (i, i = 1,nStochasts) ]
    call initProbabilisticCalculation ( probDb, size(alpha), .false., .false. )

    ! Initialize stochast data
    call setStochasticDistrib( probDb, 1, distributionShiftedLognormal2  ,  2.0d0,  0.4d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 2, distributionNormal             , 60.0d0, 10.0d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 3, distributionDeterministic      , 15.0d0,  0.0d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 4, distributionGumbelDistribution2,  0.5d0,  0.1d0, 0.0d0, 0.0d0)

    ! Initialise deterministic parameters
    call initDeterministicValue( probDb, x )

    probDb%method%AdaptiveIS%startmethod = fORMStartOne

    ! Set details:
    ! - 1000, 2000 and 10000 samples (min, max, final)
    ! - coefficient for failure 0.01 (choose the same for non-failure)
    ! - scale factor for variance 1.5
    AdaptiveIS%seedPRNG           = 1
    AdaptiveIS%minimumSamples     = 1000
    AdaptiveIS%maximumSamples     = 2000
    AdaptiveIS%Nadp               = 5
    AdaptiveIS%varCoeffFailure    = 0.01_wp
    AdaptiveIS%epsFailed          = 0.5_wp
    AdaptiveIS%minFailed          = 0
    AdaptiveIS%increaseVariance   = 1.0_wp
    AdaptiveIS%varianceFactor     = [ (1.5_wp, i = 1,nStochasts) ]
    call setParametersProbabilisticAdpMCIS( probDb, adaptiveIS )

    ! Perform computation to determine alpha and beta
    call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
    call performAdpMCIS( probDb, blighZ, x, alpha, beta, conv )
    call cleanUpWaartsTestsFunctions

    call assert_comparable(beta, betaReference, margin, "The computed beta deviates from the analytically computed value")
    call finalizeProbabilisticCalculation(probDb)

end subroutine AdaptiveImportanceSamplingTestBligh2

!> Tests for adaptive importance sampling: compare with test cases from the probabilistic toolkit.
!> Z-function with piping model conform Bligh

subroutine AdaptiveImportanceSamplingTestBligh3

    !
    ! Test performAdaptiveImportanceSampling
    !
    use distributionFunctions

    implicit none

    type(probabilisticDataStructure_data)          :: probDb

    real (kind = wp), allocatable :: alpha(:)
    real (kind = wp), allocatable :: x(:)
    integer, allocatable          :: iPoint(:)
    integer                       :: i

    real (kind = wp)              :: beta
    real (kind = wp), parameter   :: betaReference = 5.493762_wp
    real (kind = wp), parameter   :: margin = 1.0d-5

    integer, parameter            :: nStochasts = 4
    logical                       :: conv
    type(tpAdaptiveIS)            :: AdaptiveIS

    allocate( alpha(nStochasts), x(nStochasts), iPoint(nStochasts) )
    iPoint = [ (i, i = 1,nStochasts) ]
    call initProbabilisticCalculation ( probDb, size(alpha), .false., .false. )

    ! Initialize stochast data
    call setStochasticDistrib( probDb, 1, distributionShiftedLognormal2  ,  2.0d0,  0.4d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 2, distributionNormal             , 60.0d0, 10.0d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 3, distributionDeterministic      , 15.0d0,  0.0d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 4, distributionGumbelDistribution2,  0.5d0,  0.1d0, 0.0d0, 0.0d0)

    ! Initialise deterministic parameters
    call initDeterministicValue( probDb, x )

    probDb%method%AdaptiveIS%startmethod = fORMStartOne

    ! Set details:
    ! - 1000 and 2000 samples (min, max)
    ! - coefficient for failure 0.01 (choose the same for non-failure)
    ! - scale factor for variance 1.5
    AdaptiveIS%seedPRNG           = 1
    AdaptiveIS%minimumSamples     = 1000
    AdaptiveIS%maximumSamples     = 2000
    AdaptiveIS%Nadp               = 5
    AdaptiveIS%varCoeffFailure    = 0.01_wp
    AdaptiveIS%epsFailed          = 0.001_wp
    AdaptiveIS%minFailed          = 0
    AdaptiveIS%increaseVariance   = 1.0_wp
    AdaptiveIS%varianceFactor     = [ (1.5_wp, i = 1,nStochasts) ]
    call setParametersProbabilisticAdpMCIS( probDb, adaptiveIS )

    ! Perform computation to determine alpha and beta
    call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
    call performAdpMCIS( probDb, blighZ, x, alpha, beta, conv )
    call cleanUpWaartsTestsFunctions

    call assert_comparable(beta, betaReference, margin, "The computed beta deviates from the analytically computed value")

    call finalizeProbabilisticCalculation(probDb)

end subroutine AdaptiveImportanceSamplingTestBligh3

subroutine performAdpMCIS( probDb, fx, x, alfa, beta, convCriterium )
    type(probabilisticDataStructure_data)      :: probDb           !< Probabilistic data module
    procedure(zfunc)                           :: fx               !< Function implementing the z-function of the failure mechanism
    real(kind=wp), intent(out)                 :: alfa(:)          !< Alpha values
    real(kind=wp), intent(out)                 :: beta             !< Reliability index
    real(kind=wp), intent(inout)               :: x(:)             !< X values of design point
    logical,       intent(out)                 :: convCriterium    !< Convergence criterium indicator

    type(storedConvergenceData) :: convergenceData  !< struct holding all convergence data

    probDb%method%calcMethod = methodAdaptiveImportanceSampling
    probDb%method%FORM%startMethod = probDb%method%AdaptiveIS%startMethod

    probDb%method%FORM%startValue = probDb%method%AdaptiveIS%startValue
    call probCalc%run( probDb, fx, alfa, beta, x, convergenceData)
    convCriterium = convergenceData%convCriterium
end subroutine performAdpMCIS

end module performAdaptiveMCIStests
