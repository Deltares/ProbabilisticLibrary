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

!> @file vectorUtilities.f90
!!  Implement various utility functions for vectors
!
!>
!! Module with vector utility functions
!!   @ingroup general
module vectorUtilities

    use precision
    use feedback
    use errorMessages

    implicit none

    private
    public :: normalize, ssort, logLinearInterpolate, LinearInterpolate, logLinearInterpolateCyclic, LinearInterpolateCyclic
    public :: trapeziumRule, logLinearInterpolateInv
    public :: linearInterpolateAngles, linearInterpolateMulti, linearInterpolateMulti2D
    public :: resize, interpolateLine
    public :: tMessage

    interface resize
        module procedure resize1Dreal
        module procedure resize2Dreal
    end interface

    real (kind=wp), parameter      :: eps = 1d-200     ! epsilon to avoid division by zero
    real (kind=wp), parameter      :: tol = 1d-4       ! tolerance in comparing reals
    integer, parameter             :: isUnSorted = 0   ! result of check_order
    integer, parameter             :: isDescending = 1 ! idem
    integer, parameter             :: isAscending = -1 ! idem

    real(kind=wp), parameter :: pi = 4.0_wp * atan(1.0_wp)

contains

!> Function for normalizing a vector
!! If the vector is the null vector, a unit vector with uniform components is returned instead
!!   @ingroup general
function normalize( x )

    real (kind=wp), intent(in)  :: x(:)                !< vector to be normalized
    real (kind=wp)              :: normalize(size(x))  !< output vector

    real (kind=wp)              :: sumOfSquares
    integer                     :: L

    L = size(x)
    sumOfSquares = sqrt(sum(x(1:L)**2))

    if (sumOfSquares > 0.0d0) then
        normalize = x/sumOfSquares
    else
        normalize = 1.0d0 / sqrt(dble(L))
    endif

end function normalize

!> Function to interpolate and extrapolate linear on a line
!!   @ingroup general
function interpolateLine(x1, x2, f1, f2, x, error)
    real (kind=wp), intent(in)      :: x1              !< first x-value
    real (kind=wp), intent(in)      :: x2              !< second x-value
    real (kind=wp), intent(in)      :: f1              !< first function-value
    real (kind=wp), intent(in)      :: f2              !< second function-value
    real (kind=wp), intent(in)      :: x               !< x-value for which the function value is desired
    type(tMessage),   intent(out)   :: error           !< error code/message
    real (kind=wp)                  :: interpolateLine !< value of the dependent variable associated with xx

    real (kind=wp)                 :: dx               ! difference between x1 and x2
    real (kind=wp)                 :: dx2              ! difference between x1 and x

    error = errorNone
    dx = x2 - x1
    dx2 = x - x1
    if (abs(dx) > eps) then
        ! interpolate and extrapolate linear on a line
        interpolateLine = f1 + dx2 / dx * (f2 - f1)
    else
        if (abs(dx2) > eps) then
            error%errorCode = -1
            error%message   = 'division by zero in interpolateLine'
        else
            interpolateLine = 0.5_wp * (f1 + f2)
        endif
    endif

end function interpolateLine

!> Function for linearly interpolating and extrapolating
!!   @ingroup general
function linearInterpolate( X, Y, xx, error )

    real (kind=wp),   intent(in)    :: X(:)                !< vector with independent elements X - in increasing order
    real (kind=wp),   intent(in)    :: Y(:)                !< vector with dependent elements Y (depending on X)
    real (kind=wp),   intent(in)    :: xx                  !< x-value for which the dependent variable is desired
    type(tMessage),   intent(out)   :: error               !< error code/message
    real (kind=wp)                  :: linearInterpolate   !< value of the dependent variable associated with xx

    integer                              :: nelem     ! number of elements in the vector X
    integer                              :: idx       ! sought index
    integer                              :: idxm1     ! sought index - 1

    linearInterpolate = 0
    error = errorNone
    nelem = size(X,1)
    !
    ! Take care of a special case: only one data point
    !
    if ( nelem == 1 ) then
        linearInterpolate = y(1)
        return
    else if ( nelem == 0 ) then
        error%errorCode = 1
        error%message   = "linearInterpolation: no data points"
    endif
    !
    ! Check that X and Y are the same size
    !
    if ( size(x) /= size(y) ) then
        error%errorCode = 1
        error%message   = "linearInterpolation: x- and y-vectors should have the same length!"
    endif

    if (error%errorCode == 0) then

        call LocateNeighbours( x, xx, idx, idxm1)

        linearInterpolate = interpolateLine( x(idxm1), x(idx), Y(idxm1), Y(idx), xx, error )

    endif

end function linearInterpolate

