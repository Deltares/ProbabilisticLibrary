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

!> @file performDSTests.f90
!! Test the initialisation and/or other methods for NumInt
!!
!
module performDSTests
    use precision
    use ftnunit
    use feedback
    use interface_probcalc
    use interface_probcalcdata
    use interface_distributions
    use class_probCalc

    implicit none

    private

    public :: performAllDSTests

    integer, parameter :: nStochasts = 2 !< Number of stochastic variables for tests in module
    integer, parameter :: nrMethods  = 9 !< Number of DS iteration methods

    integer :: iCounter, iCounter2

contains

!> wrapper for all tests
subroutine performAllDSTests
    integer, parameter :: level = 1
    call testWithLevel(testNodFunction, "Test DS with the nod function", level)
    call testWithLevel(testNodFunction2, "Test DS with the nod function and only one sample", level)
    call testWithLevel(testDSFI, "Test DSFI", level, "not implemented yet")
    call testWithLevel(testDesignOutputOptions, "Test the several design output options of DS", level)
    call testWithLevel(testZeqZero, "Test DS with z is zero for u is zero", level, "work-in-progress")
    call testWithLevel(testZnegative, "Test DS with z is negative for u is zero", level)
    call testWithLevel(testCancel, "Test cancellation DS", level)
    call testWithLevel(testErrorHandling, "Test error handling DS", level, "work-in-progress")
end subroutine performAllDSTests

!> test DS with a nod function
subroutine testNodFunction
    type(probabilisticDataStructure_data) :: probDb                         !< Probabilistic data module
    integer                               :: iPoint(nStochasts)             !< Pointer from (U) to (X) variables
    real(kind=wp), pointer                :: x(:)                           !< X values
    real(kind=wp)                         :: alpha(nStochasts)              !< Alpha values (short vector)
    real(kind=wp)                         :: beta(nrMethods)                !< Reliability index cases
    logical                               :: convergenceCriteriumReached    !< Convergence criterium indicator
    integer                               :: counters(nrMethods)            !< number z-function evaluations per DS step
    integer                               :: j                              !< loop counter
    character(len=32)                     :: msg                            !< string for error message
    type(convDataSamplingMethods)         :: convergenceDataDS              !< convergenceData

    call setupDStests(probDb, iPoint, x)

    do j = 1, nrMethods
        if (j == 7) cycle ! 7 not implemented
        probDb%method%DS%iterationMethod = j
        probDb%method%DS%epsilonDu = merge(1d-3, 1d-5, j<=3)
        iCounter = 0
        call performDirectionalSampling ( probDb, zFuncNod, x, alpha, beta(j), convergenceCriteriumReached, convergenceDataDS )
        if (j > 1) then
            write(msg,'(a,i0)') 'diff in beta, j =', j
            call assert_comparable( beta(j), 3.110_wp, 0.002_wp, msg)
            write(msg,'(a,i0)') 'diff in alpha, j =', j
            call assert_comparable( alpha, [-sqrt(0.5_wp), sqrt(0.5_wp)], 0.01_wp, msg)
        endif
        counters(j) = iCounter
    enddo

end subroutine testNodFunction

!> test DS with a nod function, with only one sample
subroutine testNodFunction2
    integer, parameter                    :: nrCases = 3                    !< number of test cases
    type(probabilisticDataStructure_data) :: probDb                         !< Probabilistic data module
    integer                               :: iPoint(nStochasts)             !< Pointer from (U) to (X) variables
    real(kind=wp), pointer                :: x(:)                           !< X values
    real(kind=wp)                         :: alpha(nStochasts)              !< Alpha values (short vector)
    real(kind=wp)                         :: beta(nrMethods, nrCases)       !< Reliability index all cases
    logical                               :: convergenceCriteriumReached    !< Convergence criterium indicator
    integer                               :: i                              !< loop counter
    integer                               :: j                              !< loop counter
    integer                               :: counters(nrMethods, nrCases)   !< number z-function evaluations per DS step
    type(convDataSamplingMethods)         :: convergenceDataDS              !< convergenceData
    real(kind=wp), parameter              :: betaExpected(nrCases) = [18.321_wp, 3.190_wp, 2.417_wp]
    integer, parameter                    :: cases(nrCases) = [15, 860, 21247]
    !    15: largest difference between methods 1 and 2/3, for direction with non failure
    !   860: largest difference between methods 1 and 2/3, for direction with failure
    ! 21247: largest Z value in methods 2/3

    call setupDStests(probDb, iPoint, x, 1)

    do i = 1, nrCases
        probDb%method%DS%seedPRNG = cases(i)
        do j = 1, nrMethods
            if (j == 7) cycle ! 7 not implemented
            probDb%method%DS%iterationMethod = j
            iCounter = 0
            call performDirectionalSampling(probDb, zFuncNod, x, alpha, beta(j,i), convergenceCriteriumReached, convergenceDataDS)
            if (j > 3) then
                call assert_comparable(beta(j,i), betaExpected(i), 2d-3, 'diff in beta')
            endif
            counters(j,i) = iCounter
        enddo
    enddo

