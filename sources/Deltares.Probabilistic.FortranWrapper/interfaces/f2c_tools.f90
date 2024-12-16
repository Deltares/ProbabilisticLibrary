! Copyright (C) Stichting Deltares. All rights reserved.
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
module f2c_tools

implicit none

contains

subroutine copystr(s1, s2)
    character(len=*), intent(in   ) :: s1     !< input Fortran-like string
    character(len=1), intent(  out) :: s2(:)  !< output C-like string

    integer :: i, sz, last

    last = size(s2,1)
    sz = min(len_trim(s1), last)

    ! copy content
    do i = 1, sz
        s2(i) = s1(i:i)
    end do

    ! fill rest with \0 ; make sure we have at least one \0
    if (last == sz) then
        s2(last) = char(0)
    else
        do i = sz+1, last
            s2(i) = char(0)
        end do
    end if

end subroutine copystr

subroutine copystrback(s1, s2)
    character(len=*), intent(  out) :: s1     !< output Fortran-like string
    character(len=1), intent(in   ) :: s2(:)  !< input C-like string

    integer :: i, sz, last

    last = size(s2,1)
    sz = min(len(s1), last)

    ! copy content
    s1 = ' '
    do i = 1, sz
        if (s2(i) == char(0)) exit
        s1(i:i) = s2(i)
    end do
end subroutine copystrback

end module f2c_tools
