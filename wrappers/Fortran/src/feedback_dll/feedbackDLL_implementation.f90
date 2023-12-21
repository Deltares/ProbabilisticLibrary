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

!
!> @file
!! Feedback dll: error messaging and progress reporting - implementation
!!
!! To be used in combination with the feedback module.
!! feedback is the (fortran) interface to the programmer.
!! This module make it possible to write to the same file from different dll's/exe's.
!!
!! By putting all data and print statements in this dll,
!! and let feedback initialise this dll and let feedback only print using this dll,
!! all output ends up at the right location.
!<
!
submodule(feedbackDLL) feedbackDLL_implementation
    use precision
    use feedback_parameters
    use omp_lib

    implicit none

    type feedbackData                                      !< Private type with all information
        character(len=80), dimension(100) :: routine
        character(len=80), dimension(100) :: context
        integer                           :: output
        integer                           :: contextNumber
        integer                           :: stackLevel
        integer                           :: verbosityLevel
        integer                           :: lun   = -1
        logical                           :: error = .true.
    end type feedbackData

    type(feedbackData), dimension(0:maxParallelThreads) :: fbdata    !< Array holding the actual information per thread

    integer            :: fatalErrorAction = 1
    character(len=255) :: fatalErrorMessage
    integer, parameter :: id = 0
    logical            :: allow_logging = .true.
    logical            :: isFatalErrorExpected = .false. !<   if isFatalErrorExpected = .true. then log error message in fatalErrorMsgtext and set boolean isFatalErrorOccured to true
                                                         !<   if isFatalErrorExpected = .false. then same action as onfatalerrorStop
    logical            :: isFatalErrorOccured = .false.  !<

contains

!> Initialise the feedback dll
!!
!! @ingroup FeedbackDLL
module procedure feedbackInitialise
    integer :: ierr
    integer :: myid

    myid = omp_get_thread_num()

    fbdata%error          = .false.
    fbdata%stackLevel     = 0
    fbdata%contextNumber  = 0
    fbdata%verbosityLevel = verbosity
    fbdata%output         = output

    select case ( output )
        case ( onlyFile, screenAndFile )
            fbdata%lun = 10               ! For the moment

            open( fbdata(myid)%lun, file = logfile, iostat = ierr, action = 'write' )
            if ( ierr /= 0 ) then
                fbdata%error = .true.
                write( *, "(a)" ) "Error occurred opening the log file - " // trim(logfile)
                write( *, "(a)" ) "Will print to screen"
                fbdata%output = onlyScreen
            endif

        case ( onlyScreen )
            ! Nothing to do

        case default
            fbdata%error = .true.

     end select

     select case ( verbosity )
         case ( verboseNone, verboseBasic, verboseDetailed, verboseDebugging )
             ! Nothing to do
         case default
             fbdata%verbosityLevel = verboseBasic
     end select

end procedure feedbackInitialise

!> Write a message to output device (as selected in Initialise)
!!
!! @ingroup FeedbackDLL
module procedure PrintMessage
    integer :: myid

    myid = omp_get_thread_num()

    select case ( fbdata(myid)%output )
    case (onlyScreen)
        write( *, "(a)" ) trim(msgtext)
    case (onlyFile)
        if (fbdata(myid)%lun > 0 ) then
            write( fbdata(myid)%lun, "(a)" ) trim(msgtext)
        endif
    case (screenAndFile)
        write( *, "(a)" ) trim(msgtext)
        if (fbdata(myid)%lun > 0 ) then
            write( fbdata(myid)%lun, "(a)" ) trim(msgtext)
        endif
    case default
        write (*,*) "Internal error"
    end select

end procedure PrintMessage

!> Is logging active
!!
!! @ingroup FeedbackDLL
module procedure GetAllowLogging

    value = allow_logging

end procedure GetAllowLogging

!> Set logging on/off
!!
!! @ingroup FeedbackDLL
module procedure SetAllowLogging

    allow_logging = value

end procedure SetAllowLogging

!> Get the current verbosity level
!!
!! @ingroup FeedbackDLL
module procedure GetVerbosity
    integer :: myid

    myid = omp_get_thread_num()

    level = fbdata(myid)%verbosityLevel

end procedure GetVerbosity

!> Set the OnFatalErrorAction
!!
!! @ingroup FeedbackDLL
module procedure SetOnFatalErrorAction

    fatalErrorAction = action

end procedure SetOnFatalErrorAction

!> Get the current OnFatalErrorAction
!!
!! @ingroup FeedbackDLL
module procedure GetOnFatalerrorAction

    onfatalerrorAction = fatalErrorAction

end procedure GetOnFatalerrorAction

!> Get the error status (true/false)
!!
!! @ingroup FeedbackDLL
module procedure GetFeedbackError
    integer :: myid

    myid = omp_get_thread_num()

    error = fbdata(myid)%error

end procedure GetFeedbackError

!> Set the errormessage
!!
!! @ingroup FeedbackDLL
module procedure SetFatalErrorMessage

  fatalErrorMessage = message

end procedure SetFatalErrorMessage

!> Get the last error message
!!
!! @ingroup FeedbackDLL
module procedure GetFatalErrorMsg

    message = fatalErrorMessage

