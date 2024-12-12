! Copyright (C) Stichting Deltares. All rights reserved.
!
! This file is part of the Probabilistic Library.
!
! The Probabilistic Library is free software: you can redistribute it and/or modify
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
!! module for holding call back functions used by ftnunit
!<
!
module initTestSubs

    use ftnunit
    use feedback
    use feedback_parameters

    implicit none

    private

    public :: initTestProgram, initTest, prepareTests

contains

!> Routine to be called when tests are started
subroutine initTestProgram
    ! Set action for feedback when a fatal error occurs
    call SetFatalerrorAction ( onfatalerrorUnittest )
end subroutine initTestProgram

!> Routine to be called when single tests are started
subroutine initTest
    ! reset feedback flags
    call SetFatalErrorExpected ( .false.)
    call ResetFatalErrorOccured()
end subroutine initTest

!> Routine to start the testing
!! Note: This routine merely takes care that the unit tests are indeed run
subroutine prepareTests

    integer  :: lun   !< LU-number

    ! Initialization called from ftnunit before running tests
    subptrTestProgramInit => initTestProgram

    ! Initialization called from ftnunit before running each separate test
    subptrTestInit => initTest

    open( newunit = lun, file = 'ftnunit.run' )
    write( lun, '(a)' ) 'ALL'
    close( lun )

end subroutine prepareTests

end module initTestSubs
