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
module interface_special_funcs
use, intrinsic :: iso_c_binding, only: c_double
use interface_gen, only : tError
use f2c_tools, only : copystrback

implicit none

private :: c_double, tError, copystrback

contains

function betain(x, p, q, beta, ifault)
    real(kind=c_double), intent(in   ) :: x, p, q, beta
    integer,             intent(  out) :: ifault
    real(kind=c_double) :: betain
    betain = -999.0
    ifault = -999
end function betain

function dgammq(a, x, ierr, errorMessage)
    real(kind=c_double), intent(in)        :: a
    real(kind=c_double), intent(in)        :: x
    integer,             intent(out)       :: ierr
    character(len=*),    intent(inout)     :: errorMessage
    real(kind=c_double)                    :: dgammq

    dgammq = -999.0_c_double
    ierr   = -999
end function dgammq

end module interface_special_funcs
