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
!! This file contains a module with a struct to hold error and validation messages
!! and a switch between dutch and english messages
!<
!
!>
!! Module with a struct for error and validation messages and a switch between dutch and english messages
module errorMessages
implicit none

character( len= 2 ), private :: language = 'NL'         ! default language is dutch

integer, parameter :: maxMessages = 100
integer, parameter :: StrLenMessages = 256

integer, parameter :: severityUndefined = -1
integer, parameter :: severityError = 1
integer, parameter :: severityWarning = 2

!> struct for a single error/validation message
type tMessage
    integer                       :: errorCode =  0 !< Error code as defined in technical design
    integer                       :: severity  = -1 !< One of severityError, severityWarning
    character(len=StrLenMessages) :: message   = "" !< Actual error message or validation message
end type tMessage

!> struct for all messages
type TErrorMessages
    integer        :: nErrors                  !< Number of errors
    integer        :: nWarnings                !< Number of warnings
    logical        :: tooSmall                 !< Indicator for reaching the maximum size of messages
    type(tMessage) :: messages(maxMessages)    !< list of messages including errorCode and severity
end type TErrorMessages

type(tMessage), parameter :: errorNone = tMessage(0, -1, "")

contains

!> subroutine to clear the struct with all error messages
subroutine clearErrorMessages(errors)
type (TErrorMessages), intent(inout) :: errors

call initErrorMessages(errors)

end subroutine clearErrorMessages

!> subroutine to initialize the struct with all error messages
subroutine initErrorMessages(errors)
type (TErrorMessages), intent(inout) :: errors

integer :: i

errors%nErrors = 0
errors%nWarnings = 0
errors%tooSmall = .false.
do i = 1, maxMessages
    errors%messages(i)%message = ' '
    errors%messages(i)%severity = severityUndefined
    errors%messages(i)%errorCode = 0
enddo
end subroutine initErrorMessages

!> subroutine to add a message to the struct with all messages
subroutine addMessage(errors, message)
type (TErrorMessages), intent(inout) :: errors
type(tMessage), intent(in)           :: message
!
! locals
!
integer  :: nextMessage

nextMessage = errors%nErrors + errors%nWarnings + 1
if (nextMessage <= maxMessages) then
    errors%messages(nextMessage) = message
    if (message%severity == severityWarning) then
        errors%nWarnings = errors%nWarnings + 1
    else
        errors%nErrors = errors%nErrors + 1
    endif
else
    errors%tooSmall = .true.
endif
end subroutine addMessage

!>
!! Subroutine that sets the language for rangeCheck messages
!!    only strings 'NL' and 'UK' are recoqnized (lower and upper case)
!!
!! @ingroup tools
subroutine SetLanguage(lang)
use utilities, only : to_upper
character(len=*), intent(in) :: lang   !< new language ID to be used

character(len=len(lang)) :: langUpper

langUpper = to_upper(lang)

select case (langUpper)
    case ('NL', 'UK')
        language = langUpper
end select
end subroutine SetLanguage

!>
!! Subroutine that gets the language for rangeCheck messages
!!
!! @ingroup tools
subroutine GetLanguage(lang)
character(len=*), intent(out) :: lang   !< filled with current language ID

lang = language
end subroutine GetLanguage

end module errorMessages
