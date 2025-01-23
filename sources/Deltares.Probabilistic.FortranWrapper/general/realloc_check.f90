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

!> @file
!! (Re-)allocation of arrays
!<
! based on: https://svn.oss.deltares.nl/repos/delft3d/trunk/src/utils_lgpl/deltares_common/packages/deltares_common/src/malloc.f90
!>
!! Module with subroutine realloc

module m_realloc_check
    use feedback
    use precision

    implicit none

    private

    public :: realloc_check, check_alloc

    !
    ! Routines for both allocatables and pointers are required
    !
    interface realloc_check
        module procedure realloc_check_int
        module procedure realloc_check_log
        module procedure realloc_check_int_p
        module procedure realloc_check_int2d
        module procedure realloc_check_int3d
        module procedure realloc_check_double
        module procedure realloc_check_double_p
        module procedure realloc_check_double2d
        module procedure realloc_check_double2d_p
        module procedure realloc_check_double3d
        module procedure realloc_check_double4d_p
        module procedure realloc_check_single
        module procedure realloc_check_single_p
        module procedure realloc_check_single2d
        module procedure realloc_check_single2d_P
        module procedure realloc_check_single3d
        module procedure realloc_check_single4d_p
        module procedure realloc_check_string
    end interface realloc_check

contains

!> Routine to check the allocation error status
!! Useful for chechking if the allocation of an array of derived types succeeded or not
subroutine check_alloc(ierr, size1, arr_name )
   integer, intent(in)          :: ierr
   integer, intent(in)          :: size1
   character(len=*), intent(in) :: arr_name

   character(len=200)           :: msg

   if ( ierr /= 0 ) then
       write( msg, '(3a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1
       call fatalError( msg )
       return
   endif
end subroutine check_alloc

!> Routine to allocate a one-dimensional array and check that it succeeded
!! The array is deallocated if necessary and then allocated to the right size.
!! If an error occurs, this is reported via FatalError.
!! No arguments other than the bare ones, as in practice we do not use anything
!! but the size.
subroutine realloc_check_int(arr, size1, arr_name )
   implicit none
   integer, allocatable, intent(inout)          :: arr(:)
   integer, intent(in)                          :: size1
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_int

subroutine realloc_check_log(arr, size1, arr_name )
   implicit none
   logical, allocatable, intent(inout)          :: arr(:)
   integer, intent(in)                          :: size1
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_log

subroutine realloc_check_int_p(arr, size1, arr_name )
   implicit none
   integer, pointer, intent(inout)              :: arr(:)
   integer, intent(in)                          :: size1
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (associated(arr)) then
       if ( size1 == size(arr) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_int_p

subroutine realloc_check_single(arr, size1, arr_name )
   implicit none
   real(kind=sp), allocatable, intent(inout)    :: arr(:)
   integer, intent(in)                          :: size1
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_single

subroutine realloc_check_single_p(arr, size1, arr_name )
   implicit none
   real(kind=sp), pointer                       :: arr(:)
   integer, intent(in)                          :: size1
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (associated(arr)) then
       if ( size1 == size(arr) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_single_p

subroutine realloc_check_double(arr, size1, arr_name )
   implicit none
   real(kind=dp), allocatable, intent(inout)    :: arr(:)
   integer, intent(in)                          :: size1
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0)' ) 'Error allocating array ', trim(arr_name), ' - requested size: ', size1
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_double

subroutine realloc_check_string(arr, size1, arr_name )
   implicit none
   character(len=*), allocatable, intent(inout) :: arr(:)
   integer, intent(in)                          :: size1
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0)' ) 'Error allocating array ', trim(arr_name), ' - requested size: ', size1
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_string

subroutine realloc_check_double_p(arr, size1, arr_name )
   implicit none
   real(kind=dp), pointer                       :: arr(:)
   integer, intent(in)                          :: size1
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (associated(arr)) then
       if ( size1 == size(arr) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0)' ) 'Error allocating array ', trim(arr_name), ' - requested size: ', size1
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_double_p

!> Routine to allocate a two-dimensional array and check that it succeeded
!! The array is deallocated if necessary and then allocated to the right size.
!! If an error occurs, this is reported via FatalError.
!! No arguments other than the bare ones, as in practice we do not use anything
!! but the size.
subroutine realloc_check_int2d(arr, size1, size2, arr_name )
   implicit none
   integer, allocatable, intent(inout)          :: arr(:,:)
   integer, intent(in)                          :: size1, size2
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr,1) .and. size2 == size(arr,2) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1,size2), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0,a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1, ' x ', size2
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_int2d

subroutine realloc_check_single2d(arr, size1, size2, arr_name )
   implicit none
   real(kind=sp), allocatable, intent(inout)    :: arr(:,:)
   integer, intent(in)                          :: size1, size2
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr,1) .and. size2 == size(arr,2) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1,size2), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0,a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1, ' x ', size2
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_single2d

