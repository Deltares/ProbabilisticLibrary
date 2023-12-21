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

!> @file
!! Feedback library: error messaging and progress reporting
!<
!
!> @defgroup Feedback Feedback library
!! The feedback library provides a mechanism for reporting errors and progress
!!
!! <b>Short documentation of intended usage:</b>
!!
!! The library consists of the following types of routines:
!! \li Managing the library itself
!! \li Write progress messages (detailed or basic)
!! \li Write error messages
!! \li Define the context of the computation
!!
!! <b>Managing the library:</b>
!! The routine \c feedbackInitialise() initialises the library with:
!! \li Whether to write to screen only or to a file or both
!! \li Name of the log file
!! \li Detailed output or not (parameters: verboseNone to verboseDebugging)
!!
!! For performance reasons, you can set and examine the variable \c traceback.
!! This variable indicates whether keeping track of the stack is required.
!! Typical usage in tight loops:
!!
!! \code
!!     use feedback
!!
!!     ...
!!     if ( traceback ) call feedbackEnter( "section or routine" )
!!     ...
!!     if ( traceback ) call feedbackLeave
!! \endcode
!! See below for more details
!!
!! <i>(Note: initialisation has to be done for each thread and
!! each thread should have its own logfile to avoid interlacing
!! the messages)</i>
!!
!! <b>Write progress messages:</b>
!! Progress messages tell the user what the program is doing. For
!! users who are interested in the details of the computation the
!! level of detail can be set high, for others a minimum amount of
!! information is printed.
!!
!! Use the routine \c progressReport to print a short message
!! indicating the stage in the computation.
!!
!! To communicate with the user-interface, use the \c progressBarPercent
!! or \c progressBarIncrement routines.
!!
!! <b>Write error messages:</b>
!! Error messages are written with the context in which the error occurred
!! and if details are required with the known stack.
!!
!! An error message is written with the subroutine \c errorMessage,
!! a warning with \c warningMessage and a fatal error (which stops the
!! program) with \c fatalError
!!
!! To indicate where in the program the error occurs, you can use
!! the subroutine \c feedbackEnter - this routine registers where
!! in the program (source) you are, this may be a routine you enter but also
!! a section within the computation that is of interest. If you leave the
!! subroutine or section, use \c feedbackLeave to indicate this.
!! You probably should not use it inside a tight loop, but here is
!! a sketch of typical use:
!!
!! \code
!!     subroutine exampleComputation( ... )
!!     use feedback
!!     ...
!!     if ( traceback ) call feedbackEnter( "exampleComputation" )
!!     ... some computation ...
!!
!!     if ( traceback ) call feedbackEnter( "Main do-loop" )
!!     do i = 1,1000000
!!
!!         ... some computation ...
!!     enddo
!!     if ( traceback ) call feedbackLeave
!!     if ( traceback ) call feedbackLeave
!! \endcode
!!
!! <b>Note:</b> \c feedbackEnter and \c feedbackLeave must be used
!! in matching pairs!
!!
!! <b>Define the context of the computation:</b>
!! A bare error message may not be enough to understand what went wrong.
!! For the purpose of standardising the extra information that should
!! accompany an error message, the library offers the routine \c feedbackContext.
!! You can use it to define relevant text to be shown in conjunction with the
!! error message. For instance: the name of the computation method that is
!! being run or the case that you are dealing with.
!!
!! To define a completely new context, use \c feedbackDropContext.
!! \todo Add multilingual support
!! @ingroup Feedback
module feedback

    use iso_c_binding
    use precision
    use feedback_parameters

    implicit none

    private

    integer, parameter :: id = 0   !< Thread ID; multi threading is not supported yet!

    public :: feedbackInitialise
    public :: feedbackClose
    public :: feedbackError
    public :: feedbackEnter
    public :: feedbackLeave
    public :: feedbackPrintStack
    public :: feedbackPrintContext
    public :: progressReport
    public :: feedbackContext
    public :: feedbackDropContext
    public :: errorMessage
    public :: warningMessage
    public :: fatalError
    public :: SetfatalErrorAction
    public :: GetFatalErrorMessage
    public :: GetThreadID
    public :: allow_log
    public :: SetAllowLogging
    public :: FatalErrorOccured
    public :: GetFatalErrorExpected
    public :: SetFatalErrorExpected
    public :: ResetFatalErrorOccured
    public :: verbosityLevel

    interface feedbackContext                   !< Define the context for error messages @ingroup Feedback
        module procedure feedbackContextTxt
        module procedure feedbackContextInt
        module procedure feedbackContextReal
        module procedure feedbackContextDouble
    end interface

    interface errorMessage                      !< Print an error message
        module procedure errorMessageTxt
        module procedure errorMessageInt
        module procedure errorMessageReal
        module procedure errorMessageDouble
    end interface

    interface warningMessage                    !< Print a warning message
        module procedure warningMessageTxt
        module procedure warningMessageInt
        module procedure warningMessageDouble
    end interface

    interface fatalError                        !< Print an error message and stop the program
        module procedure fatalErrorTxt
        module procedure fatalErrorInt
        module procedure fatalErrorReal
        module procedure fatalErrorDouble
    end interface

    interface progressReport                    !< Write a single message to the output for debugging purposes
        module procedure progressReportTxt
        module procedure progressReportInt
        module procedure progressReportDouble
        module procedure progressReportArray
        module procedure progressReportValueAndArray
    end interface

