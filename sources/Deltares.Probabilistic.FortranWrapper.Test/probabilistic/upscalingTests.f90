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

!> @file upscalingTests.f90
!! A set of tests for upscaling probabilities in time, using subroutines in module upscaling within probabilistic \n
!<
!
module upscalingTests

    use ftnunit
    use precision
    use feedback
    use interface_combin
    use interface_distributions
    use interface_convert
    use interface_probCalc

    implicit none

    private
    public :: allUpscalingTests

contains

!> This subroutine contains tests of upscaling in time. \n
!! \n
!! First  part: \n
!! A set of tests for upscaling of probabilities in time \n
!!             (upscaling failure probabilities for equal elements, using subroutine upscaleInTime) \n
!! \n
!! Second part: \n
!! A set of tests, checking the consistency of the equivalent alpha according to the subroutines \n
!!              upscaleInTime and combineTwoElementsPartialCorrelation \n
!
subroutine allUpscalingTests
    integer, parameter :: level = 1

!   Part 2: Testing the upscaling of probabilities in time.
    call testWithLevel( upscaleInTimeTests1, "upscalingTests:  1 Upscaling failure probabilities in time", level)
    call testWithLevel( upscaleInTimeTests2, "upscalingTests:  2 Upscaling failure probabilities in time", level)
    call testWithLevel( upscaleInTimeTests3, "upscalingTests:  3 Upscaling failure probabilities in time", level)
    call testWithLevel( upscaleInTimeTests4, "upscalingTests:  4 Upscaling failure probabilities in time", level)
    call testWithLevel( upscaleInTimeTests5, "upscalingTests:  5 Upscaling failure probabilities in time", level)
    call testWithLevel( upscaleInTimeTests6, "upscalingTests:  6 Upscaling failure probabilities in time", level)
    call testWithLevel( upscaleInTimeTests7, "upscalingTests:  7 Upscaling failure probabilities in time", level)
!
!   Part 3:
!   Tests for equivalent alfa: check the consistency of the equivalent alpha routines
!   between the upscaleInTime subroutine, and the combineTwoElements routine.
!
    call testWithLevel( EquivalentAlphaTesting1, "upscalingTests:  1 Consistency of the equivalent alpha", level)
    call testWithLevel( EquivalentAlphaTesting2, "upscalingTests:  2 Consistency of the equivalent alpha", level)
    call testWithLevel( EquivalentAlphaTesting3, "upscalingTests:  3 Consistency of the equivalent alpha", level)
    call testWithLevel( EquivalentAlphaTesting4, "upscalingTests:  4 Consistency of the equivalent alpha", level)
    call testWithLevel( EquivalentAlphaTesting5, "upscalingTests:  5 Consistency of the equivalent alpha", level)
    call testWithLevel( EquivalentAlphaTesting6, "upscalingTests:  6 Consistency of the equivalent alpha", level)
!
!   Part 4:
!   Testing upscaling to largest block duration
    call testWithLevel( upscaleToLargestBlockTests1, &
        "upscalingTests:  1 Upscaling failure probabilities to largest block duration", level)
    call testWithLevel( upscaleToLargestBlockTests2, &
        "upscalingTests:  2 Upscaling failure probabilities to largest block duration", level)
    call testWithLevel( upscaleToLargestBlockTests3, &
        "upscalingTests:  3 Upscaling failure probabilities to largest block duration", level)
!
end subroutine allUpscalingTests


!> Testing the upscaling of probabilities in time. \n
!! Test with 10 fully correlated elements. \n
!! Beta=3.5, and the design point is equal to (1 1 1 1 1)/Sqrt(5) \n
!! The resulting beta and alfa's should be equal to the values for the single element \n
!!
subroutine upscaleInTimeTests1
!
!   INPUT/OUTPUT VARIABLES
!
    integer        :: nstoch                 !< Number of stochastic variables
    real(kind=wp)  :: nrElements             !< Number of time elements (e.g. tidal periods)
    real(kind=wp)  :: alpha1                 !< aid for computing the alpha values (all equal, sum of squares equals one)
    real(kind=wp)  :: margin                 !< acceptable margin for difference between beta original and beta computed
    real(kind=wp)  :: beta                   !< Reliability index of a single time element
    real(kind=wp)  :: betaORG                !< Reliability index of a single time element
    real(kind=wp)  :: alpha(5)               !< Influence coefficients of a single time element
    real(kind=wp)  :: alphaORG(5)            !< Influence coefficients of a single time element
    real(kind=wp)  :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
!
!   ASSIGN VARIABLES
!
    nstoch = 5
    nrElements = 10.0d0
    beta   = 3.5d0
    alpha1 = sqrt(1.0d0/nstoch)
    alpha  = (/ alpha1, alpha1, alpha1, alpha1, alpha1 /)     !equal alpha values
    inrhot = (/ 1.0d0, 1.0d0, 1.0d0, 1.0d0, 1.0d0 /)          !all rho values set to one

    betaORG = beta                                            !Copy of the original beta value
    alphaORG= alpha                                           !Copy of the original influence coefficients
    margin  = 0.001d0

    call upscaleInTime (nrElements, beta, alpha, inrhot)
    write(*,*) 'Beta  for one element    : ', betaORG
    write(*,*) 'Beta  for combined system: ', beta
    write(*,*) 'Alpha for one element    : ', alphaORG
    write(*,*) 'Alpha for combined system: ', alpha

    call assert_comparable(beta , betaORG , margin, &
        "The upscaled beta  should be the same as for a single element when the rho values of all variables are equal to one")
    call assert_comparable(alpha, alphaORG, margin, &
        "The upscaled alpha should be the same as for a single element when the rho values of all variables are equal to one")

end subroutine upscaleInTimeTests1


