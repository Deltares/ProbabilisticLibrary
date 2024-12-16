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
!! Test for the Waarts functions.
!! (The more complex Waarts functions are set up in the Excel sheet ..\doc\WaartsFunctions.xls)
!<
!
module waartsFunctionsTests

    use ftnunit
    use precision
    use waartsFunctions

    implicit none

    private

    public :: allWaartsFunctionsTests

    real (kind=wp), parameter :: margin = 1.0d-6

contains

!> Call all the tests
subroutine allWaartsFunctionsTests

    call testWithLevel( linearResistanceSolicitationReturnsCorrectValues, &
        "Test on correct return values for linearResistanceSolicitation", 1)
    call testWithLevel( noisyLimitStateReturnsCorrectValues, &
        "Test on correct return values for noisyLimitState", 1)
    call testWithLevel( resistanceSolicitation1QuadraticTermReturnsCorrectValues, &
        "Test on correct return values for resistanceSolicitation1QuadraticTerm", 1)
    call testWithLevel( limitState10QuadraticTermsReturnsCorrectValues, &
        "Test on correct return values for limitState10QuadraticTerms", 1)
    call testWithLevel( limitState25QuadraticTermsReturnsCorrectValues, &
        "Test on correct return values for limitState25QuadraticTerms", 1)
    call testWithLevel( convexFailureDomainReturnsCorrectValues, &
        "Test on correct return values for convexFailureDomain", 1)
    call testWithLevel( oblateSpheroidReturnsCorrectValues, &
        "Test on correct return values for oblateSpheroid", 1)
    call testWithLevel( saddleSurfaceReturnsCorrectValues, &
        "Test on correct return values for saddleSurface", 1)
    call testWithLevel( discontinuousLimitStateFunctionReturnsCorrectValues, &
        "Test on correct return values for discontinuousLimitStateFunction", 1)
    call testWithLevel( twoBranchesReturnsCorrectValues, &
        "Test on correct return values for twoBranches", 1)
    call testWithLevel( concaveFailureDomainReturnsCorrectValues, &
        "Test on correct return values for concaveFailureDomain", 1)
    call testWithLevel( seriesSystemReturnsCorrectValues, &
        "Test on correct return values for seriesSystemReturns", 1)
    call testWithLevel( parallelSystemReturnsCorrectValues, &
        "Test on correct return values for parallelSystem", 1)

end subroutine allWaartsFunctionsTests

