! Copyright (C) Stichting Deltares. All rights reserved.
!
! This file is part of Streams.
!
! The Streams free software: you can redistribute it and/or modify
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
!> This subroutine contains tests of subroutines in module combineElements within probabilistic\n
!! \n
!! First  part: \n
!! Several tests of combineTwoElementsPartialCorrelation, where two non-identical uncorrelated elements are combined \n
!! \n
!! Second part: \n
!! Several tests of combineTwoElementsPartialCorrelation, where two non-identical partially correlated elements are combined \n
!! \n
!! Third part: \n
!! Several tests of combineTwoElementsPartialCorrelation, where two fully correlated elements are combined \n
!!
subroutine allCombineElementsTests
    integer, parameter :: level = 1
    call testWithLevel( testcombineTwoElementsNoCorrelation1       , &
        "combineElementsTests:  1 OR -combination of two non-identical UNCORRELATED elements", level)
    call testWithLevel( testcombineTwoElementsNoCorrelation2       , &
        "combineElementsTests:  2 OR -combination of two non-identical UNCORRELATED elements", level)
    call testWithLevel( testcombineTwoElementsNoCorrelation3       , &
        "combineElementsTests:  3 OR -combination of two non-identical UNCORRELATED elements", level)
    call testWithLevel( testcombineTwoElementsNoCorrelation4       , &
        "combineElementsTests:  4 AND-combination of two non-identical UNCORRELATED elements", level)
    call testWithLevel( testcombineTwoElementsNoCorrelation5       , &
        "combineElementsTests:  5 AND-combination of two non-identical UNCORRELATED elements", level)
    call testWithLevel( testcombineTwoElementsNoCorrelation6       , &
        "combineElementsTests:  6 AND-combination of two non-identical UNCORRELATED elements", level)

   call testWithLevel( testcombineTwoElementsPartialCorrelation1  ,     &
       "combineElementsTests:  7 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation2  ,     &
       "combineElementsTests:  8 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation3  ,     &
       "combineElementsTests:  9 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation4  ,     &
       "combineElementsTests: 10 OR -combination of two non-identical UNCORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation5  ,     &
       "combineElementsTests: 11 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation6  ,     &
       "combineElementsTests: 12 OR -combination of two non-identical UNCORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation7  ,     &
       "combineElementsTests: 13 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPCorWithNonContrAlphas ,   &
       "combineElementsTests: 13a OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation8  ,     &
       "combineElementsTests: 14 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation9  ,     &
       "combineElementsTests: 15 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation10 ,     &
       "combineElementsTests: 16 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation11 ,     &
       "combineElementsTests: 17 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation12 ,     &
       "combineElementsTests: 18 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation13 ,     &
       "combineElementsTests: 19 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation14 ,     &
       "combineElementsTests: 20 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation15 ,     &
       "combineElementsTests: 21 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsPartialCorrelation99 ,     &
       "combineElementsTests: 22 OR -combination of two non-identical PARTIALLY CORRELATED elements", level)
!
   call testWithLevel( testcombineTwoElementsFullCorrelation1     , &
       "combineElementsTests: 23 OR -combination of two FULLY CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsFullCorrelation2     , &
       "combineElementsTests: 24 AND-combination of two FULLY CORRELATED elements", level)
!
   call testWithLevel( testcombineTwoElementsNegativeCorrelation1 , &
       "combineElementsTests: 25 OR -combination of two NEGATIVE CORRELATED elements", level)
   call testWithLevel( testcombineTwoElementsNegativeCorrelation2 , &
       "combineElementsTests: 26 AND-combination of two NEGATIVE CORRELATED elements", level)
!
   call testWithLevel( testcombineThreeElementsPartialCorrelation1, &
       "combineElementsTests: 27 OR -combination of three non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineThreeElementsPartialCorrelation2, &
       "combineElementsTests: 28 OR -combination of three non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineThreeElementsPartialCorrelation3, &
       "combineElementsTests: 29 OR -combination of three non-identical PARTIALLY CORRELATED elements", level)
   call testWithLevel( testcombineThreeElementsPartialCorrelation4, &
       "combineElementsTests: 30 OR -combination of three non-identical PARTIALLY CORRELATED elements", level)
!
   call testWithLevel( testcombineMultipleElementsSpatialCorrelated1, &
       "combineElementsTests: Test 1 for the development of a spatial correlation model", level)
   call testWithLevel( testcombineMultipleElementsSpatialCorrelated2, &
       "combineElementsTests: Test 2 for the development of a spatial correlation model", level)
   call testWithLevel( testcombineMultipleElementsSpatialCorrelated3, &
       "combineElementsTests: Test 3 for the development of a spatial correlation model", level)
   call testWithLevel( testcombinaTwoElementsPartialCorrelationPiping, &
       "combineElementsTests: test to reproduce failing piping calculation ", level)

! NOTE. The tests below are a "redo" of a subset of the tests implemented above.
!       These tests make sense only when in the present method for the calculation of the alpha() a correction is
!       applied in combineTwoElementsPartialCorrelation(). This correction is as described in the memo:
!       > Hydra-Ring: Rekenmethode combinatie mechanism en en dijkvakken.
!       > Section 3: Onnauwkeurigheid alfa-bepaling.
!       > Authors: A. Vrouwenvelder/H.Steenbergen (TNO), June 2013
!
!       The tests below represent the Testcases 1-3 described in that memo in Section 3 with the correction ENABLED.
!
   call testWithLevel( testcombineTwoElementsPartialCorrelation9a , &
       "combineElementsTests: 15a OR -combination of two non-identical PARTIALLY CORRELATED elements", level )
   call testWithLevel( testcombineTwoElementsPartialCorrelation10a, &
       "combineElementsTests: 16a OR -combination of two non-identical PARTIALLY CORRELATED elements", level )
   call testWithLevel( testcombineTwoElementsPartialCorrelation11a, &
       "combineElementsTests: 17a OR -combination of two non-identical PARTIALLY CORRELATED elements", level )
   call testWithLevel( testcombineTwoElementsPartialCorrelation12a, &
       "combineElementsTests: 18a OR -combination of two non-identical PARTIALLY CORRELATED elements", level )
   call testWithLevel( testcombineTwoElementsPartialCorrelation13a, &
       "combineElementsTests: 19a OR -combination of two non-identical PARTIALLY CORRELATED elements", level )
   call testWithLevel( testcombineTwoElementsPartialCorrelation14a, &
       "combineElementsTests: 20a OR -combination of two non-identical PARTIALLY CORRELATED elements", level )
   call testWithLevel( testcombineTwoElementsPartialCorrelation99a, &
       "combineElementsTests: 99a OR -combination of two non-identical PARTIALLY CORRELATED elements", level )

   call testWithLevel( testcombineMultipleElementsProb1, &
       "combineMultipleElementsProb: Test 1 combination of one element", level)
   call testWithLevel( testcombineMultipleElementsProb2, &
       "combineMultipleElementsProb: Test 2 combination of two equal elements", level)
   call testWithLevel( testcombineMultipleElementsProb3, &
       "combineMultipleElementsProb: Test 3 combination of ten equal elements", level)
   call testWithLevel( testcombineMultipleElementsProb4, &
       "combineMultipleElementsProb: Test 4 combination of two different elements", level)
   call testWithLevel( testcombineMultipleElementsProb5, &
       "combineMultipleElementsProb: Test 5 combination of ten different elements", level)

   call testWithLevel( testFromRealCasesA, "test from RealCases A", level)
   call testWithLevel( testFromRealCasesB, "test from RealCases B", level)
   call testWithLevel( testFromRealCasesC, "test from RealCases C", level)