end subroutine testNodFunction2

!> test DSFI with a nod function
subroutine testDSFI
    type(probabilisticDataStructure_data) :: probDb                         !< Probabilistic data module
    integer                               :: iPoint(nStochasts)             !< Pointer from (U) to (X) variables
    real(kind=wp), pointer                :: x1(:)                          !< X values first computation
    real(kind=wp), pointer                :: x2(:)                          !< X values 2nd computation
    real(kind=wp)                         :: alpha1(nStochasts)             !< Alpha values (short vector) first computation
    real(kind=wp)                         :: alpha2(nStochasts)             !< Alpha values (short vector) 2nd computation
    real(kind=wp)                         :: beta(2)                        !< Reliability index all cases
    logical                               :: conv1                          !< Convergence criterium indicator (FORM)
    logical                               :: conv2                          !< Convergence criterium indicator (DS)
    type(storedConvergenceData)           :: convergenceData1               !< convergenceData first computation
    type(storedConvergenceData)           :: convergenceData2               !< convergenceData 2nd computation
    type(tProbCalc)                       :: probCalc                       !< class prob. calculation
    real(kind=wp), parameter              :: betaExpected = 2.87564_wp
    integer, parameter                    :: numSamples = 1000

    call setupDStests(probDb, iPoint, x1, numSamples)
    call setupDStests(probDb, iPoint, x2, numSamples)

    probDb%method%calcMethod = methodDirSamplingWithFORMiterations
    probDb%method%FORM%maxIterations = 50
    probDb%method%DS%seedPRNG = 1

    call probCalc%run(probDb, zFuncNod, alpha1, beta(1), x1, conv1, conv2, convergenceData1)
    call assert_true(conv1, 'conv1 - meth. 12')
    call assert_true(conv2, 'conv2 - meth. 12')

    probDb%method%calcMethod = methodDirSamplingWithFORMiterationsStartU
    call probCalc%run(probDb, zFuncNod, alpha2, beta(2), x2, conv1, conv2, convergenceData2)
    call assert_true(conv1, 'conv1 - meth. 16')
    call assert_true(conv2, 'conv2 - meth. 16')

    call assert_equal(convergenceData1%cnvg_data_form%numberIterations, 10, '# FORM iterations u = -alpha * beta')
    call assert_equal(convergenceData2%cnvg_data_form%numberIterations, 2, '# FORM iterations u = u design point DS')

    call assert_comparable(beta(1), beta(2), 1d-12, 'beta must be equal')
    call assert_comparable(beta(1), betaExpected, 1d-5, 'beta compared to pre-computed')

    call assert_comparable(x1, [2.200_wp, -2.200_wp], 1d-3, 'x1')
    call assert_comparable(x2, [2.160_wp, -2.160_wp], 1d-3, 'x2')

end subroutine testDSFI

