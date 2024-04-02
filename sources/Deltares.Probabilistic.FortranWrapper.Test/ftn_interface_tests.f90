module ftn_interface_tests
use precision
use ftnunit
use interface_gen, only : tError, ErrMsgLength
use f2c_tools
implicit none

real(kind=wp), parameter :: margin = 1e-9_wp

private
public :: run_all_ftn_interface_tests

contains

subroutine run_all_ftn_interface_tests
    integer, parameter :: testLevel = 1

    call testWithLevel(test_ds, "test DS", testLevel)
    call testWithLevel(test_ds_errorhandling, "test error handling DS", testLevel)
    call testWithLevel(test_form_errorhandling, "test error handling FORM", testLevel, "work-in-progress")
    call testWithLevel(test_calc_distrib_inv, "test calculateDistributionInverse", testLevel)
    call testWithLevel(test_calc_distrib_inv_errorhandling, "test error handling distribution inverse", testLevel)
    call testWithLevel(test_calc_distrib, "test calc distrib", testLevel)
    call testWithLevel(test_conversions, "test conversions", testLevel)
    !call testWithLevel(test_combine, "test combine", testLevel)
    !call testWithLevel(test_correlation, "test correlation", testLevel)
    !call testWithLevel(test_correlation_error_handling, "test error handling correlation", testLevel)

end subroutine run_all_ftn_interface_tests

function zfunc(x, compSettings, e) result(z) bind(c)
    use interface_probCalc, only : tError, ErrMsgLength, computationSetting
    real(kind=dp), intent(in   ) :: x(*)
    type(computationSetting), intent(in   ) :: compSettings
    type(tError),  intent(inout) :: e
    real(kind=dp)                :: z
    character(len=ErrMsgLength)  :: message

    if (compSettings%computationId == 16) then
        z = 1.0_dp - x(1) - 0.5_dp * x(2)
    else
        z = 0.9_dp - x(1) - 0.45_dp * x(2)
    end if
    if (compSettings%computationId > 16) then
        e%icode = -1
        message = "just testing"
        call copystr(message, e%message)
    else
        e%icode = 0
    endif
end function zfunc

subroutine fillDistribs(distribs)
    use interface_probCalc
    use interface_distributions
    type(tdistrib)              :: distribs(:)

    call copystr("var1", distribs(1)%name)
    distribs(1)%distributionId = distributionNormal
    distribs(1)%params = (/1.0_dp, 2.0_dp, 0.0_dp, 0.0_dp/)

    call copystr("var2", distribs(2)%name)
    distribs(2)%distributionId = distributionNormal
    distribs(2)%params = (/1.0_dp, 2.0_dp, 0.0_dp, 0.0_dp/)
end subroutine fillDistribs

subroutine test_ds
    use interface_probCalc
    type(tdistrib)              :: distribs(2)
    type(tError)                :: ierr
    type(tMethod)               :: method
    type(tResult)               :: r
    integer                     :: i
    integer                     :: compIds(20), ipoint(2)
    character(len=ErrMsgLength) :: errmsg
    type(basicCorrelation)      :: correlations(0)
    real(kind=wp)               :: x(2)
    logical                     :: convergence

    call fillDistribs(distribs)

    method%methodId = methodDirectionalSampling
    method%rnd = GeorgeMarsaglia
    method%startMethod = fORMStartZero
    method%tolB = method%tolA
    method%numThreads = 4
    method%maxSamples = 99000
    method%progressInterval = 5000
    method%numExtraInt = 50

    compIds(1) = 16
    ipoint     = [0, 1] ! zero based, as it goes to c++

    call probCalcF2C(method, distribs, 2, 2, correlations, 0,  zfunc, textualProgress, compIds, iPoint, x, r, ierr)

    call assert_equal(ierr%iCode, 0, "return code probCalcF2C <> 0")

    if (ierr%iCode == 0) then
        call assert_comparable(r%beta, -0.22178518912_wp, margin, "diff in beta")
        call assert_comparable(r%alpha(1:2), [-0.89448_wp, -0.44710_wp], 1d-2, "diff in alpha")
        call assert_comparable(r%x(1:2), [0.59998_wp, 0.80005_wp], 1d-2, "diff in x")
        convergence = r%convergence
        call assert_false(convergence, "diff in convergence flag")
    else
        call copystrback(errmsg, ierr%message)
        call assert(errmsg)
    end if

end subroutine test_ds

