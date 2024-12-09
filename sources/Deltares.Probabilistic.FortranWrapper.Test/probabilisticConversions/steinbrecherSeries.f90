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
module steinbrecherSeries
    use precision
    use ftnunit
    
    implicit none
    
    public:: recursiveQuantile, init_series, finish_series
    private

    ! precomputed value of sqrt(pi)
    real(kind = qd), parameter :: SQRT_PI = sqrt(4.0_qd*atan(1.0_qd))
    ! precomputed value of sqrt(2)
    real(kind = qd), parameter :: SQRT_TWO = sqrt(2.0_qd)

    ! number of terms to precompute
    integer                                  :: N
    ! ratios of precomputed terms
    real(kind=qd), allocatable, dimension(:) :: ratios
    ! help array to compute ratios
    real(kind=qd), allocatable, dimension(:) :: coeF

    
    contains
    
!> recursively compute power series representation of the inverse error function, 
!! contributed by Steinbrecher (2002):
!!    http://functions.wolfram.com/GammaBetaErf/InverseErf/06/01/02/0004/
!!
    function RecursiveQuantile(z, Nmax)

    real(kind = qd), intent(in)     :: z                !< input value
    integer                         :: Nmax             !< max number of terms to use in power series
    
    real(kind = dp) :: recursiveQuantile   ! resulting value
    real(kind = qd) :: b                   ! intermediate result
    real(kind = qd) :: s                   ! intermediate result
    real(kind = qd) :: q                   ! intermediate result
    real(kind = dp) :: machine_prec        ! machine precision
    integer         :: k                   ! loop counter 
    integer         :: Nloc                ! current size of ratios array
    logical         :: repeat              ! exit loop when machine precision is reached or Nmax is reached 
    
    machine_prec = EPSILON(1._qd)
    !write(*,*) "machine precision for quad precision computations: ", machine_prec
    
    ! initialize recursive computation
    k = 0
    b = SQRT_PI*(z-0.5_qd)
    s = b
    q = b*b
    repeat = .true.

    do while (repeat)
        Nloc = size(ratios,1)
        do while (abs(b) > machine_prec .AND. k < Nloc-1 .AND. k < 47112)
            b = b*(q*ratios(k))        
            k = k + 1
            s = s + b
        enddo
        if (k==Nloc-1 .AND. 2*(Nloc-1) <= Nmax) then
            call init_extra()
        else    
            !write(*,*) "Number of indices used to compute Steinbrecher series: ", k
            repeat = .false.
        endif
    enddo
    ! return resulting value
    recursiveQuantile = SQRT_TWO*s
    
end function RecursiveQuantile

!> initalize coefficients and ratio of coefficients
subroutine init_series(Nterms)
    
    integer, intent(in) :: Nterms                     !< number of values to initialize
    integer             :: k                          ! loop counter
    integer             :: m                          ! loop counter    
    integer             :: ierr                       ! error code; 0=success
    
    call assert_false(Nterms <= 0, "SteinbrecherSeries::init_series: N expected to be positive.")
    N = Nterms
    allocate(coef(0:N), stat=ierr)
    call assert_equal(ierr, 0, "SteinbrecherSeries::init_series: error allocating array coef." )
    allocate(ratios(0:N), stat=ierr)
    call assert_equal (ierr, 0, "SteinbrecherSeries::init_series: error allocating array ratios." )

    coef(0) = 1.0_qd
    do k = 1,N
        coef(k) = 0.0_qd
        do m = 0,k-1
            coef(k) = coef(k) + (coef(m)*coef(k-m-1))/((1.0_qd*m+1.0_qd)*(2.0_qd*m + 1.0_qd))
        enddo
    enddo
    
    do k = 1, N
        coef(k) = coef(k)/(2.0_qd*k + 1.0_qd)
        ratios(k-1) = coef(k)/coef(k-1)
    enddo   
    
end subroutine init_series

!> compute extra coefficients if needed
subroutine init_extra()
    
    integer             :: k                          ! loop counter
    integer             :: m                          ! loop counter    
    integer             :: ierr                       ! error code; 0=success
    real(kind=qd), allocatable, dimension(:) :: tmp1  ! help array to compute ceof 
    real(kind=qd), allocatable, dimension(:) :: tmp2  ! help array to compute ratios 
    
    
    allocate(tmp1(0:N+N), stat=ierr)
    call assert_equal (ierr, 0, "SteinbrecherSeries::init_extra: error allocating array coef." )
    tmp1(0:N) = coef(0:N)
    call move_alloc(tmp1, coef)
    allocate(tmp2(0:N+N), stat=ierr)
    call assert_equal (ierr, 0, "SteinbrecherSeries::init_extra: error allocating array coef." )
    tmp2(0:N) = ratios(0:N)
    call move_alloc(tmp2, ratios)
        
    do k = N+1,N+N
        coef(k) = 0.0_qd
        do m = 0,k-1
            coef(k) = coef(k) + (coef(m)*coef(k-m-1))/((1.0_qd*m+1.0_qd)*(2.0_qd*m + 1.0_qd))
        enddo
    enddo
    
    do k = N+1, N+N
        coef(k) = coef(k)/(2.0_qd*k + 1.0_qd)
        ratios(k-1) = coef(k)/coef(k-1)
    enddo   
    N = N+N
    
end subroutine init_extra

!> deallocate arrays with precomputed values
subroutine finish_series()

    deallocate(coef)
    deallocate(ratios)

end subroutine finish_series

end module steinBrecherSeries
    