contains

!> Has a fatal error occured?
!! Mainly used in unit tests
!!
!! @ingroup Feedback
function FatalErrorOccured() result(value)
    use feedbackDll, only : GetSetFatalErrorOccured
    logical :: value

    call GetSetFatalErrorOccured ('get', value)
end function FatalErrorOccured

!> Reset fatal error occurance
!! Mainly used in unit tests
!!
!! @ingroup Feedback
subroutine ResetFatalErrorOccured()
    use feedbackDll, only : GetSetFatalErrorOccured
    logical :: value

    value = .false.
    call GetSetFatalErrorOccured ('set', value)
end subroutine ResetFatalErrorOccured

!> Set fatal error occurance
!! Mainly used in unit tests
!!
!! @ingroup Feedback
subroutine SetFatalErrorOccured(value)
    use feedbackDll, only : GetSetFatalErrorOccured
    logical, intent(in) :: value  !< the value of FatalErrorOccured to be set
    logical :: cpValue

    cpValue = value
    call GetSetFatalErrorOccured ('set', cpValue)
end subroutine SetFatalErrorOccured

!> Get value of FatalErrorExpected
!! Mainly used in unit tests
!!
!! @ingroup Feedback
function GetFatalErrorExpected() result (value)
    use feedbackDll, only : GetSetFatalErrorExpected
    logical :: value

    call GetSetFatalErrorExpected ('get', value)
end function GetFatalErrorExpected

!> Set the value of FatalErrorExpected
!! Mainly used in unit tests
!!
!! @ingroup Feedback
subroutine SetFatalErrorExpected(value)
    use feedbackDll, only : GetSetFatalErrorExpected
    logical, intent(in) :: value  !< the value of FatalErrorExpected to be set
    logical :: cpValue

    cpValue = value
    call GetSetFatalErrorExpected ('set', cpValue)
end subroutine SetFatalErrorExpected

!> Is logging active?
!!
!! @ingroup Feedback
logical function allow_log()
    use feedbackDll, only : GetAllowLogging
    logical :: logActive

    call GetAllowLogging (logActive)
    allow_log = logActive
end function allow_log

!> Set logging on/off
!!
!! @ingroup Feedback
subroutine SetAllowLogging(value)
    use feedbackDll, only : SetAllowLog => SetAllowLogging
    logical, intent(in) :: value  !< the value of allow_logging to be set

    call SetAllowLog (value)