subroutine test_ds_errorhandling
    use interface_probCalc
    type(tdistrib)              :: distribs(2)
    type(tError)                :: ierr
    type(tMethod)               :: method
    type(tResult)               :: r
    integer                     :: compIds(20), ipoint(2)
    character(len=ErrMsgLength) :: errmsg
    type(basicCorrelation)      :: correlations(0)
    real(kind=wp)               :: x(2)

    call fillDistribs(distribs)

    method%methodId = methodDirectionalSampling
    method%startMethod = fORMStartZero
    method%numThreads = 1
    method%numExtraInt = 50

    compIds(1) = 17
    ipoint     = [0, 1]

    call probCalcF2C(method, distribs, 2, 2, correlations, 0, zfunc, textualProgress, compIds, ipoint, x, r, ierr)

    call assert_equal(ierr%iCode, -1, "return code probCalcF2C <> 0")

    call copystrback(errmsg, ierr%message)
    call assert_equal(errmsg, "just testing", "diff in error message")

end subroutine test_ds_errorhandling

subroutine test_form_errorhandling
    use interface_probCalc
    type(tdistrib)              :: distribs(2)
    type(tError)                :: ierr
    type(tMethod)               :: method
    type(tResult)               :: r
    integer                     :: compIds(20), ipoint(2)
    character(len=ErrMsgLength) :: errmsg
    type(basicCorrelation)      :: correlations(0)
    real(kind=wp)               :: x(2)
    logical                     :: convergence

    call fillDistribs(distribs)

    method%methodId = methodFORM
    method%startMethod = fORMStartZero
    method%numThreads = 1
    method%trialLoops = 1
    method%numExtraInt = 50

    compIds(1) = 17
    ipoint     = [0, 1]

    call probCalcF2C(method, distribs, 2, 2, correlations, 0, zfunc, textualProgress, compIds, ipoint, x, r, ierr)

    call assert_equal(ierr%iCode, 0, "diff in return code probCalcF2C")
    convergence = r%convergence
    call assert_false(convergence, "diff in convergence flag")
    call assert_comparable(r%beta, 40.0_wp, margin, "diff in beta")

end subroutine test_form_errorhandling

subroutine test_calc_distrib_inv
    use interface_distributions

    type(tError)                :: ierr
    character(len=ErrMsgLength) :: errmsg
    real(kind=dp)               :: yValue
    real(kind=dp)               :: p(2) = (/12.3_dp, 2.456_dp/)

    call calculateDistributionInverse( 1.2_dp, yValue, distributionNormal, p, ierr)
    call assert_equal(ierr%iCode, 0, "return code calc_distrib_inv <> 0")
    if (ierr%iCode == 0) then
        call assert_comparable(yValue, 12.3_dp + 1.2_dp*2.456_dp, margin, "diff in yValue")
    else
        call copystrback(errmsg, ierr%message)
        call assert(errmsg)
    endif

end subroutine test_calc_distrib_inv

subroutine test_calc_distrib
    use interface_distributions

    integer                     :: ierr
    character(len=ErrMsgLength) :: errmsg
    real(kind=dp)               :: u, x
    real(kind=wp), parameter    :: mean = 3.0_wp
    real(kind=wp), parameter    :: deviation = 1.0_wp
    integer                     :: i
    real(kind=wp), parameter    :: uValues (2) = [ 0.0_wp, 1.0_wp ]
    real(kind=wp), parameter    :: xValues (2) = [ 3.0_wp, 4.0_wp ]

    do i = 1, size(uValues)
        call calculateDistribution(xValues(i), u, distributionNormal, mean, deviation, 0.0_wp, 0.0_wp, ierr, errmsg)
        call assert_comparable(u, uValues(i), 1d-12, "diff in result normal distribution")
        call assert_equal(ierr, 0, errmsg)
    end do

end subroutine test_calc_distrib

subroutine test_calc_distrib_inv_errorhandling
    use interface_distributions

    integer                     :: distribError
    type(tError)                :: ierr
    character(len=ErrMsgLength) :: errmsg
    real(kind=dp)               :: yValue
    real(kind=dp)               :: p(1) = (/0.0_dp/)

    distribError = -2
    call calculateDistributionInverse(1.2_dp, yValue, distribError, p, ierr)
    call assert_equal(ierr%iCode, -1, "return code calc_distrib_inv <> -1")
    call copystrback(errmsg, ierr%message)
    call assert_equal(errmsg, 'Unknown distribution function - code: -2', "diff in error message")

end subroutine test_calc_distrib_inv_errorhandling