!> Testing the upscaling of probabilities in time. \n
!! Test with 10 uncorrelated elements. \n
!! Beta=3.5, and the design point is equal to (1 0 0 0 0) \n
!! The resulting beta is verified and alfa's should be equal to the values for the single element \n
!!
subroutine upscaleInTimeTests2
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp)     :: nrElements             !< Number of time elements (e.g. tidal periods)
    real(kind=wp)     :: beta                   !< Reliability index of a single time element
    real(kind=wp)     :: betaORG                !< Reliability index of a single time element
    real(kind=wp)     :: betaExpectedResult     !< Reliability index for the combined elements
    real(kind=wp)     :: alpha(5)               !< Influence coefficients of a single time element
    real(kind=wp)     :: alphaORG(5)            !< Influence coefficients of a single time element
    real(kind=wp)     :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
    real(kind=wp)     :: Pf                     !< Failure probability of a single element
    real(kind=wp)     :: tmp                    !< temp placeholder for the unused non-exceedance probability
    real(kind=wp)     :: margin                 !< acceptable margin for difference between beta original and beta computed
    integer           :: ierr                   !< error code
    character(len=64) :: errorText              !< error message
!
!   ASSIGN VARIABLES
!
    nrElements = 10.0d0
    beta   = 3.5d0
    alpha  = (/ 1.0d0, 0.0d0, 0.0d0, 0.0d0, 0.0d0 /)
    inrhot = (/ 0.0d0, 0.0d0, 0.0d0, 0.0d0, 0.0d0 /)

    betaORG = beta
    alphaORG= alpha
    margin  = 0.001d0

    call PQfromBeta( beta, tmp, Pf )
    call BetaFromQ ( 1.D0-tmp**nrElements, betaExpectedResult, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    call upscaleInTime (nrElements, beta, alpha, inrhot)
    write(*,*) 'Beta          for one element    : ', betaORG
    write(*,*) 'Expected beta for combined system: ', betaExpectedResult
    write(*,*) 'Beta          for combined system: ', beta
    write(*,*) 'Alpha         for one element    : ', alphaORG
    write(*,*) 'Alpha         for combined system: ', alpha

    call assert_comparable(beta , betaExpectedResult, margin, &
        "The upscaled beta  should agree with the one that can be derived analytically for a series of" // &
        " 10 identical and uncorrelated single elements")
    call assert_comparable(alpha, alphaORG          , margin, &
        "The upscaled alpha should be the same as for a single element when the rho values of all variables are equal to zero")

end subroutine upscaleInTimeTests2


!> Testing the upscaling of probabilities in time. \n
!! Test with 10 uncorrelated elements. \n
!! Beta=3.5, and the design point is equal to (0 0.6 0 0.8 0) \n
!! The resulting beta is verified and alfa's should be equal to the values for the single element \n
!!
subroutine upscaleInTimeTests3
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp)     :: nrElements             !< Number of time elements (e.g. tidal periods)
    real(kind=wp)     :: beta                   !< Reliability index of a single time element
    real(kind=wp)     :: betaORG                !< Reliability index of a single time element
    real(kind=wp)     :: betaExpectedResult     !< Reliability index for the combined elements
    real(kind=wp)     :: alpha(5)               !< Influence coefficients of a single time element
    real(kind=wp)     :: alphaORG(5)            !< Influence coefficients of a single time element
    real(kind=wp)     :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
    real(kind=wp)     :: Pf                     !< Failure probability of a single element
    real(kind=wp)     :: tmp                    !< temp placeholder for the unused non-exceedance probability
    real(kind=wp)     :: margin                 !< acceptable margin for difference between beta original and beta computed
    integer           :: ierr                   !< error code
    character(len=64) :: errorText              !< error message
!
!   ASSIGN VARIABLES
!
    nrElements = 10.0d0
    beta   = 3.5d0
    alpha  = (/ 0.0d0, 0.6d0, 0.0d0, 0.8d0, 0.0d0 /)
    inrhot = (/ 0.0d0, 0.0d0, 0.0d0, 0.0d0, 0.0d0 /)

    betaORG = beta
    alphaORG= alpha
    margin  = 0.001d0

    call PQfromBeta( beta, tmp, Pf )
    call BetaFromQ ( 1.D0-tmp**nrElements, betaExpectedResult, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    call upscaleInTime (nrElements, beta, alpha, inrhot)
    write(*,*) 'Beta          for one element    : ', betaORG
    write(*,*) 'Expected beta for combined system: ', betaExpectedResult
    write(*,*) 'Beta          for combined system: ', beta
    write(*,*) 'Alpha         for one element    : ', alphaORG
    write(*,*) 'Alpha         for combined system: ', alpha

    call assert_comparable(beta , betaExpectedResult, margin, &
        "The upscaled beta  should agree with the one that can be derived analytically for a series " // &
        "of 10 identical and uncorrelated single elements")
    call assert_comparable(alpha, alphaORG          , margin, &
        "The upscaled alpha should be the same as for a single element when the rho values of all variables are equal to zero")

end subroutine upscaleInTimeTests3


!> Testing the upscaling of probabilities in time. \n
!! Test with 10 uncorrelated elements. \n
!! Beta=3.5, and the design point is equal to (-1 0 0 0 0) \n
!! The resulting beta is verified and alfa's should be equal to the values for the single element \n
!!
subroutine upscaleInTimeTests4
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp)     :: nrElements             !< Number of time elements (e.g. tidal periods)
    real(kind=wp)     :: beta                   !< Reliability index of a single time element
    real(kind=wp)     :: betaORG                !< Reliability index of a single time element
    real(kind=wp)     :: betaExpectedResult     !< Reliability index for the combined elements
    real(kind=wp)     :: alpha(5)               !< Influence coefficients of a single time element
    real(kind=wp)     :: alphaORG(5)            !< Influence coefficients of a single time element
    real(kind=wp)     :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
    real(kind=wp)     :: Pf                     !< Failure probability of a single element
    real(kind=wp)     :: tmp                    !< temp placeholder for the unused non-exceedance probability
    real(kind=wp)     :: margin                 !< acceptable margin for difference between beta original and beta computed
    integer           :: ierr                   !< error code
    character(len=64) :: errorText              !< error message
