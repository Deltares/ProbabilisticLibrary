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
module interface_combin
use, intrinsic :: iso_c_binding, only: c_double
use precision, only : wp
implicit none
integer, parameter :: combAND =  0
integer, parameter :: combOR  =  1

private :: combineMultipleElements_c, warnHohenbichler, upscaleLengthC, &
    combineTwoElementsPartialCorrelationC1, combineTwoElementsPartialCorrelationC2, &
    combineMultipleElementsProb_c, c_double, wp

interface
    integer function combineMultipleElements_c( betaElement, alphaElement, rho, beta, alpha, &
      combAndOrIn, nrElms, nrStoch) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double),  intent(in)  :: betaElement(*)
        real(kind=c_double),  intent(in)  :: alphaElement(*)
        real(kind=c_double),  intent(in)  :: rho(*)
        real(kind=c_double),  intent(out) :: beta
        real(kind=c_double),  intent(out) :: alpha(*)
        integer, value,       intent(in)  :: combAndOrIn, nrElms, nrStoch
    end function combineMultipleElements_c
end interface

interface
    subroutine combineMultipleElementsGeneral( betaElement, alphaElement, rho, beta, alpha, &
      combAndOrIn, combinerType, nrElms, nrStoch) bind(c, name="combineMultipleElementsGeneral")
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double),  intent(in)  :: betaElement(*)
        real(kind=c_double),  intent(in)  :: alphaElement(*)
        real(kind=c_double),  intent(in)  :: rho(*)
        real(kind=c_double),  intent(out) :: beta
        real(kind=c_double),  intent(out) :: alpha(*)
        integer, value,       intent(in)  :: combAndOrIn, combinerType, nrElms, nrStoch
    end subroutine combineMultipleElementsGeneral
end interface

interface
!>
!! This subroutine upscales from a cross section to a given section length
    integer function upscaleLengthC ( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection, &
            nStochasts ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double), intent (in)  :: betaCrossSection     !< Reliability index cross section
        real(kind=c_double), intent (in)  :: alphaCrossSection(*) !< Alpha vector cross section
        real(kind=c_double), intent (in)  :: rhoXK(*)             !< Correlation variables
        real(kind=c_double), intent (in)  :: dXK(*)               !< Correlation length variables
        real(kind=c_double), intent (in)  :: sectionLength        !< Section length
        real(kind=c_double), intent (out) :: betaSection          !< Reliability index section
        real(kind=c_double), intent (out) :: alphaSection(*)      !< Alpha vector section
        integer, value,      intent (in)  :: nStochasts           !< number of stochasts
    end function upscaleLengthC
end interface

interface
!>
!! Subroutine for upscaling random variables to the largest block duration
    subroutine upscaleToLargestBlockC( betaSmallBlock, alphaSmallBlock, rhoTSmallBlock,  &
                                      blockDurations, largestBlockDuration,             &
                                      betaLargestBlock, alphaLargestBlock, durationsLargestBlock, nStochasts) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double), intent (in)  :: betaSmallBlock           !< Reliability index input
        real(kind=c_double), intent (in)  :: alphaSmallBlock(*)       !< Alpha vector input
        real(kind=c_double), intent (in)  :: rhoTSmallBlock(*)        !< Correlations
        real(kind=c_double), intent (in)  :: blockDurations(*)        !< Block durations vector input
        real(kind=c_double), intent (in)  :: largestBlockDuration     !< Target block duration
        real(kind=c_double), intent (out) :: betaLargestBlock         !< Reliability index result
        real(kind=c_double), intent (out) :: alphaLargestBlock(*)     !< Alpha vector result
        real(kind=c_double), intent (out) :: durationsLargestBlock(*) !< Block durations vector result
        integer, value,      intent (in)  :: nStochasts               !< number of stochasts
    end subroutine upscaleToLargestBlockC
end interface