!> Test the several design output options of DS
subroutine testDesignOutputOptions
    type(probabilisticDataStructure_data) :: probDb                         !< Probabilistic data module
    integer, parameter                    :: nStochasts = 2                 !< Number of stochastic variables
    integer                               :: iPoint(nStochasts)             !< Pointer from (U) to (X) variables
    real(kind=wp), pointer                :: x(:)                           !< X values
    real(kind=wp)                         :: alpha(nStochasts)              !< Alpha values (short vector)
    real(kind=wp)                         :: beta                           !< Reliability index
    logical                               :: convergenceCriteriumReached    !< Convergence criterium indicator
    integer                               :: i                              !< loop counter
    character(len=100)                    :: msg                            !< error message
    real(kind=wp)                         :: expectedX(2,0:8)
    real(kind=wp)                         :: expectedXA(2)
    type(convDataSamplingMethods)         :: convergenceDataDS              !< convergenceData

    call setupDStests(probDb, iPoint, x, 10000)

    expectedXA = [2.1_wp, -2.1_wp]
    expectedX = 0
    expectedX(:,1) = expectedXA
    expectedX(:,2) = expectedXA
    expectedX(:,3) = expectedXA
    expectedX(:,4) = [12.34, 34.56]
    expectedX(:,5) = expectedXA
    expectedX(:,6) = [12.34, 34.56]
    expectedX(:,7) = expectedXA

    do i = 0, 7
        probDb%method%DPoption = i
        iCounter = 0
        call performDirectionalSampling ( probDb, zFuncNod, x, alpha, beta, convergenceCriteriumReached, convergenceDataDS )
        ! beta is equal to previous method DirSamplingIterMethodFastBisection
        call assert_comparable(beta, 3.11_wp, 1d-2, 'diff in beta')
        write(msg,'(a,i1)') 'diff in x; i = ', i
        if (i /= 7) then
            call assert_comparable(x, expectedX(:,i), 1d-1, msg)
        end if
    enddo
    call SetFatalErrorExpected(.false.)

end subroutine testDesignOutputOptions

!> test DS for z-function with z=0 for u=0
subroutine testZeqZero
    type(probabilisticDataStructure_data) :: probDb                         !< Probabilistic data module
    integer, parameter                    :: nStochasts = 2                 !< Number of stochastic variables
    integer                               :: iPoint(nStochasts)             !< Pointer from (U) to (X) variables
    real(kind=wp), pointer                :: x(:)                           !< X values
    real(kind=wp)                         :: alpha(nStochasts)              !< Alpha values (short vector)
    real(kind=wp)                         :: beta                           !< Reliability index
    logical                               :: convergenceCriteriumReached    !< Convergence criterium indicator
    type(convDataSamplingMethods)         :: convergenceDataDS              !< convergenceData

    call setupDStests(probDb, iPoint, x)

    iCounter = 0
    call performDirectionalSampling ( probDb, zFuncZero, x, alpha, beta, convergenceCriteriumReached, convergenceDataDS )
    call assert_comparable(beta, 0.0_wp, 1d-12, 'diff in beta')
    call assert_comparable(alpha, [sqrt(0.5_wp), sqrt(0.5_wp)], 1d-12, 'diff in alpha')
    call assert_equal(iCounter, 1, 'expect only one function evaluation')

end subroutine testZeqZero

!> test DS for z-function with z<0 for u=0
subroutine testZnegative
    type(probabilisticDataStructure_data) :: probDb                         !< Probabilistic data module
    integer                               :: iPoint(nStochasts)             !< Pointer from (U) to (X) variables
    real(kind=wp), pointer                :: x(:)                           !< X values
    real(kind=wp)                         :: alpha(nStochasts)              !< Alpha values (short vector)
    real(kind=wp)                         :: beta                           !< Reliability index
    logical                               :: convergenceCriteriumReached    !< Convergence criterium indicator
    integer                               :: i                              !< loop counter
    character(len=100)                    :: msg                            !< error message
    type(convDataSamplingMethods)         :: convergenceDataDS              !< convergenceData

    call setupDStests(probDb, iPoint, x, 10000)

    do i = 1, NrMethods + 1
        if (i == 7) cycle ! 7 not implemented
        probDb%method%DS%iterationMethod = i
        probDb%method%DS%epsilonDu = merge(1d-3, 1d-5, i<=3)
        iCounter = 0
        if (i > NrMethods) call SetFatalErrorExpected(.true.)
        call performDirectionalSampling ( probDb, zFuncNegative, x, alpha, beta, convergenceCriteriumReached, convergenceDataDS )
        if (i > NrMethods) then
            call GetFatalErrorMessage(msg)
            call assert_equal(1, index(msg, 'Fatal error: Unknown method in subroutine IterationDS: 10'), 'diff in error message')
        else
            call assert_comparable(beta, -1.412_wp, 1d-2, 'diff in beta')
            call assert_comparable(alpha, [sqrt(0.5_wp), -sqrt(0.5_wp)], 1d-1, 'diff in alpha')
        endif
    enddo
    call SetFatalErrorExpected(.false.)

