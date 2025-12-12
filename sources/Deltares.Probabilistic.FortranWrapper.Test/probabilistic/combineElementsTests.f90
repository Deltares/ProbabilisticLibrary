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
    use vectorUtilities
    use ftnunit
    use interface_combin
    use interface_convert
    use interface_distributions
    implicit none

    private
    public :: allCombineElementsTests, upscaleLengthWrapper

contains
!
!> This subroutine contains tests of subroutines in module combineElements within probabilistic
subroutine allCombineElementsTests
    integer, parameter :: level = 1

   call testWithLevel( testcombineTwoElementsFullCorrelation1     , &
       "combineElementsTests: 23 OR -combination of two FULLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsFullCorrelation2     , &
       "combineElementsTests: 24 AND-combination of two FULLY CORRELATED elements", level)

   call testWithLevel( testcombinaTwoElementsPartialCorrelationPiping, &
       "combineElementsTests: test to reproduce failing piping calculation ", level)

   call testWithLevel( testFromRealCasesA, "test from RealCases A", level)
   call testWithLevel( testFromRealCasesB, "test from RealCases B", level)
   call testWithLevel( testFromRealCasesC, "test from RealCases C", level)
!
end subroutine allCombineElementsTests

!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 fully correlated elements \n
!! Element 1 with beta= 3.0, and design point alpha= (0.6, 0.8) \n
!! Element 2 with beta= 2.5, and design point alpha= (0.6, 0.8) \n
!! RhoP = (1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testcombineTwoElementsFullCorrelation1
    integer, parameter                     :: nStochast = 2         ! number of stochastic variables
    integer                                :: iStochast             ! Index for referring to a stochastic variable
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: expectedBeta          ! Analytically computed beta that is expected for the combined elements
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: expectedAlphaC        ! The alpha() that is expected for the combined elements
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    real (kind = wp)                       :: margin = 1.0d-6       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.0d0
    beta2 =    2.5d0
    alpha1= (/ 0.6d0, 0.8d0 /)
    alpha2=    alpha1
    rhoP  = (/ 1.0d0, 1.0d0 /)

    expectedBeta  = min( beta1, beta2)    ! Because of an OR-port in the combination
    expectedAlphaC= alpha1

    call combineTwoElementsPartialCorrelationWrapper( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsFullCorrelation1
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the AND-combination of 2 fully correlated elements \n
!! Element 1 with beta= 3.0, and design point alpha= (0.6, 0.8) \n
!! Element 2 with beta= 2.5, and design point alpha= (0.6, 0.8) \n
!! RhoP = (1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testcombineTwoElementsFullCorrelation2
    integer, parameter                     :: nStochast = 2         ! number of stochastic variables
    integer                                :: iStochast             ! Index for referring to a stochastic variable
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: expectedBeta          ! Analytically computed beta that is expected for the combined elements
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: expectedAlphaC        ! The alpha() that is expected for the combined elements
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    real (kind = wp)                       :: margin = 1.0d-6       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.0d0
    beta2 =    2.5d0
    alpha1= (/ 0.6d0, 0.8d0 /)
    alpha2=    alpha1
    rhoP  = (/ 1.0d0, 1.0d0 /)

    expectedBeta  = max( beta1, beta2)    ! Because of an AND-port in the combination
    expectedAlphaC= alpha1

    call combineTwoElementsPartialCorrelationWrapper( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAND )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsFullCorrelation2
!

!> test to reproduce behaviour of issue: HRING-1329
!! beta < 0 and rho=1 for most important alpha value
!! condensed alpha's from a crashing piping calculation
subroutine testcombinaTwoElementsPartialCorrelationPiping
    real(kind=wp) :: beta1, beta2, betaC
    integer, parameter :: nStochasts = 3
    real(kind=wp) :: alpha1(nStochasts), alpha2(nStochasts), rhoP(nStochasts), alphaC(nStochasts)
    real(kind=wp), parameter :: margin = 1d-6

    beta1 = -0.672801079367189_wp
    beta2 = -1.66664947542178_wp

    alpha1 = [0.955916666381498_wp, 0.269308913244088_wp, 0.117030065288253_wp]
    alpha2 = [0.941548934494231_wp, 0.336876242987242_wp, 2.938671066518333d-005]
    rhoP =   [1.0_wp, 1.0_wp, 0.0_wp]

    call combineTwoElementsPartialCorrelationWrapper( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable(betaC, -1.66664950156622_wp, margin, "diff in betaC")
    call assert_comparable(alphaC, [ 0.941544051697653_wp, 0.336889889798984_wp, 2.939378451871372D-005], margin, "diff in alphaC")
end subroutine testcombinaTwoElementsPartialCorrelationPiping


subroutine testFromRealCasesA
    real(kind=wp)      :: beta1, beta2, betaC
    integer, parameter :: nStoch = 12
    real(kind=wp)      :: alpha1(nStoch), alpha2(nStoch), alphaC(nStoch), rhoP(nStoch)
    real(kind=wp)      :: expectedAlpha(nStoch), expectedBeta

    rhoP = 1.0_wp
    beta1 = 5.399971746_wp
    beta2 = -7.941444487_wp

    beta1 = 5.399971745953011_wp
    !beta2 = -7.941444487415977_wp

    expectedBeta = -7.956038125_wp
    alpha2 = 0.0_wp
    alpha2(6) = -1.0_wp
    alpha1( 1) =  0.06022086662_wp
    alpha1( 2) =  0.02505312816_wp
    alpha1( 3) =  0.495556994_wp
    alpha1( 4) =  0.2495783788_wp
    alpha1( 5) = -0.1659764886_wp
    alpha1( 6) =  0.0_wp
    alpha1( 7) = -0.01384083461_wp
    alpha1( 8) = -0.6138122022_wp
    alpha1( 9) = -0.2227598415_wp
    alpha1(10) = -0.04023371431_wp
    alpha1(11) = -0.4817341077_wp
    alpha1(12) = -0.008127376587_wp

    expectedAlpha( 1) =-0.2001004044_wp
    expectedAlpha( 2) =-0.3793992085_wp
    expectedAlpha( 3) =-0.2001004044_wp
    expectedAlpha( 4) =-0.3793992085_wp
    expectedAlpha( 5) =-0.2001004044_wp
    expectedAlpha( 6) =-0.3793992085_wp
    expectedAlpha( 7) =-0.3793992085_wp
    expectedAlpha( 8) =-0.2001004044_wp
    expectedAlpha( 9) =-0.2001004044_wp
    expectedAlpha(10) =-0.2001004044_wp
    expectedAlpha(11) =-0.3793992085_wp
    expectedAlpha(12) =-0.2001004044_wp

    call combineTwoElementsPartialCorrelationWrapper( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )
    call assert_comparable(betaC, expectedBeta, 1d-8, "diff in beta")
    call assert_comparable(alphaC, expectedAlpha, 1d-8, "diff in alpha")
end subroutine testFromRealCasesA

subroutine testFromRealCasesB
    real(kind=wp)      :: beta1, beta2, betaC
    integer, parameter :: nStoch = 12
    real(kind=wp)      :: alpha1(nStoch), alpha2(nStoch), alphaC(nStoch), rhoP(nStoch)
    real(kind=wp)      :: expectedAlpha(nStoch), expectedBeta

    rhoP = 1.0_wp
    beta1 =  5.39982938128391_wp
    beta2 =  -7.94144448741598_wp
    expectedBeta = -7.94144448741598_wp
    alpha2 = 0.0_wp
    alpha2(6) = -1.0_wp
    alpha1( 1) =  6.069041097195603D-2
    alpha1( 2) =  2.511615065692600D-2
    alpha1( 3) =  0.496800218126342_wp
    alpha1( 4) =  0.250204506356159_wp
    alpha1( 5) = -0.166420575897874_wp
    alpha1( 6) =  0.0_wp
    alpha1( 7) = -1.387784052625228D-2
    alpha1( 8) = -0.615454520428767_wp
    alpha1( 9) = -0.223262061487340_wp
    alpha1(10) = -3.997037183391337D-2
    alpha1(11) = -0.477587328687524_wp
    alpha1(12) = -8.197362860397718D-3

    expectedAlpha = 0.0_wp
    expectedAlpha( 1) = -0.359524390928393_wp
    expectedAlpha( 2) = -0.359524390928393_wp
    expectedAlpha( 3) =  0.401235113663744_wp
    expectedAlpha( 4) = -0.359524390928393_wp
    expectedAlpha( 5) = -0.359524390928393_wp
    expectedAlpha( 8) =  0.401235113663744_wp
    expectedAlpha(12) =  0.401235113663744_wp

    call combineTwoElementsPartialCorrelationWrapper( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )
    call assert_comparable(betaC, expectedBeta, 1d-8, "diff in beta")
    call assert_comparable(alphaC, expectedAlpha, 1d-8, "diff in alpha")
end subroutine testFromRealCasesB

subroutine testFromRealCasesC
    real(kind=wp)      :: beta1, beta2, beta3A, beta3B, beta1finalA, beta2finalA, beta1finalB, beta2finalB
    integer, parameter :: nStoch = 2
    real(kind=wp)      :: alpha1(nStoch), alpha2(nStoch), alpha3A(nStoch), rhoP(nStoch), alpha3B(nstoch)
    integer            :: i

    rhoP = 1.0_wp
    alpha2(1) = 0.0_wp
    alpha2(2) = -1.0_wp
    alpha1(1) =  1.0_wp
    alpha1(2) =  0.0_wp

    beta2finalA = -7.941444487_wp
    beta1finalA = 5.399971745953011_wp

    beta1finalB =  5.39982938128391_wp
    beta2finalB =  -7.94144448741598_wp

    do i = 1, 50
        beta1 =  beta1finalA * real(i, wp) / 50.0_wp
        beta2 =  beta2finalA * real(i, wp) / 50.0_wp
        call combineTwoElementsPartialCorrelationWrapper( beta1, alpha1, beta2, alpha2, rhoP, beta3A, alpha3A, combOR )

        beta1 =  beta1finalB * real(i, wp) / 50.0_wp
        beta2 =  beta2finalB * real(i, wp) / 50.0_wp
        call combineTwoElementsPartialCorrelationWrapper( beta1, alpha1, beta2, alpha2, rhoP, beta3B, alpha3B, combOR )
        write(*,*) i, real(alpha3A), real(alpha3B)
    end do
end subroutine testFromRealCasesC

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

!> Subroutine for combining two elements with partial correlation
    subroutine combineTwoElementsPartialCorrelationWrapper( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAndOr)
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp),  intent(in)              :: beta1             !< Reliability index of element 1
    real(kind=wp),  intent(in), target      :: alpha1(:)         !< Alpha values of element 1
    real(kind=wp),  intent(in)              :: beta2             !< Reliability index of element 2
    real(kind=wp),  intent(in), target      :: alpha2(:)         !< Alpha values of element 2
    real(kind=wp),  intent(in), target      :: rhoP(:)           !< Autocorrelation of the stochastic variables between element 1 and element 2
    real(kind=wp),  intent(out)             :: betaC             !< Reliability index of the combined elements
    real(kind=wp),  intent(out), target     :: alphaC(:)         !< Alpha values of the combined elements
    integer,        intent(in)              :: combAndOr         !< Combination type, And or Or

    type(designPoint) :: dp1, dp2, dpC
    dp1%beta = beta1
    dp1%alpha => alpha1
    dp1%rho =>rhoP
    dp2%beta = beta2
    dp2%alpha => alpha2
    dpC%alpha => alphaC
    call combineTwoElementsPartialCorrelation( dp1, dp2, dpC, combAndOr)
    betaC = dpC%beta
    end subroutine combineTwoElementsPartialCorrelationWrapper

end module combineElementsTests