!> Function for linearly interpolating and extrapolating
!!   @ingroup general
function linearInterpolateMulti( X, Y, xx, error, nUnique )

    real (kind=wp),   intent(in)    :: X(:)                !< vector with independent elements X - in increasing order
    real (kind=wp),   intent(in)    :: Y(:,:)              !< vector with dependent elements Y (depending on X)
    real (kind=wp),   intent(in)    :: xx                  !< x-value for which the dependent variable is desired
    type(tMessage),   intent(out)   :: error               !< error code/message
    integer, optional, intent(in)   :: nUnique             !< If provided, then only the first nUnique elements of X are unique, the others are ignored
    real (kind=wp)                  :: linearInterpolateMulti(size(Y,dim=2)) !< value of the dependent variables associated with xx

    integer                              :: nelem     ! number of elements in the vector X
    integer                              :: i         ! do-loop counter
    integer                              :: idx       ! sought index
    integer                              :: idxm1     ! sought index - 1

    linearInterpolateMulti = 0
    error = errorNone
    nelem = size(X,1)
    !
    ! Take care of a special case: only one data point
    !
    if ( nelem == 1 ) then
        linearInterpolateMulti = y(1,:)
        return
    else if ( nelem == 0 ) then
        error%errorCode = 1
        error%message   = "linearInterpolation: no data points"
    endif
    !
    ! Check that X and Y are the same size
    !
    if ( size(x) /= size(y,dim=1) ) then
        error%errorCode = 1
        error%message   = "linearInterpolation: x- and y-vectors should have the same length!"
    endif

    if (error%errorCode == 0) then

        if (present(nUnique)) then
            call LocateNeighbours( x(1:nUnique), xx, idx, idxm1)
        else
            call LocateNeighbours( x, xx, idx, idxm1)
        end if

        do i = 1,size(Y,dim=2)
            linearInterpolateMulti(i) = interpolateLine( x(idxm1), x(idx), Y(idxm1,i), Y(idx,i), xx, error )
        end do

    endif

end function linearInterpolateMulti

!> Function for linearly interpolating and extrapolating
!!   @ingroup general
function linearInterpolateMulti2D( X1, X2, Y, xx1, xx2, error )

    real (kind=wp),   intent(in)    :: X1(:)                                    !< vector with independent elements X1 - in increasing order
    real (kind=wp),   intent(in)    :: X2(:)                                    !< vector with independent elements X2 - in increasing order
    real (kind=wp),   intent(in)    :: Y(:,:,:)                                 !< vector with dependent elements Y (depending on X1 and X2)
    real (kind=wp),   intent(in)    :: xx1                                      !< x1-value for which the dependent variable is desired
    real (kind=wp),   intent(in)    :: xx2                                      !< x2-value for which the dependent variable is desired
    type(tMessage),   intent(out)   :: error                                    !< error code/message
    real (kind=wp)                  :: linearInterpolateMulti2D(size(Y,dim=3))  !< value of the dependent variables associated with xx

    real (kind=wp), allocatable     :: Y2(:,:)
    integer                         :: nelem1       ! number of elements in the vector X
    integer                         :: nelem2       ! number of elements in the vector X
    integer                         :: j            ! do-loop counter
    integer                         :: ierr

    allocate( y2( size(Y,dim=2),size(Y,dim=3) ), stat=ierr )
    if (ierr /= 0) then
        error%errorCode = 1
        write(error%message,*) 'Allocate error in linearInterpolateMulti2D with size ', size(Y,dim=2), ' x ', size(Y,dim=3)
        return
    endif

    linearInterpolateMulti2D = 0
    error  = errorNone
    nelem1 = size(X1,1)
    nelem2 = size(X2,1)

    !
    ! Check that X and Y are the same size
    !
    if ( size(x1) /= size(y,dim=1) ) then
        error%errorCode = 1
        error%message   = "linearInterpolation: x- and y-vectors should have the same length!"
    endif
    if ( size(x2) /= size(y,dim=2) ) then
        error%errorCode = 1
        error%message   = "linearInterpolation: x- and y-vectors should have the same length!"
    endif

    !
    ! Take care of a special case: only one data point
    !
    if ( nelem1 == 1 ) then
        y2(:,:) = y(1,:,:)
    else if ( nelem1 == 0 ) then
        error%errorCode = 1
        error%message   = "linearInterpolation: no data points"
    else
        do j = 1, nelem2
            y2(j,:)=linearInterpolateMulti( X1, Y(:,j,:), xx1, error )
        end do
    endif

    !
    if (error%errorCode == 0) then
        linearInterpolateMulti2D = linearInterpolateMulti( X2, Y2, xx2, error )
    endif

end function linearInterpolateMulti2D

