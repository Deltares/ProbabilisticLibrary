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
module interface_distributions
use, intrinsic :: iso_c_binding, only: c_int, c_double
use interface_gen, only : tError
use f2c_tools, only : copystrback

implicit none

private :: c_int, c_double, tError, copystrback

interface
    subroutine calculateDistributionInverse_c(u, y, distType, p4, ierr) bind(C)
      use, intrinsic :: iso_c_binding, only: c_int, c_double
      import tError
      integer(c_int),      intent(in   ) :: distType
      real(kind=c_double), intent(in   ) :: p4(*), u
      real(kind=c_double), intent(  out) :: y
      type(tError),        intent(  out) :: ierr
    end subroutine calculateDistributionInverse_c
end interface

interface
    subroutine calculateDistribution_c(x, u, distType, p4, ierr) bind(C)
      use, intrinsic :: iso_c_binding, only: c_int, c_double
      import tError
      integer(c_int),      intent(in   ) :: distType
      real(kind=c_double), intent(in   ) :: p4(*), x
      real(kind=c_double), intent(  out) :: u
      type(tError),        intent(  out) :: ierr
    end subroutine calculateDistribution_c
end interface

integer, parameter :: distributionUnknown            = -1
integer, parameter :: distributionDeterministic      =  0
integer, parameter :: distributionUniform            =  1
integer, parameter :: distributionNormal             =  2
integer, parameter :: distributionShiftedLognormal   =  3
integer, parameter :: distributionShiftedLognormal2  =  4
integer, parameter :: distributionShiftedExponential =  5
integer, parameter :: distributionGumbelDistribution =  6
integer, parameter :: distributionGumbelDistribution2 = 7
integer, parameter :: distributionWeibull            =  8
integer, parameter :: distributionRayleigh           =  9
integer, parameter :: distributionPareto             = 10
integer, parameter :: distributionTriangular         = 11
integer, parameter :: distributionExceedanceFrequencyTable = 12
integer, parameter :: distributionReturnPeriodTable        = 13
integer, parameter :: distributionConditionalWeibull = 14
integer, parameter :: distributionModifiedGumbel     = 15
integer, parameter :: distributionTruncatedModifiedGumbel = 16
integer, parameter :: distributionExceedanceProbabilityTable = 17
integer, parameter :: distributionRayleighN          = 18
integer, parameter :: distributionTruncatedNormal    = 19
integer, parameter :: distributionBeta                   = 20
integer, parameter :: distributionLogLinearInterpolation = 112

contains

subroutine calculateDistributionInverse(u, y, distType, p4, ierr)
    integer(c_int),      intent(in   ) :: distType
    real(kind=c_double), intent(in   ) :: p4(*), u
    real(kind=c_double), intent(  out) :: y
    type(tError),        intent(  out) :: ierr
    call calculateDistributionInverse_c(u, y, distType, p4, ierr)
end subroutine calculateDistributionInverse

function conditionalWeibull(distParams, x)
    use precision, only : wp
    use interface_convert
    real(kind = wp), intent(in)     :: x                  !<  value in the distribution for which the non-exceedance probability is desired
    real(kind = wp), intent(in)     :: distParams(:)      !<  parameters a, b, w and L
    real(kind = wp)                 :: conditionalWeibull !<  conditional

    conditionalWeibull  = distParams(4) * exp((distParams(3)/distParams(1))**distParams(2) - (x/distParams(1))**distParams(2))

end function conditionalWeibull

!> Calculates U value from X values conform distribution type.
subroutine calculateDistribution( x, u, distType, distParameters, ierr, errorMessage )
    use precision, only : wp
    real(kind=wp),    intent(in)    :: x                  !< Standard normally distributed variable
    real(kind=wp),    intent(out)   :: u                  !< Physical stochastic variable associated with u
    integer,          intent(in)    :: distType           !< Distribution type, see distributionEnumerations
    real(kind=wp),    intent(in)    :: distParameters(:)  !< Parameters 1 - 4 of distribution type
    integer,          intent(out)   :: ierr               !< error code; 0=success
    character(len=*), intent(inout) :: errorMessage       !< error message; only set in case of error

    type(tError)  :: error

    call calculateDistribution_c(x, u, distType, distParameters, error)

    ierr = error%iCode
    if (ierr /= 0) then
        call copystrback(errorMessage, error%message)
    end if

end subroutine calculateDistribution

function logNormalII( distParameters, x, ierr, errorMessage )
    use precision
    real(kind = wp), intent(in)     :: distParameters(:)      !< parametrs mean, standard deviation and shift of actual variable (not log-transformed)
    real(kind = wp), intent(in)     :: x                      !< Standard normally distributed variable u
    integer,          intent(out)   :: ierr                   !< error code; 0=success
    character(len=*), intent(inout) :: errorMessage           !< error message; only set in case of error

    real(kind = wp)                 :: logNormalII

    real(kind = wp)                 :: sigma
    real(kind = wp)                 :: mu
    real(kind = wp), parameter      :: qMin = 1.0d-300

    ierr = 0
    if (( distParameters(1) - distParameters(3)) < qMin ) then
        errorMessage = "Parameter 1 of logNormalII should be larger than parameter 3"
        ierr = -1
    else if (distParameters(2) < 0.0D0 ) then
        errorMessage = "Standard deviation should be larger than zero"
        ierr = -1
    else if ((x - distParameters(3)) < qMin ) then
        errorMessage = "The x-value of logNormalII should be larger than parameter 3"
        ierr = -1
    end if

    if (ierr == 0) then
        ! Compute standard deviation and mean of log-transformed variable from mean and standard deviation of actual variable
        sigma   = sqrt( log(1.0d0 + (distParameters(2) / (distParameters(1) - distParameters(3)))**2))
        mu      = log(distParameters(1) - distParameters(3)) - 0.5d0 * sigma * sigma

        logNormalII = log(x - distParameters(3))

        logNormalII = ( logNormalII - mu ) / sigma
    end if

