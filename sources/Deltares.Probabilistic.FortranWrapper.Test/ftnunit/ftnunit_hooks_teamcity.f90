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

! ftnunit_hooks_teamcity.f90 --
!     Auxiliary module that provides user-defineable methods
!
!     Purpose:
!     Interface with TeamCity's automatic build system
!
module ftnunit_hooks
    implicit none

    private :: remove_single_quotes

contains

! ftnunit_hook_test_start --
!     Called when the test starts
!
! Arguments:
!     text            Description of the test
!
subroutine ftnunit_hook_test_start( text )

    character(len=*) :: text

    write( *, '(3a)' ) '##teamcity[testStarted name=''',trim(text),''']'

end subroutine ftnunit_hook_test_start

! ftnunit_hook_test_stop --
!     Called when the test stops (successful or not)
!
! Arguments:
!     text            Description of the test
!
subroutine ftnunit_hook_test_stop( text )

    character(len=*) :: text

    write( *, '(3a)' ) '##teamcity[testFinished name=''',trim(text),''']'

end subroutine ftnunit_hook_test_stop

!> replace single quotes by double quotes
function remove_single_quotes(textin) result (textout)
    character(len=*), intent(in) :: textin  !< input string
    character(len=len(textin))   :: textout !< function result

    integer :: i

    textout = textin
    do i = 1, len(textout)
        if (textout(i:i) == "'") then
            textout(i:i) = '"'
        end if
    end do
end function remove_single_quotes

! ftnunit_hook_test_assertion_failed --
!     Called when an asserion failed
!
! Arguments:
!     text            Description of the test
!     assert_text     Description of the assertion
!     failure_text    Details of the failure
!
subroutine ftnunit_hook_test_assertion_failed( text, assert_text, failure_text )

    character(len=*), intent(in) :: text
    character(len=*), intent(in) :: assert_text
    character(len=*), intent(in) :: failure_text

    write(*,'(7a)') '##teamcity[testFailed name=''', trim(text),  &
    ''' message=''', remove_single_quotes(trim(assert_text)),     &
    ''' details=''', remove_single_quotes(trim(failure_text)), ''']'

end subroutine ftnunit_hook_test_assertion_failed

! ftnunit_hook_test_completed --
!     Called when the tests are completed
!
! Arguments:
!     None
!
subroutine ftnunit_hook_test_completed

#ifdef _WIN32
    call system ( 'ftnunit.html' )
#endif
end subroutine ftnunit_hook_test_completed

end module ftnunit_hooks