!> Check if linear Resistance-Solicitation problem returns correct values
subroutine linearResistanceSolicitationReturnsCorrectValues

    real (kind=wp) :: actual
    real (kind=wp) :: expected

    expected = 1.0d0
    actual = linearResistanceSolicitation( 3.0d0, 2.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    actual = linearResistanceSolicitation( 1.0d0, 3.5d0 )
    expected = -2.5d0
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine linearResistanceSolicitationReturnsCorrectValues

!> Check if noisy limit state function returns correct values
subroutine noisyLimitStateReturnsCorrectValues

    real (kind=wp) :: actual
    real (kind=wp) :: expected
    real (kind=wp), dimension(6):: x

    x(1) = 1.0d0
    x(2) = 1.0d0
    x(3) = 1.0d0
    x(4) = 1.0d0
    x(5) = 1.0d0
    x(6) = 1.0d0
    expected = -4.003038194d0
    actual = noisyLimitState( x )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    x(1) = 120.0d0
    x(2) = 120.0d0
    x(3) = 120.0d0
    x(4) = 120.0d0
    x(5) = 50.0d0
    x(6) = 40.0d0
    actual = noisyLimitState( x )
    expected = 269.9952356d0
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    x(1) = 1.0d0
    x(2) = 2.0d0
    x(3) = 3.0d0
    x(4) = 4.0d0
    x(5) = 5.0d0
    x(6) = 6.0d0
    actual = noisyLimitState( x )
    expected = -40.00365393d0
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine noisyLimitStateReturnsCorrectValues

!> Check if linear Resistance-Solicitation problem with one quadratic term returns correct values
subroutine resistanceSolicitation1QuadraticTermReturnsCorrectValues

    real (kind=wp) :: actual
    real (kind=wp) :: expected

    expected = 1.0d0
    actual = resistanceSolicitation1QuadraticTerm( 5.0d0, 2.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    actual = resistanceSolicitation1QuadraticTerm( 3.5d0, 2.0d0 )
    expected = -0.5d0
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine resistanceSolicitation1QuadraticTermReturnsCorrectValues

!> Check if Limit state function with 10 quadratic terms returns correct values
subroutine limitState10QuadraticTermsReturnsCorrectValues

    real (kind=wp)                  :: actual
    real (kind=wp)                  :: expected
    real (kind=wp)                  :: r
    real (kind=wp), dimension(10)   :: s
    integer                         :: i

    r = 0.5d0
    do i = 1, 10
      s(i) = 0.2d0
    end do
    expected = 0.38284127d0
    actual = limitState10QuadraticTerms( r, s )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    r = 1.0d0
    do i = 1, 10
      s(i) = 0.1d0 * i
    end do
    expected = 0.45d0
    actual = limitState10QuadraticTerms( r, s )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine limitState10QuadraticTermsReturnsCorrectValues

!> Check if Limit state function with 25 quadratic terms returns correct values
subroutine limitState25QuadraticTermsReturnsCorrectValues

    real (kind=wp)                  :: actual
    real (kind=wp)                  :: expected
    real (kind=wp)                  :: r
    real (kind=wp), dimension(25)   :: s
    integer                         :: i

    r = 0.5d0
    do i = 1, 25
      s(i) = 0.2d0
    end do
    expected = 0.347361673d0
    actual = limitState25QuadraticTerms( r, s )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    r = 1.0d0
    do i = 1, 25
      s(i) = 0.1d0 * i
    end do
    expected = -2.25d0
    actual = limitState25QuadraticTerms( r, s )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine limitState25QuadraticTermsReturnsCorrectValues

!> Check if Limit state function convex failure domain returns correct values
subroutine convexFailureDomainReturnsCorrectValues

    real (kind=wp) :: u1
    real (kind=wp) :: u2
    real (kind=wp) :: actual
    real (kind=wp) :: expected

    expected = 2.5d0
    u1 = 0.0d0
    u2 = 0.0d0
    actual = convexFailureDomain( u1, u2 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = 1.085786438d0
    u1 = 1.0d0
    u2 = 1.0d0
    actual = convexFailureDomain( u1, u2 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = -1.642640687d0
    u1 = 2.5d0
    u2 = 3.5d0
    actual = convexFailureDomain( u1, u2 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine convexFailureDomainReturnsCorrectValues

!> Check if Limit state function oblate spheroid terms returns correct values
subroutine oblateSpheroidReturnsCorrectValues

    real (kind=wp)                  :: actual
    real (kind=wp)                  :: expected
    real (kind=wp)                  :: r
    real (kind=wp), dimension(10)   :: s
    integer                         :: i

    r = 0.5d0
    do i = 1, 10
      s(i) = 0.2d0
    end do
    expected = 0.232491439d0
    actual = oblateSpheroid( r, s )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    r = 1.0d0
    do i = 1, 10
      s(i) = 0.1d0 * i
    end do
    expected = -1.187714032d0
    actual = oblateSpheroid( r, s )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine oblateSpheroidReturnsCorrectValues

!> Check if function saddle surface returns correct values
subroutine saddleSurfaceReturnsCorrectValues

    real (kind=wp) :: u1
    real (kind=wp) :: u2
    real (kind=wp) :: actual
    real (kind=wp) :: expected

    expected = 3.0d0
    u1 = 0.0d0
    u2 = 0.0d0
    actual = saddleSurface( u1, u2 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = 2.0
    u1 = 1.0d0
    u2 = 1.0d0
    actual = saddleSurface( u1, u2 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = -5.75d0
    u1 = 2.5d0
    u2 = 3.5d0
    actual = saddleSurface( u1, u2 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine saddleSurfaceReturnsCorrectValues

!> Check if function discontinuous limit state function returns correct values
subroutine discontinuousLimitStateFunctionReturnsCorrectValues

    real (kind=wp) :: actual
    real (kind=wp) :: expected

    expected = 0.5d0
    actual = discontinuousLimitStateFunction( 3.0d0, 2.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = 1.5d0
    actual = discontinuousLimitStateFunction( 5.0d0, 1.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    actual = discontinuousLimitStateFunction( 3.0d0, 3.0d0 )
    expected = -0.5d0
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    actual = discontinuousLimitStateFunction( 2.0d0, 3.0d0 )
    expected = -0.5d0
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine discontinuousLimitStateFunctionReturnsCorrectValues

!> Check if function discontinuous limit state returns correct values
subroutine twoBranchesReturnsCorrectValues

    real (kind=wp) :: actual
    real (kind=wp) :: expected

    expected = -1.0d0
    actual = twoBranches( 8.0d0,  6.0d0, 3.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = -2.0d0
    actual = twoBranches( 7.0d0,  4.0d0, 5.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = 5.0d0
    actual = twoBranches( 3.0d0,  2.0d0, 7.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine twoBranchesReturnsCorrectValues

!> Check if function concave failure domain returns correct values
subroutine concaveFailureDomainReturnsCorrectValues

    real (kind=wp) :: actual
    real (kind=wp) :: expected

    expected = 3.0d0
    actual = concaveFailureDomain( 0.0d0,  0.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = 1.585786438d0
    actual = concaveFailureDomain( 1.0d0,  1.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = 0.378679656d0
    actual = concaveFailureDomain( 1.0d0,  2.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine concaveFailureDomainReturnsCorrectValues

!> Check if function series systems returns correct values
subroutine seriesSystemReturnsCorrectValues

    real (kind=wp) :: actual
    real (kind=wp) :: expected

    expected = 2.930289322d0
    actual = seriesSystem( 0.0d0,  0.1d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = -0.050252532d0
    actual = seriesSystem( -1.0d0,  4.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = -9.050252532d0
    actual = seriesSystem( 10.0d0,  -4.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = -11.14213562d0
    actual = seriesSystem( -10.0d0,  -10.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine seriesSystemReturnsCorrectValues

!> Check if function parallel system returns correct values
subroutine parallelSystemReturnsCorrectValues

    real (kind=wp) :: actual
    real (kind=wp) :: expected

    expected = 2.677d0
    actual = parallelSystem( 0.0d0, 0.0d0,  0.0d0,  0.0d0,  0.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = 8.5d0
    actual = parallelSystem( 2.0d0, -4.0d0,  -2.0d0,  4.0d0,  5.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = 3.323d0
    actual = parallelSystem( 1.0d0, 2.0d0,  3.0d0,  -4.0d0,  5.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

    expected = 8.25d0
    actual = parallelSystem( 1.0d0, 2.0d0,  3.0d0,  -4.0d0,  -2.0d0 )
    call assert_comparable( actual, expected, margin, "unexpected return value" )

end subroutine parallelSystemReturnsCorrectValues

end module waartsFunctionsTests
