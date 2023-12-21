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
!! Feedback parameters: parameters needed bij feedback and feedback_dll
!<
!
module feedback_parameters
    integer, parameter  :: onfatalerrorStop                   = 1   !< Action on fatal error: exit the program
    integer, parameter  :: onfatalerrorThrowException         = 2   !< Action on fatal error: throw exception (for use as dll)
    integer, parameter  :: onfatalerrorUnittest               = 3   !< Action on fatal error: handle as unit test

    integer, parameter  :: onlyScreen       = 0             !< Mode: write to screen only @ingroup Feedback
    integer, parameter  :: onlyFile         = 1             !< Mode: write to file only @ingroup Feedback
    integer, parameter  :: screenAndFile    = 2             !< Mode: write to both screen and file @ingroup Feedback
    integer, parameter  :: dllObsolete      = 3             !< Mode: write to dll @ingroup Feedback
    integer, parameter  :: verboseNone      = -1            !< Reporting: NO messages @ingroup Feedback
    integer, parameter  :: verboseBasic     = 0             !< Reporting: basic messages only @ingroup Feedback
    integer, parameter  :: verboseDetailed  = 1             !< Reporting: detailed messages (including traceback) @ingroup Feedback
    integer, parameter  :: verboseDebugging = 2             !< Reporting: all messages @ingroup Feedback
    integer, parameter  :: verboseCPU       = verboseBasic  !< Reporting CPU time; can be altered some certain debug activities
end module feedback_parameters
