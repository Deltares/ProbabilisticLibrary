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

!> @file initProbabilisticTests.f90
!! Test for the initialisation functions for probabilistic computations
!<
!
module initProbabilisticTests

    use precision
    use ftnunit
    use equalReals
    use interface_distributions
    use interface_probcalc
    use interface_probcalcdata
    use probMethodsWaartsFunctionsTests

    implicit none

    private

    public :: allInitProbabilisticTests

contains

!> Call all the InitProbabilistic tests
!!
subroutine allInitProbabilisticTests
!
!   Entry to test CheckIfInitDeterministicValueReturnsCorrectVector()
!   Purpose: Check that subroutine initDeterministicValue() returns correct values for the mean of deterministic distributions.
    call testWithLevel( CheckIfInitDeterministicValueReturnsCorrectVector, &
        "CheckIfInitDeterministicValueReturnsCorrectVector from module initProbabilisticTests", 1)
!
!   Entry to test testInitDeterministicValueWith4Determinists()
!   Purpose: Test of subroutine initDeterministicValue with 4 deterministic parameters
    call testWithLevel( testInitDeterministicValueWith4Determinists, &
        "testInitDeterministicValueWith4Determinists from module initProbabilisticTests", 1)
!
!
!   Entry to test testInitPointerToStochastWith4Stochasts()
!   Purpose: Test of subroutine initPointerToStochast() with 4 stochastic variables
    call testWithLevel( testInitPointerToStochastWith4Stochasts, &
        "testInitPointerToStochastWith4Stochasts from module initProbabilisticTests", 1)
!
!   Entry to other tests of subroutine InitProbabilistic within [F]probabilistic
!   In the event still to develop.
!   call test( ....)
!   etc.
!
end subroutine allInitProbabilisticTests
!
!> Test if the return vector of initDeterministicValue() contains the correct mean values of the deterministic distributions
!!
subroutine CheckIfInitDeterministicValueReturnsCorrectVector
!   Function: Test of subroutine initDeterministicValue() for producing correct values for the mean of deterministic distributions.
    type( probabilisticDataStructure_data)  :: probDb
    integer, parameter                      :: nStochasts= 5           ! Number of stochastic variables in present test
    integer                                 :: iStochasts
    real( wp ), parameter                   :: margin= 1.0d-7          ! Tolerance parameter
    real( wp )                              :: xActual( nStochasts )   ! Outcome of initDeterministicValue() for the emans
    real( wp )                              :: xExpected( nStochasts ) ! Expected means that must be (re)produced
    character( len = 132)                   :: errorText               ! Diagnostic message in case of test failure

!   Initialisation of number stochastic variables in the present test
    probDb%stoVar%maxStochasts= nStochasts

!   Initialisation of the expected means of the determinisic distributions (rather arbitrary values)
    xExpected( 1 : nStochasts)      = (/ 5.0d+0, -4.0d+0, 0.0d+0, -2.0d+0, 1.0d+0 /)
!
!   Initialisation of the means to be produced by initDeterministicValue() (to prevent status undefined)
    xActual        = 2.5d+0   ! For all entries the same value; must agree with the expected values on exit of initDeterministicValue()

!   Allocate the presently required entities for the type and distribution parameters of the stochastic variables
    allocate (probDb%stoVar%distTypeX(nStochasts) )
    allocate (probDb%stoVar%distParameterX(nStochasts, 4) )

!   Initialise the presently required entities for stochasts
    probDb%stoVar%distTypeX( 1 : nStochasts)          = distributionDeterministic    ! All stochastic variables are set deterministic, except:
    probDb%stoVar%distTypeX( 2 )                      = distributionShiftedExponential
    probDb%stoVar%distParameterX( 1 : nStochasts, 1 ) = xExpected(1 : nStochasts)    ! Initialisation of the means

!   Get the deterministic values according to subroutine initDeterministicValue()
    call initDeterministicValue(probDb, xActual)

