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
!! Testing of functions in CombineElements
!<
!> Module holding all tests of combineElements
!
module combineElementsTests

    use precision
    use interface_combin
    implicit none

    private
    public :: upscaleLengthWrapper

contains
!

subroutine upscaleLengthWrapper ( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection)
        real(kind=wp), intent (in)          :: betaCrossSection       !< Reliability index cross section
        real(kind=wp), intent (in)          :: alphaCrossSection(:)   !< Alpha vector cross section
        real(kind=wp), intent (in)          :: rhoXK(:)               !< Correlation variables
        real(kind=wp), intent (in)          :: dXK(:)                 !< Correlation length variables
        real(kind=wp), intent (in)          :: sectionLength          !< Section length
        real(kind=wp), intent (out)         :: betaSection            !< Reliability index section
        real(kind=wp), intent (out)         :: alphaSection(:)        !< Alpha vector section

        type(designPoint)    :: dpCrossSection !< design point cross section
        type(designPoint)    :: dpSection      !< design point section
        integer              :: i, nStoch

        nStoch = size(alphaCrossSection)
        dpCrossSection%beta = betaCrossSection
        allocate(dpCrossSection%alpha(nStoch))
        allocate(dpCrossSection%rho(nStoch))
        allocate(dpCrossSection%correlation_length(nStoch))
        do i = 1, nStoch
            dpCrossSection%alpha(i) = alphaCrossSection(i)
            dpCrossSection%rho(i) = rhoXK(i)
            dpCrossSection%correlation_length(i) = dXK(i)
        end do
        allocate(dpSection%alpha(nStoch))
        call  upscaleLength ( dpCrossSection, sectionLength, dpSection)
        betaSection = dpSection%beta
        do i = 1, nStoch
            alphaSection(i) = dpSection%alpha(i)
        end do
        deallocate(dpCrossSection%alpha, dpCrossSection%rho, dpCrossSection%correlation_length, dpSection%alpha)
end subroutine upscaleLengthWrapper


end module combineElementsTests