interface
!> Subroutine for combining two elements with partial correlation
    integer function combineTwoElementsPartialCorrelationC1( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAndOr, &
            nStochasts, alphaI, alphaII ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double),  intent(in)           :: beta1       !< Reliability index of element 1
        real(kind=c_double),  intent(in)           :: alpha1(*)   !< Alpha values of element 1
        real(kind=c_double),  intent(in)           :: beta2       !< Reliability index of element 2
        real(kind=c_double),  intent(in)           :: alpha2(*)   !< Alpha values of element 2
        real(kind=c_double),  intent(in)           :: rhoP(*)     !< Autocorrelation of the stochastic variables between element 1 and element 2
        real(kind=c_double),  intent(out)          :: betaC       !< Reliability index of the combined elements
        real(kind=c_double),  intent(out)          :: alphaC(*)   !< Alpha values of the combined elements
        integer, value,       intent(in)           :: combAndOr   !< Combination type, And or Or
        integer, value,       intent(in)           :: nStochasts  !< number of stochasts
        real(kind=c_double), optional, intent(out) :: alphaI(*)   !< AlphaI values of the combined elements in case of a spatial correlation
        real(kind=c_double), optional, intent(out) :: alphaII(*)  !< AlphaII values of the combined elements in case of a spatial correlation
    end function combineTwoElementsPartialCorrelationC1
end interface

interface
!> Subroutine for combining two elements with partial correlation
    integer function combineTwoElementsPartialCorrelationC2( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAndOr, &
            nStochasts ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double),  intent(in)           :: beta1       !< Reliability index of element 1
        real(kind=c_double),  intent(in)           :: alpha1(*)   !< Alpha values of element 1
        real(kind=c_double),  intent(in)           :: beta2       !< Reliability index of element 2
        real(kind=c_double),  intent(in)           :: alpha2(*)   !< Alpha values of element 2
        real(kind=c_double),  intent(in)           :: rhoP(*)     !< Autocorrelation of the stochastic variables between element 1 and element 2
        real(kind=c_double),  intent(out)          :: betaC       !< Reliability index of the combined elements
        real(kind=c_double),  intent(out)          :: alphaC(*)   !< Alpha values of the combined elements
        integer, value,       intent(in)           :: combAndOr   !< Combination type, And or Or
        integer, value,       intent(in)           :: nStochasts  !< number of stochasts
    end function combineTwoElementsPartialCorrelationC2
end interface

interface
    integer function combineMultipleElementsProb_c( betaElement, alphaElement, percentages, beta, alpha, &
            combAndOrIn, nrElms, nrStoch ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double),  intent(in)    :: betaElement(*)       !< Reliability index per element
        real(kind=c_double),  intent(in)    :: alphaElement(*)      !< Alpha vector per element
        real(kind=c_double),  intent(in)    :: percentages(*)       !< Array of percentages
        real(kind=c_double),  intent(out)   :: beta                 !< Reliability index after combining over elements
        real(kind=c_double),  intent(out)   :: alpha(*)             !< Alpha vector after combining over elements
        integer, value,       intent(in)    :: combAndOrIn          !< Combination type, And or Or
        integer, value,       intent(in)    :: nrElms, nrStoch
    end function combineMultipleElementsProb_c
end interface

contains

    subroutine warnHohenbichler(n)
        use feedback, only : warningMessage
        integer, intent(in) :: n
        if (n == 1) then
            call warningMessage('FORM in Hohenbichler did not converge.')
        else if (n > 1) then
            call warningMessage('FORM in Hohenbichler did not converge. x ', n)
        end if
    end subroutine warnHohenbichler

