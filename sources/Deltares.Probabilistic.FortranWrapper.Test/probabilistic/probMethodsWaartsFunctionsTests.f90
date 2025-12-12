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

!> @file
!! Test for the computation of mechanisms
!<
!
module probMethodsWaartsFunctionsTests

    use precision
    use ftnunit
    use fileUtilities
    use feedback
    use feedback_parameters
    use interface_probCalc
    use class_probCalc
    use interface_probCalcData
    use interface_distributions
    use waartsFunctions
    use sparseWaartsTestFunctions

    implicit none

    private

    public :: allProbMethodsWaartsFunctionsNoIterationTest, &
              allProbMethodsWaartsFunctionsIterationTest, &
              initializeCalculation, &
              initializeStochast, methodDS, maxNumberIterations

    integer           :: probMethod
    integer           :: waartsFunction
    integer           :: numberIterations
    integer           :: maxNumberIterations = 5
    character(len=40) :: waartsFunctionName
    integer           :: dpOption
    integer           :: methodDS
    integer           :: invocationCount

contains

!> Call all the tests for calculateLimitStateFunction
!!
subroutine allProbMethodsWaartsFunctionsNoIterationTest

    call feedbackInitialise( onlyFile, "waartsfunctions.txt", verboseDetailed )

    numberIterations = 1
    call allProbMethodsWaartsFunctionsTests(3) ! parameter is minimum testlevel

    call testWithLevel(testDeterministicParameterHasNoInfluence, "DeterministicParameterHasNoInfluenceTest", 3)
    call testWithLevel(testErrorHandlingCalculateLimitStateFunction, "Test error handling", 1)

    call feedbackClose()
    call removeFile("waartsfunctions.txt")

end subroutine allProbMethodsWaartsFunctionsNoIterationTest

!> Call all the tests for calculateLimitStateFunction
!!
subroutine allProbMethodsWaartsFunctionsIterationTest

    call feedbackInitialise( onlyFile, "waartsfunctions.txt", verboseDetailed )

    numberIterations = maxNumberIterations
    call allProbMethodsWaartsFunctionsTests(3) ! parameter is minimum testlevel

    call feedbackClose()
    call removeFile("waartsfunctions.txt")

end subroutine allProbMethodsWaartsFunctionsIterationTest

!> Call all the Waarts functions tests
subroutine allProbMethodsWaartsFunctionsTests(minTestLevel)
    integer, intent(in)           :: minTestLevel

    integer                       :: i, j
    character(len=:), allocatable :: probMethodName
    character(len=255)            :: testName

    integer                       :: level
    integer, parameter            :: availableMethods(*) = [ &
        methodFORM, &
        methodCrudeMonteCarlo, &
        methodDirectionalSampling, &
        methodNumericalIntegration, &
        methodImportanceSampling, &
        methodFORMandDirSampling, &
        methodDirSamplingWithFORMiterations, &
        methodAdaptiveImportanceSampling]

    character(len=60), dimension(14) :: functionName = &
        (/   "LinearResistanceSolicitation           ", &     ! 1
             "NoisyLimitState                        ", &     ! 2
             "ResistanceSolicitation 1 quadratic term", &     ! 3
             "LimitState 10 quadratic terms          ", &     ! 4
             "LimitState 25 quadratic terms          ", &     ! 5
             "ConvexFailureDomain                    ", &     ! 6
             "OblateSpheroid                         ", &     ! 7
             "SaddleSurface                          ", &     ! 8
             "DiscontinuousLimitState                ", &     ! 9
             "TwoBranches                            ", &     ! 10
             "ConcaveFailureDomain                   ", &     ! 11
             "SeriesSystem                           ", &     ! 12
             "ParallelSystem                         ", &     ! 13
             "LimitState 25 quadratic terms sparse   " /)     ! 14

    dpOption = -1
    do i = 1, size(availableMethods)

        probMethod = availableMethods(i)

        if (methodDS > 1 .and. probMethod /= methodDirectionalSampling) cycle

        select case (probMethod)
            case (methodFORM)
                probMethodName = "FORM"
            case (methodCrudeMonteCarlo)
                probMethodName = "CrudeMonteCarlo"
            case (methodDirectionalSampling)
                probMethodName = "DirectionalSampling"
            case (methodNumericalIntegration)
                probMethodName = "NumericalIntegration"
            case (methodImportanceSampling)
                probMethodName = "ImportanceSampling"
            case (methodAdaptiveImportanceSampling)
                probMethodName = "AdaptiveImportanceSampling"
            case (methodFORMandDirSampling)
                probMethodName = "FORMandDirSampling"
            case (methodDirSamplingWithFORMiterations)
                probMethodName = "DirSamplingWithFORMiterations"
            case default
                cycle
        end select


    end do

end subroutine allProbMethodsWaartsFunctionsTests




