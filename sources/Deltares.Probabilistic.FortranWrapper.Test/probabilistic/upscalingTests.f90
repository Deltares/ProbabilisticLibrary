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

!   Testing upscaling to largest block duration
    call testWithLevel( upscaleToLargestBlockTests1, &
        "upscalingTests:  1 Upscaling failure probabilities to largest block duration", level)
    call testWithLevel( upscaleToLargestBlockTests2, &
        "upscalingTests:  2 Upscaling failure probabilities to largest block duration", level)
    call testWithLevel( upscaleToLargestBlockTests3, &
        "upscalingTests:  3 Upscaling failure probabilities to largest block duration", level)
    call testWithLevel( upscaleToLargestBlockTests4, &
        "upscalingTests:  4 Upscaling failure probabilities to largest block duration; multiple wind dimensions", level)
!
end subroutine allUpscalingTests

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
    real(kind=wp), target  :: alpha(5)               !< Influence coefficients of a single element
    real(kind=wp), target  :: alphaORG(5)            !< Influence coefficients of a single element
    real(kind=wp), target  :: duration(5)            !< Block duration of a single element
    real(kind=wp), target  :: durationORG(5)         !< Block duration of a single time element
    real(kind=wp)  :: maxduration            !< Maximum block duration
    real(kind=wp), target  :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
    type(designPoint) :: dpORG, dpOut
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

    dpORG%beta = beta
    dpORG%alpha => alphaORG
    dpORG%rho => inrhot
    dpORG%duration => durationORG
    dpOut%alpha => alpha
    dpOut%duration => duration
    call upscaleToLargestBlock(dpORG, maxduration, dpOut)
    beta = dpOut%beta

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
    real(kind=wp), target  :: alpha(5)               !< Influence coefficients of a single element
    real(kind=wp), target  :: alphaORG(5)            !< Influence coefficients of a single element
    real(kind=wp), target  :: duration(5)            !< Block duration of a single element
    real(kind=wp), target  :: durationORG(5)         !< Block duration of a single time element
    real(kind=wp)  :: maxduration            !< Maximum block duration
    real(kind=wp), target  :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
    type(designPoint) :: dpORG, dpOut
!
!   ASSIGN VARIABLES
!
    beta            = 3.5d0
    alpha1          = sqrt(1.0d0/5)
    alpha           = [ alpha1, alpha1, alpha1, alpha1, alpha1 ]      !equal alpha values
    duration        = [ 1.0d0, 2.0d0, 3.0d0, 4.0d0, 5.0d0 ]           !individual block durations
    inrhot          = [ 1.0d0, 1.0d0, 1.0d0, 1.0d0, 1.0d0 ]           !all rho values

    betaORG         = beta                                              !Copy of the original beta value
    alphaORG        = alpha                                             !Copy of the original influence coefficients
    durationORG     = duration                                          !Copy of the original durations
    maxduration     = maxval(duration)
    margin          = 0.001d0

    dpORG%beta = beta
    dpORG%alpha => alphaORG
    dpORG%rho => inrhot
    dpORG%duration => durationORG
    dpOut%alpha => alpha
    dpOut%duration => duration
    call upscaleToLargestBlock(dpORG, maxduration, dpOut)
    beta = dpOut%beta

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
    real(kind=wp), target :: alpha(5)               !< Influence coefficients of a single element
    real(kind=wp), target :: alphaORG(5)            !< Influence coefficients of a single element
    real(kind=wp), target :: duration(5)            !< Block duration of a single element
    real(kind=wp), target :: durationORG(5)         !< Block duration of a single time element
    real(kind=wp) :: maxduration            !< Maximum block duration
    real(kind=wp), target :: inrhot(5)              !< Autocorrelation coefficients for each of the variables, in time
    real(kind=wp)     :: Pf                     !< Failure probability of a single element
    real(kind=wp)     :: tmp                    !< temp placeholder for the unused non-exceedance probability
    real(kind=wp)     :: betaExpectedResult     !< Reliability index for the combined elements
    integer           :: ierr                   !< error code
    character(len=64) :: errorText              !< error message
    type(designPoint) :: dpORG, dpOut