end subroutine testZnegative

!> test error handling
subroutine testErrorHandling
    type(probabilisticDataStructure_data) :: probDb                         !< Probabilistic data module
    integer                               :: iPoint(nStochasts)             !< Pointer from (U) to (X) variables
    real(kind=wp), pointer                :: x(:)                           !< X values
    integer                               :: i                              !< loop counter
    real(kind=wp)                         :: alpha(nStochasts)              !< Alpha values (short vector)
    real(kind=wp)                         :: beta                           !< Reliability index
    logical                               :: convergenceCriteriumReached    !< Convergence criterium indicator
    character(len=100)                    :: msg                            !< error message
    type(convDataSamplingMethods)         :: convergenceDataDS              !< convergenceData
    integer                               :: keepNumCores                   !< make sure test runs on one processor

    call setupDStests(probDb, iPoint, x, 10000)
    keepNumCores = probDb%method%maxParallelThreads
    probDb%method%maxParallelThreads = 1

    call SetFatalErrorExpected(.true.)

    do i = -5, -1
        iCounter = i
        call performDirectionalSampling ( probDb, zFuncError, x, alpha, beta, convergenceCriteriumReached, convergenceDataDS )
        call GetFatalErrorMessage(msg)
        call assert_equal(1, index(msg, 'Fatal error: iCounter == 0'), 'diff in error message')
    enddo
    call SetFatalErrorExpected(.false.)
    probDb%method%maxParallelThreads = keepNumCores

end subroutine testErrorHandling

logical(kind=1) function cancel5steps(i, str) bind(c)
    integer, intent(in), value :: i
    character(len=1), intent(in) :: str(*)

    iCounter2 = iCounter2 + 1
    cancel5steps = (iCounter2 >= 5)
    if (i > 99) write(*,*) str(1)
end function cancel5steps

!> test cancellation of a DS run
subroutine testCancel
    type(probabilisticDataStructure_data) :: probDb                         !< Probabilistic data module
    integer                               :: iPoint(nStochasts)             !< Pointer from (U) to (X) variables
    real(kind=wp), pointer                :: x(:)                           !< X values
    real(kind=wp)                         :: alpha(nStochasts)              !< Alpha values (short vector)
    real(kind=wp)                         :: beta                           !< Reliability index
    logical                               :: convergenceCriteriumReached    !< Convergence criterium indicator
    type(convDataSamplingMethods)         :: convergenceDataDS              !< convergenceData
    integer                               :: expected

    call setupDStests(probDb, iPoint, x, 10000)
    iCounter2 = 0

    call performDirectionalSampling(probDb, zFuncNod, x, alpha, beta, convergenceCriteriumReached, convergenceDataDS, cancel5steps)
    expected = 5
    call assert_equal(iCounter2, expected, "number of samples diff")

end subroutine testCancel

subroutine init_probdb_x(probDb, x, iPoint, nStochasts)
type(probabilisticDataStructure_data), intent(out) :: probDb
real(kind=wp), pointer                             :: x(:)
integer, intent(in)                                :: nStochasts
integer, intent(in)                                :: iPoint(nStochasts)

integer :: maxIpoint, i

maxIpoint = maxval(iPoint)

call initProbabilisticCalculation ( probDb, maxIpoint, .false., .false. )

allocate(probDb%method%FORM%startValue(10))
probDb%method%FORM%startValue    = (/ ( real(i,wp) ,i=1,10 ) /)

allocate(x(maxIpoint))

end subroutine init_probdb_x