!>
!! This subroutine upscales from a cross section to a given section length
    subroutine upscaleLength ( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection)
        real(kind=c_double), intent (in)          :: betaCrossSection       !< Reliability index cross section
        real(kind=c_double), intent (in)          :: alphaCrossSection(:)   !< Alpha vector cross section
        real(kind=c_double), intent (in)          :: rhoXK(:)               !< Correlation variables
        real(kind=c_double), intent (in)          :: dXK(:)                 !< Correlation length variables
        real(kind=c_double), intent (in)          :: sectionLength          !< Section length
        real(kind=c_double), intent (out)         :: betaSection            !< Reliability index section
        real(kind=c_double), intent (out)         :: alphaSection(:)        !< Alpha vector section

        integer :: n

        n = upscaleLengthC ( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection, &
                size(rhoXK) )
        call warnHohenbichler(n)
    end subroutine upscaleLength

!>
!! Subroutine for upscaling random variables to the largest block duration
    subroutine upscaleToLargestBlock( betaSmallBlock, alphaSmallBlock, rhoTSmallBlock,  &
                                      blockDurations, largestBlockDuration,             &
                                      betaLargestBlock, alphaLargestBlock, durationsLargestBlock)
        real(kind=c_double), intent (in)  :: betaSmallBlock           !< Reliability index input
        real(kind=c_double), intent (in)  :: alphaSmallBlock(:)       !< Alpha vector input
        real(kind=c_double), intent (in)  :: rhoTSmallBlock(:)        !< Correlations
        real(kind=c_double), intent (in)  :: blockDurations(:)        !< Block durations vector input
        real(kind=c_double), intent (in)  :: largestBlockDuration     !< Target block duration
        real(kind=c_double), intent (out) :: betaLargestBlock         !< Reliability index result
        real(kind=c_double), intent (out) :: alphaLargestBlock(:)     !< Alpha vector result
        real(kind=c_double), intent (out) :: durationsLargestBlock(:) !< Block durations vector result

    call upscaleToLargestBlockC(betaSmallBlock, alphaSmallBlock, rhoTSmallBlock, blockDurations, largestBlockDuration,  &
                                betaLargestBlock, alphaLargestBlock, durationsLargestBlock, size(alphaSmallBlock))
    end subroutine upscaleToLargestBlock

!> Subroutine for combining two elements with partial correlation
    subroutine combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAndOr, alphaI, alphaII )
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=c_double),  intent(in)              :: beta1             !< Reliability index of element 1
    real(kind=c_double),  intent(in)              :: alpha1(:)         !< Alpha values of element 1
    real(kind=c_double),  intent(in)              :: beta2             !< Reliability index of element 2
    real(kind=c_double),  intent(in)              :: alpha2(:)         !< Alpha values of element 2
    real(kind=c_double),  intent(in)              :: rhoP(:)           !< Autocorrelation of the stochastic variables between element 1 and element 2
    real(kind=c_double),  intent(out)             :: betaC             !< Reliability index of the combined elements
    real(kind=c_double),  intent(out)             :: alphaC(:)         !< Alpha values of the combined elements
    integer,              intent(in)              :: combAndOr         !< Combination type, And or Or
    real(kind=c_double), optional, intent(out)    :: alphaI(:)         !< AlphaI values of the combined elements in case of a spatial correlation
    real(kind=c_double), optional, intent(out)    :: alphaII(:)        !< AlphaII values of the combined elements in case of a spatial correlation

    integer :: n

    if (present(alphaI) .and. present(alphaII)) then
        n = combineTwoElementsPartialCorrelationC1(beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAndOr, size(alpha1), &
            alphaI, alphaII)
    else
        n = combineTwoElementsPartialCorrelationC2(beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAndOr, size(alpha1))
    end if
    call warnHohenbichler(n)
end subroutine combineTwoElementsPartialCorrelation