!> Function for log-linearly interpolating and extrapolating
!!   @ingroup general
function logLinearInterpolate( X, Y, xx, error )

    real (kind=wp),   intent(in)    :: X(:)                  !< vector with independent elements X - may be unsorted
    real (kind=wp),   intent(in)    :: Y(:)                  !< vector with dependent elements Y (depending on X)
    real (kind=wp),   intent(in)    :: xx                    !< x-value for which the dependent variable is desired
    type(tMessage),   intent(out)   :: error                 !< error code/message
    real (kind=wp)                  :: logLinearInterpolate  !< value of the dependent variable associated with xx
!
!   locals
!
    integer                              :: nelem     ! number of elements in the vector X
    integer                              :: idx       ! sought index
    integer                              :: idxm1     ! sought index - 1
    real(kind=wp)                        :: x1        ! nearest x below xx
    real(kind=wp)                        :: x2        ! nearest x above xx

    error = errorNone
    nelem = size(X,1)

    !
    ! Check that X and Y are the same size
    !
    if ( size(x) /= size(y) ) then
        error%errorCode = 1
        error%message   = "loglinearInterpolation: x- and y-vectors should have the same length!"
    endif
    !
    ! Take care of a special case: only one data point
    !
    if ( nelem == 1 ) then
        logLinearInterpolate = y(1)
        return
    endif
    !
    ! This is log-interpolation: check that elements of x have positive values
    if ( any(x < 0 ) ) then
        error%errorCode = 1
        error%message   = "The vector X in function loglinearInterpolate contains non-positive values!"
    endif
    !
    ! Also check that xx is positive
    if (xx <=0) then
        error%errorCode = 1
        error%message   = "Argument xx in function loglinearInterpolate has non-positive value!"
    endif
    !
    !
    if (error%errorCode == 0) then

        call LocateNeighbours( x, xx, idx, idxm1)

        x1 = x(idxm1)
        x2 = x(idx)
        if (abs(x1 - x2) < eps) then
            if (abs(xx - x1) < tol) then
                logLinearInterpolate = 0.5_wp * (Y(idxm1) + Y(idx))
            else
                call set_nan(logLinearInterpolate)
                error%errorCode = -1
                error%message   = 'equal x values found in logLinearInterpolate'
            endif
        else if (x1 > 0.0_wp .and. x2 > 0.0_wp) then
            logLinearInterpolate = interpolateLine( log(x1), log(x2), Y(idxm1), Y(idx), log(xx), error )
        else
            !
            ! fall back on normal interpolation
            !
            logLinearInterpolate = interpolateLine( x1, x2, Y(idxm1), Y(idx), xx, error )
        endif

    endif

end function logLinearInterpolate

!> Function for inverse log-linearly interpolating and extrapolating
!!   @ingroup general
function logLinearInterpolateInv( X, Y, xx, error )

    real (kind=wp),   intent(in)    :: X(:)                     !< vector with independent elements X - may be unsorted
    real (kind=wp),   intent(in)    :: Y(:)                     !< vector with dependent elements Y (depending on X)
    real (kind=wp),   intent(in)    :: xx                       !< x-value for which the dependent variable is desired
    type(tMessage),   intent(out)   :: error                    !< error code/message
    real (kind=wp)                  :: logLinearInterpolateInv  !< value of the dependent variable associated with xx
!
!   locals
!
    integer                       :: nelem          ! number of elements in the vector X
    integer                       :: idx            ! sought index
    integer                       :: idxm1          ! sought index - 1

    error = errorNone
    nelem = size(X,1)

    ! Take care of a special case: only one data point
    !
    if ( nelem == 1 ) then
        logLinearInterpolateInv = y(1)
        return
    endif
    !
    ! Check that X and Y are the same size
    !
    if ( size(x) /= size(y) ) then
        call set_nan(logLinearInterpolateInv)
        error%errorCode = 1
        error%message   = "loglinearInterpolation: x- and y-vectors should have the same length!"
    endif

    if (error%errorCode == 0) then

        call LocateNeighbours( x, xx, idx, idxm1)

        !
        ! This is log-interpolation: check that elements of x have positive values
        ! and perform the calculation
        if ( Y(idxm1) <= 0 .or. Y(idx) <= 0 ) then
            call set_nan(logLinearInterpolateInv)
            error%errorCode = 1
            error%message   = "The vector Y in function loglinearInterpolate contains non-positive values!"
        else
            logLinearInterpolateInv = exp(interpolateLine( x(idxm1), x(idx), log(Y(idxm1)), log(Y(idx)), xx, error ))
        endif

    endif

end function logLinearInterpolateInv