!   Verify the result: compare the outcome of initDeterministicValue() to what is expected
    Loop_CheckDetermisticValue: do iStochasts= 1, nStochasts

       if (probDb%stoVar%distTypeX(iStochasts) /= distributionDeterministic) Cycle Loop_CheckDetermisticValue

!      Error/diagnostic message in case of too large relative difference for present entry iStochasts
       write ( ErrorText, '(a, 1Pg15.7, a, i0, a, g15.7, a)')                                                       &
              'On exit value ', xActual( iStochasts)  , ' is found for deterministic variable Nr. ', iStochasts,    &
              ', while'       , xExpected( iStochasts), ' was expected'

!      Verify whether (relative) difference of xActual() and xExpected is within a tolerance determined by margin
       call assert_comparable( xActual( iStochasts), xExpected( iStochasts), margin, ErrorText )

    enddo  Loop_CheckDetermisticValue

!   Erase temporary workspace
    deallocate (probDb%stoVar%distTypeX)
    deallocate (probDb%stoVar%distParameterX)

end subroutine CheckIfInitDeterministicValueReturnsCorrectVector

!> Test initDeterministicValue with 4 deterministic parameters
!!
subroutine testInitDeterministicValueWith4Determinists

    integer, parameter          :: numberStochasts = 6
    real (kind = wp), pointer   :: alfa(:)
    real (kind = wp), pointer   :: x(:)
    type(probabilisticDataStructure_data)          :: probDb
    real (kind = wp)            :: margin = 1.0d-12
    real (kind = wp)            :: mu(numberStochasts)
    real (kind = wp)            :: sigma(numberStochasts)
    integer                     :: distribution(numberStochasts)
    integer                     :: i
!
!   1. First stochastic parameter is deterministic with expected value 3
!   2. Second stochastic parameter is stochastic according to the normal distribution
!      with expected value 1 and standard deviation 1.5
!   3. Third stochastic parameter is deterministic with expected value -2
!   4. Fourth stochastic parameter is deterministic with expected value 1.2
!   5. Fifth stochastic parameter is stochastic according to the normal distribution
!      with expected value -3.2 and standard deviation 0.8
!   6. Sixth stochastic parameter is deterministic with expected value 10
!
    data mu    / 3.0d0, 1.0d0, -2.0d0, 1.2d0, -3.2d0, 10.0d0 /
    data sigma / 0.0d0, 1.5d0,  0.0d0, 0.0d0,  0.8d0,  0.0d0 /

    do i = 1, numberStochasts
        if (equalRealsRelative (sigma(i), 0.0d0, margin)) then
            distribution(i) = 0
        else
            distribution(i) = 2 ! normal distribution
        endif
    enddo
!
!   Initialize structure
!
    call initializeCalculation (probDb, numberStochasts, alfa, x)
    do i = 1, numberStochasts
        call initializeStochast (probDb, i, distribution(i), mu(i), sigma(i), 0.0d0, 0.0d0)
    enddo

    call initDeterministicValue ( probDb, x )

    call assert_comparable( x(1), mu(1), margin, "unexpected return value mu(1)" )
    call assert_comparable( probDb%stoVar%distParameterX(2,1), mu(2), margin, "unexpected return value mu(2)" )
    call assert_comparable( probDb%stoVar%distParameterX(2,2), sigma(2), margin, "unexpected return value sigma(2)" )
    call assert_comparable( x(3), mu(3), margin, "unexpected return value mu(3)" )
    call assert_comparable( x(4), mu(4), margin, "unexpected return value mu(4)" )
    call assert_comparable( probDb%stoVar%distParameterX(5,1), mu(5), margin, "unexpected return value mu(5)" )
    call assert_comparable( probDb%stoVar%distParameterX(5,2), sigma(5), margin, "unexpected return value sigma(5)" )
    call assert_comparable( x(6), mu(6), margin, "unexpected return value mu(6)" )

    deallocate (alfa, x)