!
!   ASSIGN VARIABLES
!
    nrElements = 10.0d0
    beta   = 3.5d0
    alpha  = (/ -1.0d0, 0.0d0, 0.0d0, 0.0d0, 0.0d0 /)
    inrhot = (/  0.0d0, 0.0d0, 0.0d0, 0.0d0, 0.0d0 /)

    betaORG = beta
    alphaORG= alpha
    margin  = 0.001d0

    call PQfromBeta( beta, tmp, Pf )
    call BetaFromQ ( 1.D0-tmp**nrElements, betaExpectedResult, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    call upscaleInTime (nrElements, beta, alpha, inrhot)
    write(*,*) 'Beta          for one element    : ', betaORG
    write(*,*) 'Expected beta for combined system: ', betaExpectedResult
    write(*,*) 'Beta          for combined system: ', beta
    write(*,*) 'Alpha         for one element    : ', alphaORG
    write(*,*) 'Alpha         for combined system: ', alpha

    call assert_comparable(beta , betaExpectedResult, margin, &
        "The upscaled beta  should agree with the one that can be derived analytically for a series of " // &
        "10 identical and uncorrelated single elements")
    call assert_comparable(alpha, alphaORG          , margin, &
        "The upscaled alpha should be the same as for a single element when the rho values of all variables are equal to zero")

end subroutine upscaleInTimeTests4


!> Testing the upscaling of probabilities in time. \n
!! Test with 10 uncorrelated elements. \n
!! Beta=3.5, and the design point is equal to (0 0.6 0 -0.8 0) \n
!! The resulting beta is verified and alfa's should be equal to the values for the single element \n
!!
subroutine upscaleInTimeTests5
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp)     :: nrElements             !< Number of time elements (e.g. tidal periods)
    real(kind=wp)     :: beta                   !< Reliability index of a single time element
    real(kind=wp)     :: betaORG                !< Reliability index of a single time element
    real(kind=wp)     :: betaExpectedResult     !< Reliability index for the combined elements
    real(kind=wp)     :: alpha(5)               !< Influence coefficients of a single time element
    real(kind=wp)     :: alphaORG(5)            !< Influence coefficients of a single time element
    real(kind=wp)     :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
    real(kind=wp)     :: Pf                     !< Failure probability of a single element
    real(kind=wp)     :: tmp                    !< temp placeholder for the unused non-exceedance probability
    real(kind=wp)     :: margin                 !< acceptable margin for difference between beta original and beta computed
    integer           :: ierr                   !< error code
    character(len=64) :: errorText              !< error message