!> Function for linearly interpolating cyclic data
!!   @ingroup general
function linearInterpolateCyclic( X, Y, xx, error )

    real (kind=wp),   intent(in)    :: X(:)                    !< vector with independent elements X - in increasing order
    real (kind=wp),   intent(in)    :: Y(:)                    !< vector with dependent elements Y (depending on X)
    real (kind=wp),   intent(in)    :: xx                      !< x-value for which the dependent variable is desired
    type(tMessage),   intent(out)   :: error                   !< error code/message
    real (kind=wp)                  :: linearInterpolateCyclic !< value of the dependent variable associated with xx

    integer                              :: nelem     ! number of elements in the vector X
    integer                              :: i         ! do-loop counter
    integer                              :: idx       ! sought index
    integer                              :: idxm1     ! sought index - 1
    real (kind=wp), dimension(size(X,1)) :: xsorted   ! sorted x
    integer, dimension(size(X,1))        :: IDXsorted ! sorted indices of x vector

    error = errorNone
    nelem = size(X,1)
    !
    ! Take care of a special case: only one data point
    !
    if ( nelem == 1 ) then
        linearInterpolateCyclic = y(1)
        return
    endif
    !
    ! Check that X and Y are the same size
    !
    if ( size(x) /= size(y) ) then
        error%errorCode = 1
        error%message   = "linearInterpolationCyclic: x- and y-vectors should have the same length!"
    endif

    if (error%errorCode == 0) then
        !
        ! sort x in descending order
        IDXsorted = (/(i,i=1,nelem)/)
        Xsorted=x

        ! Find first index in X for which X(index) exceeds xx
        call  SortAndLocate( Xsorted, IDXsorted, xx, idx, idxm1, .True. )
        !
        linearInterpolateCyclic = interpolateLine( Xsorted(idxm1), Xsorted(idx), Y(IDXsorted(idxm1)), Y(IDXsorted(idx)), xx, error )

    endif

end function linearInterpolateCyclic

!> Function for log-linearly interpolating and extrapolating cyclic data
!!   @ingroup general
function logLinearInterpolateCyclic( X, Y, xx, error )

    real (kind=wp),   intent(in)    :: X(:)                        !< vector with independent elements X - in decreasing order
    real (kind=wp),   intent(in)    :: Y(:)                        !< vector with dependent elements Y (depending on X)
    real (kind=wp),   intent(in)    :: xx                          !< x-value for which the dependent variable is desired
    type(tMessage),   intent(out)   :: error                       !< error code/message
    real (kind=wp)                  :: logLinearInterpolateCyclic  !< value of the dependent variable associated with xx

    real (kind=wp), dimension(size(X,1)) :: xsorted ! sorted x

    integer                       :: nelem          ! number of elements in the vector X
    integer                       :: i              ! do-loop counter
    integer                       :: idx            ! sought index
    integer                       :: idxm1          ! sought index - 1
    integer, dimension(size(X,1)) :: IDXsorted      ! sorted indices of x vector
    real(kind=wp)                 :: x1             ! nearest x below xx
    real(kind=wp)                 :: x2             ! nearest x above xx

    error = errorNone
    nelem = size(X,1)

    ! Take care of a special case: only one data point
    !
    if ( nelem == 1 ) then
        logLinearInterpolateCyclic = y(1)
        return
    endif
    !
    ! Check that X and Y are the same size
    !
    if ( size(x) /= size(y) ) then
        error%errorCode = 1
        error%message   = "loglinearInterpolationCyclic: x- and y-vectors should have the same length!"
    endif
    !
    ! This is log-interpolation: check that elements of x have positive values
    if ( any(x < 0) ) then
        error%errorCode = 1
        error%message   = "The vector X in function loglinearInterpolateCyclic contains non-positive values!"
    endif
    !
    ! Also check that xx is positive
    if (xx <=0) then
        error%errorCode = 1
        error%message   = "Argument xx in function loglinearInterpolateCyclic has non-positive value!"
    endif
    !
    !

    if (error%errorCode == 0) then
        !
        ! sort x in descending order
        IDXsorted = (/(i,i=1,nelem)/)
        Xsorted=x

        ! Find first index in X for which X(index) exceeds xx
        call  SortAndLocate( Xsorted, IDXsorted, xx, idx, idxm1, .True. )

        x1 = Xsorted(idxm1)
        x2 = Xsorted(idx)
        if (x1 > 0.0_wp .and. x2 > 0.0_wp) then
            logLinearInterpolateCyclic = interpolateLine( log(x1), log(x2), Y(IDXsorted(idxm1)), Y(IDXsorted(idx)), log(xx), error )
        else
            logLinearInterpolateCyclic = interpolateLine( x1, x2, Y(IDXsorted(idxm1)), Y(IDXsorted(idx)), xx, error )
        endif

    endif

end function logLinearInterpolateCyclic

