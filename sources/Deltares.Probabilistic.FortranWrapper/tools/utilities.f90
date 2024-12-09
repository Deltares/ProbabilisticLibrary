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

!> @file
!! General utility routines
!<
!
!>
!! Module with general utility routines
!!   @ingroup General
module utilities
    use iso_c_binding
    implicit none

    character(26), Parameter, private :: cap = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    character(26), Parameter, private :: low = 'abcdefghijklmnopqrstuvwxyz'

    character(len=*), parameter, public :: fileLastError = 'last_error.txt'

    interface replaceNulChars
        module procedure replaceNulCharsA
        module procedure replaceNulCharsB
    end interface replaceNulChars

contains

!> getFreeLuNumber
!! Routine to find a free LU-number
!!   @ingroup General
!<
!
! Note 1: the code was taken from ftnunit
! Note 2: please don't use this subroutine anymore, but: open(newunit=lun, ...)
!
subroutine getFreeLuNumber( lun )
    integer, intent(out) :: lun           !< LU-number to use

    logical       :: opend
    integer, save :: prevlun = 0

    if ( prevlun /= 0 ) then
        inquire( unit = prevlun, opened = opend )
        if ( .not. opend ) then
            lun = prevlun
            return
        endif
    endif

    do prevlun = 11,99
        inquire( unit = prevlun, opened = opend )
        if ( .not. opend ) then
            lun = prevlun
            return
        endif
    enddo

end subroutine getFreeLuNumber

!> to_lower
!! Changes a string to lower case
!!   @ingroup General
!<
! Note: the code was taken from ftnunit
!
pure function to_lower (str) result (string)
    character(*), intent(in) :: str
    character(len(str))      :: string

    integer :: ic, i

!   Capitalize each letter if it is upper case
    string = str
    do i = 1, len_trim(str)
        ic = index(cap, str(i:i))
        if (ic > 0) string(i:i) = low(ic:ic)
    end do

end function to_lower

!> to_upper
!! Changes a string to upper case
!!   @ingroup General
!<
!
pure function to_upper (str) result (string)
    character(*), intent(in) :: str
    character(len(str))      :: string

    integer :: ic, i

!   Capitalize each letter if it is lower case
    string = str
    do i = 1, len_trim(str)
        ic = index(low, str(i:i))
        if (ic > 0) string(i:i) = cap(ic:ic)
    end do

end function to_upper

!> Replaces null characters with spaces
!!   @ingroup General
function replaceNulCharsA( string )
    character(len=*) :: string
    character(len=len(string)) :: replaceNulCharsA

    integer          :: i, j

    replaceNulCharsA = string
    do i = 1,len(string)
        if (string(i:i) == c_null_char ) then
           do j = i, len(string)
              replaceNulCharsA(j:j) = ' '
           enddo
           exit
        endif
    enddo
end function replaceNulCharsA

!> Replaces null characters with spaces
!!   @ingroup General
function replaceNulCharsB( string )
    character(kind=c_char) :: string(:)
    character(len=size(string)) :: replaceNulCharsB

    integer          :: i, j

    do i = 1, size(string)
        replaceNulCharsB(i:i) = string(i)
    end do

    do i = 1, size(string)
        if (replaceNulCharsB(i:i) == c_null_char ) then
           do j = i, size(string)
              replaceNulCharsB(j:j) = ' '
           enddo
           exit
        endif
    enddo
end function replaceNulCharsB

!! find the last forward or backward slash in a string
function findLastSlash(filepath) result(ipos)
    character(len=*), intent(in) :: filepath  !< string (path) to search in
    integer                      :: ipos      !< function result

    integer                      :: iposBw    !< position of backward slash in string
    integer                      :: iposFw    !< position of forward slash in string

    iposBw = index(filepath, '\', back=.true.)
    iposFw = index(filepath, '/', back=.true.)

    ipos = max(iposBw, iposFw)

end function findLastSlash

!> strip path from a filename
subroutine stripPath(filepath, basename)
    character(len=*),              intent(in)  :: filepath  !< string (path) to search in
    character(len=:), allocatable, intent(out) :: basename  !< resulting filename

    integer :: ipos

    ipos = findLastSlash(filepath)
    if (ipos > 0) then
        basename = trim(filepath(ipos+1:))
    else
        basename = trim(filepath)
    endif
end subroutine stripPath

end module utilities
