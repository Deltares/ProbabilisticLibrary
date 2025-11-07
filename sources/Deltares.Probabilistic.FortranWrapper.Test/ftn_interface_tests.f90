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
module ftn_interface_tests
use iso_c_binding, only : c_loc
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
    call testWithLevel(test_combine, "test combine", testLevel)
    call testWithLevel(test_combine_gen, "test combine general", testLevel)

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

    distribs(1)%distributionId = distributionNormal
    distribs(1)%params = (/1.0_dp, 2.0_dp, 0.0_dp, 0.0_dp/)

    distribs(2)%distributionId = distributionNormal
    distribs(2)%params = (/1.0_dp, 2.0_dp, 0.0_dp, 0.0_dp/)
end subroutine fillDistribs

subroutine test_ds
    use interface_probCalc
    type(tdistrib)              :: distribs(2)
    type(tMethod)               :: method
    type(tResult)               :: results
    integer                     :: i
    type(tCompIds)              :: compIds
    character(len=ErrMsgLength) :: errmsg
    type(basicCorrelation)      :: correlations(0)
    real(kind=wp)               :: x(4)
    logical                     :: convergence

    call fillDistribs(distribs)

    method%methodId = methodDirectionalSampling
    method%startMethod = fORMStartZero
    method%tolB = method%tolA
    method%numThreads = 4
    method%maxSamples = 99000
    method%progressInterval = 5000
    method%numExtraInt = 50

    compIds%id = 16
    compIds%nrStochasts = 2
    compIds%nrCorrelations = 0

    call probCalcF2C(method, distribs, correlations, zfunc, textualProgress, compIds, x, results)

    call assert_equal(results%error%iCode, 0, "return code probCalcF2C <> 0")

    if (results%error%iCode == 0) then
        call assert_comparable(results%beta, -0.2220993519_wp, margin, "diff in beta")
        call assert_comparable([x(2), x(4)], [-0.89448_wp, -0.44106_wp], 1d-2, "diff in alpha")
        call assert_comparable([x(1), x(3)], [0.59998_wp, 0.80005_wp], 1d-2, "diff in x")
        convergence = results%convergence
        call assert_false(convergence, "diff in convergence flag")
    else
        call copystrback(errmsg, results%error%message)
        call assert(errmsg)
    end if

end subroutine test_ds

subroutine test_ds_errorhandling
    use interface_probCalc
    type(tdistrib)              :: distribs(2)
    type(tMethod)               :: method
    type(tResult)               :: results
    type(tCompIds)              :: compIds
    character(len=ErrMsgLength) :: errmsg
    type(basicCorrelation)      :: correlations(0)
    real(kind=wp)               :: x(2)

    call fillDistribs(distribs)

    method%methodId = methodDirectionalSampling
    method%startMethod = fORMStartZero
    method%numThreads = 1
    method%numExtraInt = 50

    compIds%id = 17
    compIds%nrStochasts = 2
    compIds%nrCorrelations = 0

    call probCalcF2C(method, distribs, correlations, zfunc, textualProgress, compIds, x, results)

    call assert_equal(results%error%iCode, 1, "return code probCalcF2C <> 0")

    call copystrback(errmsg, results%error%message)
    call assert_equal(errmsg, "just testing", "diff in error message")

end subroutine test_ds_errorhandling

subroutine test_form_errorhandling
    use interface_probCalc
    type(tdistrib)              :: distribs(2)
    type(tMethod)               :: method
    type(tResult)               :: results
    type(tCompIds)              :: compIds
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

    compIds%id = 17
    compIds%nrStochasts = 2
    compIds%nrCorrelations = 0

    call probCalcF2C(method, distribs, correlations, zfunc, textualProgress, compIds, x, results)

    call assert_equal(results%error%iCode, 0, "diff in return code probCalcF2C")
    convergence = results%convergence
    call assert_false(convergence, "diff in convergence flag")
    call assert_comparable(results%beta, 40.0_wp, margin, "diff in beta")

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
        call calculateDistribution(xValues(i), u, distributionNormal, [mean, deviation, 0.0_wp, 0.0_wp], ierr, errmsg)
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
    call combineMultipleElements( betas, alphas, rho, beta, alpha)
    call assert_comparable(beta, 2.0_wp, margin, "diff in beta")
    call assert_comparable(alpha, [sqrt(0.5_dp), sqrt(0.5_dp)], margin, "alpha in beta")

end subroutine test_combine

subroutine test_combine_gen
    use interface_combin
    implicit none
    real(kind=dp) :: betas(2)
    real(kind=dp), target :: alpha(2), rho(2), alphas(2,2)
    type(betaAlphaCF) :: dpOut
    type(multipleElements) :: elements
    type(betaAlphaCF), target :: dpIn(2)
    type(combinerSettings) :: settings
    integer                :: i, j

    betas = [2.0_dp, 3.0_dp]
    do i = 1, 2
        do j = 1, 2
            alphas(i,j) = sqrt(0.5_dp)
        end do
        dpIn(i)%alpha = c_loc(alphas(i,1))
        dpIn(i)%stride_alpha = 2
        dpIn(i)%size = 2
        dpIn(i)%beta = betas(i)
    end do
    rho = [1.0_dp, 1.0_dp]
    dpIn(1)%rho = c_loc(rho)
    elements%designPoints = c_loc(dpIn)
    elements%nElements = 2

    dpOut%alpha = c_loc(alpha)
    dpOut%size = 2

    do i = 0, 2
        settings%combAndOr = combOR
        settings%combinerType = i
        call combineMultipleElementsGeneral( elements, dpOut, settings)
        call assert_comparable(dpOut%beta, 2.0_wp, 1d-2, "diff in beta")
        call assert_comparable(alpha, [sqrt(0.5_dp), sqrt(0.5_dp)], 1d-1, "alpha in beta")
    end do

end subroutine test_combine_gen

end module ftn_interface_tests