subroutine realloc_check_single2d_p(arr, size1, size2, arr_name )
   implicit none
   real(kind=sp), pointer, intent(inout)        :: arr(:,:)
   integer, intent(in)                          :: size1, size2
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (associated(arr)) then
       if ( size1 == size(arr,1) .and. size2 == size(arr,2) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1,size2), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0,a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1, ' x ', size2
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_single2d_p

subroutine realloc_check_double2d(arr, size1, size2, arr_name )
   implicit none
   real(kind=dp), allocatable, intent(inout)    :: arr(:,:)
   integer, intent(in)                          :: size1, size2
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr,1) .and. size2 == size(arr,2) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1,size2), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0,a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1, ' x ', size2
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_double2d

subroutine realloc_check_double2d_p(arr, size1, size2, arr_name )
   implicit none
   real(kind=dp), pointer, intent(inout)         :: arr(:,:)
   integer, intent(in)                          :: size1, size2
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (associated(arr)) then
       if ( size1 == size(arr,1) .and. size2 == size(arr,2) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1,size2), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0,a,i0)' ) 'Error allocating array "', trim(arr_name), '" - requested size: ', size1, ' x ', size2
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_double2d_p

!> Routine to allocate a three-dimensional array and check that it succeeded
!! The array is deallocated if necessary and then allocated to the right size.
!! If an error occurs, this is reported via FatalError.
!! No arguments other than the bare ones, as in practice we do not use anything
!! but the size.
subroutine realloc_check_int3d(arr, size1, size2, size3, arr_name )
   implicit none
   integer, allocatable, intent(inout)          :: arr(:,:,:)
   integer, intent(in)                          :: size1, size2, size3
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr,1) .and. size2 == size(arr,2) .and. size3 == size(arr,3) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1,size2,size3), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0,a,i0,a,i0)' ) 'Error allocating array "', trim(arr_name), &
           '" - requested size: ', size1, ' x ', size2, ' x ', size3
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_int3d

subroutine realloc_check_single3d(arr, size1, size2, size3, arr_name )
   implicit none
   real(kind=sp), allocatable, intent(inout)    :: arr(:,:,:)
   integer, intent(in)                          :: size1, size2, size3
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr,1) .and. size2 == size(arr,2) .and. size3 == size(arr,3) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1,size2,size3), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0,a,i0,a,i0)' ) 'Error allocating array "', trim(arr_name), &
           '" - requested size: ', size1, ' x ', size2, ' x ', size3
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_single3d

subroutine realloc_check_double3d(arr, size1, size2, size3, arr_name )
   implicit none
   real(kind=dp), allocatable, intent(inout)    :: arr(:,:,:)
   integer, intent(in)                          :: size1, size2, size3
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (allocated(arr)) then
       if ( size1 == size(arr,1) .and. size2 == size(arr,2) .and. size3 == size(arr,3) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1,size2,size3), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0,a,i0,a,i0)' ) 'Error allocating array "', trim(arr_name), &
           '" - requested size: ', size1, ' x ', size2, ' x ', size3
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_double3d

subroutine realloc_check_single4d_p(arr, size1, size2, size3, size4, arr_name )
   implicit none
   real(kind=sp), pointer                       :: arr(:,:,:,:)
   integer, intent(in)                          :: size1, size2, size3, size4
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (associated(arr)) then
       if ( size1 == size(arr,1) .and. size2 == size(arr,2) .and. size3 == size(arr,3) .and. size4 == size(arr,4) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1,size2,size3,size4), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0,4(a,i0))' ) 'Error allocating array "', trim(arr_name), &
           '" - requested size: ', size1, ' x ', size2, ' x ', size3, ' x ', size4
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_single4d_p

subroutine realloc_check_double4d_p(arr, size1, size2, size3, size4, arr_name )
   implicit none
   real(kind=dp), pointer                       :: arr(:,:,:,:)
   integer, intent(in)                          :: size1, size2, size3, size4
   character(len=*)                             :: arr_name

   integer                                      :: localErr
   character(len=200)                           :: msg

   if (associated(arr)) then
       if ( size1 == size(arr,1) .and. size2 == size(arr,2) .and. size3 == size(arr,3) .and. size4 == size(arr,4) ) then
           return
       else
           deallocate( arr )
       endif
   endif

   allocate( arr(size1,size2,size3,size4), stat = localErr )

   if ( localErr /= 0 ) then
       write( msg, '(3a,i0,4(a,i0))' ) 'Error allocating array "', trim(arr_name), &
           '" - requested size: ', size1, ' x ', size2, ' x ', size3, ' x ', size4
       call fatalError( msg )
       return
   endif
end subroutine realloc_check_double4d_p

end module m_realloc_check