!> Function for linearly interpolating and extrapolating angles
!!   @ingroup general
function linearInterpolateAngles( X, Y, xx, error, sinY )

    real (kind=wp),   intent(in)    :: X(:)                     !< vector with independent elements X
    real (kind=wp),   intent(in)    :: Y(:)                     !< vector with dependent elements Y (depending on X)
    real (kind=wp),   intent(in)    :: xx                       !< x-value for which the dependent variable is desired
    type(tMessage),   intent(out)   :: error                    !< error code/message
    real (kind=wp),   intent(in), optional :: sinY(:)           !< If present, then Y is actually the X-part of the unit direction vector and this is the Y-part of the unit direction vector
    real (kind=wp)                  :: linearInterpolateAngles  !< value of the dependent variable associated with xx

    integer                     :: nelem                    ! number of elements in the vector X
    real(kind=wp)               :: sinAngle                 ! sine   of result variable associated with xx
    real(kind=wp)               :: cosAngle                 ! cosine of result variable associated with xx
    real(kind=wp)               :: yy                       ! interim result variable
    integer                     :: idx                      ! sought index
    integer                     :: idxm1                    ! sought index - 1

    linearInterpolateAngles = 0
    error = errorNone
    nelem = size(X,1)
    !
    ! Take care of a special case: only one data point
    !
    if ( nelem == 1 ) then
        linearInterpolateAngles = y(1)
        return
    else if ( nelem == 0 ) then
        error%errorCode = 1
        error%message   = "linearInterpolateAngles: no data points"
    endif
    !
    ! Check that X and Y are the same size
    !
    if ( size(x) /= size(y) ) then
        error%errorCode = 1
        error%message   = "linearInterpolateAngles: x- and y-vectors should have the same length!"
    endif
    if (present(sinY)) then
        if ( size(x) /= size(sinY) ) then
            error%errorCode = 1
            error%message   = "linearInterpolateAngles: x- and y-vectors should have the same length!"
        endif
    endif

    if (error%errorCode == 0) then

        call LocateNeighbours( x, xx, idx, idxm1)

        ! Determine cosine and sine of result variable through inter- or extrapolation
        if (present (sinY)) then
                                      cosAngle = interpolateLine( x(idxm1), x(idx),    Y(idxm1),    Y(idx), xx, error ) ! Y is actually sin(Y*pi/180)
            if (error%errorCode == 0) sinAngle = interpolateLine( x(idxm1), x(idx), sinY(idxm1), sinY(idx), xx, error )
        else
                           cosAngle = interpolateLine( x(idxm1), x(idx), cos(Y(idxm1) * pi / 180.0D0), cos(Y(idx) * pi /180.0D0), &
                                       xx, error )
            if (error%errorCode == 0) then
            sinAngle = interpolateLine( x(idxm1), x(idx), sin(Y(idxm1) * pi / 180.0D0), sin(Y(idx) * pi /180.0D0), xx, error )
            end if
        end if

        if (error%errorCode == 0) then
            ! Determine result variable based on sine and cosine of the angle
            if ( (sinAngle > 0.0D0) .AND. (cosAngle > 0.0D0) ) then
                yy = atan( sinAngle/cosAngle )                ! sin > 0  and  cos > 0
            elseif ( (abs(sinAngle) < almostzero) .AND. (cosAngle > 0.0D0) ) then
                yy = 0.0D0                                    ! sin = 0  and  cos > 0
            elseif (cosAngle > 0.0D0) then
                yy = 2.0D0 * pi + atan( sinAngle/cosAngle )   ! sin < 0  and  cos > 0
            elseif ( (sinAngle > 0.0D0) .AND. (abs(cosAngle) < almostzero) ) then
                yy = pi / 2.0D0                               ! sin > 0  and  cos = 0
            elseif ( (abs(cosAngle) < almostzero) ) then
                yy = 3.0D0 * pi / 2.0D0                       ! sin < 0  and  cos = 0
            else
                yy = pi + atan( sinAngle/cosAngle )           ! cos < 0
            endif
            linearInterpolateAngles = yy * 180.0D0 / pi
        else
            call set_nan(linearInterpolateAngles)
        endif
    endif

end function linearInterpolateAngles

!> Subroutine to bracket a given value xx within two elements of an array after sort in descending order.
!!   @ingroup general
subroutine SortAndLocate( X, IDXsorted, xx, locID, locIDm1, Cyclic )
    real (kind=wp), intent(inout)  :: X(:)                           !< vector with independent elements X - in decreasing order
    integer                        :: IDXsorted(:)                   !< sorted indices of x vector
    real (kind=wp), intent(in)     :: xx                             !< x-value for which the dependent variable is desired
    integer                        :: locID                          !< sought index
    integer                        :: locIDm1                        !< sought index - 1
    logical                        :: Cyclic                         !< a flag to be true if data are cyclic; false if not