end subroutine testInitDeterministicValueWith4Determinists
!
!> Test initPointerToStochast with 4 stochastic parameters
!!
subroutine testInitPointerToStochastWith4Stochasts

    integer, parameter          :: numberStochasts = 6
    real (kind = wp), pointer   :: alfa(:)
    real (kind = wp), pointer   :: x(:)
    type(probabilisticDataStructure_data)          :: probDb
    real (kind = wp)            :: margin = 1.0d-12
    real (kind = wp)            :: mu(numberStochasts)
    real (kind = wp)            :: sigma(numberStochasts)
    integer                     :: distribution(numberStochasts)
    integer                     :: ipoint(numberStochasts)       ! Pointer to stochastic variables
    integer                     :: i
    integer                     :: nstoch = 0
!
!   1. First stochastic parameter is stochastic according to the normal distribution
!      with expected value 1 and standard deviation 1.5
!   2. Second stochastic parameter is deterministic with expected value 3
!   3. Third stochastic parameter is stochastic according to the normal distribution
!      with expected value -2 and standard deviation 2.2
!   4. Fourth stochastic parameter is stochastic according to the normal distribution
!      with expected value 1.2 and standard deviation 0.05
!   5. Fifth stochastic parameter is deterministic with expected value 10
!   6. Sixth stochastic parameter is stochastic according to the normal distribution
!      with expected value -3.2 and standard deviation 0.8
!
    data mu    / 1.0d0, 3.0d0, -2.0d0, 1.20d0, 10.0d0, -3.2d0 /
    data sigma / 1.5d0, 0.0d0,  2.2d0, 0.05d0,  0.0d0,  0.8d0 /

    do i = 1, numberStochasts
        if (equalRealsRelative (sigma(i), 0.0d0, margin)) then
            distribution(i) = 0
        else
            nstoch = nstoch + 1
            distribution(i) = 2 ! normal distribution
        endif
    enddo
!
!   Initialize structure
!
    call initializeCalculation (probDb, numberStochasts, alfa, x)
    do i = 1, numberStochasts
        call initializeStochast (probDb, i, distribution(i), mu(i), sigma(i), 0.0d0, 0.0d0)
    enddo
    call initDeterministicValue ( probDb, x )

    call initPointerToStochast( probDb, nstoch, ipoint )

    call assert_comparable( probDb%stoVar%distParameterX(1,1), mu(1), margin, "unexpected return value mu(1)" )
    call assert_comparable( probDb%stoVar%distParameterX(1,2), sigma(1), margin, "unexpected return value sigma(1)" )
    call assert_comparable( x(2), mu(2), margin, "unexpected return value mu(2)" )
    call assert_comparable( probDb%stoVar%distParameterX(3,1), mu(3), margin, "unexpected return value mu(3)" )
    call assert_comparable( probDb%stoVar%distParameterX(3,2), sigma(3), margin, "unexpected return value sigma(3)" )
    call assert_comparable( probDb%stoVar%distParameterX(4,1), mu(4), margin, "unexpected return value mu(4)" )
    call assert_comparable( probDb%stoVar%distParameterX(4,2), sigma(4), margin, "unexpected return value sigma(4)" )
    call assert_comparable( x(5), mu(5), margin, "unexpected return value mu(5)" )
    call assert_comparable( probDb%stoVar%distParameterX(6,1), mu(6), margin, "unexpected return value mu(6)" )
    call assert_comparable( probDb%stoVar%distParameterX(6,2), sigma(6), margin, "unexpected return value sigma(6)" )
    call assert_equal (ipoint(1), 1, "unexpected return value ipoint(1)" )
    call assert_equal (ipoint(2), 3, "unexpected return value ipoint(2)" )
    call assert_equal (ipoint(3), 4, "unexpected return value ipoint(3)" )
    call assert_equal (ipoint(4), 6, "unexpected return value ipoint(4)" )
    call assert_equal (ipoint(5), 0, "unexpected return value ipoint(5)" )
    call assert_equal (ipoint(6), 0, "unexpected return value ipoint(6)" )

    deallocate (alfa, x)

end subroutine testInitPointerToStochastWith4Stochasts

end module initProbabilisticTests
