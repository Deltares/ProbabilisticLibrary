! Copyright (C) Stichting Deltares. All rights reserved.
!
! This file is part of the Probabilistic Library.
!
! The Probabilistic Library is free software: you can redistribute it and/or modify
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
use, intrinsic :: iso_c_binding, only: c_double, c_ptr, c_loc, c_null_ptr, c_intptr_t
use precision, only : wp
use interface_typedefs, only : designPoint
implicit none
integer, parameter :: combAND =  0
integer, parameter :: combOR  =  1

private 

public :: betaAlphaCF, DesignPoint, multipleElements, combAND, combOR, &
    combineMultipleElementsGeneral, upscaleLength, upscaleToLargestBlock, &
    combinerSettings, combineMultipleElements, combineMultipleElementsProb, &
    combineTwoElementsPartialCorrelation, calculateCombinationWithLargestCorrelation

type, bind(C) :: betaAlphaCF
    real(kind=c_double) :: beta               = 0.0_c_double
    type(c_ptr)         :: alpha              = c_null_ptr
    type(c_ptr)         :: rho                = c_null_ptr
    type(c_ptr)         :: duration           = c_null_ptr
    type(c_ptr)         :: correlation_length = c_null_ptr
    integer             :: size               = 0
    integer             :: stride_alpha       = 1
    integer             :: stride_duration    = 1
end type betaAlphaCF

type, bind(C) :: multipleElements
    type(c_ptr) :: designPoints
    integer     :: nElements
end type multipleElements

type, bind(C) :: combinerSettings
    integer :: combAndOr
    integer :: combinerType
end type combinerSettings

interface
    integer function combineMultipleElements_c( elements, dpOut) bind(c)
        import betaAlphaCF, multipleElements
        type(multipleElements),  intent(in)  :: elements
        type(betaAlphaCF),       intent(out) :: dpOut
    end function combineMultipleElements_c
end interface

interface
    subroutine combineMultipleElementsGeneral( elements, dpOut, settings) bind(c, name="combineMultipleElementsGeneral")
        import betaAlphaCF, multipleElements, combinerSettings
        type(multipleElements), intent(in)  :: elements
        type(betaAlphaCF),      intent(out) :: dpOut
        type(combinerSettings), intent(in)  :: settings
    end subroutine combineMultipleElementsGeneral
end interface