! Working variables
    integer                        :: nelem                          ! number of elements in the vector X
    integer                        :: i                              ! do-loop counter
    integer                        :: ordered                        ! X is descending, ascending or unsorted

    nelem = size(X,1)
    locID = nelem + 1 !initialize

    ordered = check_order(X, nelem)
    !
    ! sort x in descending order
    if (ordered == isUnSorted) then
        call ssort(X, IDXsorted, nelem, -1)
        ordered = isDescending
    endif

    ! Find first index in X for which X(index) exceeds xx
    !
    if (ordered == isDescending) then
        do i = 1,nelem
            if (X(i) <= xx) then
                locID   = i
                locIDm1 = i-1
                exit
            endif
        enddo
    else
        do i = 1,nelem
            if (X(i) >= xx) then
                locID   = i
                locIDm1 = i-1
                exit
            endif
        enddo
    endif
    !
    ! Interpolate or extrapolate? In both cases linear, to avoid zero derivatives
    !
    if (.Not. Cyclic) then
        if ( locID <= 1 ) then
            locID   = 2
            locIDm1 = 1
        endif
        if ( locID == nelem+1 ) then
            locID   = nelem
            locIDm1 = nelem - 1
        endif
    else ! then data are cyclic
        if ( locID == 1 .and. X(1) <= xx) then
            locID   = nelem
            locIDm1 = 1
        endif
        if (locID == nelem+1 )then
            locID   = nelem
            locIDm1 = 1
        endif
    endif

end subroutine SortAndLocate

!> Subroutine to bracket a given value xx within two elements of an array.
!!   @ingroup general
subroutine LocateNeighbours( X, xx, locID, locIDm1)
    real (kind=wp), intent(in)  :: X(:)                           !< vector with independent elements X
    real (kind=wp), intent(in)  :: xx                             !< x-value for which the dependent variable is desired
    integer, intent(out)        :: locID                          !< sought index
    integer, intent(out)        :: locIDm1                        !< sought index - 1

    ! Working variables
    integer        :: nelem                          ! number of elements in the vector X
    integer        :: i                              ! do-loop counter
    logical        :: ordered

    nelem = size(X,1)

    !First the case that probably occurs most of the time: ascending order
    !Test if ascending order is indeed the case, and immediately locate the index just above and below the sought value
    locID = 0 !initialize
    ordered = .true.
    do i = 1, nelem-1
        if (X(i) > X(i+1)) then
            ordered = .false.
            exit
        endif
        if (locID==0) then
            if (X(i) >= xx) then
                locID   = i
                locIDm1 = i-1
            endif
        end if
    enddo

    if (.not.ordered) then

        !Next, the second possible case: descending order
        !Test if descending order is indeed the case, and immediately locate the index just above and below the sought value
        locID = 0 !initialize
        ordered = .true.
        do i = 1, nelem-1
            if (X(i) < X(i+1)) then
                ordered = .false.
                exit
            endif
            if (locID==0) then
                if (X(i) <= xx) then
                    locID   = i
                    locIDm1 = i-1
                endif
            endif
        enddo

    endif

    if (ordered) then

        !
        ! Interpolate or extrapolate? In both cases linear, to avoid zero derivatives
        !
        if ( locID == 1 ) then
            locID   = 2
            locIDm1 = 1
        endif
        if ( locID == 0 ) then
            locID   = nelem
            locIDm1 = nelem - 1
        endif

    else

        !Last, the most improbable and most compute intensive case
        call LocateUnsorted( X, xx, locID, locIDm1)

    end if

end subroutine LocateNeighbours

!> Subroutine to bracket a given value xx within two elements of an unsorted array.
!!   @ingroup general
subroutine LocateUnsorted( X, xx, locID, locIDm1)
    real (kind=wp), intent(in)  :: X(:)                           !< vector with independent elements X - in decreasing order
    real (kind=wp), intent(in)  :: xx                             !< x-value for which the dependent variable is desired
    integer, intent(out)        :: locID                          !< sought index
    integer, intent(out)        :: locIDm1                        !< sought index - 1