!
! ToDo Comments of HS, 26/27.09.2012. Not to remove for the time being (HvdB).
! todo Add more "and port" tests                    ! Combinations with AND operator, additionally to the present tests using OR-combinations)
! todo correlations of 0.99 should be 1.0           ! replace correlation of 0.99 to 1.0 to check robustness of the implementation
! todo add also tests with negative values of beta
!
! todo add test for multiple elements and compare them with design document ! e.g. 3 elements, 2 stochVar, with designpoints on a circle
!
end subroutine allCombineElementsTests
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, but uncorrelated elements \n
!! Element 1 with beta= 3.0, and design point alpha= (1, 0) \n
!! Element 2 with beta= 2.5, and design point alpha= (0, 1) \n
!! RhoP = (0, 0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testCombineTwoElementsNoCorrelation1
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
    real (kind = wp)                       :: p1                    ! probability of exceedence corresponding to beta1
    real (kind = wp)                       :: p2                    ! probability of exceedence corresponding to beta2
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure
    integer                                :: ierr                  ! error code

    beta1 =    3.0d0
    beta2 =    2.5d0
    alpha1= (/ 1.0d0, 0.0d0 /)
    alpha2= (/ 0.0d0, 1.0d0 /)
    rhoP  = (/ 0.0d0, 0.0d0 /)

!   Compute analytically the beta that is expected for the OR-port combination of the present two elements:
    call PQFromBeta( beta1, p1, betaC )              ! betaC is used for temporary storage
    call PQFromBeta( beta2, p2, betaC )              ! betaC is used for temporary storage
    call betaFromQ ( 1.0d0- p1* p2    , expectedBeta, ierr, errorText )
    call assert_equal(ierr, 0, errorText)

    expectedAlphaC = (/ 0.246342425266045d0, 0.969182251096718d0 /)         ! Is somewhere pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testCombineTwoElementsNoCorrelation1
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, but uncorrelated elements \n
!! Element 1 with beta= 2.5, and design point alpha= (0, 1) \n
!! Element 2 with beta= 3.0, and design point alpha= (1, 0) \n
!! RhoP = (0, 0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! \n
!! NB. The results of this test can be compared to the preceding test to verify that the order of the elements does not affect the outcome. \n
!!
subroutine testCombineTwoElementsNoCorrelation2
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
    real (kind = wp)                       :: p1                    ! probability of exceedence corresponding to beta1
    real (kind = wp)                       :: p2                    ! probability of exceedence corresponding to beta2
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure
    integer                                :: ierr                  ! error code

    beta1 =    2.5d0
    beta2 =    3.0d0
    alpha1= (/ 0.0d0, 1.0d0 /)
    alpha2= (/ 1.0d0, 0.0d0 /)
    rhoP  = (/ 0.0d0, 0.0d0 /)

!   Compute analytically the beta that is expected for the OR-port combination of the present two elements:
    call PQFromBeta( beta1, p1, betaC )              ! betaC is used for temporay storage
    call PQFromBeta( beta2, p2, betaC )              ! betaC is used for temporay storage
    call betaFromQ ( 1.0d0- p1* p2    , expectedBeta, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    expectedAlphaC = (/ 0.246342425266045d0, 0.969182251096718d0 /)         ! Is somewhere pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testCombineTwoElementsNoCorrelation2
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, but uncorrelated elements \n
!! Element 1 with beta= 2.5, and design point alpha= (1, 0) \n
!! Element 2 with beta= 3.0, and design point alpha= (0, 1) \n
!! RhoP = (0, 0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! \n
!! NB: The results of this test can be compared to the two preceding ones to verify that now the alpha(1) and alpha(2) \n
!! of the combination are in reversed order. The reason is that for the same alpha1() and alpha2() the beta1 and beta2 were reversed. \n
!! In this way it is verified that the "beta-order" of the stochastic variables has an effect. \n
!!
subroutine testCombineTwoElementsNoCorrelation3
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
    real (kind = wp)                       :: p1                    ! probability of exceedence corresponding to beta1
    real (kind = wp)                       :: p2                    ! probability of exceedence corresponding to beta2
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure
    integer                                :: ierr                  ! error code

    beta1 =    2.5d0
    beta2 =    3.0d0
    alpha1= (/ 1.0d0, 0.0d0 /)
    alpha2= (/ 0.0d0, 1.0d0 /)
    rhoP  = (/ 0.0d0, 0.0d0 /)

!   Compute analytically the beta that is expected for the OR-port combination of the present two elements:
    call PQFromBeta( beta1, p1, betaC )              ! betaC is used for temporay storage
    call PQFromBeta( beta2, p2, betaC )              ! betaC is used for temporay storage
    call betaFromQ ( 1.0d0- p1* p2    , expectedBeta, ierr, errorText )
    call assert_equal(ierr, 0, errorText)

    expectedAlphaC= (/ 0.969182251096718d0, 0.246342425266045d0 /)          ! pre-computed
!   NB. The expectedAlphaC(1) and expectedAlphaC(2) must now be in reversed order compared to
!       the preceding tests testCombineTwoElementsNoCorrelation1 and testCombineTwoElementsNoCorrelation2

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testCombineTwoElementsNoCorrelation3
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the AND-combination of 2 non-identical, but uncorrelated elements \n
!! Element 1 with beta= 2.5, and design point alpha= (1, 0) \n
!! Element 2 with beta= 3.0, and design point alpha= (0, 1) \n
!! RhoP = (0, 0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testCombineTwoElementsNoCorrelation4
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
    real (kind = wp)                       :: q1                    ! probability of non-exceedence corresponding to beta1
    real (kind = wp)                       :: q2                    ! probability of non-exceedence corresponding to beta2
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure
    integer                                :: ierr                  ! error code

    beta1 =    2.5d0
    beta2 =    3.0d0
    alpha1= (/ 1.0d0, 0.0d0 /)
    alpha2= (/ 0.0d0, 1.0d0 /)
    rhoP  = (/ 0.0d0, 0.0d0 /)

!   Compute analytically the beta that is expected for the AND-port combination of the present two elements:
    call PQFromBeta( beta1, betaC, q1)               ! betaC is used for temporay storage
    call PQFromBeta( beta2, betaC, q2)               ! betaC is used for temporay storage
    call betaFromQ ( q1   * q2   , expectedBeta, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    expectedAlphaC = (/ 0.651828661927589d0, 0.758366267373285d0 /)         ! Is somewhere pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAND )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testCombineTwoElementsNoCorrelation4
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the AND-combination of 2 non-identical, but uncorrelated elements \n
!! Element 1 with beta= -2.5, and design point alpha= (1, 0) \n
!! Element 2 with beta= -3.0, and design point alpha= (0, 1) \n
!! RhoP = (0, 0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testCombineTwoElementsNoCorrelation5
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
    real (kind = wp)                       :: q1                    ! probability of non-exceedence corresponding to beta1
    real (kind = wp)                       :: q2                    ! probability of non-exceedence corresponding to beta2
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure
    integer                                :: ierr                  ! error code

    beta1 =    -2.5d0
    beta2 =    -3.0d0
    alpha1= (/ 1.0d0, 0.0d0 /)
    alpha2= (/ 0.0d0, 1.0d0 /)
    rhoP  = (/ 0.0d0, 0.0d0 /)

!   Compute analytically the beta that is expected for the AND-port combination of the present two elements:
    call PQFromBeta( beta1, betaC, q1)               ! betaC is used for temporay storage
    call PQFromBeta( beta2, betaC, q2)               ! betaC is used for temporay storage
    call betaFromQ ( q1   * q2   , expectedBeta, ierr, errorText )
    call assert_equal( ierr, 0 , errorText)

    expectedAlphaC = (/ 0.970353143592546d0, 0.241688910290521d0 /)         ! Is somewhere pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAND )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testCombineTwoElementsNoCorrelation5
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the AND-combination of 2 non-identical, but uncorrelated elements \n
!! Element 1 with beta= 0.0, and design point alpha= (1, 0) \n
!! Element 2 with beta= 0.0, and design point alpha= (0, 1) \n
!! RhoP = (0, 0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testCombineTwoElementsNoCorrelation6
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
    real (kind = wp)                       :: q1                    ! probability of non-exceedence corresponding to beta1
    real (kind = wp)                       :: q2                    ! probability of non-exceedence corresponding to beta2
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure
    integer                                :: ierr                  ! error code

    beta1 =    0.0d0
    beta2 =    0.0d0
    alpha1= (/ 1.0d0, 0.0d0 /)
    alpha2= (/ 0.0d0, 1.0d0 /)
    rhoP  = (/ 0.0d0, 0.0d0 /)

!   Compute analytically the beta that is expected for the AND-port combination of the present two elements:
    call PQFromBeta( beta1, betaC, q1)               ! betaC is used for temporay storage
    call PQFromBeta( beta2, betaC, q2)               ! betaC is used for temporay storage
    call betaFromQ ( q1   * q2   , expectedBeta, ierr, errorText )
    call assert_equal ( ierr, 0, errorText)

    expectedAlphaC= sqrt(0.5d0)  !   The alpha()-components of the combination must be equal because of equal beta1 and beta2

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAND )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testCombineTwoElementsNoCorrelation6
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.0, and design point alpha= (0.6, 0.8) \n
!! Element 2 with beta= 2.5, and design point alpha= (0.8, 0.6) \n
!! RhoP = (0.5, 0.5) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testcombineTwoElementsPartialCorrelation1
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
    alpha2= (/ 0.8d0, 0.6d0 /)
    rhoP  = (/ 0.5d0, 0.5d0 /)

    expectedBeta   = 2.43858507200259d0                                     ! pre-computed
    expectedAlphaC = (/ 0.777037047950730d0, 0.629454864237314d0 /)         ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )
!
    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation1
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.0, and design point alpha= (0.6, 0.8) \n
!! Element 2 with beta= 2.5, and design point alpha= (0.8, 0.6) \n
!! RhoP = (1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! \n
!! NB. With RhoP=(1, 1) the robustness of the function is tested for full correlation of the stochastic variables. \n
!
subroutine testcombineTwoElementsPartialCorrelation2
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
    alpha2= (/ 0.8d0, 0.6d0 /)
    rhoP  = (/ 1.0d0, 1.0d0  /)

    expectedBeta  =    2.49715643746047d0
    expectedAlphaC= (/ 0.795187280968291d0, 0.606363907390814d0 /)

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation2
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 2.5, and design point alpha= (0.6, 0.8) \n
!! Element 2 with beta= 2.5, and design point alpha= (0.8, 0.6) \n
!! RhoP = (1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! \n
!! NB. Particularly, with this RhoP=(1, 1) the robustness of the function is tested for full correlation of the stochastic variables. \n
!!     Because of equal beta of the two elements, the alpha(1) and alpha(2) for the combination must have the same value . \n
!
subroutine testcombineTwoElementsPartialCorrelation3
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

    beta1 =    2.5d0
    beta2 =    2.5d0
    alpha1= (/ 0.6d0, 0.8d0 /)
    alpha2= (/ 0.8d0, 0.6d0 /)
    rhoP  = (/ 1.0d0, 1.0d0 /)

    expectedBeta  = 2.38813303477200d0
    expectedAlphaC= sqrt(0.5d0)  !   The alpha()-components of the combination must be equal because of equal beta1 and beta2

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )
!
    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation3
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and uncorrelated elements \n
!! Element 1 with beta= 2.5, and design point alpha= (-0.6, 0.8) \n
!! Element 2 with beta= 2.5, and design point alpha= ( 0.8, 0.6) \n
!! RhoP = (1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! \n
!! NB. Fully correlated stochastic variables, but uncorrelated elements. \n
!! NB. A negative value is assigned to alpha1(1) of the first element. \n
!!
subroutine testcombineTwoElementsPartialCorrelation4
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
    real (kind = wp)                       :: p1                    ! probability of exceedence corresponding to beta1
    real (kind = wp)                       :: p2                    ! probability of exceedence corresponding to beta2
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure
    integer                                :: ierr                  ! error code

    beta1 =     2.5d0
    beta2 =     2.5d0
    alpha1= (/ -0.6d0, 0.8d0 /)
    alpha2= (/  0.8d0, 0.6d0 /)
    rhoP  = (/  1.0d0, 1.0d0 /)

!   Because of these alpha() and RhoP() the two elements are uncorrelated. The combined beta can then be derived analytically:
    call PQFromBeta( beta1, p1, betaC )
    call PQFromBeta( beta2, p2, betaC )
    call betaFromQ ( 1.0d0- p1* p2    , expectedBeta, ierr, errorText )
    call assert_equal( ierr, 0, errorText )

    expectedAlphaC= (/ 0.149904324474973d0, 0.988700507486318d0 /)         ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do
!
end subroutine testcombineTwoElementsPartialCorrelation4
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and correlated elements \n
!! Element 1 with beta= 2.5, and design point alpha= (-0.6, 0.8) \n
!! Element 2 with beta= 2.5, and design point alpha= (-0.8, 0.6) \n
!! RhoP = (1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! \n
!! NB. Negative values are assigned to alpha1(1) of the first element and alpha2(1) of the second element. \n
!!     It is verified that the alphaC(1) of the combination is also negative. \n
!!
subroutine testcombineTwoElementsPartialCorrelation5
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

    beta1 =     2.5d0
    beta2 =     2.5d0
    alpha1= (/ -0.6d0, 0.8d0 /)
    alpha2= (/ -0.8d0, 0.6d0 /)
    rhoP  = (/  1.0d0, 1.0d0 /)

    expectedBeta  = 2.38813303477200d0                  ! pre-computed
    expectedAlphaC= sqrt(0.5d0) * (/ -1.0d0, 1.0d0 /)   ! theoretically expected, but not exactly (re)produced
    expectedAlphaC= (/ -0.713029134520995d0, 0.701134404607448d0 /)

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation5
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and uncorrelated elements \n
!! Element 1 with beta= 2.5, and design point alpha= (-0.5,  0.5, -0.5, 0.5) \n
!! Element 2 with beta= 2.5, and design point alpha= (-0.5, -0.5,  0.5, 0.5) \n
!! RhoP = (0.9, 0.9, 0.9, 0.9) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! \n
!! NB. Present number of stochastic variables is 4 \n
!! NB. Correlated stochastic variables, but uncorrelated elements. \n
!! NB. Positive and negative values in the alpha1() and alpha2(). \n
!!
subroutine testcombineTwoElementsPartialCorrelation6
    integer, parameter                     :: nStochast = 4         ! number of stochastic variables
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
    real (kind = wp)                       :: p1                    ! probability of exceedence corresponding to beta1
    real (kind = wp)                       :: p2                    ! probability of exceedence corresponding to beta2
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure
    integer                                :: ierr                  ! error code

    beta1 =     2.5d0
    beta2 =     2.5d0
    alpha1= (/ -0.5d0,  0.5d0, -0.5d0, 0.5d0 /)
    alpha2= (/ -0.5d0, -0.5d0,  0.5d0, 0.5d0 /)
    rhoP  = (/  0.9d0,  0.9d0,  0.9d0, 0.9d0 /)

!   For these alpha() and RhoP() the two elements are uncorrelated, and beta of the present OR-combination can be derived analytically:
    call PQFromBeta( beta1, p1, betaC )              ! betaC is used for temporay storage
    call PQFromBeta( beta2, p2, betaC )              ! betaC is used for temporay storage
    call betaFromQ ( 1.0d0- p1* p2    , expectedBeta, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    expectedAlphaC = (/ -0.689062334733955d0, -0.158849035402622d0, 0.157472623165194d0, 0.689320285321795d0 /)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )
!
    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation6
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 2.5, and design point alpha= (-1.0) \n
!! Element 2 with beta= 2.5, and design point alpha= ( 1.0) \n
!! RhoP = ( 0.9 ) \n
!! The resulting beta of the combination should be equal to its expected value \n
!! The alpha of the combination of the these two elements is not well defined and is not verified \n
!! \n
!! NB. Merely one stochastic variable in the elements \n
!! NB. Negative correlation of the two elements. \n
!!
subroutine testcombineTwoElementsPartialCorrelation7
    integer, parameter                     :: nStochast = 1         ! number of stochastic variables
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: expectedBeta          ! Analytically computed beta that is expected for the combined elements
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    real (kind = wp)                       :: margin = 1.0d-6       ! acceptable margin when comparing expected beta and alpha with their computed values

    beta1 =     2.5d0
    beta2 =     2.5d0
    alpha1= (/ -1.0d0/)
    alpha2= (/  1.0d0/)
    rhoP  = (/  0.9d0/)

    expectedBeta  = 2.24390055691005d0
!   The alpha of the combination is not well defined and will not be verified

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

end subroutine testcombineTwoElementsPartialCorrelation7
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 2.5, and design point alpha= ( 1.0) \n
!! Element 2 with beta= 2.5, and design point alpha= (-1.0) \n
!! RhoP = ( 0.9 ) \n
!! The resulting beta of the combination should be equal to its expected value \n
!! The alpha of the combination of the these two elements is not well defined and is not verified \n
!! \n
!! NB. Merely one stochastic variable in the elements \n
!! NB. Negative correlation of the two elements. \n
!!
subroutine testcombineTwoElementsPartialCorrelation8
    integer, parameter                     :: nStochast = 1         ! number of stochastic variables
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: expectedBeta          ! Analytically computed beta that is expected for the combined elements
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    real (kind = wp)                       :: margin = 1.0d-6       ! acceptable margin when comparing expected beta and alpha with their computed values

    beta1 =     2.5d0
    beta2 =     2.5d0
    alpha1= (/  1.0d0/)
    alpha2= (/ -1.0d0/)
    rhoP  = (/  0.9d0/)

    expectedBeta  = 2.24390055691005d0
!   The alpha of the combination is not well defined and will not be verified

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

end subroutine testcombineTwoElementsPartialCorrelation8
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= Normalised( 0.436, 0.9  ) \n
!! Element 2 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.436) \n
!! RhoP = ( 1.0, 0.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 3: Standard case with different alpha() values. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013. \n
!!     See also the associated test in testcombineTwoElementsPartialCorrelation10 \n
subroutine testcombineTwoElementsPartialCorrelation9
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

    beta1 =    3.5d0
    beta2 =    3.5d0
    alpha1= (/ 0.436D0, 0.900D0 /)
    alpha2= (/ 0.900D0, 0.436D0 /)
    alpha1 =   normalize( alpha1 )
    alpha2 =   normalize( alpha2 )
    rhoP  = (/ 1.0D0  , 0.0D0   /)

    expectedBeta  =    3.31256284734426d0                            ! pre-computed
    expectedAlphaC= (/ 0.764786211092966d0, 0.644284138654728d0 /)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation9
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= Normalised( 0.436, 0.9, 0.0  ) \n
!! Element 2 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.0, 0.436) \n
!! RhoP = ( 1.0, 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 3: Standard case with different alpha() values. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!!     See also the associated test in testcombineTwoElementsPartialCorrelation9 \n
subroutine testcombineTwoElementsPartialCorrelation10
    integer, parameter                     :: nStochast = 3         ! number of stochastic variables
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

    beta1 =    3.5d0
    beta2 =    3.5d0
    alpha1= (/ 0.436D0, 0.900D0, 0.000D0/)
    alpha2= (/ 0.900D0, 0.000D0, 0.436D0/)
    alpha1=    normalize( alpha1 )
    alpha2=    normalize( alpha2 )
    rhoP  =    1.0d0

    expectedBeta  =    3.31256284734426d0                                                 ! pre-computed
    expectedAlphaC= (/ 0.764786211092966d0, 0.580277735143150d0, 0.279963928782266d0 /)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation10
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8  ) \n
!! Element 2 with beta= 3.5, and design point alpha= ( 0.6, 0.8  ) \n
!! RhoP = ( 1.0, 0.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 1: Standardcase. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!!     See also the associated test in testcombineTwoElementsPartialCorrelation12 \n
!!
subroutine testcombineTwoElementsPartialCorrelation11
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

    beta1 =    3.5d0
    beta2 =    beta1
    alpha1= (/ 0.6D0, 0.8d0 /)
    alpha2=    alpha1
    rhoP  = (/ 1.0d0, 0.0d0 /)

    expectedBeta  =    3.3121491224946d0                            ! pre-computed
    expectedAlphaC= (/ 0.634952576684642d0, 0.772551115047758d0 /)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation11
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8, 0.0 ) \n
!! Element 2 with beta= 3.5, and design point alpha= ( 0.6, 0.0, 0.8 ) \n
!! RhoP = ( 1.0, 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 1: Standardcase. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!!     See also the associated test in testcombineTwoElementsPartialCorrelation11 \n
subroutine testcombineTwoElementsPartialCorrelation12
    integer, parameter                     :: nStochast = 3         ! number of stochastic variables
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

    beta1 =    3.5d0
    beta2 =    beta1
    alpha1= (/ 0.6d0, 0.8d0, 0.0d0 /)
    alpha2= (/ 0.6d0, 0.0d0, 0.8d0 /)
    rhoP  = (/ 1.0d0, 1.0d0, 1.0d0 /)

    expectedBeta     =    3.31214912249458d0                                                 ! pre-computed
    expectedAlphaC   = (/ 0.634952576684642d0, 0.546276132263498d0, 0.546276132263498d0 /)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation12
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8  ) \n
!! Element 2 with beta= 3.8, and design point alpha= ( 0.6, 0.8  ) \n
!! RhoP = ( 1.0, 0.0 ) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 2: Standardcase with different beta-values. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!!     See also the associated test in testcombineTwoElementsPartialCorrelation14 \n
subroutine testcombineTwoElementsPartialCorrelation13
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
    real (kind = wp)                       :: margin = 1.0d-9       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

!   For "old" combineTwoElementsPartialCorrelation, without correction of the alpha_equivalent
    beta1 =    3.5d0
    beta2 =    3.8d0
    alpha1= (/ 0.6d0, 0.8d0 /)
    alpha2=    alpha1
    rhoP  = (/ 1.0d0, 0.0d0 /)

    expectedBeta  =    3.42801564331459d0                            ! pre-computed
    expectedAlphaC= (/ 0.631154181145020d0, 0.775657398355201d0 /)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation13
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 3 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8, 0.0 ) \n
!! Element 2 with beta= 3.8, and design point alpha= ( 0.6, 0.0, 0.8 ) \n
!! RhoP = ( 1.0, 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 2: Standardcase with different beta-values. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!!     See also the associated test in testcombineTwoElementsPartialCorrelation13 \n
!!
subroutine testcombineTwoElementsPartialCorrelation14
    integer, parameter                     :: nStochast = 3         ! number of stochastic variables
    integer                                :: iStochast             ! Index for referring to a stochastic variable
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: expectedBeta          ! Analytically computed beta that is expected for the combined elements
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp)                       :: expectedAlphaC2EQ, alphaC2EQ, dAlphaC2EQ     !  To be described
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: expectedAlphaC        ! The alpha() that is expected for the combined elements
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    real (kind = wp)                       :: margin = 1.0d-9       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

!   For "old" combineTwoElementsPartialCorrelation, without correction of the alpha_equivalent
    beta1 =    3.5d0
    beta2 =    3.8d0
    alpha1= (/ 0.6d0, 0.8d0, 0.0d0 /)
    alpha2= (/ 0.6d0, 0.0d0, 0.8d0 /)
    rhoP  = (/ 1.0d0, 1.0d0, 1.0d0 /)

    expectedBeta     =    3.42801564331459d0                                                 ! pre-computed
    expectedAlphaC   = (/ 0.631154181145020d0,  0.735508613458735d0, 0.246315811817203d0 /)   ! pre-computed
!   expectedAlphaC2EQ= sqrt( expectedAlphaC(2)**2 + expectedAlphaC(3)**2 )
    expectedAlphaC2EQ= 0.727073922142432d0

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)
    alphaC2EQ= sqrt( alphaC(2)**2 + alphaC(3)**2 )
    dAlphaC2EQ= alphaC2EQ - expectedAlphaC2EQ

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation14
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= ( 0.8, 0.6 ) \n
!! Element 2 with beta= 9.5, and design point alpha= ( 0.8, 0.6 ) \n
!! RhoP = ( 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testcombineTwoElementsPartialCorrelation15
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
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.5d0
    beta2 =    9.5d0
    alpha1= (/ 0.8d0, 0.6d0 /)
    alpha2= (/ 0.8d0, 0.6d0 /)
    rhoP  = (/ 1.0d0, 0.0d0 /)

    expectedBeta  =    3.5_wp               ! pre-computed
    expectedAlphaC= (/ 0.8_wp, 0.6_wp /)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)
    call assert_comparable( betaC, expectedBeta, 1d-6, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), 1d-6, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation15

!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.0, and dimension 10 of design point alpha \n
!! Element 2 with beta= 3.5, and dimension 10 of design point alpha \n
!! RhoP = ( 1.0, ..., 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
subroutine testcombineTwoElementsPartialCorrelation99
    integer, parameter                     :: nStochast = 10        ! number of stochastic variables
    integer                                :: iStochast             ! Index for referring to a stochastic variable
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: expectedBetaC         ! beta that is expected for the combined elements  (with    alpha() correction in combineElements())
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: expectedAlphaC        ! The alpha() that is expected for the combined elements (with    alpha() correction in combineElements())
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    real (kind = wp)                       :: margin = 1.0d-9       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.0d0
    beta2 =    3.5d0
    alpha1= (/ 0.6d0, 0.8d0, 0.4d0, 0.0d0, 0.2d0, 0.7d0, 0.3d0, 0.8d0, 0.9d0, 1.0d0 /)
    alpha2= (/ 0.1d0, 0.0d0, 0.7d0, 0.9d0, 0.8d0, 0.1d0, 0.9d0, 0.5d0, 0.3d0, 0.0d0 /)
    alpha1 =   normalize( alpha1 )
    alpha2 =   normalize( alpha2 )
    rhoP  =    1.0d0

!   Expected beta and alpha():
    expectedBetaC    =    2.95370049889346d0                          ! pre-computed
    expectedAlphaC   = (/ 0.277865981946082d0, 0.361174823684065d0, 0.244040175818021d0, &
                          0.906352706365079d-1, 0.168592815519657d0, 0.323038934343359d0, &
                          0.222247930634836d0, 0.395033459584985d0, 0.427126699802553d0, &
                          0.451566604194081d0 /) ! PreComputed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

!   Check computed beta, and alpha()
    call assert_comparable( betaC, expectedBetaC  , margin, "An unexpected value is found for the beta of the combined elements" )
!
    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast)  , margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation99
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8  ) \n
!! Element 2 with beta= 3.8, and design point alpha= ( 0.6, 0.8  ) \n
!! RhoP = ( 1.0, 0.0 ) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 2: Standardcase with different beta-values. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!!     See also the associated test in testcombineTwoElementsPartialCorrelation13 \n
subroutine testcombineTwoElementsPCorWithNonContrAlphas
    integer, parameter                     :: nStochast = 5         ! number of stochastic variables
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
    real (kind = wp)                       :: margin = 1.0d-9       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure


    beta1 =    3.5d0
    beta2 =    3.8d0
    alpha1= (/ 0.0D0, 0.6d0, 0.0D0, 0.8d0, 0.0D0 /)
    alpha2=    alpha1
    rhoP  = (/ 0.0D0, 1.0d0, 0.5D0, 0.0d0, 1.0D0 /)

    expectedBeta  =    3.42801564331459d0                                                 ! pre-computed
    expectedAlphaC= (/ 0.0D0, 0.631154181145020d0, 0.0D0, 0.775657398355201d0, 0.0D0 /)   ! pre-computed and based on testcombineTwoElementsPartialCorrelation13

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPCorWithNonContrAlphas
!
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

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )

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

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAND )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsFullCorrelation2
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 fully negatively correlated elements \n
!! Element 1 with beta= 3.0, and design point alpha= ( 1.0, 0.0) \n
!! Element 2 with beta= 2.5, and design point alpha= (-1.0, 0.0) \n
!! RhoP = (1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testcombineTwoElementsNegativeCorrelation1
    integer, parameter                     :: nStochast = 2         ! number of stochastic variables
    integer                                :: iStochast             ! Index for referring to a stochastic variable
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: expectedBeta          ! Analytically computed beta that is expected for the combined elements
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp)                       :: p1                    ! Probability of non-exceedence associated to beta1
    real (kind = wp)                       :: q1                    ! Probability of exceedence associated to beta1
    real (kind = wp)                       :: p2                    ! Probability of non-exceedence associated to beta2
    real (kind = wp)                       :: q2                    ! Probability of exceedence associated to beta2
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: expectedAlphaC        ! The alpha() that is expected for the combined elements
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    real (kind = wp)                       :: margin = 1.0d-6       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure
    integer                                :: ierr                  ! error code

    beta1 =    3.0d0
    beta2 =    2.5d0
    alpha1= (/ 1.0d0, 0.0d0 /)
    alpha2=    -alpha1
    rhoP  = (/ 1.0d0, 1.0d0 /)

!   For an OR combination the resulting beta can analytically be computed
    call PQfromBeta( beta1, p1, q1)
    call PQfromBeta( beta2, p2, q2)
!   Probability of failure is then
    betaC =  q1 + q2             ! temporary use of betaC
!   Corresponding beta:
    call betaFromQ( betaC, expectedBeta, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    expectedAlphaC= alpha2

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsNegativeCorrelation1
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the AND-combination of 2 fully negatively correlated elements \n
!! Element 1 with beta= 3.0, and design point alpha= ( 1.0, 0.0) \n
!! Element 2 with beta= 2.5, and design point alpha= (-1.0, 0.0) \n
!! RhoP = (1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testcombineTwoElementsNegativeCorrelation2
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
    alpha1= (/ 1.0d0, 0.0d0 /)
    alpha2=    -alpha1
    rhoP  = (/ 1.0d0, 1.0d0 /)

!   For the present alpha and beta it can be analytically shown that no failure can occur
!   Theoretically the expected beta of the combination is then +infinity. Practically:
    expectedBeta  = 40.0_wp

    expectedAlphaC= [ sqrt(0.5_wp), sqrt(0.5_wp) ]

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combAND )

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsNegativeCorrelation2

!
!
!> Test of combineThreeElementsPartialCorrelation \n
!! Testing the OR-combination of 3 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= Normalised( 0.436, 0.9, 0.0  , 0.0) \n
!! Element 2 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.0, 0.436, 0.0) \n
!! Element 3 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.0, 0.0, 0.436) \n
!! RhoP = ( 1.0, 1.0, 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testcombineThreeElementsPartialCorrelation1
    integer, parameter                     :: nStochast = 4         ! number of stochastic variables
    integer                                :: iStochast             ! Index for referring to a stochastic variable
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: beta3                 ! beta for the third element
    real (kind = wp)                       :: expectedBeta          ! Analytically computed beta that is expected for the combined elements
    real (kind = wp)                       :: betaC12               ! beta for the combination of 1 and 2
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: alpha3                ! alpha() for the third element
    real (kind = wp), dimension(nStochast) :: alphaC12              ! The alpha() for the combined elements 1 and 2
    real (kind = wp), dimension(nStochast) :: expectedAlphaC        ! The alpha() that is expected for the combined elements
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    real (kind = wp)                       :: margin = 1.0d-6       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.5d0
    beta2 =    3.5d0
    beta3 =    3.5d0
    alpha1= (/ 0.436D0, 0.900D0, 0.000D0, 0.000D0 /)
    alpha2= (/ 0.900D0, 0.000D0, 0.436D0, 0.000D0 /)
    alpha3= (/ 0.900D0, 0.000D0, 0.000D0, 0.436D0 /)
    alpha1=    normalize( alpha1 )
    alpha2=    normalize( alpha2 )
    alpha3=    normalize( alpha3 )
    rhoP  =    1.0d0

    expectedBeta  =    3.21126288603446d0                                                 ! pre-computed
    expectedAlphaC= (/ 0.833189348302311d0, 0.471747264980611d0, 0.227197897520706d0, 0.177851463927219d0/)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC12, alphaC12, combOR)
    call combineTwoElementsPartialCorrelation( betaC12, alphaC12, beta3, alpha3, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineThreeElementsPartialCorrelation1

!
!
!> Test of combineThreeElementsPartialCorrelation \n
!! Testing the OR-combination of 3 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.6, 0.0, 0.0) \n
!! Element 2 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.6, 0.0) \n
!! Element 3 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.0, 0.6) \n
!! RhoP = ( 1.0, 1.0, 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testcombineThreeElementsPartialCorrelation2
    integer, parameter                     :: nStochast = 4         ! number of stochastic variables
    integer                                :: iStochast             ! Index for referring to a stochastic variable
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: beta3                 ! beta for the third element
    real (kind = wp)                       :: expectedBeta          ! Analytically computed beta that is expected for the combined elements
    real (kind = wp)                       :: betaC12               ! beta for the combination of 1 and 2
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: alpha3                ! alpha() for the third element
    real (kind = wp), dimension(nStochast) :: alphaC12              ! The alpha() for the combined elements 1 and 2
    real (kind = wp), dimension(nStochast) :: expectedAlphaC        ! The alpha() that is expected for the combined elements
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    real (kind = wp)                       :: margin = 1.0d-6       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.5d0
    beta2 =    3.5d0
    beta3 =    3.5d0
    alpha1= (/ 0.8D0, 0.6D0, 0.0D0, 0.0D0 /)
    alpha2= (/ 0.8D0, 0.0D0, 0.6D0, 0.0D0 /)
    alpha3= (/ 0.8D0, 0.0D0, 0.0D0, 0.6D0 /)
    alpha1=    normalize( alpha1 )
    alpha2=    normalize( alpha2 )
    alpha3=    normalize( alpha3 )
    rhoP  =    1.0d0

    expectedBeta  =    3.21478388977581d0                                                ! pre-computed
    expectedAlphaC= (/ 0.844702211809561d0,  0.330698381721459d0,  0.330698381721459d0, 0.260298547859284d0 /)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC12, alphaC12, combOR)
    call combineTwoElementsPartialCorrelation( betaC12, alphaC12, beta3, alpha3, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineThreeElementsPartialCorrelation2
!
!
!> Test of combineThreeElementsPartialCorrelation \n
!! Testing the OR-combination of 3 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.6, 0.0, 0.0) \n
!! Element 2 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.6, 0.0) \n
!! Element 3 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.0, 0.6) \n
!! RhoP = ( 1.0, 1.0, 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testcombineThreeElementsPartialCorrelation3
    integer, parameter                     :: nStochast = 3         ! number of stochastic variables
    integer                                :: iStochast             ! Index for referring to a stochastic variable
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: beta3                 ! beta for the third element
    real (kind = wp)                       :: expectedBeta          ! Analytically computed beta that is expected for the combined elements
    real (kind = wp)                       :: betaC12               ! beta for the combination of 1 and 2
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: alpha3                ! alpha() for the third element
    real (kind = wp), dimension(nStochast) :: alphaC12              ! The alpha() for the combined elements 1 and 2
    real (kind = wp), dimension(nStochast) :: expectedAlphaC        ! The alpha() that is expected for the combined elements
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure
    integer                                :: ierr                  ! error code
    real (kind = wp)                       :: p1, p2, p3, q1, q2, q3
!
    beta1 =    3.3d0
    beta2 =    3.3d0
    beta3 =    3.5d0
    alpha1= (/ 1.0D0, 0.0D0, 0.0D0 /)
    alpha2= (/ 0.0D0, 1.0D0, 0.0D0 /)
    alpha3= (/ 0.0D0, 0.0D0, 1.0D0 /)
    alpha1=    normalize( alpha1 )
    alpha2=    normalize( alpha2 )
    alpha3=    normalize( alpha3 )
    rhoP  =    1.0d0

! test
    call PQFromBeta( beta1, p1, q1 )
    call PQFromBeta( beta2, p2, q2 )
    call PQFromBeta( beta3, p3, q3 )
    call betaFromQ ( 1.0d0- p1* p2* p3, betaC, ierr, errorText )
    call assert_equal( ierr, 0, errorText)
!
    expectedBeta  =    3.03591902024806_wp                                                   ! pre-computed
    expectedAlphaC= (/ 0.730696091970909_wp, 0.572806816016527_wp, 0.371450632929127_wp /)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC12, alphaC12, combOR)
    call combineTwoElementsPartialCorrelation( betaC12, alphaC12, beta3, alpha3, rhoP, betaC, alphaC, combOR)

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta3, alpha3, rhoP, betaC12, alphaC12, combOR)
    call combineTwoElementsPartialCorrelation( betaC12, alphaC12, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, 1d-6, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), 1d-6, errorText)
    end do