subroutine test_conversions
    use interface_convert

    real(kind=dp) :: x, y, p, q
    type(tError)  :: ierr

    x = 0.5_dp
    call BetaFromQ(x, y, ierr)
    call assert_equal(ierr%iCode, 0, "return value")
    call assert_comparable(y, 0.0_wp, margin, "diff in result BetaFromQ")
    call BetaFromQ(x, y)
    call assert_comparable(y, 0.0_wp, margin, "diff in result BetaFromQ")
    x = 4.0_dp
    y = QfromBeta(x)
    call assert_comparable(y, 3.1671241833e-5_wp, margin, "diff in result QfromBeta")
    call ReturnTimeFromBeta(x, y)
    call assert_comparable(y, 31573.885532_wp, margin, "diff in result ReturnTimeFromBeta")
    call FreqFromBeta(x, y)
    call assert_comparable(y, 3.16717433775e-5_wp, margin, "diff in result FreqFromBeta")
    call LogQFromBeta(x, y)
    call assert_comparable(y, 10.36008565_wp, margin, "diff in result LogQFromBeta")
    call PQfromBeta(x, p, q)
    call assert_comparable(q, 3.1671241833e-5_wp, margin, "diff in result PQfromBeta")
    call assert_comparable(p, 0.99996832876_wp, margin, "diff in result PfromBeta")
    p = PfromBeta(x)
    call assert_comparable(p, 0.99996832876_wp, margin, "diff in result PfromBeta")
end subroutine test_conversions

subroutine test_combine
    use interface_combin
    implicit none
    real(kind=dp) :: betas(2), alphas(2,2), rho(2), beta, alpha(2)
    integer i, j
    betas = [2.0_dp, 3.0_dp]
    do i = 1, 2
        do j = 1, 2
            alphas(i,j) = sqrt(0.5_dp)
        end do
    end do
    rho = [1.0_dp, 1.0_dp]
    call combineMultipleElements( betas, alphas, rho, beta, alpha, combOR)
    call assert_comparable(beta, 2.0_wp, margin, "diff in beta")
    call assert_comparable(alpha, [sqrt(0.5_dp), sqrt(0.5_dp)], margin, "alpha in beta")

end subroutine test_combine

subroutine test_correlation
    use interface_correlation
    type(correlationModelParameters) :: correlationParameters
    logical :: typeUCorrelation
    real(kind=wp) :: u1, u2

    correlationParameters%correlationId = correlationVolker
    correlationParameters%correlationParameter1 = 0.873d0      !A
    correlationParameters%correlationParameter2 = 0.236d0      !B
    correlationParameters%correlationParameter3 = 0.356d0      !rho
    correlationParameters%correlationParameter4 = 0.67d0       !M
    correlationParameters%correlationParameter5 = 0.0010d0     !aK
    correlationParameters%correlationParameter6 = 0.2347d0     !bK
    correlationParameters%correlationParameter7 = -0.5771d0    !cK
    correlationParameters%correlationParameter8 = 0.02d0       !d

    !
    ! Conditional Weibull water level statistics - Hoek van Holland parameters
    !
    correlationParameters%distribution   = distributionConditionalWeibull
    correlationParameters%distParameter1 = 0.0157d0 !scale
    correlationParameters%distParameter2 = 0.57d0   !shape
    correlationParameters%distParameter3 = 1.97d0   !thresh
    correlationParameters%distParameter4 = 7.237d0  !lambda

    u1 = 1.0_wp
    u2 = 1.2_wp
    typeUCorrelation = .false.
    call calculateCorrelation(u1, u2, correlationParameters, typeUCorrelation)
    call assert_comparable(u2, 19.2414951_wp, 1d-8, "diff in Volker calculation")

end subroutine test_correlation

subroutine test_correlation_error_handling
    use interface_correlation
    use feedback
    type(correlationModelParameters) :: correlationParameters
    real(kind=wp) :: u1, u2
    character(len=64) :: msg
    character(len=*), parameter :: msg_expected = 'Fatal error: Unknown correlation model - ID.'

    correlationParameters%correlationId = 12345

    u1 = 1.0_wp
    u2 = 1.2_wp
    call SetFatalErrorExpected(.true.)
    call calculateCorrelation(u1, u2, correlationParameters)
    call GetFatalErrorMessage(msg)
    call assert_equal(msg, msg_expected, "difference in error message")
    call SetFatalErrorExpected(.false.)

end subroutine test_correlation_error_handling

end module ftn_interface_tests