! Working variables
    integer        :: nelem                          ! number of elements in the vector X
    integer        :: i                              ! do-loop counter
    real (kind=wp) :: dX,dXlocID, dXlocIDm1, xxtemp

    !initialize
    nelem = size(X,1)
    locID = 0
    locIdm1 = 0
    dXlocID = huge(X(1))
    dXlocIDm1 = -dXlocID

    !Find indices for the values in X that are just above and just below xx
    do i = 1,nelem
        dX = X(i)-xx
        if (dX>eps) then
            !This point in X(:) lies above xx -> maybe a candidate for locID?
            if (dXlocID>dX) then
                locID = i
                dXlocID = dX
            end if
        else if (dX<-eps) then
            !This point in X(:) lies below xx -> maybe a candidate for locIDm1?
            if (dXlocIDm1<dX) then
                locIDm1 = i
                dXlocIDm1 = dX
            end if
        else
            !We have found a point in X(:) that is exactly equal to xx -> no need to search any further!
            locID = i
            locIDm1 = i
            return
        end if
    end do

    !Deal with the cases xx<min(X) and xx>max(X) to allow linear extrapolation instead of interpolation
    if (locID==0) then
        !No points in X(:) above xx found: take the point of max(X) for locID and the one directly below it for locIDm1
        locID = maxloc(X,1)
        xxTemp = X(locID)
        dXlocIDm1 = -huge(X(1))
        do i = 1,nelem
            dX = X(i)-xxTemp
            if (dX<-eps) then
                !This point in X(:) lies below xxTemp -> maybe a candidate for locIDm1?
                if (dXlocIDm1<=dX) then
                    locIDm1 = i
                    dXlocIDm1 = dX
                end if
            end if
        end do
    else if (locIDm1==0) then
        !No points in X(:) below xx found: take the point of min(X) for locIDm1 and the one directly above it for locID
        locIDm1 = minloc(X,1)
        xxTemp = X(locIDm1)
        dXlocID = huge(X(1))
        do i = 1,nelem
            dX = X(i)-xxTemp
            if (dX>eps) then
                !This point in X(:) lies above xxTemp -> maybe a candidate for locID?
                if (dXlocID>=dX) then
                    locID = i
                    dXlocID = dX
                end if
            end if
        end do
    end if

end subroutine LocateUnsorted

!> Function to check whether an array is sorted or not
!> Result is: isUnSorted, isDescending or isAscending
!!   @ingroup general
integer function check_order(X, nelem)
integer, intent(in) :: nelem          !< array size
real(kind=wp), intent(in) :: X(nelem) !< array to check

integer :: i            ! loop counter
logical :: descending   ! array is descending
logical :: ascending    ! array is ascending

descending = .true.
do i = 1, nelem-1
    if (X(i) < X(i+1)) then
        descending = .false.
        exit
    endif
enddo

ascending = .true.
do i = 1, nelem-1
    if (X(i) > X(i+1)) then
        ascending = .false.
        exit
    endif
enddo

if (descending) then
    check_order = isDescending
else if (ascending) then
    check_order = isAscending
else
    check_order = isUnSorted
endif
end function check_order

!> Function for the Trapezium rule
!!   @ingroup general
function stepSize( i, x, error )

    integer,          intent(in)    :: i             !< index of the vector x
    real (kind=wp),   intent(in)    :: x(:)          !< vector of x-values
    type(tMessage),   intent(out)   :: error         !< error code/message
    real (kind=wp)                  :: stepSize      !< Step size

    integer                     :: nx                ! size of the vector x

    error = errorNone
    nx = size(x,1)
    if ((i < 1) .or. (i > nx)) then
        !
        ! Give fatal error if i isn't an element of the vector x
        !
        error%errorCode = 1
        write(error%message, '(a,i0)') "The index i in the function stepSize isn't an element of the vector x", i
        call set_nan(stepSize)
    elseif (i == nx) then
        stepSize = (x(nx)  - x(nx-1)) / 2.0d0
    elseif (i == 1) then
        stepSize = (x(2)   - x(1)   ) / 2.0d0
    else
        stepSize = (x(i+1) - x(i-1) ) / 2.0d0
    endif

end function stepSize

!> Function for the Trapezium rule
!!   @ingroup general
function trapeziumRule( x, f, error )

    real (kind=wp), intent(in)      :: x(:)            !< vector of x-values
    real (kind=wp), intent(in)      :: f(:)            !< vector of function-values
    type(tMessage),   intent(out)   :: error         !< error code/message
    real (kind=wp)                  :: trapeziumRule   !< value after the Trapezium Rule

    integer                     :: ix                  ! index in the vector x
    integer                     :: nx                  ! size of the vector x

    error = errorNone
    !
    ! Check that x and f are the same size
    if (size(x) /= size(f)) then
        !
        ! Give fatal error if x and f aren't of the same size
        !
        error%errorCode = 1
        write(error%message, '(a,i0)') "The two vector in the function trapezium rule aren't of the same size", nx
    endif
    !
    ! Compute the integral by using the Trapezium rule
    !
    nx = size(x,1)
    trapeziumRule = 0.0
    do ix = 1, nx
        if (error%errorCode == 0) then
            trapeziumRule = trapeziumRule + f(ix) * stepSize (ix, x, error )
        endif
    enddo

