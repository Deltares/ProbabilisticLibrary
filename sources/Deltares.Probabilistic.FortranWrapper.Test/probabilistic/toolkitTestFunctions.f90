! Copyright (C) Stichting Deltares and State of the Netherlands 2023. All rights reserved.
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

!> Module holding the four Z functions for testing the extended and the adaptive importance sampling procedures.
!! They were defined firstly via the probabilistic toolkit, hence the name.

module toolkitTestFunctions
    use precision
    use interface_probCalc

    implicit none

contains

function linearIsShiftZ( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = 1.98_wp - (x(1) + x(2))

end function linearIsShiftZ

function nonlinearIsZ( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = 3.0_wp - x(2) + (4.0_wp * x(1))**4

end function nonlinearIsZ

function manyIsZ( x, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: x(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = 6.0_wp - (x(1) + 2.0_wp * x(2) - 3.0_wp * x(3) + 1.5_wp * x(4)**2 - 3.5_wp * x(5)**3 + &
                  1.3_wp * sqrt(x(6)) - sqrt(x(7)+1.0_wp) + 3.2_wp * x(8) - x(9)**2 + &
                  x(10) * x(11) + x(12)**2 - 2.1_wp * x(13)**2 * x(1))

end function manyIsZ

end module toolkitTestFunctions