end subroutine testcombineThreeElementsPartialCorrelation3

!
!
!> Test of combineThreeElementsPartialCorrelation \n
!! Testing the OR-combination of 3 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.6, 0.0, 0.0) \n
!! Element 2 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.6, 0.0) \n
!! Element 3 with beta= 3.5, and design point alpha= Normalised( 0.8, 0.0, 0.0, 0.6) \n
!! RhoP = ( 1.0, 1.0, 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!!
subroutine testcombineThreeElementsPartialCorrelation4
    integer, parameter                     :: nStochast = 3         ! number of stochastic variables
    integer                                :: iStochast             ! Index for referring to a stochastic variable
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: beta3                 ! beta for the third element
    real (kind = wp)                       :: expectedBeta          ! Analytically computed beta that is expected for the combined elements
    real (kind = wp)                       :: betaC12               ! beta for the combination of 1 and 2
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: alpha3                ! alpha() for the third element
    real (kind = wp), dimension(nStochast) :: alphaC12              ! The alpha() for the combined elements 1 and 2
    real (kind = wp), dimension(nStochast) :: expectedAlphaC        ! The alpha() that is expected for the combined elements
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.3d0
    beta2 =    3.3d0
    beta3 =    3.5d0
    alpha1= (/ 0.8D0, 0.5D0, 0.33D0 /)
    alpha2= (/ 0.6D0, 0.7D0, 0.39D0 /)
    alpha3= (/ 0.6D0, 0.5D0, 0.63D0 /)
    alpha1=    normalize( alpha1 )
    alpha2=    normalize( alpha2 )
    alpha3=    normalize( alpha3 )
    rhoP  = (/ 1.0D0, 0.7D0, 0.0D0 /)

    expectedBeta  =    3.06335717484026_wp                                                   ! pre-computed
    expectedAlphaC= (/ 0.711080695307172_wp, 0.613705694896886_wp, 0.343117421318999_wp /)   ! pre-computed

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC12, alphaC12, combOR)
    call combineTwoElementsPartialCorrelation( betaC12, alphaC12, beta3, alpha3, rhoP, betaC, alphaC, combOR)

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta3, alpha3, rhoP, betaC12, alphaC12, combOR)
    call combineTwoElementsPartialCorrelation( betaC12, alphaC12, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call combineTwoElementsPartialCorrelation( beta2, alpha2, beta3, alpha3, rhoP, betaC12, alphaC12, combOR)
    call combineTwoElementsPartialCorrelation( betaC12, alphaC12, beta1, alpha1, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, 1d-6, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), 1d-6, errorText)
    end do