end subroutine SetAllowLogging

!> Get the tread ID
!! Note that multi threading is not implemented, so always the value 0 is returned
!!
!! @ingroup Feedback
subroutine GetThreadID(threadId)
    use feedbackDll, only : GetId => GetThreadId
    integer, intent(out) :: threadId !< the current thread ID (always 0)

     call GetId(threadId)
end subroutine GetThreadID

!> Set the value of FatalErrorAction
!! possible values:
!!    onfatalerrorStop           = 1 : Action on fatal error: exit the program
!!    onfatalerrorThrowException = 2 : Action on fatal error: throw exception (for use as dll)
!!    onfatalerrorUnittest       = 3 : Action on fatal error: handle as unit test
!!
!! @ingroup Feedback
subroutine SetfatalErrorAction(action)
    use feedbackDll, only : SetAction => SetOnFatalErrorAction
    integer, intent(in) :: action !< the value of FatalErrorAction to be set (1, 2 or 3)

    call SetAction (action)
end subroutine SetfatalErrorAction

!> Get the last of FatalErrorMessage
!!
!! @ingroup Feedback
subroutine GetFatalErrorMessage(message)
    use feedbackDll, only : GetFatalErrorMsg
    character(len=*), intent(out) :: message  !< the last error message

    call GetFatalErrorMsg (message)
end subroutine GetFatalErrorMessage

!> Return the error status of the current thread
!! If an error has been reported (via the errorMessage routines), this
!! routine will return true, otherwise false
!!
!! @ingroup Feedback
subroutine feedbackError( error )
    use feedbackDll, only : GetFeedbackError

    logical :: error                 !< True if there has been some error reported

    call GetFeedbackError ( error )
end subroutine feedbackError


!> Close the log file
!! @ingroup Feedback
subroutine feedbackClose(noStackCheck)
    use feedbackDll, only : GetStackLevel, GetUnitNumber, CloseFile
    logical, intent(in), optional :: noStackCheck

    integer :: lun, stacklevel
    logical :: stackCheck

    stackCheck = .true.
    if (present(noStackCheck)) stackCheck = .not. noStackCheck

    call GetUnitNumber(lun)

    if ( lun > 0 ) then
        if (stackCheck) then
            call GetStackLevel(stackLevel)
            if ( stackLevel > 0 ) then
                call errorMessage( "Stack not empty - possible programming error" )
            endif
        endif
        call CloseFile
    endif

end subroutine feedbackClose


!> Initialise the feedback subsystem
!!
!! @ingroup Feedback
subroutine feedbackInitialise( output, logfile, verbosity )
    use feedbackDll, only : Initialise => feedbackInitialise
    integer, intent(in)          :: output               !< option: output to screen or file
    character(len=*), intent(in) :: logfile              !< name of the log file to be used
    integer, intent(in)          :: verbosity            !< level for displaying the messages

    call Initialise( output, logfile, verbosity )

end subroutine feedbackInitialise


!> Write a single message to the output for the benefit of the user
!! @ingroup Feedback
subroutine progressReportTxt( level, msgtext )
    use feedbackDll, only : PrintMessage
    integer, intent(in)          :: level        !< Level of verbosity
    character(len=*), intent(in) :: msgtext      !< Text to write

    logical :: error

    call feedbackError( error )
    if ( error ) return

    if ( level <= verbosityLevel() ) then
        call PrintMessage ( msgtext )
    endif

end subroutine progressReportTxt

