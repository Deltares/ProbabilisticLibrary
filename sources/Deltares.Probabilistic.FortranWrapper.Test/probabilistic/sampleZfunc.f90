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
!! Simple Z functions for testing probabilistic techniques.
!<
!
module sampleFunctions

    use precision
    use interface_probcalc

    implicit none

    private

    public :: simpleZ, simpleSystem

contains

!> Simple z function: valueB - u1 - u2, where u1 and u2 are standard normally distributed, and where beta = 4
function simpleZ( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%iCode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = simpleSystem ( x(1),  x(2), x(3))

end function simpleZ

function simpleSystem( x1, x2, valueB )

    real (kind=wp), intent(in)   :: x1               !< input parameter x1
    real (kind=wp), intent(in)   :: x2               !< input parameter x2
    real (kind=wp), intent(in)   :: valueB           !< input parameter valueB
    real (kind=wp)               :: simpleSystem     !< output parameter

    simpleSystem = valueB - x1 - x2

end function simpleSystem


end module sampleFunctions