!> setup helper function
subroutine setupDStests(probDb, iPoint, x, maxSamples)
    type(probabilisticDataStructure_data), intent(out) :: probDb !< Probabilistic data module
    integer, intent(out) :: iPoint(:)
    real(kind=wp), pointer, intent(out) :: x(:)
    integer, optional, intent(in) :: maxSamples

    iPoint(1) = 1
    iPoint(2) = 2
    call init_probdb_x(probDb, x, iPoint, nStochasts)

    probDb%stovar%maxstochasts = nStochasts
    !allocate(probDb%stovar%activex(nStochasts))
    probDb%stovar%activex = 1

    probDb%stoVar%distTypeX = distributionNormal
    probDb%stoVar%distParameterX = 0.0_wp
    probDb%stoVar%distParameterX(:,2) = 1.0_wp

    associate ( DS => probDb%method%DS)
        DS%seedPRNG = 1
        DS%minimumSamples = 10000
        if (present(maxSamples)) then
            DS%maximumSamples = maxSamples
            DS%minimumSamples = min(DS%minimumSamples, maxSamples)
        else
            DS%maximumSamples = 40000
        endif
        DS%varCoeffFailure = 0.1_wp
        DS%varCoeffNoFailure = 0.1_wp
        DS%epsilonDu = 1d-3
    end associate

    probDb%method%DS%maximumLengthU = 20.0_wp
    probDb%method%DS%du1 = 1.0_wp
    probDb%method%DS%iterations1 = 20
    probDb%method%DPoption = designPointXfromU

    probDb%method%DS%iterationMethod = DirSamplingIterMethodRobust
end subroutine setupDStests

!> emulate a z function as found for for Structure 2017 Z21
function zFuncNod( u, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: u(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    real(kind=wp) :: r, line1, line2

    ierr%iCode = 0

    r = 2.0_wp + u(1) - u(2)

    line1 = 250.0_wp - r / 5.0_wp
    line2 = (6.4_wp-r) * 1d5

    z = min(line1, line2)

    if (compSetting%designPointSetting == designPointOutputTRUE) then
        u(1) = 12.34  ! test that x can be updated in design point
        u(2) = 34.56
    end if

    iCounter = iCounter + 1

end function zFuncNod

function zFuncZero( u, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: u(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%iCode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = 0.0_wp * u(1)

    iCounter = iCounter + 1

end function zFuncZero

function zFuncNegative( u, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: u(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%iCode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = u(1) -u(2) - 2.0_wp

    iCounter = iCounter + 1

end function zFuncNegative

function zFuncError( u, compSetting, ierr ) result(z) bind(c)
    use f2c_tools
    real(kind=wp),            intent(inout) :: u(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = u(1) -u(2) - 2.0_wp

    iCounter = iCounter + 1

    ierr%iCode = 0
    if (iCounter == 0) then
        ierr%iCode = -1
        call copystr('iCounter == 0', ierr%Message)
    endif

end function zFuncError

subroutine performDirectionalSampling( probDb, fx, x, alfa, beta, convCriterium, convergenceData, pc )
    type(probabilisticDataStructure_data)      :: probDb           !< Probabilistic data module
    procedure(zfunc)                           :: fx               !< Function implementing the z-function of the failure mechanism
    real(kind=wp), intent(out)                 :: alfa(:)          !< Alpha values
    real(kind=wp), intent(out)                 :: beta             !< Reliability index
    real(kind=wp), intent(inout)               :: x(:)             !< X values of design point
    logical,       intent(out)                 :: convCriterium    !< Convergence criterium indicator
    type(convDataSamplingMethods), intent(out) :: convergenceData  !< struct holding convergence data for sampling methods
    procedure(progressCancel), optional        :: pc               !< progress/cancel function

    type(storedConvergenceData)   :: allConvergenceData  !< struct holding all convergence data
    logical                       :: conv
    type(tProbCalc)               :: probCalc            !< class prob. calculation

    probDb%method%calcMethod = methodDirectionalSampling
    call probCalc%run( probDb, fx, alfa, beta, x, conv, convCriterium, allConvergenceData, pc=pc)
    convergenceData = allConvergenceData%cnvg_data_ds
end subroutine performDirectionalSampling

end module performDSTests