end subroutine testcombineThreeElementsPartialCorrelation4

!
!> Test of combine multiple elements spatial correlated \n
!! This test gives the results as calculated with the method residual correlation
!!
subroutine testcombineMultipleElementsSpatialCorrelated1
    integer, parameter                                              :: nStochast = 4        ! Number of stochastic variables
    integer, parameter                                              :: nElements = 20       ! Number of elements
    real (kind = wp)                                                :: betaCrossSection     ! Beta for the cross section
    real (kind = wp), dimension(nStochast)                          :: alphaCrossSection
    real (kind = wp), dimension(nElements)                          :: betaElement      ! Beta for the elements
    real (kind = wp), dimension(:, :), allocatable                  :: alphaElement     ! Alpha() for the elements
    real (kind = wp)                                                :: betaSection      ! Beta for the section
    real (kind = wp), dimension(nStochast)                          :: alphaSection     ! Alpha() for the section
    real (kind = wp)                                                :: betaC            ! Beta for the combination of the elements
    real (kind = wp), dimension(nStochast)                          :: alphaC           ! Alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast)                          :: rhoXK            !
    real (kind = wp), dimension(nStochast)                          :: dXK              !
    real (kind = wp)                                                :: sectionLength    !
    integer                                                         :: i                !
    integer                                                         :: iStochast        ! Index for referring to a stochastic variable
    integer                                                         :: ierr             ! Error code of allocate
    character( len = 132)                                           :: errorText        ! Diagnostic message in case of test failure
    real (kind = wp), dimension(nStochast)                          :: expectedAlphaC
    real (kind = wp)                                                :: expectedBeta
