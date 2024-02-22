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

interface calculateDistributionInverse
    module procedure calculateDistributionInverse1
    module procedure calculateDistributionInverse2
end interface calculateDistributionInverse

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

!> Calculates X value from U values conform distribution type.
subroutine calculateDistributionInverse1( u, x, distType, distParameter1, distParameter2, &
               distParameter3, distParameter4, ierr, errorMessage )
    real(kind=c_double), intent(in)        :: u                  !< Standard normally distributed variable
    real(kind=c_double), intent(out)       :: x                  !< Physical stochastic variable associated with u
    integer,             intent(in)        :: distType           !< Distribution type, see distributionEnumerations
    real(kind=c_double), intent(in)        :: distParameter1     !< Parameter 1 of distribution type
    real(kind=c_double), intent(in)        :: distParameter2     !< Parameter 2 of distribution type
    real(kind=c_double), intent(in)        :: distParameter3     !< Parameter 3 of distribution type
    real(kind=c_double), intent(in)        :: distParameter4     !< Parameter 4 of distribution type
    integer            , intent(out)       :: ierr               !< error code; 0=success
    character(len=*),    intent(inout)     :: errorMessage       !< error message; only set in case of error

    type(tError)        :: ierror
    real(kind=c_double) :: p4(4)

    p4 = [distParameter1, distParameter2, distParameter3, distParameter4]
    call calculateDistributionInverse_c(u, x, distType, p4, ierror)
    ierr = ierror%iCode
    if (ierr /= 0) then
        call copystrback(errorMessage, ierror%message)
    end if
end subroutine calculateDistributionInverse1

subroutine calculateDistributionInverse2(u, y, distType, p4, ierr)
    integer(c_int),      intent(in   ) :: distType
    real(kind=c_double), intent(in   ) :: p4(*), u
    real(kind=c_double), intent(  out) :: y
    type(tError),        intent(  out) :: ierr
    call calculateDistributionInverse_c(u, y, distType, p4, ierr)
end subroutine calculateDistributionInverse2

function conditionalWeibull(distParameter1, distParameter2, distParameter3, distParameter4, x)
    use precision, only : wp
    real(kind = wp), intent(in)     :: x                  !<  value in the distribution for which the non-exceedance probability is desired
    real(kind = wp), intent(in)     :: distParameter1     !<  a, scale parameter
    real(kind = wp), intent(in)     :: distParameter2     !<  b, shape parameter
    real(kind = wp), intent(in)     :: distParameter3     !<  w, threshold parameter
    real(kind = wp), intent(in)     :: distParameter4     !<  L, lambda
    real(kind = wp)                 :: conditionalWeibull !<  conditional 

    conditionalWeibull  = distParameter4 * exp((distParameter3/distParameter1)**distParameter2 - (x/distParameter1)**distParameter2)

end function conditionalWeibull
!> Calculates U value from X values conform distribution type.
subroutine calculateDistribution( x, u, distType, distParameter1, distParameter2, &
               distParameter3, distParameter4, ierr, errorMessage )
    use precision, only : wp
    real(kind=wp),    intent(in)    :: x                  !< Standard normally distributed variable
    real(kind=wp),    intent(out)   :: u                  !< Physical stochastic variable associated with u
    integer,          intent(in)    :: distType           !< Distribution type, see distributionEnumerations
    real(kind=wp),    intent(in)    :: distParameter1     !< Parameter 1 of distribution type
    real(kind=wp),    intent(in)    :: distParameter2     !< Parameter 2 of distribution type
    real(kind=wp),    intent(in)    :: distParameter3     !< Parameter 3 of distribution type
    real(kind=wp),    intent(in)    :: distParameter4     !< Parameter 4 of distribution type
    integer,          intent(out)   :: ierr               !< error code; 0=success
    character(len=*), intent(inout) :: errorMessage       !< error message; only set in case of error

    real(kind=wp) :: p4(4)
    type(tError)  :: error

    p4(1) = distParameter1
    p4(2) = distParameter2
    p4(3) = distParameter3
    p4(4) = distParameter4

    call calculateDistribution_c(x, u, distType, p4, error)

    ierr = error%iCode
    if (ierr /= 0) then
        call copystrback(errorMessage, error%message)
    end if