!> Initializes calculation
subroutine initializeCalculation (probDb, mvar, alfa, x)

    type(probabilisticDataStructure_data)          :: probDb
    integer,       intent(in)   :: mvar
    real (kind = wp), pointer   :: alfa(:)
    real (kind = wp), pointer   :: x(:)

    allocate ( alfa(mvar) )
    allocate ( x(mvar) )

    call initProbabilisticCalculation ( probDb, mvar, .false., .false. )

    !
    ! Seeding option for MonteCarlo and Directional Sampling: use fixed seed
    !
    probDb%method%CMC%seedPRNG = 1234
    probDb%method%DS%seedPRNG = 1234

end subroutine initializeCalculation


!> Initializes stochast
subroutine initializeStochast (probDb, stnr, sttype, par1, par2, par3, par4)

    type(probabilisticDataStructure_data)           :: probDb
    integer,       intent(in)    :: stnr
    integer,       intent(in)    :: sttype
    real(kind=wp), intent(in)    :: par1
    real(kind=wp), intent(in)    :: par2
    real(kind=wp), intent(in)    :: par3
    real(kind=wp), intent(in)    :: par4


    call setStochasticDistrib( probDb, stnr, sttype, par1, par2, par3, par4 )

end subroutine initializeStochast


!> Do calculation
subroutine iterateMechanism (probDb, convergenceData, z_func, probMethod, alfa, combinedBeta, x)
    type(probabilisticDataStructure_data)   :: probDb
    type(storedConvergenceData)             :: convergenceData
    procedure(zfunc)                        :: z_func
    integer,       intent(in)               :: probMethod
    real(kind=wp), intent(inout), target   :: alfa(:)
    real(kind=wp), intent(  out)           :: combinedBeta
    real(kind=wp), intent(inout), target   :: x(:)

    integer                      :: i
    type(designPoint)            :: dp

    combinedBeta = 0.0_wp

    probDb%method%calcMethod = probMethod
    dp%alpha => alfa
    dp%x     => x
    do i = 1, numberIterations
        call probCalc%run( probDb, z_func, dp, convergenceData )

        combinedBeta = combinedBeta + dp%beta / numberIterations
    end do

end subroutine iterateMechanism

!> Test if a deterministic parameter has indeed no influence
subroutine testDeterministicParameterHasNoInfluence

    type(probabilisticDataStructure_data)  :: probDb
    type(storedConvergenceData)            :: convergenceData
    type(designPoint)                      :: dp1, dp2

   ! Initialization of mechanism
   call initializeCalculation (probDb, 2, dp1%alpha, dp1%x)

   probDb%method%FORM%trialLoops = 10

   ! Initialize stochast data
   call initializeStochast ( probDb, 1, distributionDeterministic, 5.0d0, 0.0D0, 0.0d0, 0.0d0)
   call initializeStochast ( probDb, 2, distributionNormal       , 2.0d0, 1.0d0, 0.0d0, 0.0d0)

   ! Perform computation numberIterations times
   probDb%method%calcMethod = methodFORM
   call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
   call probCalc%run( probDb, zLinearResistanceSolicitation, dp1, convergenceData )
   call cleanUpWaartsTestsFunctions

   call finalizeProbabilisticCalculation(probDb)

   !! Now again but with deterministic value fixed in z function

   call initializeCalculation (probDb, 1, dp2%alpha, dp2%x)

   ! Initialize stochast data
   call initializeStochast ( probDb, 1, distributionNormal, 2.0d0, 1.0d0, 0.0d0, 0.0d0)

   ! Perform computation numberIterations times
   probDb%method%calcMethod = methodFORM
   call probCalc%run( probDb, zLinearResistanceSolicitationFixed, dp2, convergenceData )

   call assert_comparable( dp2%beta, dp1%beta, 0.01d0, "Deterministic parameter has influence: different beta's" )
   call finalizeProbabilisticCalculation(probDb)

end subroutine testDeterministicParameterHasNoInfluence

!> Test if a deterministic parameter has indeed no influence
subroutine testErrorHandlingCalculateLimitStateFunction

    type(probabilisticDataStructure_data)  :: probDb
    type(storedConvergenceData)            :: convergenceData
    character(len=255)                     :: message

    integer                     :: ipos

    type(tProbCalc)             :: probCalc            !< class prob. calculation
    type(designPoint)           :: dp

   ! Initialization of mechanism with no stochastic parameters
   call initializeCalculation (probDb, 0, dp%alpha, dp%x)

   probDb%method%FORM%trialLoops = 10

   ! Initialize stochast data
   !call initializeStochast ( probDb, 1, distributionDeterministic, 5.0d0, 0.0D0, 0.0d0, 0.0d0)
   !call initializeStochast ( probDb, 2, distributionNormal       , 2.0d0, 1.0d0, 0.0d0, 0.0d0)

   ! Perform computation numberIterations times
   call SetFatalErrorExpected(.true.)
   probDb%method%calcMethod = methodFORM
   call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
   call probCalc%run( probDb, zLinearResistanceSolicitation, dp, convergenceData )
   call cleanUpWaartsTestsFunctions

   call SetFatalErrorExpected(.false.)
   call GetFatalErrorMessage(message)
   call assert_equal(message, 'Fatal error: No stochastic parameters found', '1st message from calculateLimitStateFunction')

   call finalizeProbabilisticCalculation(probDb)

   !! Now again but with deterministic value fixed in z function
   !! But with an unknown method

   call initializeCalculation (probDb, 1, dp%alpha, dp%x)

   ! Initialize stochast data
   call initializeStochast ( probDb, 1, distributionNormal, 2.0d0, 1.0d0, 0.0d0, 0.0d0)

   ! Perform computation numberIterations times
   call SetFatalErrorExpected(.true.)
   probDb%method%calcMethod = 99
   call probCalc%run( probDb, zLinearResistanceSolicitationFixed, dp, convergenceData )
   call SetFatalErrorExpected(.false.)
   call GetFatalErrorMessage(message)
   ipos = index(message, "99")
   call assert_true(ipos > 0, '2nd message from calculateLimitStateFunction')

   call finalizeProbabilisticCalculation(probDb)