!
    betaCrossSection     = 5.0d0
    alphaCrossSection(1) = 0.6D0
    alphaCrossSection(2) = sqrt(0.5D0 - 0.36)
    alphaCrossSection(3) = 0.6D0
    alphaCrossSection(4) = sqrt(0.5D0 - 0.36)
    rhoXK                = (/  0.5D0,  0.5D0,  0.2D0,  0.2D0 /)
    dXK                  = (/ 500.D0, 300.D0, 500.D0, 300.D0 /)
    sectionLength        = 100.D0
!
    allocate (alphaElement(nElements, nStochast), stat=ierr)
    call assert_equal(ierr, 0, "allocation of alphaElement failed")

    expectedBeta  =    4.38787743765301_wp                                                                         ! pre-computed
    expectedAlphaC= (/ 0.635285167139092_wp, 0.393519326675495_wp, 0.565059833788674_wp, 0.349660807332817_wp /)   ! pre-computed
!
    call upscaleLengthWrapper ( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection )
    betaElement = betaSection
    do i = 1, nElements
        alphaElement(i,:) = alphaSection
    end do
    call combineMultipleElements( betaElement, alphaElement, rhoXK, betaC, alphaC )
!
    call assert_comparable( betaC, expectedBeta, 1d-6, "An unexpected value is found for the beta of the combined elements" )
