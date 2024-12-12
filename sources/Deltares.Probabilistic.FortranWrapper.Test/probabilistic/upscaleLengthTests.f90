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

!> @file upscaleLengthTests.f90
!! A set of tests for upscaling probabilities in time, using subroutines in module upscaling within probabilistic \n
!<
!
module upscaleLengthTests

    use ftnunit
    use precision
    use feedback
    use interface_combin
    use interface_distributions
    use interface_probCalc
    use interface_convert
    use vectorUtilities
    use combineElementsTests, only : upscaleLengthWrapper

    implicit none

    private
    public :: allUpscaleLengthTests
    real(kind=wp), parameter :: pi =  4.0_wp * atan(1.0_wp)

contains

!> This subroutine contains tests of spatial upscaling from a cross section to a segment. \n
!! \n
!! A set of tests, checking the subroutine upscaleLength \n
!
subroutine allUpscaleLengthTests
!
   call testWithLevel( upscaleLengthTests11, &
       "upscaleLengthTests: 11 Test length effect in upscaling from cross section to section", 1)
   call testWithLevel( upscaleLengthTests16, &
       "upscaleLengthTests: 16 Test length effect in upscaling from cross section to section", 1)

end subroutine allUpscaleLengthTests


!> Testing the upscaling from a cross section to a section. \n
!! Here test for subroutine upscaleLength(). \n
!! Test with rhoZ= 0.8, and breach length not prescribed. \n
subroutine upscaleLengthTests11

    integer, parameter   :: nStoch= 5                     !< Number of stochastic variables
    integer              :: iStoch                        !< Entry for the alpha() vector
    real( kind= wp)      :: betaCrossSection              !< Reliability index of a cross section
    real( kind= wp)      :: betaSection                   !< Reliability index of the section after upscaling
    real( kind= wp)      :: expectedBetaSection           !< Expected reliability index of the section after upscaling
    real( kind= wp)      :: alphaCrossSection( nStoch)    !< Alpha vector cross section
    real( kind= wp)      :: alphaSection( nStoch)         !< Alpha vector for the section after upscaling
    real( kind= wp)      :: expectedAlphaSection( nStoch) !< Alpha vector that is expected for the section after upscaling
    real( kind= wp)      :: rhoXK( nStoch)                !< Correlation variables
    real( kind= wp)      :: dXK( nStoch)                  !< Correlation length variables
    real( kind= wp)      :: sectionLength                 !< Length of the section
    real( kind= wp)      :: myMargin                      !< Acceptable margin for difference between expected and computed beta
    character( len= 132) :: errorText                     !< character string for diagnostic message in case of test failure

!   Initialisation section and cross section parameters
    betaCrossSection   = 5.0d0
    alphaCrossSection  = (/ 0.0d0, 0.6d0, 0.0d0, -0.8d0, 0.0d0 /)
    rhoXK              = 0.8d0
    dXK                = 200.0d0
    sectionLength      = 250.0d0
    myMargin           = 1.0d-6

    call upscaleLengthWrapper( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection)
!   No breach length provided. In upscaleLength() it will be set to 158.533091904240
!   The expected value of beta is taken from a previous calculation; this test must ensure that the code does not change the result
    expectedBetaSection= 4.9094794340831d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected')
!
!   The expected value of alpha() is the same as the one for the cross section because of uniform rhoXK and dXK
    expectedAlphaSection= alphaCrossSection
    do iStoch= 1, nStoch
       write( errorText, '(a, i0, a)')                                           &
             "An unexpected value is found for design point entry alpha(", iStoch, ") after upscaling"
       call assert_comparable( alphaSection( iStoch), expectedAlphaSection( iStoch), myMargin, errorText)
    end do
end subroutine upscaleLengthTests11

!> Testing the upscaling from a cross section to a section. \n
!! alphaSection should match alphaCrossSection
!! for sectionLength < deltaL=dz/betaCrossSection*sqrt(pi)/(1-rho_z)
subroutine upscaleLengthTests16
    !use vectorUtilities
    integer, parameter   :: nStoch= 5                     !< Number of stochastic variables
    integer              :: iStoch                        !< Entry for the alpha() vector
    real( kind= wp)      :: betaCrossSection              !< Reliability index of a cross section
    real( kind= wp)      :: betaSection                   !< Reliability index of the section after upscaling
    real( kind= wp)      :: alphaCrossSection( nStoch)    !< Alpha vector cross section
    real( kind= wp)      :: alphaSection( nStoch)         !< Alpha vector for the section after upscaling
    real( kind= wp)      :: rhoXK( nStoch)                !< Correlation variables
    real( kind= wp)      :: dXK( nStoch)                  !< Correlation length variables
    real( kind= wp)      :: myMargin                      !< Acceptable margin for difference between expected and computed beta
    character( len= 132) :: errorText                     !< character string for diagnostic message in case of test failure
    real( kind= wp), dimension(5) :: sectionLengths = (/100.d0,150.d0,170.d0,175.d0,177.d0/)  !< test for various lengths, all below 177.245, which is the calculated deltaL   
    integer              :: iLength