!
!   ASSIGN VARIABLES
!
    beta            = 3.5d0
    alpha           = [ 0.0d0, 0.6d0, 0.0d0, 0.8d0, 0.0d0 ]           !alpha values
    duration        = [ 20.0d0, 2.0d0, 1.0d0, 2.0d0, 10.0d0 ]         !individual block durations
    inrhot          = [ 0.0d0, 0.0d0, 0.0d0, 0.0d0, 0.0d0 ]           !all rho values set to one

    betaORG         = beta                                              !Copy of the original beta value
    alphaORG        = alpha                                             !Copy of the original influence coefficients
    durationORG     = duration                                          !Copy of the original durations
    maxduration     = maxval(duration)
    margin          = 0.001d0

    call PQfromBeta( beta, tmp, Pf )
    call BetaFromQ ( 1.D0-tmp**10.0d0, betaExpectedResult, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    dpORG%beta = beta
    dpORG%alpha => alphaORG
    dpORG%rho => inrhot
    dpORG%duration => durationORG
    dpOut%alpha => alpha
    dpOut%duration => duration
    call upscaleToLargestBlock(dpORG, maxduration, dpOut)
    beta = dpOut%beta

    call assert_comparable(beta , betaExpectedResult, margin, &
        'The upscaled beta should agree with the one that can be derived analytically ' // &
        'for a series of 10 identical and uncorrelated single elements')
    call assert_comparable(alpha, alphaORG          , margin, &
        'The upscaled alpha should be the same as for a single element when the rho values of all variables are equal to zero')

end subroutine upscaleToLargestBlockTests3

! test identical to upscaleToLargestBlockTests3, but data is in a multi dimensional array
subroutine upscaleToLargestBlockTests4
!
!   INPUT/OUTPUT VARIABLES
!
    integer, parameter         :: nrStochasts = 5       !< number of stochasts
    real(kind=wp)              :: margin                !< acceptable margin for difference between beta original and beta computed
    real(kind=wp)              :: beta                  !< Reliability index of a single element
    real(kind=wp)              :: betaORG               !< Reliability index of a single element
    real(kind=wp), target              :: alpha(nrStochasts)    !< Influence coefficients of a single element
    real(kind=wp), allocatable, target :: alphaORG(:,:)         !< Influence coefficients of a single element
    real(kind=wp), target              :: duration(nrStochasts) !< Block duration of a single element
    real(kind=wp), allocatable, target :: durationORG(:,:)      !< Block duration of a single time element
    real(kind=wp)                      :: maxduration           !< Maximum block duration
    real(kind=wp), target              :: inrhot(5)             !< Autocorrelation coefficients for each of the variables, in time
    real(kind=wp)              :: Pf                    !< Failure probability of a single element
    real(kind=wp)              :: tmp                   !< temp placeholder for the unused non-exceedance probability
    real(kind=wp)              :: betaExpectedResult    !< Reliability index for the combined elements
    integer                    :: i                     !< loop counter
    integer                    :: ierr                  !< error code
    character(len=64)          :: errorText             !< error message
    integer, parameter         :: nrWindDirs = 16       !< number of wind directions
    type(designPoint)          :: dpORG, dpOut
!
!   ASSIGN VARIABLES
!
    beta            = 3.5d0
    alpha           = [ 0.0d0, 0.6d0, 0.0d0, 0.8d0, 0.0d0 ]           !alpha values
    duration        = [ 20.0d0, 2.0d0, 1.0d0, 2.0d0, 10.0d0 ]         !individual block durations
    inrhot          = [ 0.0d0, 0.0d0, 0.0d0, 0.0d0, 0.0d0 ]           !all rho values set to one

    allocate(alphaORG(nrWindDirs, nrStochasts))
    allocate(durationORG(nrWindDirs, nrStochasts))
    betaORG         = beta                                              !Copy of the original beta value
    do i = 1, nrWindDirs
        alphaORG(i,:)    = alpha                                             !Copy of the original influence coefficients
        durationORG(i,:) = duration                                          !Copy of the original durations
    end do
    maxduration     = maxval(duration)
    margin          = 0.001d0

    call PQfromBeta( beta, tmp, Pf )
    call BetaFromQ ( 1.D0-tmp**10.0d0, betaExpectedResult, ierr, errorText )
    call assert_equal( ierr, 0, errorText)

    do i = 1, nrWindDirs
        dpORG%beta = betaORG
        dpORG%alpha => alphaORG(i,:)
        dpORG%rho => inrhot
        dpORG%duration => durationORG(i, :)
        dpOut%alpha => alpha
        dpOut%duration => duration
        call upscaleToLargestBlock(dpORG, maxduration, dpOut)
        beta = dpOut%beta

        call assert_comparable(beta , betaExpectedResult, margin, &
            'The upscaled beta should agree with the one that can be derived analytically ' // &
            'for a series of 10 identical and uncorrelated single elements')
        call assert_comparable(alpha, alphaORG(i,:)          , margin, &
            'The upscaled alpha should be the same as for a single element when the rho values of all variables are equal to zero')
    end do

end subroutine upscaleToLargestBlockTests4

end module upscalingTests