!
    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), 1d-6, errorText)
    end do

end subroutine testcombineMultipleElementsSpatialCorrelated1
!
!> Test of combine multiple elements spatial correlated \n
!! This test gives the results as calculated with the method residual correlation
!!
subroutine testcombineMultipleElementsSpatialCorrelated2
    integer, parameter                                              :: nStochast = 4        ! Number of stochastic variables
    integer, parameter                                              :: nElements = 20       ! Number of elements
    real (kind = wp)                                                :: betaCrossSection     ! Beta for the cross section
    real (kind = wp), dimension(nStochast)                          :: alphaCrossSection
    real (kind = wp), dimension(nElements)                          :: betaElement      ! Beta for the elements
    real (kind = wp), dimension(:, :), allocatable                  :: alphaElement     ! Alpha() for the elements
    real (kind = wp)                                                :: betaSection      ! Beta for the section
    real (kind = wp), dimension(nStochast)                          :: alphaSection     ! Alpha() for the section
    real (kind = wp)                                                :: betaC            ! Beta for the combination of the elements
    real (kind = wp), dimension(nStochast)                          :: alphaC           ! Alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast)                          :: rhoXK            !
    real (kind = wp), dimension(nStochast)                          :: rhoXAK           !
    real (kind = wp), dimension(nStochast)                          :: dXK              !
    real (kind = wp)                                                :: sectionLength    !
    integer                                                         :: i                !
    integer                                                         :: iStochast        ! Index for referring to a stochastic variable
    integer                                                         :: ierr             ! Error code of allocate
    character( len = 132)                                           :: errorText        ! Diagnostic message in case of test failure
    real (kind = wp), dimension(nStochast)                          :: expectedAlphaC
    real (kind = wp)                                                :: expectedBeta
!
    betaCrossSection     = 5.0d0
    alphaCrossSection(1) = 0.6D0
    alphaCrossSection(2) = sqrt(0.5D0 - 0.36_wp)
    alphaCrossSection(3) = 0.6D0
    alphaCrossSection(4) = sqrt(0.5D0 - 0.36_wp)
    rhoXK                = (/  0.5D0,  0.5D0,  0.2D0,  0.2D0 /)
    dXK                  = (/ 500.D0, 300.D0, 500.D0, 300.D0 /)
    sectionLength        = 100.D0
!
    allocate (alphaElement(nElements, nStochast), stat=ierr)
    call assert_equal(ierr, 0, "allocation of alphaElement failed")

    expectedBeta  =    4.61415014812874_wp                                                                         ! pre-computed
    expectedAlphaC= (/0.597708361676096_wp, 0.375385677437277_wp, 0.599181645055476_wp, 0.377904305064187_wp /)   ! pre-computed
!
    call upscaleLengthWrapper ( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection )
    betaElement = betaSection
    do i = 1, nElements
        alphaElement(i,:) = alphaSection
    end do
    do i = 1, nStochast
        rhoXAK(i) = rhoXK(i) + (1.D0 - rhoXK(i)) * exp(- sectionLength**2 / dXK(i)**2)
    end do
    call combineMultipleElements( betaElement, alphaElement, rhoXAK, betaC, alphaC )
!
    call assert_comparable( betaC, expectedBeta, 1d-6, "An unexpected value is found for the beta of the combined elements" )
!
    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), 1d-6, errorText)
    end do

end subroutine testcombineMultipleElementsSpatialCorrelated2
!
!> Test of combine multiple elements spatial correlated \n
!! This test gives the results as calculated with the method residual correlation
!!
subroutine testcombineMultipleElementsSpatialCorrelated3
    integer, parameter                                              :: nStochast = 4        ! Number of stochastic variables
    integer, parameter                                              :: nElements = 20       ! Number of elements
    real (kind = wp)                                                :: betaCrossSection     ! Beta for the cross section
    real (kind = wp), dimension(nStochast)                          :: alphaCrossSection
    real (kind = wp)                                                :: betaSection      ! Beta for the section
    real (kind = wp), dimension(nStochast)                          :: alphaSection     ! Alpha() for the section
    real (kind = wp), dimension(nStochast)                          :: rhoXK            !
    real (kind = wp), dimension(nStochast)                          :: dXK              !
    real (kind = wp)                                                :: sectionLength    !
    integer                                                         :: iStochast        ! Index for referring to a stochastic variable
    character( len = 132)                                           :: errorText        ! Diagnostic message in case of test failure
    real (kind = wp), dimension(nStochast)                          :: expectedAlphaC
    real (kind = wp)                                                :: expectedBeta
!
    betaCrossSection     = 5.0d0
    alphaCrossSection(1) = 0.6D0
    alphaCrossSection(2) = sqrt(0.5D0 - 0.36_wp)
    alphaCrossSection(3) = 0.6D0
    alphaCrossSection(4) = sqrt(0.5D0 - 0.36_wp)
    rhoXK                = (/  0.5D0,  0.5D0,  0.2D0,  0.2D0 /)
    dXK                  = (/ 500.D0, 300.D0, 500.D0, 300.D0 /)
    sectionLength        = 100.D0
!
    expectedBeta  =    4.50641035819668_wp                                                                         ! pre-computed
    expectedAlphaC= (/ 0.578741673689891_wp, 0.385418150246354_wp, 0.598199853682860_wp, 0.398331344045516_wp /)   ! pre-computed
!
    call upscaleLengthWrapper ( betaCrossSection, alphaCrossSection, rhoXK, dXK, nElements * sectionLength, &
        betaSection, alphaSection )
!
    call assert_comparable( betaSection, expectedBeta, 1d-6, "An unexpected value is found for the beta of the combined elements" )
!
    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaSection(iStochast), expectedAlphaC(iStochast), 1d-6, errorText)
    end do

end subroutine testcombineMultipleElementsSpatialCorrelated3
!