end subroutine testErrorHandlingCalculateLimitStateFunction

!> Linear resistance solicitation function with generic interface
function zLinearResistanceSolicitationFixed( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = linearResistanceSolicitation( 5.0d0, x(1) )

    invocationCount = invocationCount + 1

end function zLinearResistanceSolicitationFixed


!> Noisy limit state function with generic interface
function zNoisyLimitState( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = noisyLimitState( x )

    invocationCount = invocationCount + 1

end function zNoisyLimitState


!> Resistance solicitation with one quadratic term with generic interface
function zResistanceSolicitation1QuadraticTerm( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = resistanceSolicitation1QuadraticTerm( x ( 1 ), x ( 2)  )

    invocationCount = invocationCount + 1

end function zResistanceSolicitation1QuadraticTerm


!> Limit state function with ten quadratic terms with generic interface
function zLimitState10QuadraticTerms( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = limitState10QuadraticTerms( x ( 1 ), x ( 2 : 11)  )

    invocationCount = invocationCount + 1

end function zLimitState10QuadraticTerms


!> Limit state function with 25 quadratic terms with generic interface
function zLimitState25QuadraticTerms( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = limitState25QuadraticTerms( x ( 1 ), x ( 2 : 26)  )

    invocationCount = invocationCount + 1

end function zLimitState25QuadraticTerms

!> Convex failure domain with generic interface
function zConvexFailureDomain( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = convexFailureDomain( x ( 1 ), x ( 2 )  )

    invocationCount = invocationCount + 1

end function zConvexFailureDomain

!> Saddle surface with generic interface
function zSaddleSurface( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = saddleSurface( x ( 1 ), x ( 2 )  )

    invocationCount = invocationCount + 1

end function zSaddleSurface


!> Discontinuous limit state with generic interface
function zDiscontinuousLimitState( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = discontinuousLimitStateFunction( x ( 1 ), x ( 2 )  )

    invocationCount = invocationCount + 1

end function zDiscontinuousLimitState


!> Two branches with generic interface
function zTwoBranches( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = twoBranches( x ( 1 ), x ( 2 ), x ( 3 ) )

    invocationCount = invocationCount + 1

end function zTwoBranches


!> Concave failure domain with generic interface
function zConcaveFailureDomain( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = concaveFailureDomain( x ( 1 ), x ( 2 ) )

    invocationCount = invocationCount + 1

end function zConcaveFailureDomain


!> Series system with generic interface
function zSeriesSystem( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = seriesSystem( x ( 1 ), x ( 2 ) )

    invocationCount = invocationCount + 1

end function zSeriesSystem


!> Parallel system with generic interface
function zParallelSystem( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = parallelSystem( x( 1), x( 2), x( 3), x( 4), x( 5) )

    invocationCount = invocationCount + 1

end function zParallelSystem

subroutine initializeNumericalIntegration (uMin, uMax, nrIntervals)

    real (kind=wp), intent(out) :: uMin            !< Minimum value for u
    real (kind=wp), intent(out) :: uMax            !< Maximum value for u
    integer, intent(out)        :: nrIntervals     !< Number of integration intervals for numerical integration

    if (probMethod == methodNumericalIntegration) then
        uMin = -5.0d0
        uMax =  5.0d0

        !
        ! The number of intervals is chosen as a function of the number of stochasts
        ! out of practical considerations of the run-time
        !
        select case (waartsFunction)
            case (5)
                nrIntervals = 2
            case (4, 7)
                nrIntervals = 3
            case (2)
                nrIntervals = 10
            case (13)
                nrIntervals = 20
            case default
                nrIntervals = 100
        end select
    endif

end subroutine initializeNumericalIntegration

subroutine assert_almost_zero( x, margin, message )
    real(kind=wp), intent(in) :: x, margin
    character(len=*), intent(in) :: message

    call assert_true(abs(x) < margin, message)

end subroutine assert_almost_zero

end module probMethodsWaartsFunctionsTests