end function trapeziumRule

!> Subroutine to sort a vector in either ascending or descending order.
!!   @ingroup general
subroutine ssort (x, iy, n, kflag, nUnique)
    integer, intent(in)            :: n                            !< size of x and iy
    real (kind=wp)                 :: x(n)                         !< vector of x-values
    integer, intent(inout)         :: iy(n)                        !< vector of indices to contain sorted indices of x
    integer, intent (in)           :: kflag                        !< a flag to indicate ascending(positive) or descending (negative) sorting
    integer, optional, intent(out) :: nUnique                      !< if provided, the first nUnique array elements in x contain only unique values
                                                                   !! and the non-unique elements are moved to x(nUnique+1:n)

!     .. local scalars ..
    integer        :: i, iswap(1), iswap1
    integer        :: ntemp

    ntemp = n
    i=1
    do while (i <= ntemp)
        if (i < ntemp) then
            if (kflag < 0) then
               iswap=maxloc(x(i:ntemp))
            else
               iswap=minloc(x(i:ntemp))
            endif
            iswap1=iswap(1)+i-1
            if(iswap1/=i) then
                call swapr(x(i), x(iswap1))
                call swapi(iy(i), iy(iswap1))
            endif
        endif

        if (present(nUnique).and.(i>1)) then
            if (x(i) == x(i-1)) then
                ! A non-unique value is encountered. Move it outside of the first ntemp values in x and iy
                ntemp = ntemp-1
                call swapr(x(ntemp+1), x(i))
                call swapi(iy(ntemp+1), iy(i))
                i=i-1 !The value moved to x(i) from x(ntemp+1) still has to be sorted, hence we redo i=i-1+1
            end if
        end if
        i=i+1

    enddo

    if (present(nUnique)) nUnique = ntemp

end subroutine ssort

!> helper routine to swap 2 integers in sorting routine
subroutine swapi(i1, i2)
    integer, intent(inout) :: i1  !< first number to swap
    integer, intent(inout) :: i2  !< second number to swap

    integer                :: ii

    ii = i1
    i1 = i2
    i2 = ii
end subroutine swapi

!> helper routine to swap 2 doubles in sorting routine
subroutine swapr(r1, r2)
    real(kind=wp), intent(inout) :: r1  !< first number to swap
    real(kind=wp), intent(inout) :: r2  !< second number to swap

    real(kind=wp)                :: rr

    rr = r1
    r1 = r2
    r2 = rr
end subroutine swapr

!> helper function to avoid unnecessary deallocate/allocate of an 1D array
subroutine resize1Dreal(array, newSize, keep)
    real(kind=wp), allocatable, intent(inout) :: array(:)     !< Array to be resized
    integer                   , intent(in)    :: newSize      !< new size for array
    logical, optional         , intent(in)    :: keep         !< copy values to new array

    logical :: ready   ! flag to indicate that allocate can be skipped
    logical :: keep_   ! local copy of keep
    integer :: ierr    ! error code of allocate
    integer :: overlap, i
    real(kind=wp), allocatable :: new_array(:)

    keep_ = .false.
    if (present(keep)) keep_ = keep

    ready = .false.
    if (allocated(array)) then
        if (size(array) == newSize) then
            ready = .true.
        endif
    endif

    if (.not. ready) then
        allocate(new_array(newSize), stat=ierr)
        if ( ierr /= 0 ) call fatalError ( "Allocation of array failed - size: ", newSize )
        if (allocated(array)) then
            if (keep_) then
                overlap = min(newSize, size(array))
                do i = 1, overlap
                    new_array(i) = array(i)
                enddo
            endif
            deallocate(array)
        endif
        call move_alloc(new_array, array)
    endif
end subroutine resize1Dreal

!> helper function to avoid unnecessary deallocate/allocate of a 2D array
subroutine resize2Dreal(array, newSize1, newSize2)
    real(kind=wp), allocatable, intent(inout) :: array(:,:)    !< Array to be resized
    integer                   , intent(in)    :: newSize1      !< new size first dimension for array
    integer                   , intent(in)    :: newSize2      !< new size second dimension for array

    logical :: ready   ! flag to indicate that allocate can be skipped
    integer :: ierr    ! error code of allocate

    ready = .false.
    if (allocated(array)) then
        if (size(array,1) == newSize1 .and. size(array,2) == newSize2 ) then
            ready = .true.
        else
            deallocate(array)
        endif
    endif

    if (.not. ready) then
        allocate(array(newSize1, newSize2), stat=ierr)
        if ( ierr /= 0 ) call fatalError ( "Allocation of array failed - size: ", newSize1 * newSize2 )
    endif
end subroutine resize2Dreal

end module vectorUtilities