!
!   ASSIGN VARIABLES
!
    nrElements = 10.0d0
    beta   = 3.5d0
    alpha  = (/ 0.0d0, 0.6d0, 0.0d0, -0.8d0, 0.0d0 /)
    inrhot = (/ 0.0d0, 0.0d0, 0.0d0,  0.0d0, 0.0d0 /)

    betaORG = beta
    alphaORG= alpha
    margin  = 0.001d0

    call PQfromBeta( beta, tmp, Pf )
    call BetaFromQ ( 1.D0-tmp**nrElements, betaExpectedResult, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    call upscaleInTime (nrElements, beta, alpha, inrhot)
    write(*,*) 'Beta          for one element    : ', betaORG
    write(*,*) 'Expected beta for combined system: ', betaExpectedResult
    write(*,*) 'Beta          for combined system: ', beta
    write(*,*) 'Alpha         for one element    : ', alphaORG
    write(*,*) 'Alpha         for combined system: ', alpha

    call assert_comparable(beta , betaExpectedResult, margin, &
        "The upscaled beta  should agree with the one that can be derived analytically " // &
        "for a series of 10 identical and uncorrelated single elements")
    call assert_comparable(alpha, alphaORG          , margin, &
        "The upscaled alpha should be the same as for a single element when the rho values of all variables are equal to zero")

end subroutine upscaleInTimeTests5
!
!
!> Testing the upscaling of probabilities in time. \n
!! Test with 10 almost fully correlated elements. \n
!! Beta= 3.5, and the design point is equal to (0, 0.6, 0, -0.8, 0) \n
!! The resulting beta is verified and alfa's should be equal to the values for the single element \n
!!
subroutine upscaleInTimeTests6
!
!   INPUT/OUTPUT VARIABLES
!
    integer, parameter:: nStoch= 5              !< Number of stochastic variables
    real( kind= wp)   :: nrElements             !< Number of time elements (e.g. tidal periods)
    real( kind= wp)   :: beta                   !< Reliability index of a single time element
    real( kind= wp)   :: betaORG                !< Reliability index of a single time element
    real( kind= wp)   :: betaExpectedResult     !< Reliability index for the combined elements
    real( kind= wp)   :: alpha( nStoch)         !< Influence coefficients of a single time element
    real( kind= wp)   :: alphaORG( nStoch)      !< Influence coefficients of a single time element
    real( kind= wp)   :: inRhoT( nStoch)        !< Autocorrelation coefficients for each of the variables, in time
    real( kind= wp)   :: margin                 !< acceptable margin for difference between beta original and beta computed
!
!   ASSIGN VARIABLES
!
    nrElements= 10.0d0
    beta      = 3.5d0
    alpha     = (/ 0.0d0, 0.6d0, 0.0d0, -0.8d0, 0.0d0 /)
    inRhoT    = 0.999999d0

    betaORG   = beta
    alphaORG  = alpha
    margin    = 1.0d-6

    call upscaleInTime( nrElements, beta, alpha, inRhoT)
!   Set the precomputed result that is expected for beta:
    betaExpectedResult= 3.49846239808633d0

    call assert_comparable( beta , betaExpectedResult, margin, "The upscaled beta  should agree the precomputed value")
    call assert_comparable( alpha, alphaORG          , margin, "The upscaled alpha should be the same as for a single element")

end subroutine upscaleInTimeTests6

!> case from HRING-1349
!! two almost identical situations before upscaling in time.
!! however, the resulting alpha(12) differs largely
subroutine upscaleInTimeTests7
!
!   INPUT/OUTPUT VARIABLES
!
    integer, parameter         :: nStoch= 12                !< Number of stochastic variables
    real( kind= wp), parameter :: nrElements = 365.25_wp    !< Number of time elements (e.g. tidal periods)
    real( kind= wp)            :: beta                      !< Reliability index of a single time element
    real( kind= wp)            :: alpha( nStoch)            !< Influence coefficients of a single time element
    real( kind= wp)            :: inRhoT( nStoch)           !< Autocorrelation coefficients for each of the variables, in time
    real( kind= wp), parameter :: margin =  1.0d-3          !< acceptable margin for difference between beta original and beta computed
!
!   ASSIGN VARIABLES
!

    ! case 1:
    beta      = 11.57868694964_wp
    alpha     = [ -3.161166262316398E-6_wp,  0.151673576904618_wp,    -8.746544990273167E-2_wp, &
                  -8.746544990273167E-2_wp, -0.156371455735438_wp,    -0.912570913068033_wp, &
                   0.250863571511060_wp,     3.568922013287609E-4_wp,  0.203773500205695_wp, &
                   3.810363930428925E-4_wp,  1.919024503886323E-4_wp, -7.296398126214022E-4_wp]

    inRhoT    = 1.0_wp
    inRhoT(nStoch) = 0.0_wp

    call upscaleInTime( nrElements, beta, alpha, inRhoT)

    call assert_comparable( beta , 11.5765466697374_wp, margin, "The upscaled beta (case 1) should agree the precomputed value")
    call assert_comparable( alpha(nStoch), -0.311985E-3_wp , margin, &
        "The upscaled alpha (case 1) should be the same as for a single element")

    ! case 2:
    ! relative differences with case 1 approx. 1E-4
    beta      = 11.5805537878769_wp
    alpha     = [ -3.161065009975362E-6_wp,  0.151647352510170_wp,    -8.745032983732001E-2_wp, &
                  -8.745032983732001E-2_wp, -0.156344425365967_wp,    -0.912591229826987_wp, &
                   0.250861020035668_wp,     3.568801510567352E-4_wp,  0.203738889137470_wp, &
                   3.809716860474264E-4_wp,  1.918698629447029E-4_wp, -7.296245519395209E-4_wp]

    inRhoT    = 1.0_wp
    inRhoT(nStoch) = 0.0_wp

    call upscaleInTime( nrElements, beta, alpha, inRhoT)

    call assert_comparable( beta , 11.5783936419198_wp, margin, "The upscaled beta (case 2) should agree the precomputed value")
    call assert_comparable( alpha(nStoch), -0.27259E-3_wp , margin, &
        "The upscaled alpha (case 2) should be the same as for a single element")

end subroutine upscaleInTimeTests7

!> Testing of the consistency of the equivalent alpha according to the subroutines upscaleInTime and combineTwoElementsPartialCorrelation \n
!! Test with 2 partially correlated elements. \n
!! Beta is 3.5, the design point is equal to (0.6 0.8), and Rho is equal to (0.5 0.5) \n
!! The resulting beta is verified and resulting alfa's should be (approximately) equal \n
!!
subroutine EquivalentAlphaTesting1
!
    integer, parameter :: nstoch = 2             !  Number of stochastic variables
    real(kind=wp)      :: nrElements             !  Number of time elements (e.g. tidal periods)
    real(kind=wp)      :: margin                 !  acceptable margin for difference between beta original and beta computed
    real(kind=wp)      :: beta                   !  Reliability index of a single element (initial, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: betaORG                !  Reliability index of a single element (copy of initial, to input into the combine routine)
    real(kind=wp)      :: betaC                  !  Reliability index of a single element (combined beta, output of the combineTwoElements routine)
    real(kind=wp)      :: alpha   (nstoch)       !  Influence coefficients of a single element (initial alphas, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: alphaORG(nstoch)       !  Influence coeffs of a single element (copy of the initial, to input into the combine routine)
    real(kind=wp)      :: alphaC  (nstoch)       !  Influence coeffs of a single element (combined alphas, output of the combineTwoElements routine)
    real(kind=wp)      :: inRhoT  (nstoch)       !  Autocorrelation coefficients for each of the variables, in time
    real(kind=wp)      :: alphaI  (nstoch)       !  returned influence coefficients
    real(kind=wp)      :: alphaII (nstoch)       !  returned influence coefficients
!
!   Initialisations:
    nrElements  = 2.0d0
    margin      = 1.0d-3
!
    beta        = 3.5d0
    alpha       = (/ 0.6d0, 0.8d0 /)
    inRhoT      = (/ 0.5d0, 0.5d0 /)

    betaORG     = beta       ! copy of original beta
    alphaORG    = alpha      ! copy of original alpha()

    call upscaleInTime (nrElements, beta, alpha, inRhoT)
    call combineTwoElementsPartialCorrelation(betaORG, alphaORG, betaORG, alphaORG, inRhoT, betaC, alphaC, combOR, alphai, alphaii)

    write(*,*) 'Beta  - upscaling: ', beta
    write(*,*) 'Beta  - combining: ', betaC
    write(*,*) 'Alpha - upscaling: ', alpha
    write(*,*) 'Alpha - combining: ', alphaC

    call assert_comparable(beta , betaC , margin, &
        "The upscaled beta should be the same as for the combined beta (comparing simple and full methods)")
    call assert_comparable(alpha, alphaC, margin, &
    "The upscaled equivalent alphas should be the same as for the combined equivalent alphas (comparing simple and full methods)")
    call assert_comparable(alphai,  [0.51895497_wp, 0.69215321_wp], 1d-7, "diff in alphai")
    call assert_comparable(alphaii, [0.30066624_wp, 0.40150901_wp], 1d-7, "diff in alphaii")

end subroutine EquivalentAlphaTesting1


!> Testing of the consistency of the equivalent alpha according to the subroutines upscaleInTime and combineTwoElementsPartialCorrelation \n
!! Test with 2 partially correlated elements. \n
!! Beta is 3.5, the design point is equal to (0.6 0.8), and Rho is equal to (1.0 0.5) \n
!! The resulting beta is verified and resulting alfa's should be (approximately) equal
!!
subroutine EquivalentAlphaTesting2
!
    integer, parameter :: nstoch = 2             !  Number of stochastic variables
    real(kind=wp)      :: nrElements             !  Number of time elements (e.g. tidal periods)
    real(kind=wp)      :: margin                 !  acceptable margin for difference between beta original and beta computed
    real(kind=wp)      :: beta                   !  Reliability index of a single element (initial, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: betaORG                !  Reliability index of a single element (copy of initial, to input into the combine routine)
    real(kind=wp)      :: betaC                  !  Reliability index of a single element (combined beta, output of the combineTwoElements routine)
    real(kind=wp)      :: alpha   (nstoch)       !  Influence coefficients of a single element (initial alphas, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: alphaORG(nstoch)       !  Influence coeffs of a single element (copy of the initial, to input into the combine routine)
    real(kind=wp)      :: alphaC  (nstoch)       !  Influence coeffs of a single element (combined alphas, output of the combineTwoElements routine)
    real(kind=wp)      :: inRhoT  (nstoch)       !  Autocorrelation coefficients for each of the variables, in time
!
!   Initialisations:
    nrElements  = 2.0d0
    margin      = 1.0d-3
!
    beta        = 3.5d0
    alpha       = (/ 0.6d0, 0.8d0 /)
    inRhoT      = (/ 1.0d0, 0.5d0 /)

    betaORG     = beta       ! copy of original beta
    alphaORG    = alpha      ! copy of original alpha()

    call upscaleInTime (nrElements, beta, alpha, inRhoT)
    call combineTwoElementsPartialCorrelation ( betaORG, alphaORG, betaORG, alphaORG, inRhoT, betaC, alphaC, combOR )

    write(*,*) 'Beta  - upscaling: ', beta
    write(*,*) 'Beta  - combining: ', betaC
    write(*,*) 'Alpha - upscaling: ', alpha
    write(*,*) 'Alpha - combining: ', alphaC

    call assert_comparable(beta , betaC , margin, &
        "The upscaled beta should be the same as for the combined beta (comparing simple and full methods)")
    ! \TODO:
    ! The next assert is disabled. Change of the combination routine to later developments will give better results.
    ! However at this moment the new combinition routine is not agreed.
    !call assert_comparable(alpha, alphaC, margin, "The upscaled equivalent alphas should be the same as for the combined equivalent alphas (comparing simple and full methods)")

end subroutine EquivalentAlphaTesting2

!> Testing of the consistency of the equivalent alpha according to the subroutines upscaleInTime and combineTwoElementsPartialCorrelation \n
!! Test with 2 fully correlated elements. \n
!! Beta is 3.5, the design point is equal to (1.0 0.0), and Rho is equal to (1.0 0.0) \n
!! The resulting beta is verified and resulting alfa's should be (approximately) equal \n
!!
subroutine EquivalentAlphaTesting3
!
    integer, parameter :: nstoch = 2             !  Number of stochastic variables
    real(kind=wp)      :: nrElements             !  Number of time elements (e.g. tidal periods)
    real(kind=wp)      :: margin                 !  acceptable margin for difference between beta original and beta computed
    real(kind=wp)      :: beta                   !  Reliability index of a single element (initial, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: betaORG                !  Reliability index of a single element (copy of initial, to input into the combine routine)
    real(kind=wp)      :: betaC                  !  Reliability index of a single element (combined beta, output of the combineTwoElements routine)
    real(kind=wp)      :: alpha   (nstoch)       !  Influence coefficients of a single element (initial alphas, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: alphaORG(nstoch)       !  Influence coeffs of a single element (copy of the initial, to input into the combine routine)
    real(kind=wp)      :: alphaC  (nstoch)       !  Influence coeffs of a single element (combined alphas, output of the combineTwoElements routine)
    real(kind=wp)      :: inRhoT  (nstoch)       !  Autocorrelation coefficients for each of the variables, in time
!
!   Initialisations:
    nrElements  = 2.0d0
    margin      = 1.0d-3
!
    beta        = 3.5d0
    alpha       = (/ 1.0d0, 0.0d0 /)
    inRhoT      = (/ 1.0d0, 0.0d0 /)

    betaORG     = beta       ! copy of original beta
    alphaORG    = alpha      ! copy of original alpha()

    call upscaleInTime (nrElements, beta, alpha, inrhot)
    call combineTwoElementsPartialCorrelation ( betaORG, alphaORG, betaORG, alphaORG, inRhoT, betaC, alphaC, combOR )

    write(*,*) 'Beta  - upscaling: ', beta
    write(*,*) 'Beta  - combining: ', betaC
    write(*,*) 'Alpha - upscaling: ', alpha
    write(*,*) 'Alpha - combining: ', alphaC

    call assert_comparable(beta , betaC , margin, &
        "The upscaled beta should be the same as for the combined beta (comparing simple and full methods)")
    call assert_comparable(alpha, alphaC, margin, &
    "The upscaled equivalent alphas should be the same as for the combined equivalent alphas (comparing simple and full methods)")

end subroutine EquivalentAlphaTesting3

!> Testing of the consistency of the equivalent alpha according to the subroutines upscaleInTime and combineTwoElementsPartialCorrelation \n
!! Test with 2 uncorrelated elements. \n
!! Beta is 3.5, the design point is equal to (1.0 0.0), and Rho is equal to (0.0 1.0) \n
!! The resulting beta is verified and resulting alfa's should be (approximately) equal \n
!!
subroutine EquivalentAlphaTesting4
!
    integer, parameter :: nstoch = 2             !  Number of stochastic variables
    real(kind=wp)      :: nrElements             !  Number of time elements (e.g. tidal periods)
    real(kind=wp)      :: margin                 !  acceptable margin for difference between beta original and beta computed
    real(kind=wp)      :: beta                   !  Reliability index of a single element (initial, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: betaORG                !  Reliability index of a single element (copy of initial, to input into the combine routine)
    real(kind=wp)      :: betaC                  !  Reliability index of a single element (combined beta, output of the combineTwoElements routine)
    real(kind=wp)      :: alpha   (nstoch)       !  Influence coefficients of a single element (initial alphas, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: alphaORG(nstoch)       !  Influence coeffs of a single element (copy of the initial, to input into the combine routine)
    real(kind=wp)      :: alphaC  (nstoch)       !  Influence coeffs of a single element (combined alphas, output of the combineTwoElements routine)
    real(kind=wp)      :: inRhoT  (nstoch)       !  Autocorrelation coefficients for each of the variables, in time
!
!   Initialisations:
    nrElements  = 2.0d0
    margin      = 1.0d-3
!
    beta        = 3.5d0
    alpha       = (/ 1.0d0, 0.0d0 /)
    inRhoT      = (/ 0.0d0, 1.0d0 /)

    betaORG     = beta       ! copy of original beta
    alphaORG    = alpha      ! copy of original alpha()

    call upscaleInTime (nrElements, beta, alpha, inrhot)
    call combineTwoElementsPartialCorrelation ( betaORG, alphaORG, betaORG, alphaORG, inRhoT, betaC, alphaC, combOR )

    write(*,*) 'Beta  - upscaling: ', beta
    write(*,*) 'Beta  - combining: ', betaC
    write(*,*) 'Alpha - upscaling: ', alpha
    write(*,*) 'Alpha - combining: ', alphaC

    call assert_comparable(beta , betaC , margin, &
        "The upscaled beta should be the same as for the combined beta (comparing simple and full methods)")
    call assert_comparable(alpha, alphaC, margin, &
    "The upscaled equivalent alphas should be the same as for the combined equivalent alphas (comparing simple and full methods)")

end subroutine EquivalentAlphaTesting4

!> Testing of the consistency of the equivalent alpha according to the subroutines upscaleInTime and combineTwoElementsPartialCorrelation \n
!! Test with 2 partially correlated elements. \n
!! Beta is 3.5, the design point is equal to (0.6 -0.8), and Rho is equal to (1.0 0.5) \n
!! The resulting beta is verified and resulting alfa's should be (approximately) equal \n
!!
subroutine EquivalentAlphaTesting5
!
    integer, parameter :: nstoch = 2             !  Number of stochastic variables
    real(kind=wp)      :: nrElements             !  Number of time elements (e.g. tidal periods)
    real(kind=wp)      :: margin                 !  acceptable margin for difference between beta original and beta computed
    real(kind=wp)      :: beta                   !  Reliability index of a single element (initial, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: betaORG                !  Reliability index of a single element (copy of initial, to input into the combine routine)
    real(kind=wp)      :: betaC                  !  Reliability index of a single element (combined beta, output of the combineTwoElements routine)
    real(kind=wp)      :: alpha   (nstoch)       !  Influence coefficients of a single element (initial alphas, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: alphaORG(nstoch)       !  Influence coeffs of a single element (copy of the initial, to input into the combine routine)
    real(kind=wp)      :: alphaC  (nstoch)       !  Influence coeffs of a single element (combined alphas, output of the combineTwoElements routine)
    real(kind=wp)      :: inRhoT  (nstoch)       !  Autocorrelation coefficients for each of the variables, in time
!
!   Initialisations:
    nrElements  = 2.0d0
    margin      = 1.0d-3
!
    beta        = 3.5d0
    alpha       = (/ 0.6d0, -0.8d0 /)
    inRhoT      = (/ 1.0d0,  0.5d0 /)

    betaORG     = beta       ! copy of original beta
    alphaORG    = alpha      ! copy of original alpha()

    call upscaleInTime (nrElements, beta, alpha, inRhoT)
    call combineTwoElementsPartialCorrelation ( betaORG, alphaORG, betaORG, alphaORG, inRhoT, betaC, alphaC, combOR )

    write(*,*) 'Beta  - upscaling: ', beta
    write(*,*) 'Beta  - combining: ', betaC
    write(*,*) 'Alpha - upscaling: ', alpha
    write(*,*) 'Alpha - combining: ', alphaC

    call assert_comparable(beta , betaC , margin, &
        "The upscaled beta should be the same as for the combined beta (comparing simple and full methods)")
    ! \TODO:
    ! The next assert is disabled. Change of the combination routine to later developments will give better results.
    ! However at this moment the new combinition routine is not agreed.
    !call assert_comparable(alpha, alphaC, margin, "The upscaled equivalent alphas should be the same as for the combined equivalent alphas (comparing simple and full methods)")

end subroutine EquivalentAlphaTesting5

!> Testing of the consistency of the equivalent alpha according to the subroutines upscaleInTime and combineTwoElementsPartialCorrelation \n
!! Test with 2 partially correlated elements. \n
!! Beta is 3.5, the design point is equal to (0.6 -0.8), and Rho is equal to (1.0 0.0) \n
!! The resulting beta is verified and resulting alfa's should be (approximately) equal \n
!!
subroutine EquivalentAlphaTesting6
!
    integer, parameter :: nstoch = 2             !  Number of stochastic variables
    real(kind=wp)      :: nrElements             !  Number of time elements (e.g. tidal periods)
    real(kind=wp)      :: margin                 !  acceptable margin for difference between beta original and beta computed
    real(kind=wp)      :: beta                   !  Reliability index of a single element (initial, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: betaORG                !  Reliability index of a single element (copy of initial, to input into the combine routine)
    real(kind=wp)      :: betaC                  !  Reliability index of a single element (combined beta, output of the combineTwoElements routine)
    real(kind=wp)      :: alpha   (nstoch)       !  Influence coefficients of a single element (initial alphas, but gets overwritten by upscaleInTime routine)
    real(kind=wp)      :: alphaORG(nstoch)       !  Influence coeffs of a single element (copy of the initial, to input into the combine routine)
    real(kind=wp)      :: alphaC  (nstoch)       !  Influence coeffs of a single element (combined alphas, output of the combineTwoElements routine)
    real(kind=wp)      :: inRhoT  (nstoch)       !  Autocorrelation coefficients for each of the variables, in time
!
!   Initialisations:
    nrElements  = 2.0d0
    margin      = 1.0d-3
!
    beta        = 3.5d0
    alpha       = (/ 0.6d0, -0.8d0 /)
    inRhoT      = (/ 1.0d0,  0.0d0 /)

    betaORG     = beta       ! copy of original beta
    alphaORG    = alpha      ! copy of original alpha()

    call upscaleInTime (nrElements, beta, alpha, inRhoT)
    call combineTwoElementsPartialCorrelation ( betaORG, alphaORG, betaORG, alphaORG, inRhoT, betaC, alphaC, combOR )

    write(*,*) 'Beta  - upscaling: ', beta
    write(*,*) 'Beta  - combining: ', betaC
    write(*,*) 'Alpha - upscaling: ', alpha
    write(*,*) 'Alpha - combining: ', alphaC

    call assert_comparable(beta , betaC , margin, &
        "The upscaled beta should be the same as for the combined beta (comparing simple and full methods)")
    ! \TODO:
    ! The next assert is disabled. Change of the combination routine to later developments will give better results.
    ! However at this moment the new combinition routine is not agreed.
    !call assert_comparable(alpha, alphaC, margin, "The upscaled equivalent alphas should be the same as for the combined equivalent alphas (comparing simple and full methods)")

end subroutine EquivalentAlphaTesting6

!> Testing the upscaling of probabilities to largest block duration. \n
!! Test with 5 random variables, beta=3.5, alpha is random and block durations are equal. \n
!! The resulting beta and alfa's should be equal to the values for the single element \n
!!
subroutine upscaleToLargestBlockTests1
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp)  :: margin                 !< acceptable margin for difference between beta original and beta computed
    real(kind=wp)  :: beta                   !< Reliability index of a single element
    real(kind=wp)  :: betaORG                !< Reliability index of a single element
    real(kind=wp)  :: alpha(5)               !< Influence coefficients of a single element
    real(kind=wp)  :: alphaORG(5)            !< Influence coefficients of a single element
    real(kind=wp)  :: duration(5)            !< Block duration of a single element
    real(kind=wp)  :: durationORG(5)         !< Block duration of a single time element
    real(kind=wp)  :: maxduration            !< Maximum block duration
    real(kind=wp)  :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
!
!   ASSIGN VARIABLES
!
    beta            = 3.5d0
    alpha           = (/ 1, 2, 3, 4, 5/)                                !alpha values
    alpha           = alpha / sqrt(sum(alpha*alpha))                    !normalize alpha values
    duration        = (/ 1.0d0, 1.0d0, 1.0d0, 1.0d0, 1.0d0 /)           !individual block durations
    inrhot          = (/ 0.1d0, 0.3d0, 0.5d0, 0.7d0, 0.9d0 /)           !all rho values

    betaORG         = beta                                              !Copy of the original beta value
    alphaORG        = alpha                                             !Copy of the original influence coefficients
    durationORG     = duration                                          !Copy of the original durations
    maxduration     = maxval(duration)
    margin          = 0.001d0

    call upscaleToLargestBlock(betaORG, alphaORG, inrhot, durationORG, maxduration, beta, alpha, duration)
    write(*,*) 'Beta  for one element    :          ', betaORG
    write(*,*) 'Beta  for combined system:          ', beta
    write(*,*) 'Alpha for one element    :          ', alphaORG
    write(*,*) 'Alpha for combined system:          ', alpha
    write(*,*) 'Block duration for one element    : ', durationORG
    write(*,*) 'Block duration for combined system: ', duration

    call assert_comparable(beta , betaORG , margin, &
        "The upscaled beta should be the same as for a single element when the block durations are equal")
    call assert_comparable(alpha, alphaORG, margin, &
        "The upscaled alpha should be the same as for a single element when the block durations are equal")

end subroutine upscaleToLargestBlockTests1


!> Testing the upscaling of probabilities to largest block duration. \n
!! Test with 5 random variables, beta=3.5, alpha is random and block durations are equal. \n
!! The resulting beta and alfa's should be equal to the values for the single element \n
!!
subroutine upscaleToLargestBlockTests2
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp)  :: margin                 !< acceptable margin for difference between beta original and beta computed
    real(kind=wp)  :: beta                   !< Reliability index of a single element
    real(kind=wp)  :: betaORG                !< Reliability index of a single element
    real(kind=wp)  :: alpha1                 !< Influence coefficients of a single element
    real(kind=wp)  :: alpha(5)               !< Influence coefficients of a single element
    real(kind=wp)  :: alphaORG(5)            !< Influence coefficients of a single element
    real(kind=wp)  :: duration(5)            !< Block duration of a single element
    real(kind=wp)  :: durationORG(5)         !< Block duration of a single time element
    real(kind=wp)  :: maxduration            !< Maximum block duration
    real(kind=wp)  :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
!
!   ASSIGN VARIABLES
!
    beta            = 3.5d0
    alpha1          = sqrt(1.0d0/5)
    alpha           = (/ alpha1, alpha1, alpha1, alpha1, alpha1 /)      !equal alpha values
    duration        = (/ 1.0d0, 2.0d0, 3.0d0, 4.0d0, 5.0d0 /)           !individual block durations
    inrhot          = (/ 1.0d0, 1.0d0, 1.0d0, 1.0d0, 1.0d0 /)           !all rho values

    betaORG         = beta                                              !Copy of the original beta value
    alphaORG        = alpha                                             !Copy of the original influence coefficients
    durationORG     = duration                                          !Copy of the original durations
    maxduration     = maxval(duration)
    margin          = 0.001d0

    call upscaleToLargestBlock(betaORG, alphaORG, inrhot, durationORG, maxduration, beta, alpha, duration)
    write(*,*) 'Beta  for one element    :          ', betaORG
    write(*,*) 'Beta  for combined system:          ', beta
    write(*,*) 'Alpha for one element    :          ', alphaORG
    write(*,*) 'Alpha for combined system:          ', alpha
    write(*,*) 'Block duration for one element    : ', durationORG
    write(*,*) 'Block duration for combined system: ', duration

    call assert_comparable(beta , betaORG , margin, &
        "The upscaled beta  should be the same as for a single element when the rho values of all variables are equal to one")
    call assert_comparable(alpha, alphaORG, margin, &
        "The upscaled alpha should be the same as for a single element when the rho values of all variables are equal to one")

end subroutine upscaleToLargestBlockTests2

!> Testing the upscaling of probabilities to largest block duration. \n
!! Test with 5 random variables, beta=3.5, design point is equal to (0 0.6 0 0.8 0) \n
!! Auto-correlations are all set to zero. Block durations of variables with alpha>0 differ by a factor 10 with the maximum. \n
!! The resulting beta is verified (see upscaleInTimeTest3) and alfa's should be equal to the values for the single element. \n
!!
subroutine upscaleToLargestBlockTests3
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp)     :: margin                 !< acceptable margin for difference between beta original and beta computed
    real(kind=wp)     :: beta                   !< Reliability index of a single element
    real(kind=wp)     :: betaORG                !< Reliability index of a single element
    real(kind=wp)     :: alpha(5)               !< Influence coefficients of a single element
    real(kind=wp)     :: alphaORG(5)            !< Influence coefficients of a single element
    real(kind=wp)     :: duration(5)            !< Block duration of a single element
    real(kind=wp)     :: durationORG(5)         !< Block duration of a single time element
    real(kind=wp)     :: maxduration            !< Maximum block duration
    real(kind=wp)     :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
    real(kind=wp)     :: Pf                     !< Failure probability of a single element
    real(kind=wp)     :: tmp                    !< temp placeholder for the unused non-exceedance probability
    real(kind=wp)     :: betaExpectedResult     !< Reliability index for the combined elements
    integer           :: ierr                   !< error code
    character(len=64) :: errorText              !< error message
!
!   ASSIGN VARIABLES
!
    beta            = 3.5d0
    alpha           = (/ 0.0d0, 0.6d0, 0.0d0, 0.8d0, 0.0d0 /)           !alpha values
    duration        = (/ 20.0d0, 2.0d0, 1.0d0, 2.0d0, 10.0d0 /)         !individual block durations
    inrhot          = (/ 0.0d0, 0.0d0, 0.0d0, 0.0d0, 0.0d0 /)           !all rho values set to one

    betaORG         = beta                                              !Copy of the original beta value
    alphaORG        = alpha                                             !Copy of the original influence coefficients
    durationORG     = duration                                          !Copy of the original durations
    maxduration     = maxval(duration)
    margin          = 0.001d0

    call PQfromBeta( beta, tmp, Pf )
    call BetaFromQ ( 1.D0-tmp**10.0d0, betaExpectedResult, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    call upscaleToLargestBlock(betaORG, alphaORG, inrhot, durationORG, maxduration, beta, alpha, duration)
    write(*,*) 'Beta  for one element    :          ', betaORG
    write(*,*) 'Expected beta for combined system:  ', betaExpectedResult
    write(*,*) 'Beta  for combined system:          ', beta
    write(*,*) 'Alpha for one element    :          ', alphaORG
    write(*,*) 'Alpha for combined system:          ', alpha
    write(*,*) 'Block duration for one element    : ', durationORG
    write(*,*) 'Block duration for combined system: ', duration

    call assert_comparable(beta , betaExpectedResult, margin, &
        'The upscaled beta should agree with the one that can be derived analytically ' // &
        'for a series of 10 identical and uncorrelated single elements')
    call assert_comparable(alpha, alphaORG          , margin, &
        'The upscaled alpha should be the same as for a single element when the rho values of all variables are equal to zero')

end subroutine upscaleToLargestBlockTests3

end module upscalingTests
