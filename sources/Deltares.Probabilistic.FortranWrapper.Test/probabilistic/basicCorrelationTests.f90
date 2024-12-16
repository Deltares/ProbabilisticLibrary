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

module basicCorrelationTests

    use ftnunit
    use precision
    use interface_probcalc
    use class_probcalc

    implicit none

    private

    public :: allBasicCorrelationTests

contains

!> This subroutine contains the calls for all the tests for the basic correlation routines
!> Note that no specific clean-up is required, as there are no pointers that get allocated
subroutine allBasicCorrelationTests
    integer, parameter :: level = 1

    call testWithLevel( registerOneCorrelationTest, "Test registering a single correlation", level)
    call testWithLevel( registerOutofRangeCorrelationTest, "Test registering a correlation coefficient out of range", level)
    call testWithLevel( registerManyCorrelationsTest, "Test registering many correlation pairs, forcing a reallocation", level)
    call testWithLevel( testFormWithCorrelation, "test Form with correlation", level)

end subroutine allBasicCorrelationTests

!> Test for registering a single correlation:
!> Simply check that the administration is behaving as required
subroutine registerOneCorrelationTest

    type(probabilisticDataStructure_data) :: probDb

    integer                               :: idx1, idx2
    real (kind = wp)                      :: correlation

    !
    ! This should be refused - hence nothing happens
    !
    correlation = 0.5

    idx1 = 1
    idx2 = 1
    call registerCorrelation( probDb, idx1, idx2, correlation )

    call assert_false( probDb%number_correlations > 0, "Using correlation must be false" )
    call assert_false( allocated(probDb%basic_correlation), "Array with correlation information must not be allocated" )

    !
    ! Register a single, proper correlation pair with a proper value
    !
    idx1 = 1
    idx2 = 2
    call registerCorrelation( probDb, idx1, idx2, correlation )

    call assert_true( probDb%number_correlations > 0, "Using correlation must be true" )
    call assert_true( allocated(probDb%basic_correlation), "Array with correlation information must be allocated" )
    if ( allocated(probDb%basic_correlation) ) then
        call assert_equal( probDb%number_correlations, 1, "Array with correlation information must have one used entry" )
        call assert_comparable( probDb%basic_correlation(1)%correlation, 0.5_wp, 1.0e-6_wp, &
            "The stored correlation coefficient must be 0.5")
    endif
end subroutine registerOneCorrelationTest

!> Test for registering a correlation value that is out of range:
!> Check that the stored value is clipped between -1 and 1
subroutine registerOutofRangeCorrelationTest

    type(probabilisticDataStructure_data) :: probDb

    integer                               :: idx1, idx2
    real (kind = wp)                      :: correlation

    !
    ! The correlation value is out of range - expecting corrections (both ways)
    !
    correlation = 2.5

    idx1 = 1
    idx2 = 2
    call registerCorrelation( probDb, idx1, idx2, correlation )

    call assert_equal( probDb%number_correlations, 1 , "number_correlations must be 1" )
    call assert_true( allocated(probDb%basic_correlation), "Array with correlation information must be allocated" )
    if ( allocated(probDb%basic_correlation) ) then
        call assert_equal( probDb%number_correlations, 1, "Array with correlation information must have one used entry" )
        call assert_comparable( probDb%basic_correlation(1)%correlation, 1.0_wp, 1.0e-6_wp, &
            "The stored correlation coefficient must be 1.0")
    endif

    correlation = -2.5

    idx1 = 3
    idx2 = 4
    call registerCorrelation( probDb, idx1, idx2, correlation )

    call assert_equal( probDb%number_correlations, 2, "number_correlations must be 2" )
    call assert_true( allocated(probDb%basic_correlation), "Array with correlation information must be allocated" )
    if ( allocated(probDb%basic_correlation) ) then
        call assert_equal( probDb%number_correlations, 2, "Array with correlation information must have two used entries" )
        call assert_comparable( probDb%basic_correlation(2)%correlation, -1.0_wp, 1.0e-6_wp, &
            "The stored correlation coefficient must be -1.0")
    endif
end subroutine registerOutofRangeCorrelationTest

!> Test for registering many correlations - see that the reallocation is correct
subroutine registerManyCorrelationsTest

    type(probabilisticDataStructure_data) :: probDb

    integer                               :: i, idx1, idx2, cnt
    real (kind = wp)                      :: correlation

    correlation = 0.5

    cnt = 0
    do idx1 = 1,20
        do idx2 = 1,idx1-1
            call registerCorrelation( probDb, idx1, idx2, correlation )
            cnt = cnt + 1
        enddo
    enddo

    call assert_equal( probDb%number_correlations, cnt, "Array with correlation information must have enough entries" )

    do i = 1,cnt
        call assert_comparable( probDb%basic_correlation(i)%correlation, correlation, 1.0e-6_wp, &
            "The stored correlation coefficient must be 0.5")
    enddo
end subroutine registerManyCorrelationsTest

subroutine testFormWithCorrelation
    integer, parameter                     :: nStochasts = 3
    integer, dimension(nStochasts)         :: iPoint = (/ 1, 2, 3 /)
    real(kind=wp), dimension(nStochasts), target :: alfaN
    real(kind=wp), pointer                 :: x(:)
    type(storedConvergenceData)            :: convergenceData  !< struct holding all convergence data
    type(probabilisticDataStructure_data)  :: probDb
    type(tProbCalc)                        :: probCalc            !< class prob. calculation
    type(designPoint)                      :: dp

    call init_probdb_x(probDb, x, iPoint, nStochasts)
    probDb%stovar%maxstochasts = nStochasts

    probDb%stovar%activex = 1
    probDb%stoVar%distTypeX = distributionNormal
    probDb%stoVar%distParameterX = 0.0_wp
    probDb%stoVar%distParameterX(:,2) = 1.0_wp
    probDb%method%calcmethod = methodFORM
    probDb%method%form%startmethod = fORMStartRaySearch
    probDb%method%form%maxIterations = 50

    call registerCorrelation( probDb, 1, 2, 0.1_wp )
    call registerCorrelation( probDb, 2, 3, 0.1_wp )

    dp%alpha => alfaN
    dp%x     => x
    call probCalc%run( probDb, zFuncSimpleA, dp, convergenceData )

    call assert_comparable( dp%beta, 0.66270_wp, 1.0e-5_wp, "diff in beta")

end subroutine testFormWithCorrelation

function zFuncSimpleA( u, compSetting, ierr ) result(z) bind(c)
    real(kind=wp),            intent(inout) :: u(*)
    real(kind=wp)                           :: z
    type(computationSetting), intent(in   ) :: compSetting  !< struct with computation settings
    type(tError),             intent(inout) :: ierr

    ierr%iCode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = 0.5 - sum(u(1:3)**2)

end function zFuncSimpleA

end module basicCorrelationTests
