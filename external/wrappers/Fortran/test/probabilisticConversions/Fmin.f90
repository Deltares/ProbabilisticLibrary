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
!! This file contains a module with functions to perform line search
!<
!
!>
!! Module with functions to test quality of reals

module Fmin

    use precision

    implicit none

    private
    public :: golden
    
    interface golden
        module procedure golden8, golden16
    end interface golden

contains


!> golden search function modified from Numerical Recipes
!! double precision version
subroutine golden8(f,ax,bx,cx,tol,x,fx,fnExtraArg)

    real (kind=wp), intent(in)     :: ax               !< Lower bound of the searching domain
    real (kind=wp), intent(in)     :: bx               !< Upper bound of the searching domain
    real (kind=wp), intent(in)     :: cx               !< Initial value of the optimizied variable
    real (kind=wp), intent(in)     :: tol              !< Tolerance of converged optimum function value
    real (kind=wp)                 :: x                !< Optimum value of the optimized variable
    real (kind=wp)                 :: f                !< Function handler to evaluate function value
    real (kind=wp)                 :: fx               !< Optimum function value on exit
    real (kind=wp), optional       :: fnExtraArg(:)    !< A vector of extra dummary variables of "f"

    real (kind=wp)                 :: R                ! Relaxation factor
    real (kind=wp)                 :: C                !
    integer, parameter             :: maxiter=300      ! safe guard for number of iteration
 
    external f
    parameter (R=.61803399d0,C=1.d0-R)
    real *8 f1,f2,x0,x1,x2,x3
    integer iter

    !
    x0=ax
    x3=cx
    if(abs(cx-bx)>abs(bx-ax))then
        x1=bx
        x2=bx+C*(cx-bx)
    else
        x2=bx
        x1=bx-C*(bx-ax)
    endif
    if (present(fnExtraArg)) then
        f1=f(x1,fnExtraArg)
        f2=f(x2,fnExtraArg)
    else
        f1=f(x1)
        f2=f(x2)
    endif

    iter=0
    do while(abs(x3-x0) > tol*(abs(x1)+abs(x2)) .and. iter <= maxiter)
        iter=iter+1
        if(f2<f1)then
            x0=x1
            x1=x2
            x2=R*x1+C*x3
            f1=f2
            if (present(fnExtraArg)) then
                f2=f(x2,fnExtraArg)
            else
                f2=f(x2)
            endif
        else
            x3=x2
            x2=x1
            x1=R*x2+C*x0
            f2=f1
            if (present(fnExtraArg)) then
                f1=f(x1,fnExtraArg)
            else
                f1=f(x1)
            endif
        endif
    enddo
    
    if (f1<f2) then
        fx=f1
        x=x1
    else
        fx=f2
        x=x2
    endif

end subroutine golden8

!> golden search function modified from Numerical Recipes
!! quad precision version
subroutine golden16(f,ax,bx,cx,tol,x,fx,fnExtraArg)

    real (kind=qd), intent(in)     :: ax               !< Lower bound of the searching domain
    real (kind=qd), intent(in)     :: bx               !< Upper bound of the searching domain
    real (kind=qd), intent(in)     :: cx               !< Initial value of the optimizied variable
    real (kind=qd), intent(in)     :: tol              !< Tolerance of converged optimum function value
    real (kind=qd)                 :: x                !< Optimum value of the optimized variable
    real (kind=qd)                 :: f                !< Function handler to evaluate function value
    real (kind=qd)                 :: fx               !< Optimum function value on exit
    real (kind=qd), optional       :: fnExtraArg(:)    !< A vector of extra dummary variables of "f"

    real (kind=qd)                 :: R                ! Relaxation factor
    real (kind=qd)                 :: C                !
    integer, parameter             :: maxiter=300      ! safe guard for number of iteration
 
    external f
    parameter (R=.61803399q0, C=1.q0-R)
    real(kind=qd) :: f1,f2,x0,x1,x2,x3
    integer iter

    !
    x0=ax
    x3=cx
    if(abs(cx-bx)>abs(bx-ax))then
        x1=bx
        x2=bx+C*(cx-bx)
    else
        x2=bx
        x1=bx-C*(bx-ax)
    endif
    if (present(fnExtraArg)) then
        f1=f(x1,fnExtraArg)
        f2=f(x2,fnExtraArg)
    else
        f1=f(x1)
        f2=f(x2)
    endif

    iter=0
    do while(abs(x3-x0) > tol*(abs(x1)+abs(x2)) .and. iter <= maxiter)
        iter=iter+1
        if(f2<f1)then
            x0=x1
            x1=x2
            x2=R*x1+C*x3
            f1=f2
            if (present(fnExtraArg)) then
                f2=f(x2,fnExtraArg)
            else
                f2=f(x2)
            endif
        else
            x3=x2
            x2=x1
            x1=R*x2+C*x0
            f2=f1
            if (present(fnExtraArg)) then
                f1=f(x1,fnExtraArg)
            else
                f1=f(x1)
            endif
        endif
    enddo
    
    if (f1<f2) then
        fx=f1
        x=x1
    else
        fx=f2
        x=x2
    endif

end subroutine golden16

End module Fmin