end subroutine calculateDistribution

function logNormalII( distParameter1, distParameter2, distParameter3, x, ierr, errorMessage )
    use precision
    real(kind = wp), intent(in)     :: distParameter1         !< m, mean of actual variable (not log-transformed)
    real(kind = wp), intent(in)     :: distParameter2         !< s, standard deviation of actual variable (not log-transformed)
    real(kind = wp), intent(in)     :: distParameter3         !< Epsilon, horizontal shifting
    real(kind = wp), intent(in)     :: x                      !< Standard normally distributed variable u
    integer,          intent(out)   :: ierr                   !< error code; 0=success
    character(len=*), intent(inout) :: errorMessage           !< error message; only set in case of error

    real(kind = wp)                 :: logNormalII

    real(kind = wp)                 :: sigma
    real(kind = wp)                 :: mu
    real(kind = wp), parameter      :: qMin = 1.0d-300

    ierr = 0
    if (( distParameter1 - distParameter3) < qMin ) then
        errorMessage = "Parameter 1 of logNormalII should be larger than parameter 3"
        ierr = -1
    else if (distParameter2 < 0.0D0 ) then
        errorMessage = "Standard deviation should be larger than zero"
        ierr = -1
    else if ((x - distParameter3) < qMin ) then
        errorMessage = "The x-value of logNormalII should be larger than parameter 3"
        ierr = -1
    end if

    if (ierr == 0) then
        ! Compute standard deviation and mean of log-transformed variable from mean and standard deviation of actual variable
        sigma   = sqrt( log(1.0d0 + (distParameter2 / (distParameter1 - distParameter3))**2))
        mu      = log(distParameter1 - distParameter3) - 0.5d0 * sigma * sigma

        logNormalII = log(x - distParameter3)

        logNormalII = ( logNormalII - mu ) / sigma
    end if

end function logNormalII

function truncatedNormal(u, mean, deviation, minimum, maximum, ierr, errorMessage)
    use precision
    use interface_convert
    real(kind=wp), intent(in)       :: u                !< input u
    real(kind=wp), intent(in)       :: mean             !< a parameter: mean
    real(kind=wp), intent(in)       :: deviation        !< b parameter: deviation
    real(kind=wp), intent(in)       :: minimum          !< c parameter: minimum
    real(kind=wp), intent(in)       :: maximum          !< d parameter: maximum 
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
    if (.not. inputCheckTruncatedNormal(deviation, minimum, maximum, errorMessage)) then
        call set_nan(truncatedNormal)
        ierr = -1
    else
        !
        ! actual computation
        !

        ! transform limits to u-space
        au = (minimum - mean) / deviation
        bu = (maximum - mean) / deviation

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

        truncatedNormal = mean + zt * deviation
    endif

end function truncatedNormal

!> validation for truncated normal
function inputCheckTruncatedNormal(deviation, minimum, maximum, errorMessage)
    use precision
    real(kind=wp), intent(in)       :: deviation        !< b parameter: deviation
    real(kind=wp), intent(in)       :: minimum          !< c parameter: minimum
    real(kind=wp), intent(in)       :: maximum          !< d parameter: maximum 
    character(len=*), intent(inout) :: errorMessage     !< error message, only set in case of an error
    logical                         :: inputCheckTruncatedNormal  !< function result

    real(kind=wp), parameter :: zero = 0.0_wp        ! constant 0.0
    real(kind=wp), parameter :: diffMinMax = 1e-4_wp ! smallest allowed difference between minimum and maximum

    if (deviation <= zero) then
        inputCheckTruncatedNormal = .false.
        write(errorMessage,*) 'Truncated Normal: deviation must be > 0; found: ', deviation
    else if (minimum > maximum) then
        inputCheckTruncatedNormal = .false.
        write(errorMessage,*) 'Truncated Normal: minimum must be < maximum; found: ', minimum, maximum
    else if ((maximum - minimum) < diffMinMax) then
        inputCheckTruncatedNormal = .false.
        write(errorMessage,*) 'Truncated Normal: minimum and maximum are too close; difference found: ', maximum - minimum
    else
        inputCheckTruncatedNormal = .true.
    endif
end function inputCheckTruncatedNormal

end module interface_distributions
