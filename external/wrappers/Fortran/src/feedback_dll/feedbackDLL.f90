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
!! Feedback dll: error messaging and progress reporting - interface
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
module feedbackDLL
    implicit none

interface

!> Initialise the feedback dll
!!
!! @ingroup FeedbackDLL
module subroutine feedbackInitialise( output, logfile, verbosity )
 !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"Initialise" :: feedbackInitialise

    integer, intent(in)          :: output               !< option: output to screen, file, both, or another dll
    character(len=*), intent(in) :: logfile              !< name of the log file to be used (only used if writing to file)
    integer, intent(in)          :: verbosity            !< level for displaying the messages
end subroutine feedbackInitialise

!> Write a message to output device (as selected in Initialise)
!!
!! @ingroup FeedbackDLL
module subroutine PrintMessage(msgtext)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"PrintMessage" :: PrintMessage
    character(len=*), intent(in) :: msgtext    !< text to be printed to screen, file or dll
end subroutine PrintMessage

!> Is logging active
!!
!! @ingroup FeedbackDLL
module subroutine GetAllowLogging(value)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetAllowLogging" :: GetAllowLogging
    logical, intent(out) :: value !< the current value of allow_logging
end subroutine GetAllowLogging

!> Set logging on/off
!!
!! @ingroup FeedbackDLL
module subroutine SetAllowLogging(value)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"SetAllowLogging" :: SetAllowLogging
    logical, intent(in) :: value  !< the value for allow_logging to be set
end subroutine SetAllowLogging

!> Get the current verbosity level
!!
!! @ingroup FeedbackDLL
module subroutine GetVerbosity(level)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetVerbosity" :: GetVerbosity
    integer, intent(out) :: level !< the current verbosity level: -1, .. , 2
end subroutine GetVerbosity

!> Set the OnFatalErrorAction
!!
!! @ingroup FeedbackDLL
module subroutine SetOnFatalErrorAction(action)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"SetOnFatalErrorAction" :: SetOnFatalErrorAction
    integer, intent(in) :: action !< FatalErrorAction to be set. Must be in range: 1, 2 or 3
end subroutine SetOnFatalErrorAction

!> Get the current OnFatalErrorAction
!!
!! @ingroup FeedbackDLL
module subroutine GetOnFatalerrorAction(onfatalerrorAction)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetOnFatalErrorAction" :: getonfatalerrorAction
    integer, intent(out) :: onfatalerrorAction  !< current FatalErrorAction: 1, 2 or 3
end subroutine GetOnFatalerrorAction

!> Get the error status (true/false)
!!
!! @ingroup FeedbackDLL
module subroutine GetFeedbackError ( error )
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetFeedbackError" :: GetFeedbackError
    logical, intent(out) :: error  !< last error status
end subroutine GetFeedbackError

!> Set the errormessage
!!
!! @ingroup FeedbackDLL
module subroutine SetFatalErrorMessage( message )
  !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"SetFatalErrorMessage" :: SetFatalErrorMessage
  character(len=*), intent(in) :: message  !< error message to be set
end subroutine SetFatalErrorMessage

!> Get the last error message
!!
!! @ingroup FeedbackDLL
module subroutine GetFatalErrorMsg( message )
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetFatalErrorMsg" :: GetFatalErrorMsg
    character(len=*), intent(out) :: message  !< current error message
end subroutine GetFatalErrorMsg

!> Get the verbosity level
!!
!! @ingroup FeedbackDLL
module subroutine GetVerbosityLevel(verbosityLevel)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetVerbosityLevel" :: GetVerbosityLevel
    integer, intent(out) :: verbosityLevel  !< current verbosityLevel: -1, .., 2
end subroutine GetVerbosityLevel

!> Add a routine name to the call stack
!!
!! @ingroup FeedbackDLL
module subroutine PushRoutine( routine, success )
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"PushRoutine" :: PushRoutine
    character(len=*), intent(in) :: routine  !< routine name to be added to the stack
    logical, intent(out) :: success          !< .false., if the stack already reached it maximum size
end subroutine PushRoutine

!> Removes and get a routine name from the call stack
!!
!! @ingroup FeedbackDLL
module subroutine PopRoutine( success )
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"PopRoutine" :: PopRoutine
    logical, intent(out) :: success  !< if .false., the stack is already empty
end subroutine PopRoutine

!> Print the call stack
!!
!! @ingroup FeedbackDLL
module subroutine PrintStack
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"PrintStack" :: PrintStack
end subroutine PrintStack

!> Get the number of routines in the call stack
!!
!! @ingroup FeedbackDLL
module subroutine GetStackLevel(level)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetStackLevel" :: GetStackLevel
    integer, intent(out) :: level   !< number of routines in the call stack
end subroutine GetStackLevel

!> Closes the output file
!!
!! @ingroup FeedbackDLL
module subroutine CloseFile
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"CloseFile" :: CloseFile
end subroutine CloseFile

!> Get the unit number of the logfile
!!
!! @ingroup FeedbackDLL
module subroutine GetUnitNumber(lun)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetUnitNumber" :: GetUnitNumber
    integer, intent(out) :: lun   !< integer unit number of logfile
end subroutine GetUnitNumber

!> Get the thread Id
!!
!! @ingroup FeedbackDLL
module subroutine GetThreadId(id)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetThreadId" :: GetThreadId
    integer, intent(out) :: id   !< integer id for thread number
end subroutine GetThreadId

!> Reset the context stack
!!
!! @ingroup FeedbackDLL
module subroutine DropContext
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"DropContext" :: DropContext
end subroutine DropContext

!> Print the context stack
!!
!! @ingroup FeedbackDLL
module subroutine PrintContext
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"PrintContext" :: PrintContext
end subroutine PrintContext

!> Add a line of text to the context stack
!!
!! @ingroup FeedbackDLL
module subroutine ContextTxt( text, success )
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"ContextTxt" :: ContextTxt
    character(len=*), intent(in) :: text  !< line of text to be pushed on the context stack
    logical, intent(out) :: success       !< .false., if the stack already reached it maximum size
end subroutine ContextTxt

!> Get or Set the FatalErrorExpected
!! if func=get: get the value
!! if func=set: set the value
!!
!! @ingroup FeedbackDLL
module subroutine GetSetFatalErrorExpected(func, value)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetSetFatalErrorExpected" :: GetSetFatalErrorExpected
    logical, intent(inout) :: value      !< the value of FatalErrorExpected
    character(len=3), intent(in) :: func !< the action: 'get' or 'set' (case sensitive!)
end subroutine GetSetFatalErrorExpected

!> Get or Set the FatalErrorOccured
!! if func=get: get the value
!! if func=set: set the value
!!
!! @ingroup FeedbackDLL
module subroutine GetSetFatalErrorOccured(func, value)
    !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"GetSetFatalErrorOccured" :: GetSetFatalErrorOccured
    logical, intent(inout) :: value      !< the value of FatalErrorOccured
    character(len=3), intent(in) :: func !< the action: 'get' or 'set' (case sensitive!)
end subroutine GetSetFatalErrorOccured

!
!> wrapper for C ThrowException
module subroutine ThrowExceptionFB(message)
 !DEC$ ATTRIBUTES DLLEXPORT, ALIAS:"ThrowExceptionFB" :: ThrowExceptionFB
    character(len=*), intent(in) :: message
end subroutine ThrowExceptionFB

end interface

end module feedbackDLL