interface
!>
!! This subroutine upscales from a cross section to a given section length
    integer function upscaleLengthC ( dpCrossSection, sectionLength, dpSection ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        import betaAlphaCF
        type(betaAlphaCF),   intent (in)    :: dpCrossSection     !< design point cross section
        real(kind=c_double), intent (in)    :: sectionLength      !< Section length
        type(betaAlphaCF),   intent (inout) :: dpSection          !< design point section
    end function upscaleLengthC
end interface

interface
!>
!! Subroutine for upscaling random variables to the largest block duration
    subroutine upscaleToLargestBlockC( dpSmallBlock, largestBlockDuration, dpLargestBlock) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        import betaAlphaCF
        type(betaAlphaCF),   intent (in)    :: dpSmallBlock           !< design point input
        real(kind=c_double), intent (in)    :: largestBlockDuration   !< Target block duration
        type(betaAlphaCF),   intent (inout) :: dpLargestBlock         !< design point result
    end subroutine upscaleToLargestBlockC
end interface

interface
!> Subroutine for combining two elements with partial correlation
    integer function combineTwoElementsPartialCorrelationC2( dp1, dp2, dpC, combAndOr ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        import betaAlphaCF
        type(betaAlphaCF),    intent(in)           :: dp1         !< design point of element 1
        type(betaAlphaCF),    intent(in)           :: dp2         !< design point of element 2
        type(betaAlphaCF),    intent(inout)        :: dpC         !< design point of the combined elements
        integer, value,       intent(in)           :: combAndOr   !< Combination type, And or Or
    end function combineTwoElementsPartialCorrelationC2
end interface

interface
    integer function combineMultipleElementsProb_c( elements,  percentages, dpOut ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        import :: betaAlphaCF, multipleElements
        type(multipleElements), intent(in)    :: elements         !< input design points
        real(kind=c_double),    intent(in)    :: percentages(*)   !< Array of percentages
        type(betaAlphaCF),      intent(out)   :: dpOut            !< Reliability index after combining over elements
    end function combineMultipleElementsProb_c
end interface

interface
    subroutine calculate_combination_with_largest_correlation_c(elements, i1max, i2max) bind(c)
        import :: multipleElements
        type(multipleElements) :: elements
        integer :: i1max
        integer :: i2max
    end subroutine calculate_combination_with_largest_correlation_c
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
    subroutine upscaleLength ( dpCrossSection, sectionLength, dpSection)
        type(designPoint),   intent (in), target  :: dpCrossSection !< design point cross section
        real(kind=c_double), intent (in)          :: sectionLength  !< Section length
        type(designPoint),   intent (inout), target :: dpSection      !< design point section

        integer :: n
        type(betaAlphaCF) :: dpCrossSectionC, dpSectionC

        dpCrossSectionC%beta  = dpCrossSection%beta
        dpCrossSectionC%size  = size(dpCrossSection%alpha)
        dpCrossSectionC%alpha = c_loc(dpCrossSection%alpha)
        dpCrossSectionC%rho   = c_loc(dpCrossSection%rho)
        dpCrossSectionC%correlation_length = c_loc(dpCrossSection%correlation_length)

        dpSectionC%size  = size(dpSection%alpha)
        dpSectionC%alpha = c_loc(dpSection%alpha)

        n = upscaleLengthC ( dpCrossSectionC, sectionLength, dpSectionC )
        call warnHohenbichler(n)
        dpSection%beta = dpSectionC%beta
    end subroutine upscaleLength

    !> helper function to get the address and stride of an array
    subroutine fill_loc_stride(array, loc, stride)
    real(kind=wp), intent(in   ), target :: array(:)  !< input array
    type(c_ptr),   intent(  out)         :: loc       !< address of the array
    integer,       intent(  out)         :: stride    !< stride in terms of doubles

    type(c_ptr)              :: loc2
    integer(kind=c_intptr_t) :: lc1, lc2

    loc = c_loc(array(1))
    if (is_contiguous(array)) then
        stride = 1
    else
        loc2 = c_loc(array(2))
        lc1 = transfer(loc, lc1)
        lc2 = transfer(loc2, lc2)
        stride = (lc2 - lc1) / sizeof(array(1))
    end if
    end subroutine fill_loc_stride

!>
!! Subroutine for upscaling random variables to the largest block duration
    subroutine upscaleToLargestBlock( dpSmallBlock, largestBlockDuration, dpLargestBlock )
    type(designPoint), intent(in)       :: dpSmallBlock             !< design point input
    real(kind=wp), intent (in)          :: largestBlockDuration     !< Target block duration
    type(designPoint), intent(inout)    :: dpLargestBlock           !< resulting design point

    type(betaAlphaCF) :: dpSmall, dpLargest

    dpSmall%beta       = dpSmallBlock%beta
    dpSmall%size       = size(dpSmallBlock%alpha)
    call fill_loc_stride(dpSmallBlock%alpha, dpSmall%alpha, dpSmall%stride_alpha)
    dpSmall%rho        = c_loc(dpSmallBlock%rho)
    call fill_loc_stride(dpSmallBlock%Duration, dpSmall%duration, dpSmall%stride_duration)

    dpLargest%size     = size(dpLargestBlock%alpha)
    call fill_loc_stride(dpLargestBlock%alpha, dpLargest%alpha, dpLargest%stride_alpha)
    call fill_loc_stride(dpLargestBlock%duration, dpLargest%duration, dpLargest%stride_duration)

    call upscaleToLargestBlockC(dpSmall, largestBlockDuration, dpLargest )

    dpLargestBlock%beta = dpLargest%beta

    end subroutine upscaleToLargestBlock

!> Subroutine for combining two elements with partial correlation
    subroutine combineTwoElementsPartialCorrelation( elm1, elm2, elmC, combAndOr)
!
!   INPUT/OUTPUT VARIABLES
!
    type(designPoint),    intent(in)     :: elm1       !< design point of element 1
    type(designPoint),    intent(in)     :: elm2       !< design point of element 2
    type(designPoint),    intent(inout)  :: elmC       !< design point of resulting element
    integer,              intent(in)     :: combAndOr  !< Combination type, And or Or

    integer :: n
    type(betaAlphaCF) :: dp1, dp2, dpC

    dp1%beta = elm1%beta
    dp1%size = size(elm1%alpha)
    dp1%alpha = c_loc(elm1%alpha)
    dp1%rho = c_loc( elm1%rho)
    dp2%beta = elm2%beta
    dp2%size = size(elm2%alpha)
    dp2%alpha = c_loc(elm2%alpha)
    dpC%size = size(elmC%alpha)
    dpC%alpha = c_loc(elmC%alpha)
    n = combineTwoElementsPartialCorrelationC2(dp1, dp2, dpC, combAndOr)
    elmC%beta = dpC%beta
    call warnHohenbichler(n)
end subroutine combineTwoElementsPartialCorrelation

subroutine combineMultipleElements( betaElement, alphaElement, rho, beta, alpha)
    real(kind=c_double),  intent(in)  :: betaElement(:)
    real(kind=c_double),  intent(in), target  :: alphaElement(:,:)
    real(kind=c_double),  intent(in), target  :: rho(:)
    real(kind=c_double),  intent(out) :: beta
    real(kind=c_double),  intent(out), target :: alpha(:)

    integer :: nrElms, nrStoch, n, i
    type(betaAlphaCF) :: dpOut
    type(multipleElements) :: elements
    type(betaAlphaCF), allocatable, target :: dpIn(:)

    nrElms =  size(betaElement)
    nrStoch = size(rho)
    allocate(dpIn(nrElms))
    
    do i = 1, nrElms
        dpIn(i)%beta = betaElement(i)
        dpIn(i)%alpha = c_loc(alphaElement(i,1))
        dpIn(i)%size = nrStoch
        dpIn(i)%stride_alpha = nrElms
    end do
    dpIn(1)%rho = c_loc(rho)
    elements%designPoints = c_loc(dpIn)
    elements%nElements = nrElms

    call fill_loc_stride(alpha, dpOut%alpha, dpOut%stride_alpha)
    dpOut%size = nrStoch
    n = combineMultipleElements_c(elements, dpOut)
    beta = dpOut%beta

    call warnHohenbichler(n)

end subroutine combineMultipleElements

!> This subroutine calculates the reliability index (beta) and alpha values combining over elements
subroutine combineMultipleElementsProb( betaElement, alphaElement, percentages, dpOutF )
    real(kind=c_double),  intent(in)         :: betaElement(:)       !< Reliability index per element
    real(kind=c_double),  intent(in), target :: alphaElement(:,:)    !< Alpha vector per element
    real(kind=c_double),  intent(in)         :: percentages(:)       !< Array of percentages
    type(designPoint),    intent(out)        :: dpOutF               !< design point after combining over elements

    integer :: nrElms, n, nrStoch, i
    type(betaAlphaCF) :: dpOut
    type(multipleElements) :: elements
    type(betaAlphaCF), allocatable, target :: dpIn(:)

    nrElms = size(betaElement)
    elements%nElements =nrElms
    nrStoch = size(dpOutF%alpha)
    allocate(dpIn(elements%nElements))
    do i = 1, elements%nElements
        dpIn(i)%beta = betaElement(i)
        dpIn(i)%alpha = c_loc(alphaElement(i,1))
        dpIn(i)%size = nrStoch
        dpIn(i)%stride_alpha = nrElms
    end do
    elements%designPoints = c_loc(dpIn)

    dpOut%size = size(dpOutF%alpha)
    call fill_loc_stride(dpOutF%alpha, dpOut%alpha, dpOut%stride_alpha)

    n = combineMultipleElementsProb_c( elements, percentages, dpOut)
    dpOutF%beta = dpOut%beta

    call warnHohenbichler(n)

end subroutine combineMultipleElementsProb

subroutine calculateCombinationWithLargestCorrelation( rhoP, alpha, i1max, i2max)
    use feedback, only : fatalError
    real(kind=wp),  intent(in)          :: rhoP(:)     !< Autocorrelation the random variables between elements
    real(kind=wp),  intent(in), target  :: alpha(:,:)  !< Alpha vector per element
    integer,        intent(out)         :: i1max       !< Index of first element with the largest correlation
    integer,        intent(out)         :: i2max       !< Index of second element with the largest correlation

    type(betaAlphaCF), allocatable, target :: dpIn(:)
    type(multipleElements)                 :: elements
    integer                                :: i
    integer                                :: nStochasts !< Number of random variables
    integer                                :: nElements  !< Number of elements to be combined

    nStochasts = size(rhoP)
    nElements = size(alpha,1)
    !
    ! Two elements can't be computed if there is only one element
    !
    if (nElements < 2) then
        call fatalError( "The subroutine calculateCombinationWithLargestCorrelation is called with only one element" )
        return
    end if

    elements%nElements =nElements
    allocate(dpIn(elements%nElements))
    do i = 1, elements%nElements
        dpIn(i)%beta = 0.0_wp
        dpIn(i)%alpha = c_loc(alpha(i,1))
        dpIn(i)%size = nStochasts
        dpIn(i)%stride_alpha = nElements
    end do
    elements%designPoints = c_loc(dpIn)

    call fill_loc_stride(rhoP, dpIn(1)%rho, i)

    call calculate_combination_with_largest_correlation_c(elements, i1max, i2max)
    i1max = i1max + 1 ! zero based => one based
    i2max = i2max + 1 ! idem
end subroutine calculateCombinationWithLargestCorrelation

end module interface_combin
