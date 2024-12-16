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
module mTpXData
use precision, only : wp
implicit none

private

type, public :: tCpData
    integer,      allocatable  :: iPoint(:)    ! Pointer to active variables used in the limit state function
    integer                    :: nStochActive
    real(kind=wp), allocatable :: xHR(:)
contains
    procedure :: copyDense2Full => cpDense2Full
    procedure :: copyFull2Dense => cpFull2Dense
end type tCpData

contains

!> copy x-vector from Hydra-Ring to problib
subroutine cpFull2Dense(this, xFull, xDense)
    class(tCpData),  intent(inout) :: this
    real(kind=wp),   intent(in   ) :: xFull(:)
    real(kind=wp),   intent(inout) :: xDense(this%nStochActive)

    integer :: i

    do i = 1, this%nStochActive
        xDense(i) = xFull(this%iPoint(i))
    end do
end subroutine cpFull2Dense

!> copy x-vector from problib to Hydra-Ring
subroutine cpDense2Full(this, xDense, xFull)
    class(tCpData),  intent(inout) :: this
    real(kind=wp),   intent(in)    :: xDense(this%nStochActive)
    real(kind=wp),   intent(inout) :: xFull(:)

    integer :: i

    xFull = this%xHR
    do i = 1, this%nStochActive
        xFull(this%iPoint(i)) = xDense(i)
    end do
end subroutine cpDense2Full


end module mTpXData
