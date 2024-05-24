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
!! Waarts functions. These functions are used for testing the probabilistic methods
!<
!
module waartsFunctions

    use precision

    implicit none

    private

    public :: convexFailureDomain, discontinuousLimitStateFunction, concaveFailureDomain, &
              limitState10QuadraticTerms, limitState25QuadraticTerms, twoBranches, saddleSurface, &
              linearResistanceSolicitation, noisyLimitState, oblateSpheroid, &
              resistanceSolicitation1QuadraticTerm, seriesSystem, parallelSystem

contains

!> Standard linear Resistance-Solicitation problem
function linearResistanceSolicitation(r, s)

    real (kind=wp), intent(in) :: r                               !< input parameter r
    real (kind=wp), intent(in) :: s                               !< input parameter s
    real (kind=wp)             :: linearResistanceSolicitation    !< output parameter

    linearResistanceSolicitation = r - s

end function linearResistanceSolicitation

!> Noisy limit state function
function noisyLimitState(x)

    real (kind=wp), dimension(6), intent(in)    :: x                !< input array; 6 parameters
    real (kind=wp)                              :: noisyLimitState  !< output parameter
    real (kind=wp)                              :: summation

    summation = sum( sin( 100.0d0 * x ) )

    noisyLimitState = x(1) + 2 * x(2) +  2 * x(3) + x(4) - 5 * x(5) -5 * x(6) + 0.001d0 * summation

end function noisyLimitState

!> Resistance-solicitation problem with one quadratic term
function resistanceSolicitation1QuadraticTerm( r, s )

    real (kind=wp), intent(in) :: r                                     !< input parameter r
    real (kind=wp), intent(in) :: s                                     !< input parameter s
    real (kind=wp)             :: resistanceSolicitation1QuadraticTerm  !< output parameter

    resistanceSolicitation1QuadraticTerm = r - s * s

end function resistanceSolicitation1QuadraticTerm

!> Limit state function with 10 quadratic terms
function limitState10QuadraticTerms( r, s )

    real (kind=wp), intent(in)                  :: r                           !< input parameter r
    real (kind=wp), dimension(10), intent(in)   :: s                           !< input parameter array(10) s
    real (kind=wp)                              :: limitState10QuadraticTerms  !< output parameter
    real (kind=wp)                              :: summation
    integer                                     :: i

    summation = 0.0d0
    do i = 1, 10
      summation = summation + s(i) ** 2 / i
    end do

    limitState10QuadraticTerms = r - summation

end function limitState10QuadraticTerms

!> Limit state function with 25 quadratic terms
function limitState25QuadraticTerms( r, s )

    real (kind=wp), intent(in)                  :: r                           !< input parameter r
    real (kind=wp), dimension(25), intent(in)   :: s                           !< input parameter array(25) s
    real (kind=wp)                              :: limitState25QuadraticTerms  !< output parameter
    real (kind=wp)                              :: summation
    integer                                     :: i

    summation = 0.0d0
    do i = 1, 25
      summation = summation + s(i) ** 2 / i
    end do

    limitState25QuadraticTerms = r - summation

end function limitState25QuadraticTerms

!> Convex failure domain function
function convexFailureDomain( u1, u2 )

    real (kind=wp), intent(in)                  :: u1                          !< input parameter u1
    real (kind=wp), intent(in)                  :: u2                          !< input parameter u2
    real (kind=wp)                              :: convexFailureDomain         !< output parameter

    convexFailureDomain = 0.1d0 * ( u1 - u2 ) ** 2 - ( u1 + u2 ) / sqrt( 2.0d0 ) + 2.5d0

end function convexFailureDomain

!> Oblate spheroid function
function oblateSpheroid(r, s)

    real (kind=wp), intent(in)                  :: r                            !< input parameter r
    real (kind=wp), dimension(10), intent(in)   :: s                            !< input parameter array(10) s
    real (kind=wp)                              :: summation
    real (kind=wp)                              :: oblateSpheroid               !< output parameter
    integer                                     :: i

    summation = 0.0d0
    do i = 1, 10
      summation = summation + s(i) ** 2 / ( 1.0d0 + i / 10.0d0)
    end do
    oblateSpheroid = r - summation