subroutine combineMultipleElements( betaElement, alphaElement, rho, beta, alpha, combAndOrIn)
    real(kind=c_double),  intent(in)  :: betaElement(:)
    real(kind=c_double),  intent(in)  :: alphaElement(:,:)
    real(kind=c_double),  intent(in)  :: rho(:)
    real(kind=c_double),  intent(out) :: beta
    real(kind=c_double),  intent(out) :: alpha(:)
    integer, optional,    intent(in)  :: combAndOrIn

    integer :: nrElms, nrStoch, combAndOr, n

    nrElms =  size(betaElement)
    nrStoch = size(rho)
    if ( present(combAndOrIn) ) then
        combAndOr = combAndOrIn
    else
        combAndOr = combOr
    end if

    n = combineMultipleElements_c(betaElement, alphaElement, rho, beta, alpha, combAndOr, nrElms, nrStoch)

    call warnHohenbichler(n)

end subroutine combineMultipleElements

!> This subroutine calculates the reliability index (beta) and alpha values combining over elements
subroutine combineMultipleElementsProb( betaElement, alphaElement, percentages, beta, alpha, combAndOrIn )
    real(kind=c_double),  intent(in)    :: betaElement(:)       !< Reliability index per element
    real(kind=c_double),  intent(in)    :: alphaElement(:,:)    !< Alpha vector per element
    real(kind=c_double),  intent(in)    :: percentages(:)       !< Array of percentages
    real(kind=c_double),  intent(out)   :: beta                 !< Reliability index after combining over elements
    real(kind=c_double),  intent(out)   :: alpha(:)             !< Alpha vector after combining over elements
    integer, optional,    intent(in)    :: combAndOrIn          !< Combination type, And or Or

    integer :: nrElms, nrStoch, combAndOr, n

    nrElms =  size(betaElement)
    nrStoch = size(alpha)
    if ( present(combAndOrIn) ) then
        combAndOr = combAndOrIn
    else
        combAndOr = combOr
    end if

    n = combineMultipleElementsProb_c( betaElement, alphaElement, percentages, beta, alpha, combAndOr, nrElms, nrStoch)

    call warnHohenbichler(n)

end subroutine combineMultipleElementsProb

! TODO convert to wrapper to cpp code
subroutine calculateCombinationWithLargestCorrelation( nStochasts, rhoP, nElements, alpha, i1max, i2max)
    use precision, only : wp
    use feedback, only : fatalError
    integer,        intent(in)  :: nStochasts           !< Number of random variables
    real(kind=wp),  intent(in)  :: rhoP(nStochasts)     !< Autocorrelation the random variables between elements
    integer,        intent(in)  :: nElements            !< Number of elements to be combined (for instance tidal periods)
    real(kind=wp),  intent(in)  :: alpha(:,:)           !< Alpha vector per element
    integer,        intent(out) :: i1max                !< Index of first element with the largest correlation
    integer,        intent(out) :: i2max                !< Index of second element with the largest correlation

    integer                     :: i1                   ! Do-loop counter1
    integer                     :: i2                   ! Do-loop counter2
    integer                     :: j                    ! Do-loop counter for the random variables
    real                        :: rhoT                 ! Correlation between elements
    real(kind=wp)               :: rhoMax               ! Largest correlation between two elements in the vector
    !
    ! Two elements can't be computed if there is only one element
    !
    if (nElements < 2) then
        call fatalError( "The subroutine calculateCombinationWithLargestCorrelation is called with only one element" )
        return
    end if
    !
    ! Initialize rhoMax and the indices for the maximum element
    !
    rhoMax = -1.0d0
    i1max  = -1
    i2max  = -1
    !
    !   Determine which two elements have the highest correlation
    !
    do i1 = 1, nElements - 1
        do i2 = i1 + 1, nElements
            !
            ! Computate correlation
            !
            rhoT = 0.0d0
            do j = 1, nStochasts
                rhoT = rhoT + alpha(i1, j) * alpha(i2, j) * rhoP(j)
            end do
            if (rhoT > rhoMax .or. (i1 == 1 .and. i2 == 2)) then
                !
                ! For the first combination the parameters i1max, i2max and rhoMax are set
                !
                i1max  = i1
                i2max  = i2
                rhoMax = rhoT
            end if
        end do
    end do
end subroutine calculateCombinationWithLargestCorrelation

end module interface_combin
