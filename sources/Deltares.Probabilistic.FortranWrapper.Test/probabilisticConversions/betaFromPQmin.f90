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

!> @file betaFromPQmin.f90
!! Test for the computation of P and Q from beta
!<
!>
!! Alternative subroutine for computing the reliability index (beta) for a given 
!! exceedance probability. The routine is used to verify the implementation of betaFromQ.
!! The routine minimize an objective function defines as the square of difference between
!! log-q of given probability of eceedance and log of 1 obtained by sought beta value. Ideally,
!! such objective function should tend to zero. The optimization problem is solved with
!! golden section search (i.e., no derivative required).

module betaFromPQminModule

    use precision
    use ftnunit
    use interface_convert
    use Fmin

    implicit none
    
    private
    
    public :: betaFromPQmin
    
    interface betaFromPQmin
        module procedure betaFromPQmin8
    end interface betaFromPQmin
    
    interface sqrlogQ
        module procedure sqrlogQ8
    end interface sqrlogQ

contains

!> This subroutine is an alternative implementation of betaFromq.
!> It minimizes an objective function of square difference of the minimum of p and q.
!> It uses golden search method to perform minimization.
!> double precision version
!> @ingroup probabilisticTests
!
subroutine betaFromPQmin8( pin, qin, beta )
    real(kind = wp), parameter :: qMin = 1.0d-300
! INPUT/OUTPUT VARIABLES
!
    real(kind=wp), intent(in)  :: qin                     !< Probability of exceedance
    real(kind=wp), intent(in)  :: pin                     !< Probability of exceedance

    real(kind=wp), intent(out) :: beta                    !< Reliability index
!
! LOCAL VARIABLES
!
    real(kind=wp)              :: q                       ! working quadratic Probability of exceedance
    real(kind=wp)              :: qW                      ! working q
    real(kind=wp)              :: beta0                   ! initial reliability index
    real(kind=wp)              :: tol                     ! tolerance for the line search routine
    real(kind=wp)              :: logq2find(1)            ! log of q
    real(kind=wp)              :: fvalue                  ! objective function value
    real(kind=wp)              :: margin                  ! Relative value for the margin
!
!   Define the stop criteria i.e. the relative value of the margin
!   and the maximum number of iterations
!
! Initialization

!       The standard normal distribution is symmetric around the value zero.
!       So it is sufficient to consider the probability of exceedance between
!       0 and 0.5. Probabilities of exceedance above 0.5 are transformed to values
!       between 0 and 0.5 by q = p

    if ( pin < qin ) then
        q = pin
    else
        q = qin
    endif
    
    logq2find=-999.d0
    beta=-99999.d0
    qW=dble(q)
    margin = 1D-20 !epsilon(1.0d0)

    if (qW<qMin) then
        qW=qMin
    endif
    
    if (qW < 0.5D0-margin .and. qw > 0.5D0+margin) then
        beta = 0.d0
        return
    endif
    
    logq2find=log10(qW)
!
!   Initial estimate for the reliability index (beta). Two approximations are used
!   One approximation is used for values between 0 and 0.15. The other one is used
!   for values between 0.15 and 0.5
!
    if ( qW < 0.15d0 ) then
        beta0 = sqrt(-2.0d0 * log(5.0d0 * max( qW, qMin )))
    else
        beta0 = 1.464795d0 - 2.929590d0 * qW
    endif
    
    tol=1D-20
    call golden(sqrlogQ8,0.D0,38.39D0,beta0,tol,beta,fvalue,logq2find)
    !call golden(sqrlogQ,-38.39D0,0.D0,-beta0,tol,beta,fvalue,logq2find)
    
!   If the probability of exceedance of the input is more than 0.5,
!   the computed reliability index (beta) is adjusted by beta = -beta
!
    if ( pin < qin ) then
        beta = -beta
    endif

end subroutine betaFromPQmin8

!> Objective function used within betaFromQmin.
!> This function is the square of difference of log-transformed values of probability
!> of exceedance.
!> @ingroup probabilisticTests
!
function sqrlogQ8(beta,logq2find)

    real(kind=wp)  :: logq2find               !< log of p or q
    real(kind=wp), intent(in) :: beta         !< Reliability index

! local variables.    
    real(kind=wp)             :: qtmp         !< Probability of exceedance
    real(kind=wp)             :: ptmp         !< Probability of non-exceedance
    real(kind=wp)             :: sqrlogQ8
    real(kind=wp)             :: y


    call PQfromBeta(beta,ptmp,qtmp)
    y=log10(qtmp)
    sqrlogQ8=(y-logq2find)** 2.d0

end function sqrlogQ8

end module betaFromPQminModule
