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
module precision
    integer, parameter :: sp = 4
    integer, parameter :: dp = 8
    integer, parameter :: wp = 8
    integer, parameter :: qd = selected_real_kind(33, 4931)  !< Quadruple precision

    real(kind=wp), parameter :: almostzero = 1.0d-30           !< Definition of zero for the machine

    contains

    !> set_nan
    !! set number to NaN, double precision version
    subroutine set_nan(value)
    use, intrinsic :: iso_fortran_env
    use, intrinsic :: ieee_arithmetic
    real(kind=dp), intent(out) :: value   !< double to be set to NaN

    value = ieee_value(value, ieee_quiet_nan)

    end subroutine set_nan

end module precision