end procedure GetFatalErrorMsg

!> Get the verbosity level
!!
!! @ingroup FeedbackDLL
module procedure GetVerbosityLevel

    integer :: myid

    myid = omp_get_thread_num()

    verbosityLevel= fbdata(myid)%verbosityLevel

end procedure GetVerbosityLevel

!> Add a routine name to the call stack
!!
!! @ingroup FeedbackDLL
module procedure PushRoutine

    integer :: myid

    myid = omp_get_thread_num()

    if ( fbdata(myid)%stackLevel < size(fbdata(myid)%routine) ) then
        fbdata(myid)%stackLevel = fbdata(myid)%stackLevel + 1
        fbdata(myid)%routine(fbdata(myid)%stackLevel) = routine
        success = .true.
    else
        success = .false.
    endif

end procedure PushRoutine

!> Removes and get a routine name from the call stack
!!
!! @ingroup FeedbackDLL
module procedure PopRoutine

    integer :: myid

    myid = omp_get_thread_num()

    if ( fbdata(myid)%stackLevel > 0 ) then
        fbdata(myid)%stackLevel = fbdata(myid)%stackLevel - 1
        success = .true.
    else
        success = .false.
    endif

end procedure PopRoutine

!> Print the call stack
!!
!! @ingroup FeedbackDLL
module procedure PrintStack

    integer :: i
    integer :: current
    integer :: myid

    myid = omp_get_thread_num()

    if ( fbdata(myid)%verbosityLevel >= verboseDetailed ) then
        current = fbdata(myid)%stackLevel
        if ( current > 0 ) then
            call PrintMessage("In " // trim(fbdata(myid)%routine(current)))
            do i = fbdata(myid)%stackLevel-1,1,-1
                call PrintMessage("    Called by " // trim(fbdata(myid)%routine(i)))
            enddo
        else
            call PrintMessage("(empty stack)")
        endif
    endif

end procedure PrintStack

!> Get the number of routines in the call stack
!!
!! @ingroup FeedbackDLL
module procedure GetStackLevel

    integer :: myid

    myid = omp_get_thread_num()

    level =  fbdata(myid)%stackLevel

end procedure GetStackLevel

!> Closes the output file
!!
!! @ingroup FeedbackDLL
module procedure CloseFile
    integer :: myid
    integer :: unitClosed
    integer :: i

    myid = omp_get_thread_num()

    unitClosed = fbdata(myid)%lun
    close( fbdata(myid)%lun )

    do i = 0, maxParallelThreads
        if (fbdata(i)%lun == unitClosed) then
            fbdata(i)%lun = -1
        endif
    enddo

end procedure CloseFile

!> Get the unit number of the logfile
!!
!! @ingroup FeedbackDLL
module procedure GetUnitNumber
    integer :: myid

    myid = omp_get_thread_num()

    lun = fbdata(myid)%lun

end procedure GetUnitNumber

!> Get the thread Id
!!
!! @ingroup FeedbackDLL
module procedure GetThreadId
    id = omp_get_thread_num()
end procedure GetThreadId

!> Reset the context stack
!!
!! @ingroup FeedbackDLL
module procedure DropContext
    integer :: myid

    myid = omp_get_thread_num()

    fbdata(myid)%contextNumber = 0

end procedure DropContext

!> Print the context stack
!!
!! @ingroup FeedbackDLL
module procedure PrintContext
    integer :: i

    integer :: myid

    myid = omp_get_thread_num()

    if ( fbdata(myid)%contextNumber > 0 ) then
        call PrintMessage("Context: ")
    endif

    do i = 1,fbdata(myid)%contextNumber
        call PrintMessage("    " // trim(fbdata(myid)%context(i)))
    enddo

end procedure PrintContext

!> Add a line of text to the context stack
!!
!! @ingroup FeedbackDLL
module procedure ContextTxt
    integer :: i
    integer :: myid

    myid = omp_get_thread_num()

    if ( fbdata(myid)%contextNumber < size(fbdata(myid)%context) ) then
        i = fbdata(myid)%contextNumber + 1
        fbdata(myid)%contextNumber = i
        fbdata(myid)%context(i) = text
        success = .true.
    else
        success = .false.
    endif

end procedure ContextTxt

!> Get or Set the FatalErrorExpected
!! if func=get: get the value
!! if func=set: set the value
!!
!! @ingroup FeedbackDLL
module procedure GetSetFatalErrorExpected

    if (func == 'get') then
        value = isFatalErrorExpected
    else
        isFatalErrorExpected = value
    endif

end procedure GetSetFatalErrorExpected

!> Get or Set the FatalErrorOccured
!! if func=get: get the value
!! if func=set: set the value
!!
!! @ingroup FeedbackDLL
module procedure GetSetFatalErrorOccured

    if (func == 'get') then
        value = isFatalErrorOccured
    else
        isFatalErrorOccured = value
    endif

end procedure GetSetFatalErrorOccured

!> wrapper for C function ThrowException
module procedure ThrowExceptionFB
    !call ThrowException(message)
    write(*,*) message
    stop 1
end procedure ThrowExceptionFB

end submodule feedbackDLL_implementation
