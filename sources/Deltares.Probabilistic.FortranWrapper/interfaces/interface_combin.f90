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
use, intrinsic :: iso_c_binding, only: c_double, c_ptr, c_loc, c_null_ptr, c_intptr_t
use precision, only : wp
implicit none
integer, parameter :: combAND =  0
integer, parameter :: combOR  =  1

private :: combineMultipleElements_c, warnHohenbichler, upscaleLengthC, &
    combineTwoElementsPartialCorrelationC1, combineTwoElementsPartialCorrelationC2, &
    combineMultipleElementsProb_c, c_double,  c_ptr, c_loc, c_null_ptr, c_intptr_t, wp

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

type DesignPoint
    real(kind=wp) :: beta
    real(kind=wp), pointer :: alpha(:)
    real(kind=wp), pointer :: rho(:)
    real(kind=wp), pointer :: duration(:)
    real(kind=wp), pointer :: correlation_length(:)
end type DesignPoint

type combinerSettings
    integer combAndOr
    integer combinerType
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
    integer function combineTwoElementsPartialCorrelationC1( dp1, dp2, rhoP, dpC, combAndOr, alphaI, alphaII ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        import betaAlphaCF
        type(betaAlphaCF),    intent(in)           :: dp1         !< design point of element 1
        type(betaAlphaCF),    intent(in)           :: dp2         !< design point of element 2
        real(kind=c_double),  intent(in)           :: rhoP(*)     !< Autocorrelation of the stochastic variables between element 1 and element 2
        type(betaAlphaCF),    intent(inout)        :: dpC         !< design point of the combined elements
        integer, value,       intent(in)           :: combAndOr   !< Combination type, And or Or
        real(kind=c_double),  intent(out)          :: alphaI(*)   !< AlphaI values of the combined elements in case of a spatial correlation
        real(kind=c_double),  intent(out)          :: alphaII(*)  !< AlphaII values of the combined elements in case of a spatial correlation
    end function combineTwoElementsPartialCorrelationC1
end interface

interface
!> Subroutine for combining two elements with partial correlation
    integer function combineTwoElementsPartialCorrelationC2( dp1, dp2, rhoP, dpC, combAndOr ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        import betaAlphaCF
        type(betaAlphaCF),    intent(in)           :: dp1         !< design point of element 1
        type(betaAlphaCF),    intent(in)           :: dp2         !< design point of element 2
        real(kind=c_double),  intent(in)           :: rhoP(*)     !< Autocorrelation of the stochastic variables between element 1 and element 2
        type(betaAlphaCF),    intent(inout)        :: dpC         !< design point of the combined elements
        integer, value,       intent(in)           :: combAndOr   !< Combination type, And or Or
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

    subroutine fill_loc_stride(array, loc, stride)
    real(kind=wp), intent(in   ), target :: array(:)
    type(c_ptr),   intent(  out)         :: loc
    integer,       intent(  out)         :: stride
    type(c_ptr)     :: loc2
    integer(kind=c_intptr_t) :: lc1, lc2

    loc = c_loc(array(1))
    loc2 = c_loc(array(2))
    lc1 = transfer(loc, lc1)
    lc2 = transfer(loc2, lc2)
    stride = (lc2 - lc1) / sizeof(array(1))
    end subroutine fill_loc_stride

!>
!! Subroutine for upscaling random variables to the largest block duration
    subroutine upscaleToLargestBlock( betaSmallBlock, alphaSmallBlock, rhoTSmallBlock,  &
                                      blockDurations, largestBlockDuration,             &
                                      betaLargestBlock, alphaLargestBlock, durationsLargestBlock)
    real(kind=wp), intent (in)          :: betaSmallBlock           !< Reliability index input
    real(kind=wp), intent (in), target  :: alphaSmallBlock(:)       !< Alpha vector input
    real(kind=wp), intent (in), target  :: rhoTSmallBlock(:)        !< Correlations
    real(kind=wp), intent (in), target  :: blockDurations(:)        !< Block durations vector input
    real(kind=wp), intent (in)          :: largestBlockDuration     !< Target block duration
    real(kind=wp), intent (out)         :: betaLargestBlock         !< Reliability index result
    real(kind=wp), intent (out), target :: alphaLargestBlock(:)     !< Alpha vector result
    real(kind=wp), intent (out), target :: durationsLargestBlock(:) !< Block durations vector result

    type(betaAlphaCF) :: dpSmall, dpLargest

    dpSmall%beta       = betaSmallBlock
    dpSmall%size       = size(alphaSmallBlock)
    call fill_loc_stride(alphaSmallBlock, dpSmall%alpha, dpSmall%stride_alpha)
    dpSmall%rho        = c_loc(rhoTSmallBlock)
    call fill_loc_stride(blockDurations, dpSmall%duration, dpSmall%stride_duration)

    dpLargest%size     = size(alphaLargestBlock)
    call fill_loc_stride(alphaLargestBlock, dpLargest%alpha, dpLargest%stride_alpha)
    call fill_loc_stride(durationsLargestBlock, dpLargest%duration, dpLargest%stride_duration)

    call upscaleToLargestBlockC(dpSmall, largestBlockDuration, dpLargest )

    betaLargestBlock = dpLargest%beta

    end subroutine upscaleToLargestBlock

!> Subroutine for combining two elements with partial correlation
    subroutine combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAndOr, alphaI, alphaII )
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=c_double),  intent(in)              :: beta1             !< Reliability index of element 1
    real(kind=c_double),  intent(in), target      :: alpha1(:)         !< Alpha values of element 1
    real(kind=c_double),  intent(in)              :: beta2             !< Reliability index of element 2
    real(kind=c_double),  intent(in), target      :: alpha2(:)         !< Alpha values of element 2
    real(kind=c_double),  intent(in)              :: rhoP(:)           !< Autocorrelation of the stochastic variables between element 1 and element 2
    real(kind=c_double),  intent(out)             :: betaC             !< Reliability index of the combined elements
    real(kind=c_double),  intent(out), target     :: alphaC(:)         !< Alpha values of the combined elements
    integer,              intent(in)              :: combAndOr         !< Combination type, And or Or
    real(kind=c_double), optional, intent(out)    :: alphaI(:)         !< AlphaI values of the combined elements in case of a spatial correlation
    real(kind=c_double), optional, intent(out)    :: alphaII(:)        !< AlphaII values of the combined elements in case of a spatial correlation

    integer :: n
    type(betaAlphaCF) :: dp1, dp2, dpC

    dp1%beta = beta1
    dp1%size = size(alpha1)
    dp1%alpha = c_loc(alpha1)
    dp2%beta = beta2
    dp2%size = size(alpha2)
    dp2%alpha = c_loc(alpha2)
    dpC%size = size(alphaC)
    dpC%alpha = c_loc(alphaC)
    if (present(alphaI) .and. present(alphaII)) then
        n = combineTwoElementsPartialCorrelationC1(dp1, dp2, rhoP, dpC, combAndOr, alphaI, alphaII)
    else
        n = combineTwoElementsPartialCorrelationC2(dp1, dp2, rhoP, dpC, combAndOr)
    end if
    betaC = dpC%beta
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