!
! NOTE. The tests below are a redo of a subset of the tests implemented above. In the preceding tests it is
! assumed that the 'standard' method is followed in combineTwoElementsPartialCorrelation() for the calculation
! of the alpha() of a combination of two elements (standard method as used till now, i.e. January 2014).
!
! In the memo:
!        Hydra-Ring: Rekenmethode combinatie mechanism en en dijkvakken.
!        Section 3: Onnauwkeurigheid alfa-bepaling.
!        Authors: A.Vrouwenvelder/H.Steenbergen (TNO), June 2013
! a correction method for the computation of the alpha() is described. This correction is for neglected
! area of fail domain within the procedure for finding equivalent alpha(), when using numerical differentiation
! of the identified reliability index.
! In the memo the results of tests with and without the correction are described. A part of these tests are
! also carried out in the present series of tests.
! The code for such a correction of the alpha() is already included in subroutine combineTwoElementsPartialCorrelation()
! but this code is NOT yet activated in the standard version.
! The tests below provide tests for the case that the correction procedure is activated. As long as this is not the
! standard the following tests must NOT be executed.
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= Normalised( 0.436, 0.9, 0.0  ) \n
!! Element 2 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.0, 0.436) \n
!! RhoP = ( 1.0, 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 3: Standard case with different alpha() values. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!! See also the associated test in testcombineTwoElementsPartialCorrelation9() \n
subroutine testcombineTwoElementsPartialCorrelation9a
    integer, parameter                     :: nStochast = 3         ! number of stochastic variables
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

    beta1 =    3.5d0
    beta2 =    3.5d0
    alpha1= (/ 0.436D0, 0.900D0, 0.000D0/)
    alpha2= (/ 0.900D0, 0.000D0, 0.436D0/)
    alpha1=    normalize( alpha1 )
    alpha2=    normalize( alpha2 )
    rhoP  =    1.0d0

    expectedBeta  =    3.31256284734426d0                                                 ! pre-computed
    expectedAlphaC= (/ 0.764785923688505d0, 0.580278077398837d0, 0.279964004504562d0 /)   ! pre-computed with    alpha() correction

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation9a
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= Normalised( 0.436, 0.9  ) \n
!! Element 2 with beta= 3.5, and design point alpha= Normalised( 0.9  , 0.436) \n
!! RhoP = ( 1.0, 0.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 3: Standard case with different alpha() values. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!! See also the associated test in testcombineTwoElementsPartialCorrelation10() \n
subroutine testcombineTwoElementsPartialCorrelation10a
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

    beta1 =    3.5d0
    beta2 =    3.5d0
    alpha1= (/ 0.436D0, 0.900D0 /)
    alpha2= (/ 0.900D0, 0.436D0 /)
    alpha1 =   normalize( alpha1 )
    alpha2 =   normalize( alpha2 )
    rhoP  = (/ 1.0D0  , 0.0D0   /)

    expectedBeta  =    3.31256284734426d0                            ! pre-computed
    expectedAlphaC= (/ 0.764785923688505d0, 0.64428447981301d0 /)   ! pre-computed with    alpha() correction

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation10a
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8  ) \n
!! Element 2 with beta= 3.5, and design point alpha= ( 0.6, 0.8  ) \n
!! RhoP = ( 1.0, 0.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 1: Standardcase. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!! See also testcombineTwoElementsPartialCorrelation11()
subroutine testcombineTwoElementsPartialCorrelation11a
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
    real (kind = wp)                       :: margin = 1.0d-9       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.5d0
    beta2 =    beta1
    alpha1= (/ 0.6d0, 0.8d0 /)
    alpha2=    alpha1
    rhoP  = (/ 1.0d0, 0.0d0 /)

    expectedBeta  =    3.31214912249458d0                            ! pre-computed
    expectedAlphaC= (/ 0.634952167871736d0, 0.772551451047102d0 /)   ! pre-computed with alpha() correction

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation11a
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8, 0.0 ) \n
!! Element 2 with beta= 3.5, and design point alpha= ( 0.6, 0.0, 0.8 ) \n
!! RhoP = ( 1.0, 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 1: Standardcase. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!! See also testcombineTwoElementsPartialCorrelation12()
subroutine testcombineTwoElementsPartialCorrelation12a
    integer, parameter                     :: nStochast = 3         ! number of stochastic variables
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
    real (kind = wp)                       :: margin = 1.0d-9       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

!   For "potential new" combineTwoElementsPartialCorrelation, with correction of the alpha_equivalent

    beta1 =    3.5d0
    beta2 =    beta1
    alpha1= (/ 0.6d0, 0.8d0, 0.0d0 /)
    alpha2= (/ 0.6d0, 0.0d0, 0.8d0 /)
    rhoP  = (/ 1.0d0, 1.0d0, 1.0d0 /)

    expectedBeta     =    3.31214912249458d0                                                 ! pre-computed
    expectedAlphaC   = (/ 0.634952167871736d0, 0.546276369850913d0, 0.546276369850913d0 /)   ! pre-computed with alpha() correction

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation12a

!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8  ) \n
!! Element 2 with beta= 3.8, and design point alpha= ( 0.6, 0.8  ) \n
!! RhoP = ( 1.0, 0.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 2: Standardcase with different beta-values. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!!     See also the associated test in testcombineTwoElementsPartialCorrelation13() \n
subroutine testcombineTwoElementsPartialCorrelation13a
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
    real (kind = wp)                       :: margin = 1.0d-9       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.5d0
    beta2 =    3.8d0
    alpha1= (/ 0.6d0, 0.8d0 /)
    alpha2=    alpha1
    rhoP  = (/ 1.0d0, 0.0d0 /)

    expectedBeta  =    3.42801564331459d0                            ! pre-computed
    expectedAlphaC= (/ 0.631154181145020d0, 0.775657398355201d0 /)   ! pre-computed with alpha() correction

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation13a
!
!
!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.5, and design point alpha= ( 0.6, 0.8, 0.0 ) \n
!! Element 2 with beta= 3.8, and design point alpha= ( 0.6, 0.0, 0.8 ) \n
!! RhoP = ( 1.0, 1.0, 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 2: Standardcase with different beta-values. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!!     See also the associated test in testcombineTwoElementsPartialCorrelation14 \n
subroutine testcombineTwoElementsPartialCorrelation14a
    integer, parameter                     :: nStochast = 3         ! number of stochastic variables
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
    real (kind = wp)                       :: margin = 1.0d-9       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.5d0
    beta2 =    3.8d0
    alpha1= (/ 0.6d0, 0.8d0, 0.0d0 /)
    alpha2= (/ 0.6d0, 0.0d0, 0.8d0 /)
    rhoP  = (/ 1.0d0, 1.0d0, 1.0d0 /)

    expectedBeta     =    3.42801564331459d0                                                  ! pre-computed
    expectedAlphaC   = (/ 0.631154181145020d0, 0.735508613458735d0, 0.246315811817203d0 /)    ! pre-computed with alpha() correction

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable( betaC, expectedBeta, margin, "An unexpected value is found for the beta of the combined elements" )

    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast), margin, errorText)
    end do

end subroutine testcombineTwoElementsPartialCorrelation14a

!> Test of combineTwoElementsPartialCorrelation \n
!! Testing the OR-combination of 2 non-identical, and partially correlated elements \n
!! Element 1 with beta= 3.0, and dimension 10 of design point alpha \n
!! Element 2 with beta= 3.5, and dimension 10 of design point alpha \n
!! RhoP = ( 1.0, ..., 1.0) \n
!! The resulting beta and alphas of the combination should be equal to their expected values \n
!! NB. Present test is for a correction of the alpha in combineElements() as described in memo: \n
!!     Hydra-Ring: Rekenmethode combinatie mechanismen en dijkvakken.\n
!!     Section 3. Onnauwkeurigheid alfa-bepaling. \n
!!     Testcase 2: Standardcase with different beta-values. \n
!!     A.Vrouwenvelder/H.Steenbergen (TNO), June 2013 \n
!!     See also the associated test in testcombineTwoElementsPartialCorrelation99 \n
subroutine testcombineTwoElementsPartialCorrelation99a
    integer, parameter                     :: nStochast = 10        ! number of stochastic variables
    integer                                :: iStochast             ! Index for referring to a stochastic variable
    real (kind = wp)                       :: beta1                 ! beta for the first  element
    real (kind = wp)                       :: beta2                 ! beta for the second element
    real (kind = wp)                       :: expectedBetaC         ! beta that is expected for the combined elements  (with    alpha() correction in combineElements())
    real (kind = wp)                       :: expectedBetaNoC       ! beta that is expected for the combined elements  (without alpha() correction in combineElements())
    real (kind = wp)                       :: betaC                 ! beta for the combination of the elements
    real (kind = wp)                       :: maxDifAlpha           ! maximum difference of two alpha()
    real (kind = wp)                       :: nrmDifAlpha           ! root mean square difference of two alpha()
    real (kind = wp), dimension(nStochast) :: alpha1                ! alpha() for the first  element
    real (kind = wp), dimension(nStochast) :: alpha2                ! alpha() for the second element
    real (kind = wp), dimension(nStochast) :: diffAlpha             ! difference between two alpha() vectors
    real (kind = wp), dimension(nStochast) :: expectedAlphaC        ! The alpha() that is expected for the combined elements (with    alpha() correction in combineElements())
    real (kind = wp), dimension(nStochast) :: expectedAlphaNoC      ! The alpha() that is expected for the combined elements (without alpha() correction in combineElements())
    real (kind = wp), dimension(nStochast) :: alphaC                ! alpha() for the combination of the elements
    real (kind = wp), dimension(nStochast) :: rhoP                  ! correlation of the stochastic variables
    real (kind = wp)                       :: margin = 1.0d-9       ! acceptable margin when comparing expected beta and alpha with their computed values
    character( len = 132)                  :: errorText             ! Diagnostic message in case of test failure

    beta1 =    3.0d0
    beta2 =    3.5d0
    alpha1= (/ 0.6d0, 0.8d0, 0.4d0, 0.0d0, 0.2d0, 0.7d0, 0.3d0, 0.8d0, 0.9d0, 1.0d0 /)
    alpha2= (/ 0.1d0, 0.0d0, 0.7d0, 0.9d0, 0.8d0, 0.1d0, 0.9d0, 0.5d0, 0.3d0, 0.0d0 /)
    alpha1 =   normalize( alpha1 )
    alpha2 =   normalize( alpha2 )
    rhoP  =    1.0d0

!   Expected beta and alpha() when the alpha() correction in combineElements() is used:
    expectedBetaC    =    2.95370049889346d0                             ! pre-computed
    expectedAlphaC   = (/ 0.277865981946082d0, 0.361174823684065d0, 0.244040175818021d0, &
                          0.906352706365079d-1, 0.168592815519657d0, 0.323038934343359d0, &
                          0.222247930634836d0, 0.395033459584985d0, 0.427126699802553d0, &
                          0.451566604194081d0 /) ! PreComputed

!   Expected beta and alpha() when the alpha() correction in combineElements() is NOT used:
    expectedBetaNoC  =    2.95369220100359d0                          ! pre-computed
    expectedAlphaNoC = (/ 0.276718484722571d0   , 0.355992578625044d0, 0.246929500663167d0, &
                          8.934246282493763D-002, 0.168082000877036d0, 0.321229137047491d0, &
                          0.222421540240202d0   , 0.404946735567986d0, 0.429818797361036d0, &
                          0.445087400330955d0                                             /)     ! PreComputed
!   Maximum and L2-norm of the difference of expectedAlphaC() and expectedAlphaNoC
    diffAlpha  = expectedAlphaC - expectedAlphaNoC
    maxDifAlpha= maxval( abs( diffAlpha) )              ! In present case the result will be 1.808143227518799D-002
    nrmDifAlpha= sqrt  ( sum ( diffAlpha * diffAlpha) ) ! In present case the result will be 2.501029768417005D-002

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

!   Check computed beta when no alpha() correction is applied:
!   call assert_comparable( betaC, expectedBetaNoC, margin, "An unexpected value is found for the beta of the combined elements" )

!   Check computed beta when alpha() correction is activated:
    call assert_comparable( betaC, expectedBetaC  , margin, "An unexpected value is found for the beta of the combined elements" )
!
    do iStochast= 1, nStochast
       write( errorText, '(a, i0, a)')                                                    &
             "An unexpected value is found for design point entry  alpha(", iStochast,    &
             ")  of the combined elements"
!      call assert_comparable( alphaC(iStochast), expectedAlphaNoC(iStochast), margin, errorText)    ! In case no   alpha() correction
       call assert_comparable( alphaC(iStochast), expectedAlphaC(iStochast)  , margin, errorText)    ! With alpha() correction
    end do

end subroutine testcombineTwoElementsPartialCorrelation99a

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

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR)

    call assert_comparable(betaC, -1.66664950156622_wp, margin, "diff in betaC")
    call assert_comparable(alphaC, [ 0.941544051697653_wp, 0.336889889798984_wp, 2.939378451871372D-005], margin, "diff in alphaC")
