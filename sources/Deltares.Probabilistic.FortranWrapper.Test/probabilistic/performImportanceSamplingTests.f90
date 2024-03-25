! Copyright (C) Stichting Deltares 2023. All rights reserved.
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

module performImportanceSamplingTests

    use ftnunit
    use precision
    use interface_probcalc
    use interface_probcalcdata
    use interface_distributions
    use sampleFunctions

    implicit none

    private

    public :: allPerformImportanceSamplingTests

    real (kind = wp), parameter :: betaKnown = 2.82842712474619d0
    real (kind = wp), parameter :: margin = 0.1d0

contains
!> This subroutine contains the calls for all the tests for the probablistic computation techniques.

subroutine allPerformImportanceSamplingTests

    call testWithLevel( importanceSamplingTest1, "Basic Importance sampling test", 1)
    call testWithLevel( importanceSamplingTest2, "Test effect of variance in importance sampling routine", 3)

end subroutine allPerformImportanceSamplingTests

!> Test for importance sampling.
!> A simple Z function is called in which the failure probability can be analytically computed.
!! The returned beta value is then compared with the (in Matlab-computed) known beta. The test passes if the difference in beta
!! is less than 0.1

subroutine importanceSamplingTest1

    !
    ! Test performImportanceSampling
    !

    implicit none

    type(probabilisticDataStructure_data) :: probDb
    real (kind = wp), pointer             :: alfa(:)
    real (kind = wp), pointer             :: x(:)
    integer, pointer                      :: iPoint(:)
    real (kind = wp)                      :: beta
    logical                               :: convCriterium
    integer                               :: nStochasts

    allocate( alfa(2), x(3), iPoint(2) )
    nStochasts = 2
    iPoint     = (/ 1, 2 /)
    call initProbabilisticCalculation ( probDb, 3, .false., .false. )

    ! Initialize stochast data
    call setStochasticDistrib( probDb, 1, distributionNormal       , 0.0d0, 1.0D0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 2, distributionNormal       , 0.0d0, 1.0d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 3, distributionDeterministic, 4.0d0, 0.0d0, 0.0d0, 0.0d0)

    ! Initialise deterministic parameters
    call initDeterministicValue( probDb, x )

    probDb%method%calcMethod = methodImportanceSampling

    ! Perform computation to determine alpha and beta
    call performImportanceSampling( probDb, simpleZ, nStochasts, iPoint, x, alfa, beta, convCriterium )
    write(*,*) 'Beta computed', beta
    write(*,*) 'Beta known', betaKnown
    write(*,*) 'Alpha: ', alfa

    call assert_comparable(beta, betaKnown, margin, "The computed beta deviates from the analytically computed value")
    call finalizeProbabilisticCalculation(probDb)

end subroutine importanceSamplingTest1

subroutine importanceSamplingTest2

    !
    ! Test effect variance factor in performImportanceSampling
    !

    implicit none

    type(probabilisticDataStructure_data) :: probDb
    real (kind = wp), pointer             :: alfa(:)
    real (kind = wp), pointer             :: x(:)
    integer, pointer                      :: iPoint(:)
    real (kind = wp)                      :: beta
    logical                               :: convCriterium
    integer                               :: nStochasts
    integer                               :: k
    real(kind=wp)                         :: variance

    allocate( alfa(2), x(3), iPoint(2) )
    nStochasts = 2
    iPoint     = (/ 1, 2 /)
    call initProbabilisticCalculation ( probDb, 3, .false., .false. )

    ! Initialize stochast data
    call setStochasticDistrib( probDb, 1, distributionNormal       , 0.0d0, 1.0D0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 2, distributionNormal       , 0.0d0, 1.0d0, 0.0d0, 0.0d0)
    call setStochasticDistrib( probDb, 3, distributionDeterministic, 4.0d0, 0.0d0, 0.0d0, 0.0d0)

    ! Initialise deterministic parameters
    call initDeterministicValue( probDb, x )

    probdb%method%is%minimumsamples = probdb%method%is%maximumsamples

    ! Perform computation to determine alpha and beta
    do k = 0, 10
        variance = 1.0_wp + 0.25_wp * real(k, wp)
        probDb%method%is%variancefactor = variance
        probDb%method%calcMethod = methodImportanceSampling
        call performImportanceSampling( probDb, simpleZ, nStochasts, iPoint, x, alfa, beta, convCriterium )
        call assert_comparable(beta, betaKnown, margin, "The computed beta deviates from the analytically computed value")
        write(*,'(a,f4.2,2(x,f10.8))') 'variance, betas = ', variance, beta
    end do

    call finalizeProbabilisticCalculation(probDb)

end subroutine importanceSamplingTest2


subroutine performImportanceSampling( probDb, fx, nStochasts, iPoint, x, alfa, beta, convCriterium )
    type(probabilisticDataStructure_data)      :: probDb           !< Probabilistic data module
    procedure(zfunc)                           :: fx               !< Function implementing the z-function of the failure mechanism
    real(kind=wp), intent(out)                 :: alfa(:)          !< Alpha values
    real(kind=wp), intent(out)                 :: beta             !< Reliability index
    real(kind=wp), intent(inout)               :: x(:)             !< X values of design point
    logical,       intent(out)                 :: convCriterium    !< Convergence criterium indicator
    integer,       intent(in)                  :: iPoint(*)        !< Pointer to stochastic variables
    integer,       intent(in)                  :: nStochasts       !< number of active stochasts

    type(storedConvergenceData) :: convergenceData  !< struct holding all convergence data
    logical :: conv

    call calculateLimitStateFunction( probDb, fx, alfa, beta, x, conv, convCriterium, convergenceData)
end subroutine performImportanceSampling

end module performImportanceSamplingTests
