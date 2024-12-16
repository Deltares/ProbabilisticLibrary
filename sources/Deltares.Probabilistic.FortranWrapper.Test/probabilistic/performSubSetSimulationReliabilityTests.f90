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
module performSubSetSimulationReliabilityTests
    use ftnunit
    use precision
    use interface_probcalc
    use interface_probcalcdata
    use interface_distributions
    use sparseWaartsTestFunctions

    implicit none

    private

    public :: allPerformSubSetSimulationReliabilityTests

    real (kind = wp), parameter :: margin = 0.001d0

contains

!> This subroutine contains the calls for all the tests for the SubSetSimulation reliability tests.

subroutine allPerformSubSetSimulationReliabilityTests

    call testWithLevel( MarkovChainTest, "Basic SubSetSimulation Reliability test: MarkovChain", 1)
    call testWithLevel( AdaptiveConditionalTest, "Basic SubSetSimulation Reliability test: AdaptiveConditional", 1)

end subroutine allPerformSubSetSimulationReliabilityTests

subroutine runSubSetTest(SampleMethod, expectedBeta)
    implicit none
    integer,                    intent(in) :: SampleMethod
    real(kind=wp),              intent(in) :: expectedBeta

    type(probabilisticDataStructure_data) :: probDb
    type(storedConvergenceData)           :: convergenceData  !< struct holding all convergence data
    integer, pointer                      :: iPoint(:)
    logical                               :: convCriterium
    integer                               :: nStochasts
    type(designPoint)                     :: dp

    allocate( dp%alpha(2), dp%x(3), iPoint(2) )
    nStochasts = 2
    iPoint     = (/ 1, 2 /)
    call initProbabilisticCalculation ( probDb, 3, .false., .false. )

    ! Initialize stochast data
    call setStochasticDistrib( probDb, 1, distributionNormal       , 0.0d0, 1.0D0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 2, distributionNormal       , 0.0d0, 1.0d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 3, distributionDeterministic, 4.0d0, 0.0d0, 0.0d0, 0.0d0)

    ! Initialise deterministic parameters
    call initDeterministicValue( probDb, dp%x )

    probDb%method%calcMethod = methodSubSetSimulationReliability
    probDb%method%SubSetSimulationReliability%MarkovChainDeviation = 0.5_wp
    probDb%method%SubSetSimulationReliability%VariationCoefficient = 0.02_wp
    probDb%method%SubSetSimulationReliability%SampleMethod = SampleMethod

    ! Perform computation to determine alpha and beta
    call initSparseWaartsTestsFunctions(probDb%stovar%maxStochasts, probDb%method%maxParallelThreads)
    call probCalc%run( probDb, simpleZ, dp, convergenceData)
    convCriterium = convergenceData%convCriterium

    call cleanUpWaartsTestsFunctions

    call assert_comparable(dp%beta, expectedBeta, margin, "The computed beta deviates from the analytically computed value")
    call finalizeProbabilisticCalculation(probDb)

    deallocate(dp%alpha, dp%x, iPoint)

end subroutine runSubSetTest

subroutine MarkovChainTest
    implicit none

    call runSubSetTest(MarkovChain, 2.757_wp)

end subroutine MarkovChainTest

subroutine AdaptiveConditionalTest

    implicit none

    call runSubSetTest(AdaptiveConditional, 2.8388_wp)

end subroutine AdaptiveConditionalTest


end module performSubSetSimulationReliabilityTests
