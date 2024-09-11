! Copyright (C) Stichting Deltares 2024. All rights reserved.
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
    integer, parameter            :: availableMethods(*) = [7, 5, 6, 12, 11, 1, 3, 4] ! TODO the rest is not implemented yet

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
            case (methodCrudeMonteCarloWithFORMiterations)
                probMethodName = "CrudeMonteCarloWithFORMiterations"
            case (methodImportanceSamplingWithFORMiterations)
                probMethodName = "ImportanceSamplingWithFORMiterations"
            case (methodFORMandDirSamplingWithFORMiterations)
                probMethodName = "FORMandDirSamplingWithFORMiterations"
            case (methodDirSamplingWithFORMiterationsStartU)
                probMethodName = "DirSamplingWithFORMiterationsStartU"
            case (methodFORMandDirSamplingWithFORMiterationsStartU)
                probMethodName = "FORMandDirSamplingWithFORMiterationsStartU"
            case default
                cycle
        end select

        do j = 1, size(functionName)

            waartsFunction     = j
            waartsFunctionName = functionName(j)

            level = minTestLevel
            if (i > 10) then
                select case (j)
                case(9, 10, 13)
                    cycle    ! skip some tests for "new" methods
                end select
            endif

            ! some levelling in tests
            if (probMethod==methodNumericalIntegration) then
                if (trim(functionName(waartsFunction)) == "ParallelSystem") then
                    level = max(3, minTestLevel)
                elseif ( trim(functionName(waartsFunction)) == "NoisyLimitState") then
                    level = max(2, minTestLevel)
                end if
            end if

            if ( i == methodDirectionalSampling) then
                write(testName, '(4a,i0,a,i0)') trim(probMethodName), " + ", trim(waartsFunctionName), " numIt = ", &
                    numberIterations, " MethodDS = ", methodDS
            else
                write(testName, '(4a,i0)') trim(probMethodName), " + ", trim(waartsFunctionName), " numIt = ", numberIterations
            endif

            ! Note: Some calculations may be neglected, because these calculations are practically impossible
            if ( probMethod /= methodNumericalIntegration .or. index(functionName(j), "LimitState 25 quadratic terms" ) == 0) then
                invocationCount = 0
                call progressReport( 1, "Test: "//trim(testName) )
                call progressReport( 1, "    Function: "//trim(waartsFunctionName) )

                call testWithLevel( testProbabilisticWithFunction, testName, level )

                call progressReport( 1, "    Number of invocations: ", invocationCount )
            end if

        end do

    end do

    if (numberIterations == 1 .and. any(availableMethods == methodNumericalIntegration)) then
        probMethod         = methodNumericalIntegration
        waartsFunction     = 1
        waartsFunctionName = functionName(1)
        probMethodName     = "NumericalIntegration"
        do i = 0, 5
            dpOption = i
            write(testName, '(4a,i1)') trim(probMethodName), " + ", trim(waartsFunctionName), ' dpOption=', dpOption
            if (i == 5) call SetFatalErrorExpected(.true.)

            invocationCount = 0
            call progressReport( 1, "Test: "//trim(testName) )
            call progressReport( 1, "    Function: "//trim(waartsFunctionName) )

            call testWithLevel( testProbabilisticWithFunction, testName, 2)

            call progressReport( 1, "    Number of invocations: ", invocationCount )

            if (i == 5) call SetFatalErrorExpected(.false.)
        enddo
    endif

end subroutine allProbMethodsWaartsFunctionsTests


!> Test betaFromQ with the expected value
subroutine testProbabilisticWithFunction ( )
    use omp_lib
    type(probabilisticDataStructure_data) :: probDb
    type(storedConvergenceData)           :: convergenceData
    integer                     :: i = 0
    logical                     :: conv
    real (kind = wp), pointer   :: alfa(:)
    real (kind = wp), pointer   :: x(:)
    real (kind = wp)            :: actualBeta = 0.0d0
    real (kind = wp)            :: margin = 1.0d-5
    real (kind = wp)            :: betaFactor
    logical                     :: runOption
    integer                     :: oldNumberIterations

    ! we expect a more precise result if the number of iterations increase
    if (numberIterations <= 1) then
        betaFactor = 1.0d0
    else
        betaFactor = 0.5d0
    end if

    if (probMethod == methodDirectionalSampling .or. probMethod == methodFORM) then
        call omp_set_num_threads(4)
        probDb%method%chunkSizeOpenMP = 200
    endif

    select case (waartsFunction)

        case (1) ! Linear resistance solicitation

            ! Initialization of mechanism
            call initializeCalculation (probDb, 2, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            if (dpOption >= 0) then
                probDb%method%DPoption = dpOption
            endif

            ! Initialize stochast data
            call initializeStochast ( probDb, 1, distributionNormal, 7.0d0, 1.0D0, 0.0d0, 0.0d0)
            call initializeStochast ( probDb, 2, distributionNormal, 2.0d0, 1.0d0, 0.0d0, 0.0d0)

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Perform computation numberIterations times
            call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
            call iterateMechanism ( probDb, convergenceData, zLinearResistanceSolicitation, probMethod, alfa, actualBeta, x, conv)
            call updateCounter(invocationCount)
            call cleanUpWaartsTestsFunctions

            select case (probDb%method%DPoption)
            case (designPointRMinZFunc)
                if (probMethod == methodNumericalIntegration) then
                    call assert_comparable(  3.56231d0, actualBeta, margin, "Linear resistance solicitation: Beta" )
                    call assert_comparable(  0.707106781186547d0, alfa(1), margin, "Linear resistance solicitation: Alfa(1)" )
                    call assert_comparable( -0.707106781186547d0, alfa(2), margin, "Linear resistance solicitation: Alfa(2)" )
                else if (probMethod == methodImportanceSampling) then
                    call assert_comparable( 3.498302d0, actualBeta, margin, "Linear resistance solicitation: Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 18076, "no samples")
                else if (probMethod < 10) then
                    call assert_comparable( 3.54d0, actualBeta, 0.05d0 * betaFactor, "Linear resistance solicitation: Beta" )
                endif
            case (designPointNone)
                call assert_comparable(  0d0, x(1), margin, "dpOption designPointNone: x(1)" )
                call assert_comparable(  0d0, x(2), margin, "dpOption designPointNone: x(2)" )
            case (designPointXfromU, designPointXCorrelatedFromU, designPointRMin)
                call assert_comparable(  4.481d0, x(1), 1d-3, "dpOption designPointXfromU: x(1)" )
                call assert_comparable(  4.519d0, x(2), 1d-3, "dpOption designPointXfromU: x(2)" )
            end select
            call finalizeProbabilisticCalculation(probDb)

        case (2) ! Noisy limit state

            ! Initialization of mechanism
            call initializeCalculation (probDb, 6, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            ! Initialize stochast data
            call initializeStochast (probDb, 1, distributionShiftedLogNormal2, 120.0d0, 12.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 2, distributionShiftedLogNormal2, 120.0d0, 12.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 3, distributionShiftedLogNormal2, 120.0d0, 12.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 4, distributionShiftedLogNormal2, 120.0d0, 12.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 5, distributionShiftedLogNormal2, 50.0d0, 15.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 6, distributionShiftedLogNormal2, 40.0d0, 12.0D0, 0.0d0, 0.0d0)

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Perform computation numberIterations times
            call iterateMechanism (probDb, convergenceData, zNoisyLimitState, probMethod, alfa, actualBeta, x, conv)

            select case (probMethod)

                case (methodNumericalIntegration)
                    call assert_comparable(  2.16254d0, actualBeta, margin, "Noisy limit state: Beta" )
                    call assert_comparable(  0.182574185835055d0, alfa(1), margin, "Noisy limit state: Alfa(1)" )
                    call assert_comparable(  0.547722557505166d0, alfa(2), margin, "Noisy limit state: Alfa(2)" )
                    call assert_comparable(  0.182574185835055d0, alfa(3), margin, "Noisy limit state: Alfa(3)" )
                    call assert_comparable(  0.182574185835055d0, alfa(4), margin, "Noisy limit state: Alfa(4)" )
                    call assert_comparable( -0.547722557505166d0, alfa(5), margin, "Noisy limit state: Alfa(5)" )
                    call assert_comparable( -0.547722557505166d0, alfa(6), margin, "Noisy limit state: Alfa(6)" )

                case (methodFORM)
                    call assert_comparable( 2.348d0, actualBeta, 1d-3, "Noisy limit state: Beta" )
                case (methodCrudeMonteCarlo)
                    call assert_comparable( 2.34491717949861d0, actualBeta, margin, "Noisy limit state: Beta" )
                case (methodImportanceSampling)
                    call assert_comparable( 2.27d0, actualBeta, 1d-2, "Noisy limit state: Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 15258, "no samples")
                case default
                    call assert_true( conv, "No convergence" )
            end select

            call finalizeProbabilisticCalculation(probDb)

        case (3) ! Resistance solicitation with 1 quadratic term

            ! Initialization of mechanism
            call initializeCalculation (probDb, 2, alfa, x)
            probDb%method%DS%iterationMethod = methodDS


            ! Initialize stochast data
            call initializeStochast (probDb, 1, distributionNormal, 11.0d0, 1.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 2, distributionNormal, 1.5d0, 0.5d0, 0.0d0, 0.0d0)

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Perform computation numberIterations times
            call iterateMechanism (probDb, convergenceData, zResistanceSolicitation1QuadraticTerm, probMethod, &
                    alfa, actualBeta, x, conv)

            select case (probMethod)

                case (methodNumericalIntegration)
                    call assert_comparable(  3.455623d0, actualBeta, margin, &
                        "Resistance solicitation with 1 quadratic term: Beta" )
                    call assert_comparable(  0.358979d0, alfa(1), margin, &
                        "Resistance solicitation with 1 quadratic term: Alfa(1)" )
                    call assert_comparable( -0.9333456d0, alfa(2), margin, &
                        "Resistance solicitation with 1 quadratic term: Alfa(2)" )
                    block
                        type(tError) :: ierr
                        real(kind=wp) :: z
                        type(computationSetting) :: idata
                        idata%designPointSetting = -999
                        z = zResistanceSolicitation1QuadraticTerm(x, idata, ierr)
                        call assert_true(abs(z) < 1d-1, "small z")
                    end block

                case (methodFORM)
                    call assert_comparable( 3.46d0, actualBeta, 0.05d0 * betaFactor, &
                        "Resistance solicitation with 1 quadratic term: Beta" )
                case (methodCrudeMonteCarlo)
                    call assert_comparable( 3.45028260d0, actualBeta, 1d-8, &
                        "Resistance solicitation with 1 quadratic term: Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 100000, "no samples")
                case (methodImportanceSampling)
                    call assert_comparable( 3.47d0, actualBeta, 1d-2, &
                        "Resistance solicitation with 1 quadratic term: Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 17852, "no samples")
                case default
                    call assert_true( conv, "No convergence" )
            end select

            call finalizeProbabilisticCalculation(probDb)

        case (4) ! Limit state with 10 quadratic terms

            ! Initialization of mechanism
            call initializeCalculation (probDb, 11, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            ! Initialize stochast data
            call initializeStochast (probDb, 1, distributionNormal, 0.5d0, 0.1D0, 0.0d0, 0.0d0)

            do i = 1, 10
              call initializeStochast (probDb, i + 1, distributionNormal, 0.2d0, 0.1d0, 0.0d0, 0.0d0)
            end do

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Perform computation numberIterations times
            call iterateMechanism( probDb, convergenceData, zLimitState10QuadraticTerms, probMethod, alfa, actualBeta, x, conv)

            select case(probMethod)
                case ( methodFORM )
                    ! Compare to the value given by Paul Waarts
                    call assert_comparable( 3.20d0, actualBeta, 0.05d0 * betaFactor, "Limit state with 10 quadratic terms: Beta" )

                case ( methodNumericalINtegration )
                    call assert_comparable(  2.3146639d0, actualBeta, margin, "Limit state with 10 quadratic terms: Beta" )
                    call assert_comparable(  0.707106781186547d0, alfa(1), margin, "Limit state with 10 quadratic terms: Alfa(1)" )
                    call assert_almost_zero( alfa(2), margin, "Limit state with 10 quadratic terms: Alfa(2)" )
                    call assert_almost_zero( alfa(3), margin, "Limit state with 10 quadratic terms: Alfa(3)" )
                    call assert_almost_zero( alfa(4), margin, "Limit state with 10 quadratic terms: Alfa(4)" )
                    call assert_comparable( -0.707106781186548d0, alfa(5), margin, "Limit state with 10 quadratic terms: Alfa(5)" )
                    call assert_almost_zero( alfa(6), margin, "Limit state with 10 quadratic terms: Alfa(6)" )
                    call assert_almost_zero( alfa(7), margin, "Limit state with 10 quadratic terms: Alfa(7)" )
                    call assert_almost_zero( alfa(8), margin, "Limit state with 10 quadratic terms: Alfa(8)" )
                    call assert_almost_zero( alfa(9), margin, "Limit state with 10 quadratic terms: Alfa(9)" )
                    call assert_almost_zero( alfa(10), margin, "Limit state with 10 quadratic terms: Alfa(10)" )
                    call assert_almost_zero( alfa(11), margin, "Limit state with 10 quadratic terms: Alfa(11)" )

                case (methodImportanceSampling)
                    call assert_comparable( 2.9865829522d0, actualBeta, 1d-8, "Limit state with 10 quadratic terms: Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 100000, "no samples")

                case default
                    call assert_true( conv, "No convergence" )

            end select

            call finalizeProbabilisticCalculation(probDb)

        case (5, 14) ! Limit state with 25 quadratic terms

            ! Initialization of mechanism
            if (waartsFunction == 5) then
                call initializeCalculation (probDb, 26, alfa, x)
                probDb%method%DS%iterationMethod = methodDS

                ! Initialize stochast data
                call initializeStochast (probDb, 1, distributionNormal, 0.5d0, 0.1D0, 0.0d0, 0.0d0)

                do i = 1, 25
                    call initializeStochast (probDb, i + 1, distributionNormal, 0.2d0, 0.1d0, 0.0d0, 0.0d0)
                end do
            else
                call initializeCalculation (probDb, 30, alfa, x)
                probDb%method%DS%iterationMethod = methodDS

                ! Initialize stochast data
                call initializeStochast (probDb, 30, distributionNormal, 0.5d0, 0.1D0, 0.0d0, 0.0d0)

                do i = 1, 25
                    call initializeStochast (probDb, i + 2, distributionNormal, 0.2d0, 0.1d0, 0.0d0, 0.0d0)
                end do
            endif

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! increase accuracy, to see effect of OpenMP
            if (probMethod == methodDirectionalSampling) then
                probDb%method%DS%varCoeffFailure   = 0.01d0
                probDb%method%DS%varCoeffNoFailure = 0.01d0
                probDb%method%DS%maximumSamples    = 75000
            endif
            probDb%method%AdaptiveIS%minFailed = 10

            ! Perform computation numberIterations times
            if (waartsFunction == 5) then
                call iterateMechanism( probDb, convergenceData, zLimitState25QuadraticTerms, probMethod, alfa, actualBeta, x, conv)
            else
                x = 0.0_wp
                call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
                call iterateMechanism( probDb, convergenceData, zLimitState25QuadraticTermsSparse, &
                        probMethod, alfa, actualBeta, x, conv)
                call updateCounter(invocationCount)
                call cleanUpWaartsTestsFunctions
            endif

            select case (probMethod)
                case (methodFORM)
                    call assert_comparable( 2.92d0, actualBeta, 0.05d0 * betaFactor, trim(waartsFunctionName) // ": Beta" )
                case (methodDirectionalSampling)
                    call assert_comparable( 2.63d0, actualBeta, 0.05d0 * betaFactor, trim(waartsFunctionName) // ": Beta" )
                case (methodImportanceSampling)
                    if (waartsFunction == 5) then
                        call assert_comparable( 2.622263100d0, actualBeta, 1d-8, trim(waartsFunctionName) // ": Beta" )
                    else
                        ! TODO margin must be reduced
                        call assert_comparable( 2.643205260d0, actualBeta, 0.05d0, trim(waartsFunctionName) // ": Beta" )
                    end if
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 100000, "no samples")
                case (methodAdaptiveImportanceSampling)
                    if (waartsFunction == 5) then
                        call assert_comparable( 2.66d0, actualBeta, 1d-2, trim(waartsFunctionName) // ": Beta" )
                    else
                        call assert_comparable( 2.31d0, actualBeta, 1d-2, trim(waartsFunctionName) // ": Beta" )
                    end if
                case default
                    call assert_true( conv, "No convergence" )
            end select

            call finalizeProbabilisticCalculation(probDb)

        case (6) ! Convex failure domain

            ! Initialization of mechanism
            call initializeCalculation (probDb, 2, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            ! Initialize stochast data
            call initializeStochast (probDb, 1, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 2, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Perform computation numberIterations times
            call iterateMechanism (probDb, convergenceData, zConvexFailureDomain, probMethod, alfa, actualBeta, x, conv)

            select case (probMethod)
                case (methodFORM)
                    ! We need to compare to the correct estimate for the method, not the exact value
                    call assert_comparable( 2.50d0, actualBeta, 0.05d0 * betaFactor, "Convex failure domain: Beta" )

                case (methodNumericalIntegration)
                    call assert_comparable(  2.63526752586810d0, actualBeta, margin, "Convex failure domain: Beta" )
                    call assert_comparable( -0.687199853876083d0, alfa(1), margin, "Convex failure domain: Alfa(1)" )
                    call assert_comparable( -0.726468416954716d0, alfa(2), margin, "Convex failure domain: Alfa(2)" )

                case (methodDirectionalSampling)
                    call assert_comparable( 2.63d0, actualBeta, 0.05d0 * betaFactor, "Convex failure domain: Beta" )

                case (methodImportanceSampling)
                    call assert_comparable( 2.67d0, actualBeta, 1d-2, trim(waartsFunctionName) // ": Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 5402, "no samples")

                case default
                    call assert_true( conv, "No convergence" )
            end select

            call finalizeProbabilisticCalculation(probDb)

        case (7) ! Oblate spheroid

            ! Initialization of mechanism
            call initializeCalculation (probDb, 11, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            ! Initialize stochast data
            call initializeStochast (probDb, 1, distributionDeterministic, 10.0d0, 0.5D0, 0.0d0, 0.0d0)

            do i = 1, 10
                call initializeStochast (probDb, i + 1, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)
            end do

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            probDb%method%FORM%startMethod = fORMStartOne

            ! Perform computation numberIterations times
            call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
            call iterateMechanism (probDb, convergenceData, zOblateSpheroid, probMethod, alfa, actualBeta, x, conv)
            call updateCounter(invocationCount)
            call cleanUpWaartsTestsFunctions

            select case (probMethod)
                case ( methodFORM )
                    call assert_comparable( 3.30d0, actualBeta, 0.15d0 * betaFactor, "Oblate spheroid: Beta" )

                case ( methodNumericalIntegration )
                    call assert_comparable( 0.56649404_wp, actualBeta, margin, "Oblate spheroid: Beta" )
                    call assert_almost_zero( alfa(1), margin, "Oblate spheroid: Alfa(1)" )
                    call assert_comparable( 1.0d0, alfa(2), margin, "Oblate spheroid: Alfa(2)" )
                    call assert_almost_zero( alfa(3), margin, "Oblate spheroid: Alfa(3)" )
                    call assert_almost_zero( alfa(4), margin, "Oblate spheroid: Alfa(4)" )
                    call assert_almost_zero( alfa(5), margin, "Oblate spheroid: Alfa(5)" )
                    call assert_almost_zero( alfa(6), margin, "Oblate spheroid: Alfa(6)" )
                    call assert_almost_zero( alfa(7), margin, "Oblate spheroid: Alfa(7)" )
                    call assert_almost_zero( alfa(8), margin, "Oblate spheroid: Alfa(8)" )
                    call assert_almost_zero( alfa(9), margin, "Oblate spheroid: Alfa(9)" )
                    call assert_almost_zero( alfa(10), margin, "Oblate spheroid: Alfa(10)" )
                    call assert_almost_zero( alfa(11), margin, "Oblate spheroid: Alfa(11)" )

                case (methodImportanceSampling)
                    call assert_comparable( 1.09d0, actualBeta, 1d-2, trim(waartsFunctionName) // ": Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 42012, "no samples")

                case default
                    call assert_true( conv, "No convergence" )
            end select

            call finalizeProbabilisticCalculation(probDb)

        case (8) ! Saddle surface

            ! Initialization of mechanism
            call initializeCalculation (probDb, 2, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            ! Set the starting vector for FORM, otherwise we start in the saddle point itself
            probDb%method%FORM%startMethod = fORMStartOne

            ! Initialize stochast data
            call initializeStochast (probDb, 1, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 2, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Perform computation numberIterations times
            call iterateMechanism (probDb, convergenceData, zSaddleSurface, probMethod, alfa, actualBeta, x, conv)

            select case (probMethod)
                case (methodFORM)
                    ! We find the value 2.45 as in Waart's FORM NLPQL/RFLS, probably because of the different
                    ! start vector
                    call assert_comparable( 2.45d0, actualBeta, 0.05d0 * betaFactor, "Saddle surface: Beta" )

                case (methodNumericalIntegration)
                    call assert_comparable( 2.32764660158888d0, actualBeta, margin, "Saddle surface: Beta" )
                    call assert_comparable( 0.707106781186548d0, alfa(1), margin, "Saddle surface: Alfa(1)" )
                    call assert_comparable( 0.707106781186548d0, alfa(2), margin, "Saddle surface: Alfa(2)" )

                case (methodDirectionalSampling)
                    call assert_comparable( 2.34d0, actualBeta, 0.05d0 * betaFactor, "Saddle surface: Beta" )

                case (methodImportanceSampling)
                    call assert_comparable( 2.32d0, actualBeta, 1d-2, trim(waartsFunctionName) // ": Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 21892, "no samples")

                case default
                    call assert_true( conv, "No convergence" )
            end select

            call finalizeProbabilisticCalculation(probDb)

        case (9) ! Discontinuous limit state

            ! Initialization of mechanism
            call initializeCalculation (probDb, 2, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            ! Initialize stochast data
            call initializeStochast (probDb, 1, distributionNormal, 15.0d0, 2.5D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 2, distributionNormal, 5.0d0, 0.5D0, 0.0d0, 0.0d0)

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Perform computation numberIterations times
            call iterateMechanism (probDb, convergenceData, zDiscontinuousLimitState, probMethod, alfa, actualBeta, x, conv)

            ! Note:
            ! Our FORM routine fails (crashes), Waarts reports an error

            select case (probMethod)
                case (methodFORM, methodFORMandDirSampling)
                    call assert_false( conv, "Discontinuous limit state: expected non convergence" )

                case (methodNumericalIntegration)
                    call assert_comparable(  3.83249521d0, actualBeta, margin, "Discontinuous limit state: Beta" )
                    call assert_comparable(  0.975260429306580d0, alfa(1), margin, "Discontinuous limit state: Alfa(1)" )
                    call assert_comparable( -0.221059030642825d0, alfa(2), margin, "Discontinuous limit state: Alfa(2)" )

                case (methodDirectionalSampling)
                    call assert_comparable( 3.83d0, actualBeta, 0.10d0 * betaFactor, "Discontinuous limit state: Beta" )

                case (methodCrudeMonteCarlo)
                    call assert_comparable( 4.01281081d0, actualBeta, 1d-8, trim(waartsFunctionName) // ": Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 100000, "no samples")

                case (methodImportanceSampling)
                    call assert_comparable( 3.85d0, actualBeta, 1d-2, trim(waartsFunctionName) // ": Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 38017, "no samples")

                case default
                    call assert_true( conv, "No convergence" )
            end select

            call finalizeProbabilisticCalculation(probDb)

        case (10) ! Two branches

            ! Initialization of mechanism
            call initializeCalculation (probDb, 3, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            ! Initialize stochast data
            call initializeStochast (probDb, 1, distributionNormal, 10.0d0, 0.5D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 2, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 3, distributionNormal, 4.0d0, 1.0D0, 0.0d0, 0.0d0)

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            oldNumberIterations = numberIterations
            runOption = .true.

            if (probMethod == methodCrudeMonteCarlo) then  ! MonteCarlo and high beta expected => adapt number of samples

                probDb%method%CMC%maximumSamples = 10000000
                runOption = numberIterations > 1 ! Only execute this task in integration test

                numberIterations = 1
            end if

            if (runOption) then

                ! Perform computation numberIterations times
                call iterateMechanism (probDb, convergenceData, zTwoBranches, probMethod, alfa, actualBeta, x, conv)

                select case (probMethod)
                    case (methodFORM)
                        ! Waarts reports 2.21 for FORM RFLS, with error message
                        ! result depends on estimateBetaNonConv
                        call assert_comparable( 2.44948974d0, actualBeta, 0.10d0 * betaFactor, "Two branches: Beta" )

                    case (methodNumericalIntegration)
                        call assert_comparable( 5.03d0, actualBeta, 0.10d0 * betaFactor, "Two branches: Beta" )
                        call assert_comparable(  0.442037277331605d0, alfa(1), margin, "Two branches: Alfa(1)" )
                        call assert_comparable( -0.873293157655122d0, alfa(2), margin, "Two branches: Alfa(2)" )
                        call assert_comparable( -0.204846543153671d0, alfa(3), margin, "Two branches: Alfa(3)" )

                    case (methodDirectionalSampling, methodFORMandDirSampling, methodDirSamplingWithFORMiterations)
                        call assert_comparable( 5.03d0, actualBeta, 0.10d0 * betaFactor, "Two branches: Beta" )

                    case (methodCrudeMonteCarlo)
                        call assert_comparable( 4.935367445d0, actualBeta, 1d-8, trim(waartsFunctionName) // ": Beta" )
                        call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 10000000, "no samples")

                    case (methodImportanceSampling)
                        call assert_comparable( 5.020725937d0, actualBeta, 1d-8, trim(waartsFunctionName) // ": Beta" )
                        call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 100000, "no samples")

                    case default
                        call assert_true( conv, "No convergence" )
                    end select
            end if

            numberIterations = oldNumberIterations

            call finalizeProbabilisticCalculation(probDb)

        case (11) ! Concave failure domain

            ! Initialization of mechanism
            call initializeCalculation (probDb, 2, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            ! Initialize stochast data
            call initializeStochast (probDb, 1, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 2, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Set the starting vector for FORM, see note by Waarts
            ! Instead of (1,1) use a line search to start
            ! The methods (0,0) and (1,1) as start vector both give a value 3.0
            probDb%method%FORM%startMethod = fORMStartRaySearch
            probDb%method%FORM%startValue  = (/ 1.0, 0.0 /)

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Perform computation numberIterations times
            call iterateMechanism (probDb, convergenceData, zConcaveFailureDomain, probMethod, alfa, actualBeta, x, conv)

            select case (probMethod)
                case (methodFORM)
                    call assert_comparable( 1.66d0, actualBeta, 0.10d0 * betaFactor, "Concave failure domain: Beta" )

                case (methodNumericalIntegration)
                    call assert_comparable( 1.26d0, actualBeta, 0.10d0 * betaFactor, "Concave failure domain: Beta" )
                    call assert_comparable( 0.386726676d0, alfa(1), margin, "Concave failure domain: Alfa(1)" )
                    call assert_comparable( -0.92219438d0, alfa(2), margin, "Concave failure domain: Alfa(2)" )

                case (methodDirectionalSampling)
                    call assert_comparable( 1.26d0, actualBeta, 0.10d0 * betaFactor, "Concave failure domain: Beta" )

                case (methodImportanceSampling)
                    call assert_comparable( 1.28d0, actualBeta, 1d-2, trim(waartsFunctionName) // ": Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 1923, "no samples")

                case default
                    call assert_true( conv, "No convergence" )
            end select

            call finalizeProbabilisticCalculation(probDb)

        case (12) ! Series system

            ! Initialization of mechanism
            call initializeCalculation (probDb, 2, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            ! Set the starting vector for FORM
            probDb%method%FORM%startMethod = fORMStartOne

            ! Initialize stochast data
            call initializeStochast (probDb, 1, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)
            call initializeStochast (probDb, 2, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Perform computation numberIterations times
            call iterateMechanism (probDb, convergenceData, zSeriesSystem, probMethod, alfa, actualBeta, x, conv)

            select case (probMethod)
                case (methodFORM)
                    call assert_comparable( 3.00d0, actualBeta, 0.05d0 * betaFactor, "Series system: Beta" )

                case (methodNumericalIntegration)
                    call assert_comparable( 2.842646d0, actualBeta, margin, "Series system: Beta" )
                    ! function is symmetric in x; so sign of alpha can flip
                    call assert_comparable( 0.707106781186548d0, abs(alfa(1)), margin, "Series system: Alfa(1)" )
                    call assert_comparable( 0.707106781186548d0, abs(alfa(2)), margin, "Series system: Alfa(2)" )

                case (methodDirectionalSampling)
                    call assert_comparable( 2.9d0, actualBeta, 0.1d0, "Series system: Beta" )

                case (methodImportanceSampling)
                    call assert_comparable( 2.84d0, actualBeta, 1d-2, trim(waartsFunctionName) // ": Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 99217, "no samples")

                case default
                    call assert_true( conv, "No convergence" )
            end select

            call finalizeProbabilisticCalculation(probDb)

        case (13) ! Parallel system

            ! Initialization of mechanism
            call initializeCalculation (probDb, 5, alfa, x)
            probDb%method%DS%iterationMethod = methodDS

            ! Initialize stochast data
            do i = 1, 5
                call initializeStochast (probDb, i, distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)
            end do

            ! Initialize the number of integration intervals for numerical integration
            call initializeNumericalIntegration( probDb%method%NI%minimumUvalue, probDb%method%NI%maximumUvalue, &
                     probDb%method%NI%numberIntervals )

            ! Initialize random generator
            probDb%method%IS%seedPRNG  = 1
            probDb%method%DS%seedPRNG  = 1
            probDb%method%CMC%seedPRNG = 1

            ! Perform computation numberIterations times
            call iterateMechanism (probDb, convergenceData, zParallelSystem, probMethod, alfa, actualBeta, x, conv)

            select case (probMethod)
                case (methodFORM, methodFORMandDirSampling, methodDirSamplingWithFORMiterations)
                    ! Waarts shows an error entry for FORM in this case, we do get a value
                    call assert_false ( conv, "Parallel system: convergence flag" )

                case (methodNumericalIntegration)
                    call assert_comparable( 3.56514503038931d0, actualBeta, margin, "Parallel system: Beta" )
                    call assert_comparable( -0.409615960259520d0, alfa(1), margin, "Parallel system: Alfa(1)" )
                    call assert_comparable( -0.573462344363328d0, alfa(2), margin, "Parallel system: Alfa(2)" )
                    call assert_comparable( -0.409615960259520d0, alfa(3), margin, "Parallel system: Alfa(3)" )
                    call assert_comparable( -0.409615960259520d0, alfa(4), margin, "Parallel system: Alfa(4)" )
                    call assert_comparable( -0.409615960259520d0, alfa(5), margin, "Parallel system: Alfa(5)" )
                case (methodDirectionalSampling)
                    call assert_comparable( 3.52d0, actualBeta, 0.05d0 * betaFactor, "Parallel system: Beta" )
                case (methodCrudeMonteCarlo)
                    call assert_comparable( 3.503028640d0, actualBeta, 1d-8, trim(waartsFunctionName) // ": Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 100000, "no samples")
                case (methodImportanceSampling)
                    call assert_comparable( 3.46d0, actualBeta, 1d-2, trim(waartsFunctionName) // ": Beta" )
                    call assert_equal(convergenceData%cnvg_data_ds%numberSamples, 99573, "no samples")
                case default
                    call assert_true( conv, "No convergence" )
            end select

            call finalizeProbabilisticCalculation(probDb)

    end select

end subroutine testProbabilisticWithFunction


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
subroutine iterateMechanism (probDb, convergenceData, z_func, probMethod, alfa, combinedBeta, x, conv)
    type(probabilisticDataStructure_data)   :: probDb
    type(storedConvergenceData)             :: convergenceData
    procedure(zfunc)                        :: z_func
    integer,       intent(in)    :: probMethod
    real(kind=wp), intent(out)   :: alfa(:)
    real(kind=wp), intent(out)   :: combinedBeta
    real(kind=wp), intent(out)   :: x(:)
    logical,       intent(out)   :: conv

    integer                      :: i
    real(kind=wp)                :: beta
    logical                      :: convCriterium

    combinedBeta = 0.0_wp

    probDb%method%calcMethod = probMethod
    do i = 1, numberIterations
        call probCalc%run( probDb, z_func, alfa, beta, x, conv, convCriterium, convergenceData )

        combinedBeta = combinedBeta + beta / numberIterations
    end do

end subroutine iterateMechanism

!> Test if a deterministic parameter has indeed no influence
subroutine testDeterministicParameterHasNoInfluence

    type(probabilisticDataStructure_data)  :: probDb
    type(storedConvergenceData)            :: convergenceData
    logical                     :: conv
    real (kind = wp), pointer   :: alfa(:)
    real (kind = wp), pointer   :: x(:)
    real (kind = wp)            :: beta = 0.0d0
    real (kind = wp)            :: beta2= 0.0d0

    logical                     :: convCriterium

   ! Initialization of mechanism
   call initializeCalculation (probDb, 2, alfa, x)

   probDb%method%FORM%trialLoops = 10

   ! Initialize stochast data
   call initializeStochast ( probDb, 1, distributionDeterministic, 5.0d0, 0.0D0, 0.0d0, 0.0d0)
   call initializeStochast ( probDb, 2, distributionNormal       , 2.0d0, 1.0d0, 0.0d0, 0.0d0)

   ! Perform computation numberIterations times
   probDb%method%calcMethod = methodFORM
   call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
   call probCalc%run( probDb, zLinearResistanceSolicitation, alfa, beta, x, conv, convCriterium, convergenceData )
   call cleanUpWaartsTestsFunctions

   call finalizeProbabilisticCalculation(probDb)

   !! Now again but with deterministic value fixed in z function

   call initializeCalculation (probDb, 1, alfa, x)

   ! Initialize stochast data
   call initializeStochast ( probDb, 1, distributionNormal, 2.0d0, 1.0d0, 0.0d0, 0.0d0)

   ! Perform computation numberIterations times
   probDb%method%calcMethod = methodFORM
   call probCalc%run( probDb, zLinearResistanceSolicitationFixed, &
       alfa, beta2, x, conv, convCriterium, convergenceData )

   call assert_comparable( beta2, beta, 0.01d0, "Deterministic parameter has influence: different beta's" )
   call finalizeProbabilisticCalculation(probDb)

end subroutine testDeterministicParameterHasNoInfluence

!> Test if a deterministic parameter has indeed no influence
subroutine testErrorHandlingCalculateLimitStateFunction

    type(probabilisticDataStructure_data)  :: probDb
    type(storedConvergenceData)            :: convergenceData
    character(len=255)                     :: message

    integer                     :: ipos
    real (kind = wp), pointer   :: alfa(:)
    real (kind = wp), pointer   :: x(:)
    real (kind = wp)            :: beta = 0.0d0
    real (kind = wp)            :: beta2= 0.0d0

    logical                     :: convCriterium, conv
    type(tProbCalc)             :: probCalc            !< class prob. calculation

   ! Initialization of mechanism with no stochastic parameters
   call initializeCalculation (probDb, 0, alfa, x)

   probDb%method%FORM%trialLoops = 10

   ! Initialize stochast data
   !call initializeStochast ( probDb, 1, distributionDeterministic, 5.0d0, 0.0D0, 0.0d0, 0.0d0)
   !call initializeStochast ( probDb, 2, distributionNormal       , 2.0d0, 1.0d0, 0.0d0, 0.0d0)

   ! Perform computation numberIterations times
   call SetFatalErrorExpected(.true.)
   probDb%method%calcMethod = methodFORM
   call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
   call probCalc%run( probDb, zLinearResistanceSolicitation, alfa, beta, x, conv, convCriterium, convergenceData )
   call cleanUpWaartsTestsFunctions

   call SetFatalErrorExpected(.false.)
   call GetFatalErrorMessage(message)
   call assert_equal(message, 'Fatal error: No stochastic parameters found', '1st message from calculateLimitStateFunction')

   call finalizeProbabilisticCalculation(probDb)

   !! Now again but with deterministic value fixed in z function
   !! But with an unknown method

   call initializeCalculation (probDb, 1, alfa, x)

   ! Initialize stochast data
   call initializeStochast ( probDb, 1, distributionNormal, 2.0d0, 1.0d0, 0.0d0, 0.0d0)

   ! Perform computation numberIterations times
   call SetFatalErrorExpected(.true.)
   probDb%method%calcMethod = 99
   call probCalc%run( probDb, zLinearResistanceSolicitationFixed, alfa, beta2, x, conv, convCriterium, convergenceData )
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