end subroutine testcombinaTwoElementsPartialCorrelationPiping

! > test testcombineMultipleElementsProb
subroutine testcombineMultipleElementsProb1

    integer, parameter          :: nStochasts = 3
    integer, parameter          :: nElements = 1
    real(kind=wp)               :: betaElement(nElements)               !< Reliability index per element
    real(kind=wp)               :: alphaElement(nElements,nStochasts)   !< Alpha vector per element
    real(kind=wp)               :: percentages(nElements)               !< Array of percentages
    real(kind=wp)               :: beta                                 !< Reliability index after combining over elements
    real(kind=wp)               :: alpha(nStochasts)                    !< Alpha vector after combining over elements
    real(kind=wp)               :: betaExpected                         !< Expected reliability index after combining over elements
    real(kind=wp)               :: alphaExpected(nStochasts)            !< Expected alpha vector after combining over elements
    real(kind=wp), parameter    :: margin = 1d-6

    betaElement(1)    = 3.5_wp
    alphaElement(1,1) = 1.0_wp
    alphaElement(1,2) = 0.0_wp
    alphaElement(1,3) = 0.0_wp
    percentages(1)    = 100.0_wp

    betaExpected     = betaElement(1)
    alphaExpected(1) = alphaElement(1,1)
    alphaExpected(2) = alphaElement(1,2)
    alphaExpected(3) = alphaElement(1,3)

    call combineMultipleElementsProb( betaElement, alphaElement, percentages, beta, alpha, combOr )

    call assert_comparable(beta, betaExpected, margin, "difference in beta")
    call assert_comparable(alpha, alphaExpected, margin, "difference in alpha")

end subroutine testcombineMultipleElementsProb1

! > test testcombineMultipleElementsProb
subroutine testcombineMultipleElementsProb2
    
    integer, parameter          :: nStochasts = 3
    integer, parameter          :: nElements = 2
    real(kind=wp)               :: betaElement(nElements)               !< Reliability index per element
    real(kind=wp)               :: alphaElement(nElements,nStochasts)   !< Alpha vector per element
    real(kind=wp)               :: percentages(nElements)               !< Array of percentages
    real(kind=wp)               :: beta                                 !< Reliability index after combining over elements
    real(kind=wp)               :: alpha(nStochasts)                    !< Alpha vector after combining over elements
    real(kind=wp)               :: betaExpected                         !< Expected reliability index after combining over elements
    real(kind=wp)               :: alphaExpected(nStochasts)            !< Expected alpha vector after combining over elements
    real(kind=wp), parameter    :: margin = 1d-7
    integer                     :: i

    do i = 1, nElements
        betaElement(i)    = 3.5_wp
        alphaElement(i,1) = 1.0_wp
        alphaElement(i,2) = 0.0_wp
        alphaElement(i,3) = 0.0_wp
        percentages(i)    = 50.0_wp
    enddo

    betaExpected     = 3.5000155_wp ! precomputed; slightly different from 3.5 due to term pfu * pf2pf1
    alphaExpected(1) = alphaElement(1,1)
    alphaExpected(2) = alphaElement(1,2)
    alphaExpected(3) = alphaElement(1,3)

    call combineMultipleElementsProb( betaElement, alphaElement, percentages, beta, alpha, combOr )

    call assert_comparable(beta, betaExpected, margin, "difference in beta")
    call assert_comparable(alpha, alphaExpected, margin, "difference in alpha")

end subroutine testcombineMultipleElementsProb2

! > test testcombineMultipleElementsProb
subroutine testcombineMultipleElementsProb3

    integer, parameter          :: nStochasts = 3
    integer, parameter          :: nElements = 10
    real(kind=wp)               :: betaElement(nElements)               !< Reliability index per element
    real(kind=wp)               :: alphaElement(nElements,nStochasts)   !< Alpha vector per element
    real(kind=wp)               :: percentages(nElements)               !< Array of percentages
    real(kind=wp)               :: beta                                 !< Reliability index after combining over elements
    real(kind=wp)               :: alpha(nStochasts)                    !< Alpha vector after combining over elements
    real(kind=wp)               :: betaExpected                         !< Expected reliability index after combining over elements
    real(kind=wp)               :: alphaExpected(nStochasts)            !< Expected alpha vector after combining over elements
    real(kind=wp), parameter    :: margin = 1d-7
    integer                     :: i

    do i = 1, nElements
        betaElement(i)    = 3.5_wp
        alphaElement(i,1) = 1.0_wp
        alphaElement(i,2) = 0.0_wp
        alphaElement(i,3) = 0.0_wp
        percentages(i)    = 10.0_wp
    enddo

    betaExpected     = 3.5000279_wp  ! precomputed; slightly different from 3.5 due to term pfu * pf2pf1
    alphaExpected(1) = alphaElement(1,1)
    alphaExpected(2) = alphaElement(1,2)
    alphaExpected(3) = alphaElement(1,3)

    call combineMultipleElementsProb( betaElement, alphaElement, percentages, beta, alpha, combOr )

    call assert_comparable(beta, betaExpected, margin, "difference in beta")
    call assert_comparable(alpha, alphaExpected, margin, "difference in alpha")
    
end subroutine testcombineMultipleElementsProb3

! > test testcombineMultipleElementsProb
subroutine testcombineMultipleElementsProb4
    
    integer, parameter          :: nStochasts = 3
    integer, parameter          :: nElements = 2
    real(kind=wp)               :: betaElement(nElements)               !< Reliability index per element
    real(kind=wp)               :: alphaElement(nElements,nStochasts)   !< Alpha vector per element
    real(kind=wp)               :: percentages(nElements)               !< Array of percentages
    real(kind=wp)               :: beta                                 !< Reliability index after combining over elements
    real(kind=wp)               :: alpha(nStochasts)                    !< Alpha vector after combining over elements
    real(kind=wp)               :: betaExpected                         !< Expected reliability index after combining over elements
    real(kind=wp)               :: alphaExpected(nStochasts)            !< Expected alpha vector after combining over elements
    real(kind=wp), parameter    :: margin = 1d-3
    integer                     :: i

    betaElement(1) = 3.0_wp
    betaElement(2) = 3.5_wp

    do i = 1, nElements
        alphaElement(i,1) = 1.0_wp
        alphaElement(i,2) = 0.0_wp
        alphaElement(i,3) = 0.0_wp
    enddo

    percentages(1) = 30.0_wp
    percentages(2) = 70.0_wp

    betaExpected     = 3.25457554349_wp ! pre-computed with Excel
    alphaExpected(1) = alphaElement(1,1)
    alphaExpected(2) = alphaElement(1,2)
    alphaExpected(3) = alphaElement(1,3)

    call combineMultipleElementsProb( betaElement, alphaElement, percentages, beta, alpha, combOr )

    call assert_comparable(beta, betaExpected, margin, "difference in beta")
    call assert_comparable(alpha, alphaExpected, margin, "difference in alpha")

end subroutine testcombineMultipleElementsProb4

! > test testcombineMultipleElementsProb
subroutine testcombineMultipleElementsProb5
    
    integer, parameter          :: nStochasts = 3
    integer, parameter          :: nElements = 10
    real(kind=wp)               :: betaElement(nElements)               !< Reliability index per element
    real(kind=wp)               :: alphaElement(nElements,nStochasts)   !< Alpha vector per element
    real(kind=wp)               :: percentages(nElements)               !< Array of percentages
    real(kind=wp)               :: beta                                 !< Reliability index after combining over elements
    real(kind=wp)               :: alpha(nStochasts)                    !< Alpha vector after combining over elements
    real(kind=wp)               :: betaExpected                         !< Expected reliability index after combining over elements
    real(kind=wp)               :: alphaExpected(nStochasts)            !< Expected alpha vector after combining over elements
    real(kind=wp), parameter    :: margin = 1d-3
    integer                     :: i

    do i = 1, nElements
        betaElement(i)    = 3.0_wp + (i-1)*0.1_wp
        alphaElement(i,1) = 1.0_wp
        alphaElement(i,2) = 0.0_wp
        alphaElement(i,3) = 0.0_wp
        percentages(i)    = 10.0_wp
    enddo

    betaExpected     = 3.32348724653_wp ! pre-computed with Excel
    alphaExpected(1) = alphaElement(1,1)
    alphaExpected(2) = alphaElement(1,2)
    alphaExpected(3) = alphaElement(1,3)

    call combineMultipleElementsProb( betaElement, alphaElement, percentages, beta, alpha, combOr )

    call assert_comparable(beta, betaExpected, margin, "difference in beta")
    call assert_comparable(alpha, alphaExpected, margin, "difference in alpha")

end subroutine testcombineMultipleElementsProb5

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

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )
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

    call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, betaC, alphaC, combOR )
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
        call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, beta3A, alpha3A, combOR )

        beta1 =  beta1finalB * real(i, wp) / 50.0_wp
        beta2 =  beta2finalB * real(i, wp) / 50.0_wp
        call combineTwoElementsPartialCorrelation( beta1, alpha1, beta2, alpha2, rhoP, beta3B, alpha3B, combOR )
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
end module combineElementsTests