end function oblateSpheroid

!> Saddle surface function
function saddleSurface( u1, u2 )

    real (kind=wp), intent(in)                  :: u1                           !< input parameter u1
    real (kind=wp), intent(in)                  :: u2                           !< input parameter u2
    real (kind=wp)                              :: saddleSurface                !< output parameter

    saddleSurface = 3.0d0 - u1 * u2

end function saddleSurface

!> Discontinuous limit state function
function discontinuousLimitStateFunction( r, s )

    real (kind=wp), intent(in) :: r                                 !< input parameter r
    real (kind=wp), intent(in) :: s                                 !< input parameter s
    real (kind=wp)             :: discontinuousLimitStateFunction   !< output parameter

    if (r >= s) then
      discontinuousLimitStateFunction = -0.5d0 + sqrt( r - s )
    else
      discontinuousLimitStateFunction = -0.5d0
    endif

end function discontinuousLimitStateFunction

!> Two branches
function twoBranches( x1, x2, x3 )

    real (kind=wp), intent(in) :: x1            !< input parameter x1
    real (kind=wp), intent(in) :: x2            !< input parameter x2
    real (kind=wp), intent(in) :: x3            !< input parameter x3
    real (kind=wp)             :: twoBranches   !< output parameter

    if (x3 <= 5.0d0) then
      twoBranches = x1 -x2 -x3
    else
      twoBranches = x3 - x2
    endif

end function twoBranches

!> Concave failure domain
function concaveFailureDomain( u1, u2 )

    real (kind=wp), intent(in)                  :: u1                       !< input parameter u1
    real (kind=wp), intent(in)                  :: u2                       !< input parameter u2
    real (kind=wp)                              :: concaveFailureDomain     !< output parameter

    concaveFailureDomain = -0.5d0 * ( u1 * u1 + u2 * u2 - 2.0d0 * u1 * u2) - ( u1 + u2 ) / sqrt(2.0d0) + 3.0d0

end function concaveFailureDomain

!> Series system
function seriesSystem(u1, u2)

    real (kind=wp), intent(in)                  :: u1               !< input parameter u1
    real (kind=wp), intent(in)                  :: u2               !< input parameter u2
    real (kind=wp)                              :: seriesSystem     !< output parameter
    real (kind=wp)                              :: g1
    real (kind=wp)                              :: g2
    real (kind=wp)                              :: g3
    real (kind=wp)                              :: g4

    g1 = 0.1d0 * ( u1 - u2 ) ** 2 - ( u1 + u2 ) / sqrt( 2.0d0 ) + 3.0d0
    g2 = 0.1d0 * ( u1 - u2 ) ** 2 + ( u1 + u2 ) / sqrt( 2.0d0 ) + 3.0d0
    g3 = u1 - u2 + 3.5d0 * sqrt( 2.0d0 )
    g4 = -u1 + u2 + 3.5d0 * sqrt( 2.0d0 )
    seriesSystem = min( g1, g2, g3, g4 )

end function seriesSystem

!> Series system
function parallelSystem( u1, u2, u3, u4, u5 )

    real (kind=wp), intent(in)                  :: u1               !< input parameter u1
    real (kind=wp), intent(in)                  :: u2               !< input parameter u2
    real (kind=wp), intent(in)                  :: u3               !< input parameter u3
    real (kind=wp), intent(in)                  :: u4               !< input parameter u4
    real (kind=wp), intent(in)                  :: u5               !< input parameter u5
    real (kind=wp)                              :: parallelSystem   !< output parameter
    real (kind=wp)                              :: g1
    real (kind=wp)                              :: g2
    real (kind=wp)                              :: g3
    real (kind=wp)                              :: g4

    g1 = 2.677d0 - u1 - u2
    g2 = 2.500d0 - u2 - u3
    g3 = 2.323d0 - u3 - u4
    g4 = 2.250d0 - u4 - u5
    parallelSystem = max( g1, g2, g3, g4 )

end function parallelSystem

end module waartsFunctions