end function logNormalII

function truncatedNormal(u, distParameters, ierr, errorMessage)
    use precision
    use interface_convert
    real(kind=wp), intent(in)       :: u                !< input u
    real(kind=wp), intent(in)       :: distParameters(:) !< parameters: mean, deviation, minimum, maximum
    integer, intent(out)            :: ierr             !< error code; 0=success
    character(len=*), intent(inout) :: errorMessage     !< error message, only set in case of an error
    real(kind=wp)                   :: truncatedNormal  !< function result
    !
    !  locals
    !
    real(kind=wp) :: au      ! minimum transformed to u-space
    real(kind=wp) :: bu      ! maximum transformed to u-space
    real(kind=wp) :: pa      ! exceedance probability of minimum if it was normal
    real(kind=wp) :: qb      ! exceedance non-probability of maximum if it was normal
    real(kind=wp) :: dummy   ! not used argument of pQFromBeta
    real(kind=wp) :: factor  ! factor such that the truncated shape has a total area of 1
    real(kind=wp) :: p       ! exceedance probability of the requested u
    real(kind=wp) :: q       ! exceedance non-probability of the requested u
    real(kind=wp) :: qzt     ! implicit given by: factor * (qzt - qb) = qz
    real(kind=wp) :: pzt     ! implicit given by: factor * (qzt - pa) = pz
    real(kind=wp) :: zt      ! zt is the u-value in the truncated distribution,
                             ! which has the same exceedance probability as u in the standard normal distribution
    real(kind=wp), parameter :: half = 0.5_wp        ! constant 0.5
    real(kind=wp), parameter :: one  = 1.0_wp        ! constant 1.0
    
    ierr = 0
    !
    ! input check
    !
    if (.not. inputCheckTruncatedNormal(distParameters, errorMessage)) then
        call set_nan(truncatedNormal)
        ierr = -1
    else
        !
        ! actual computation
        !

        ! transform limits to u-space
        au = (distParameters(3) - distParameters(1)) / distParameters(2)
        bu = (distParameters(4) - distParameters(1)) / distParameters(2)

        ! detect exceedance probability of limits if it were a normal standard distribution
        call pQFromBeta(au, pa, dummy)
        call pQFromBeta(bu, dummy, qb)

        ! the shape of the distribution must be multiplied with a factor, so that the truncated shape has a total area of 1
        factor = one / (one - pa - qb)

        ! get the exceedance probability of the requested u
        call pQFromBeta(u, p, q)

        if ( q < half) then
            ! zt is the u-value in the truncated distribution,
            ! which has the same exceedance probability as u in the standard normal distribution
            ! the area in the truncated distribution is: factor * (qzt - qb) = qz
            qzt = q / factor + qb
            call betaFromQ( qzt, zt, ierr, errorMessage )
        else
            ! zt is the u-value in the truncated distribution,
            ! which has the same exceeedance probability as u in the standard normal distribution
            ! the area in the truncated distribution is: factor * (qzt - pa) = pz
            pzt = p / factor + pa
            call betaFromQ( pzt, zt, ierr, errorMessage )
            zt = -zt
        endif

        truncatedNormal = distParameters(1) + zt * distParameters(2)
    endif

end function truncatedNormal

!> validation for truncated normal
function inputCheckTruncatedNormal(distParameters, errorMessage)
    use precision
    real(kind=wp), intent(in)       :: distParameters(:) !< parameters: mean, deviation, minimum, maximum
    character(len=*), intent(inout) :: errorMessage     !< error message, only set in case of an error
    logical                         :: inputCheckTruncatedNormal  !< function result

    real(kind=wp), parameter :: zero = 0.0_wp        ! constant 0.0
    real(kind=wp), parameter :: diffMinMax = 1e-4_wp ! smallest allowed difference between minimum and maximum

    if (distParameters(2) <= zero) then
        inputCheckTruncatedNormal = .false.
        write(errorMessage,*) 'Truncated Normal: deviation must be > 0; found: ', distParameters(2)
    else if (distParameters(3) > distParameters(4)) then
        inputCheckTruncatedNormal = .false.
        write(errorMessage,*) 'Truncated Normal: minimum must be < maximum; found: ', distParameters(3), distParameters(4)
    else if ((distParameters(4) - distParameters(3)) < diffMinMax) then
        inputCheckTruncatedNormal = .false.
        write(errorMessage,*) 'Truncated Normal: minimum and maximum are too close; difference found: ', &
            distParameters(4) - distParameters(3)
    else
        inputCheckTruncatedNormal = .true.
    endif
end function inputCheckTruncatedNormal

end module interface_distributions