!> Write a single message with an integer value
!! @ingroup Feedback
subroutine progressReportInt( level, msgtext, value )

    integer, intent(in)          :: level      !< Level of verbosity
    character(len=*), intent(in) :: msgtext    !< Error message to write
    integer, intent(in)          :: value      !< Integer value to add

    character(len=40) :: buffer

    write( buffer, "(x,i0)" ) value

    call progressReport( level, trim(msgtext) // trim(buffer) )

end subroutine progressReportInt

!> Write a single message with a double precision real value
!! @ingroup Feedback
subroutine progressReportDouble( level, msgtext, value )

    integer, intent(in)               :: level      !< Level of verbosity
    character(len=*), intent(in)      :: msgtext    !< Error message to write
    real(kind=dp), intent(in)         :: value      !< Double value to add

    character(len=40) :: buffer

    write( buffer, "(x,g22.12)" ) value

    call progressReport( level, trim(msgtext) // trim(buffer) )

end subroutine progressReportDouble

!> Print array values to report file
!!
!! @ingroup Feedback
subroutine progressReportArray ( level, msgText, x, size )

    integer,          intent(in) :: level      !< Level of verbosity
    character(len=*), intent(in) :: msgtext    !< Error message to write
    real(kind=wp),    intent(in) :: x(:)       !< x vector to write
    integer,          intent(in) :: size       !< the length of x

    integer, parameter :: numbersPerLine = 5
    character(len=256) :: numbers
    character(len=40)  :: buffer
    integer            :: i, ii

    numbers = msgtext
    do i = 1, size
        write(buffer,*) x(i)
        numbers = trim(numbers) // ' ' // adjustl(buffer)
        ii = mod(i, numbersPerLine)
        if (ii == 0 .or. i == size) then
            call progressReport( level, trim(numbers) )
            numbers = ' '
        endif
    enddo

end subroutine progressReportArray

!> Print z-value and array values to report file
!!
!! @ingroup Feedback
subroutine progressReportValueAndArray ( level, msgText, z, x, size )

    integer, intent(in)          :: level      !< Level of verbosity
    character(len=*), intent(in) :: msgtext    !< Error message to write
    real(kind=wp),    intent(in) :: z          !< z value to write
    real(kind=wp),    intent(in) :: x(:)       !< x vector to write
    integer,          intent(in) :: size       !< the length of x

    character(len=40)  :: buffer

    write(buffer,*) z

    call progressReport( level, msgtext // ' ' // adjustl(buffer), x, size)

end subroutine progressReportValueAndArray

!> Register a new routine level
!! @ingroup Feedback
subroutine feedbackEnter( routine )
    use feedbackDll, only : PushRoutine
    character(len=*) :: routine              !< Name of the routine to add
    logical :: success

    call PushRoutine( routine, success )
    if ( .not. success ) then
        call errorMessage( "Limit to traceback stack reached - possible programming error" )
    endif

end subroutine feedbackEnter

!> Drop a routine level from the stack trace
!! @ingroup Feedback
subroutine feedbackLeave
    use feedbackDll, only : PopRoutine
    logical :: success

    call PopRoutine( success )

    if (.not. success ) then
        call errorMessage( "Traceback stack already empty - programming error" )
    endif

end subroutine feedbackLeave

!> Print the stack trace as known via feedbackEnter and feedbackLeave
!! @ingroup Feedback
subroutine feedbackPrintStack
    use feedbackDll, only : PrintStack

    call PrintStack

end subroutine feedbackPrintStack

!> Write an error message
!! @ingroup Feedback
subroutine errorMessageTxt( msgtext )
    use feedbackDll, only : PrintMessage, GetVerbosity
    character(len=*), intent(in) :: msgtext    !< Error message to write
    integer :: verbosityLevel

    call PrintMessage ( "Error: " // trim(msgtext) )

    call feedbackPrintContext

    call GetVerbosity ( verbosityLevel )
    if ( verbosityLevel >= verboseDetailed ) then
        call feedbackPrintStack
    endif

end subroutine errorMessageTxt

!> Write an error message with an integer value
!! @ingroup Feedback
subroutine errorMessageInt( msgtext, value )

    character(len=*), intent(in) :: msgtext    !< Error message to write
    integer, intent(in)          :: value      !< Integer value to add

    character(len=16) :: buffer

    write( buffer, "(x,i0)" ) value

    call errorMessage( trim(msgtext) // trim(buffer) )

end subroutine errorMessageInt

!> Write an error message with a real value
!! @ingroup Feedback
subroutine errorMessageReal( msgtext, value )

    character(len=*), intent(in) :: msgtext    !< Error message to write
    real, intent(in)             :: value      !< Integer value to add

    character(len=16) :: buffer

    write( buffer, "(x,g15.6)" ) value

    call errorMessage( trim(msgtext) // trim(buffer) )

end subroutine errorMessageReal

!> Write an error message with a double precision real value
!! @ingroup Feedback
subroutine errorMessageDouble( msgtext, value )

    character(len=*), intent(in)      :: msgtext    !< Error message to write
    real(kind=dp), intent(in)         :: value      !< Integer value to add

    character(len=23) :: buffer

    write( buffer, "(x,g22.12)" ) value

    call errorMessage( trim(msgtext) // trim(buffer) )

end subroutine errorMessageDouble

!> Write a warning message
!! @ingroup Feedback
subroutine warningMessageTxt( msgtext )
    use feedbackDll, only : PrintMessage, GetVerbosity
    character(len=*), intent(in) :: msgtext    !< Error message to write

    integer :: verbosity

    call PrintMessage ( "Warning: " // trim(msgtext) )

    call GetVerbosity(verbosity)
    if ( verbosity >= verboseDetailed ) then
        call feedbackPrintContext
        call feedbackPrintStack
    endif

end subroutine warningMessageTxt

!> Write a warning message with an integer value
!! @ingroup Feedback
subroutine warningMessageInt( msgtext, value )

    character(len=*), intent(in) :: msgtext    !< Error message to write
    integer, intent(in)          :: value      !< Integer value to add

    character(len=16) :: buffer

    write( buffer, "(x,i0)" ) value

    call warningMessage( trim(msgtext) // trim(buffer) )

end subroutine warningMessageInt

!> Write a warning message with a double precision real value
!! @ingroup Feedback
subroutine warningMessageDouble( msgtext, value )

    character(len=*), intent(in)      :: msgtext    !< Error message to write
    real(kind=dp), intent(in)         :: value      !< Real (double) value to add

    character(len=23) :: buffer

    write( buffer, "(x,g22.12)" ) value

    call warningMessage( trim(msgtext) // trim(buffer) )

end subroutine warningMessageDouble

!> Write a fatal error message
!! @ingroup Feedback
subroutine fatalErrorTxt( msgtext )
    use feedbackDll, only : PrintMessage, GetOnFatalErrorAction, SetFatalErrorMessage, ThrowExceptionFB
    character(len=*), intent(in) :: msgtext    !< Error message to write

    integer :: action

    call PrintMessage ( "Fatal error: " // trim(msgtext) )

    call GetOnFatalErrorAction(action)

    select case (action)
        case( onfatalerrorUnittest )
            call CreateLastErrorFile(msgtext)
            call feedbackPrintContext
            call feedbackPrintStack
            if (GetFatalErrorExpected()) then
                call SetFatalErrorOccured (.true.)
                call SetFatalErrorMessage ( "Fatal error: " // trim(msgtext) )
            else
                stop -1
            endif
        case ( onfatalerrorThrowException )
            call CreateLastErrorFile(msgtext)
            call feedbackClose(.true.)
            call ThrowExceptionFB( trim(msgtext) )
        !case( onfatalerrorStop ) is handled as default action
        case default
            call CreateLastErrorFile(msgtext)
            call feedbackPrintContext
            call feedbackPrintStack
            stop -1
    end select

end subroutine fatalErrorTxt

!> helper function for fatalErrorTxt
subroutine CreateLastErrorFile(msgtext)
    character(len=*), intent(in) :: msgtext  !< message to be printed in file

    integer                      :: lun      ! logical unit number
    integer                      :: ierr     ! error code of opening / writing
    character(len=*), parameter  :: fileLastError = 'last_error.txt'

    open (newunit=lun, file = fileLastError, iostat = ierr, action='write')
    if (ierr == 0) write(lun,'(a)', iostat=ierr) trim(msgtext)
    close(lun, iostat=ierr)

end subroutine CreateLastErrorFile

!> Write a fatal error message with an integer value
!! @ingroup Feedback
subroutine fatalErrorInt( msgtext, value )

    character(len=*), intent(in) :: msgtext    !< Error message to write
    integer, intent(in)          :: value      !< Integer value to add

    character(len=16) :: buffer

    write( buffer, "(x,i0)" ) value

    call fatalError( trim(msgtext) // trim(buffer) )

end subroutine fatalErrorInt

!> Write a fatal error message with a real value
!! @ingroup Feedback
subroutine fatalErrorReal( msgtext, value )

    character(len=*), intent(in) :: msgtext    !< Error message to write
    real, intent(in)             :: value      !< Integer value to add

    character(len=16) :: buffer

    write( buffer, "(x,g15.6)" ) value

    call fatalError( trim(msgtext) // trim(buffer) )

end subroutine fatalErrorReal

!> Write a fatal error message with a double precision real value
!! @ingroup Feedback
subroutine fatalErrorDouble( msgtext, value )

    character(len=*), intent(in)      :: msgtext    !< Error message to write
    real(kind=dp), intent(in)         :: value      !< Integer value to add

    character(len=23) :: buffer

    write( buffer, "(x,g22.12)" ) value

    call fatalError( trim(msgtext) // trim(buffer) )

end subroutine fatalErrorDouble

!> Drop the entire context
!! @ingroup Feedback
subroutine feedbackDropContext
    use feedbackDll, only : DropContext

    call DropContext()
end subroutine feedbackDropContext

!> Register a context for error messages
!! @ingroup Feedback
subroutine feedbackPrintContext
    use feedbackDll, only : PrintContext

    call PrintContext()
end subroutine feedbackPrintContext

!> Register a context for error messages
!! @ingroup Feedback
subroutine feedbackContextTxt( text )
    use feedbackDll, only : ContextTxt
    character(len=*), intent(in) :: text  !< text to be added to the context stack
    logical :: success

    call ContextTxt ( text, success )

    if ( .not. success ) then
        call errorMessage( "Too many text strings for context array - possible program error" )
    endif

end subroutine feedbackContextTxt

!> Register a context for error messages - with integer value
!! @ingroup Feedback
subroutine feedbackContextInt( text, value )

    character(len=*), intent(in) :: text   !< text to be added to the context stack
    integer,          intent(in) :: value  !< number to be added to the text

    character(len=16) :: buffer

    write(buffer, "(i0)" ) value

    call feedbackContext ( trim(text) // " " // trim(buffer))

end subroutine feedbackContextInt

!> Register a context for error messages - with real value
!! @ingroup Feedback
subroutine feedbackContextReal( text, value )

    character(len=*), intent(in) :: text   !< text to be added to the context stack
    real(kind=sp),    intent(in) :: value  !< number to be added to the text

    character(len=15) :: buffer

    write(buffer, "(g15.5)" ) value

    call feedbackContext ( trim(text) // " " // trim(buffer) )

end subroutine feedbackContextReal

!> Register a context for error messages - with double precision real value
!! @ingroup Feedback
subroutine feedbackContextDouble( text, value )

    character(len=*), intent(in) :: text   !< text to be added to the context stack
    real(kind=dp),    intent(in) :: value  !< number to be added to the text

    character(len=21) :: buffer

    write(buffer, "(g21.12)" ) value

    call feedbackContext ( trim(text) // " " // trim(buffer) )

end subroutine feedbackContextDouble

!> wrapper to get verbosity level from feedback dll
integer function verbosityLevel()
    use feedbackDll, only : GetVerbosityLevel
    call GetVerbosityLevel(verbosityLevel)

end function verbosityLevel

end module feedback
