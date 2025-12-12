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
!! Waarts functions. These functions are used for testing the probabilistic methods
!<
!
module waartsFunctions

    use precision

    implicit none

    private

    public :: linearResistanceSolicitation, limitState25QuadraticTerms, oblateSpheroid

contains

!> Standard linear Resistance-Solicitation problem
function linearResistanceSolicitation(r, s)

    real (kind=wp), intent(in) :: r                               !< input parameter r
    real (kind=wp), intent(in) :: s                               !< input parameter s
    real (kind=wp)             :: linearResistanceSolicitation    !< output parameter

    linearResistanceSolicitation = r - s

end function linearResistanceSolicitation

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

end module waartsFunctions