!   Initialisation section and cross section parameters
    betaCrossSection   = 5.0d0
    alphaCrossSection  = (/   1.0d0,  -2.0d0,  4.0d0,   0.0d0,  -3.0d0 /)
    alphaCrossSection  = normalize( alphaCrossSection)
    rhoXK              = (/   0.8d0,   0.0d0,  1.0d0,   0.5d0,   1.0d0 /)
    dXK                = (/ 100.0d0, 200.0d0, 50.0d0, 150.0d0, 300.0d0 /)
    myMargin           = 1.0d-6

    do iLength=1,size(sectionLengths)
       call upscaleLengthWrapper( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLengths(iLength), &
           betaSection, alphaSection)
!
!      The expected value of alphaSection() is alphaCrossSection for the given length
       do iStoch= 1, nStoch
          write( errorText, '(a, i0, a)')                                           &
             "alphaSection differs from alphaCrossSection for design point entry alpha(", iStoch, ") after upscaling"
          call assert_comparable( alphaSection( iStoch), alphaCrossSection( iStoch), myMargin, errorText)
       end do
    end do
end subroutine upscaleLengthTests16


!> Alternative method for the computation of beta in upscalelength from cross section to section \n
!! Semi analytical approach through a series expansion. \n
!! Can be used instead of the presently used numerical integration method in ComputeBetaSection() \n
subroutine ComputeBetaSectionThroughSeriesExpansion( betaCrossSection, sectionLength, breachL, rhoZ, dz, betaSection)

    real( kind= wp), intent(in)  :: betaCrossSection     !< Reliability index of the cross section
    real( kind= wp), intent(in)  :: sectionLength        !< Length of the section
    real( kind= wp), intent(in)  :: breachL              !< Breach length
    real( kind= wp), intent(in)  :: rhoZ                 !< Correlation Z-function
    real( kind= wp), intent(in)  :: dz                   !< Correlation length
    real( kind= wp), intent(out) :: betaSection          !< Reliability index of the section after upscaling

!   Auxiliary parameters used in the computation         :
    integer        , parameter   :: kMax   = 1001        ! Maximum number of terms of the series that are taken into account
    integer                      :: k                    ! Loop index in series expansion
    real( kind= wp), parameter   :: epsilon= 1.0d-30     ! Tolerance for truncation in series expansion
    real( kind= wp)              :: p                    ! Non-failure probability of the cross section
    real( kind= wp)              :: pf                   ! Failure probability of the cross section
    real( kind= wp)              :: pfX                  ! Failure probability of the section
    real( kind= wp)              :: deltaPfX             ! increment for pfX in series expansion
    real( kind= wp)              :: a                    ! sqrt( 1.0d0- rhoZ)
    real( kind= wp)              :: b                    !-sqrt( rhoZ)     / sqrt( 1.0d0- rhoZ)
    real( kind= wp)              :: c                    ! max( ( sectionLength - breachL ), 0.D0) /( sqrt( 2.d0)* pi  ) / dz
    real( kind= wp)              :: dk                   ! double precision of integer loop index k
    real( kind= wp)              :: factor               ! Multiplication factor in the series expansion
    integer                      :: ierr                 ! error code
    character(len=64)            :: errorText            ! error message

!   Failure/NonFailure probabilities of the cross section:
    p     = PFromBeta( betaCrossSection )
    pf    = QFromBeta( betaCrossSection )

!   Initialisation of constants in the computation:
    a     = dsqrt( 1.0d0- rhoZ)
    c     = max( ( sectionLength - breachL ), 0.D0) /( sqrt( 2.d0)* pi  ) / dz

!   Initialisation on entry to the first term (i.e. for k= 1) in the series expansion
    pfX   = 0.0d0
    factor= 1.0d0
!   Loop in the series expansion:
    summationKSeries: do k= 1, kMax     ! below the term for k=0 is separately dealt with
        dk      = dble( k )
        factor  =-factor* c   /   dk
        b       = 1.0d0 + rhoZ* ( dk   - 1.0d+0)
        deltaPfX= factor* a* exp(-0.5d0* dk* betaCrossSection* betaCrossSection/ b) / sqrt( b)
        pfX     = pfX   + deltaPfX
        if( dabs( deltaPfX)< epsilon) Exit summationKSeries
    enddo summationKSeries

!   Add the k=0 term to pfX:
    pfX = 1.0d0+ pfX
!   Exceedence probability:
    pfX = 1.0d0- p   * pfX
    pfX = max( pfX   , pf)
!   Compute corresponding beta
    call betaFromQ( pfX, betaSection, ierr, errorText)
    call assert_equal( ierr, 0, errorText)

end subroutine ComputeBetaSectionThroughSeriesExpansion

end module upscaleLengthTests
