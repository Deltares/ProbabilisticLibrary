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
!   Testing upscaleLength: length effect in upscaling from cross section to a section
    call testWithLevel( upscaleLengthTests1, &
        "upscaleLengthTests:  1 Test length effect in upscaling from cross section to section", 1)
    call testWithLevel( upscaleLengthTests2, &
        "upscaleLengthTests:  2 Test length effect in upscaling from cross section to section", 1)
    call testWithLevel( upscaleLengthTests3, &
        "upscaleLengthTests:  3 Test length effect in upscaling from cross section to section", 1)
    call testWithLevel( upscaleLengthTests4, &
        "upscaleLengthTests:  4 Test length effect in upscaling from cross section to section", 1)
    call testWithLevel( upscaleLengthTests5, &
        "upscaleLengthTests:  5 Test length effect in upscaling from cross section to section", 1)

!AM: failing with CodeCoverage?
   call testWithLevel( upscaleLengthTests6, &
       "upscaleLengthTests:  6 Test length effect in upscaling from cross section to section", 1)
   call testWithLevel( upscaleLengthTests7, &
       "upscaleLengthTests:  7 Test length effect in upscaling from cross section to section", 1)
   call testWithLevel( upscaleLengthTests8, &
       "upscaleLengthTests:  8 Test length effect in upscaling from cross section to section", 1)
   call testWithLevel( upscaleLengthTests9, &
       "upscaleLengthTests:  9 Test length effect in upscaling from cross section to section", 1)
!
   call testWithLevel( upscaleLengthTests10, &
       "upscaleLengthTests: 10 Test length effect in upscaling from cross section to section", 1)
   call testWithLevel( upscaleLengthTests11, &
       "upscaleLengthTests: 11 Test length effect in upscaling from cross section to section", 1)
   call testWithLevel( upscaleLengthTests12, &
       "upscaleLengthTests: 12 Test length effect in upscaling from cross section to section", 1)
   call testWithLevel( upscaleLengthTests13, &
       "upscaleLengthTests: 13 Test length effect in upscaling from cross section to section", 1)
   call testWithLevel( upscaleLengthTests14, &
       "upscaleLengthTests: 14 Test length effect in upscaling from cross section to section", 1)
   call testWithLevel( upscaleLengthTests15, &
       "upscaleLengthTests: 15 Test length effect in upscaling from cross section to section", 1)
   call testWithLevel( upscaleLengthTests16, &
       "upscaleLengthTests: 16 Test length effect in upscaling from cross section to section", 1)

end subroutine allUpscaleLengthTests

!> Testing the upscaling from a cross section to a section. \n
!! Here the part governed by subroutine ComputeBetaSection(). \n
!! BetaCrossSection= 5, sectionLength= 250, breach length= 250, rhoZ= 0.8, corr. length dz= 125. \n
!! The resulting betaSection is compared to a precomputed result.\n
subroutine upscaleLengthTests1

    real( kind= wp)  :: betaCrossSection       !< Reliability index of a cross section
    real( kind= wp)  :: sectionLength          !< Length of the section
    real( kind= wp)  :: breachL                !< Breach Length
    real( kind= wp)  :: rhoZ                   !< Correlation Z-function
    real( kind= wp)  :: dz                     !< Correlation length
    real( kind= wp)  :: deltaL                 !< Delta L (used by Hohenbichler if rhoZ <= 0.001D0)
    real( kind= wp)  :: betaSection            !< Reliability index of the section after upscaling
    real( kind= wp)  :: expectedBetaSection    !< expected reliability index for the combination
    real( kind= wp)  :: myMargin               !< acceptable margin for difference between beta expected and beta computed

    betaCrossSection= 5.0
    sectionLength   = 250.0d0
    breachL         = 250.0d0
    rhoZ            = 0.8d0
    dz              = 125.0d0
!   Compute deltaL  ( only used by Hohenbichler when rhoZ<=0.001)
    deltaL          = min( dz / betaCrossSection * sqrt( pi ) / sqrt( 1.D0 - rhoZ ), sectionLength)
    deltaL          = max( deltaL, 0.01D0 )
    myMargin        = 1.0d-6

!   Compute the beta for the section:
    call ComputeBetaSection( betaCrossSection, sectionLength, breachL, rhoZ, dz, deltaL, betaSection)
!   The expected beta must be the beta of the cross section because breach length is greater/equal to length of section
    expectedBetaSection= betaCrossSection
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected' )

end subroutine upscaleLengthTests1

!> Testing the upscaling from a cross section to a section. \n
!! Here the part governed by subroutine ComputeBetaSection(). \n
!! BetaCrossSection= 5, sectionLength= 250, breach length= 200 , rhoZ= 0.8, corr. length dz= 125. \n
!! The resulting betaSection is compared to a precomputed result.\n
subroutine upscaleLengthTests2

    real( kind= wp)  :: betaCrossSection       !< Reliability index of a cross section
    real( kind= wp)  :: sectionLength          !< Length of the section
    real( kind= wp)  :: breachL                !< Breach Length
    real( kind= wp)  :: rhoZ                   !< Correlation Z-function
    real( kind= wp)  :: dz                     !< Correlation length
    real( kind= wp)  :: deltaL                 !< Delta L (used by Hohenbichler if rhoZ <= 0.001D0)
    real( kind= wp)  :: betaSection            !< Reliability index of the section after upscaling
    real( kind= wp)  :: expectedBetaSection    !< expected reliability index for the combination
    real( kind= wp)  :: myMargin               !< acceptable margin for difference between beta expected and beta computed

    betaCrossSection= 5.0
    sectionLength   = 250.0d0
    breachL         = 200.0d0
    rhoZ            = 0.8d0
    dz              = 125.0d0
!   Compute deltaL  ( only used by Hohenbichler when rhoZ<=0.001)
    deltaL          = min( dz / betaCrossSection * sqrt( pi ) / sqrt( 1.D0 - rhoZ ), sectionLength)
    deltaL          = max( deltaL, 0.01D0 )
    myMargin        = 1.0d-6

!   Compute the beta for the section:
    call ComputeBetaSection( betaCrossSection, sectionLength, breachL, rhoZ, dz, deltaL, betaSection)
!   Set the expected beta to a precomputed value:
    expectedBetaSection= 4.91876404645842d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected' )

end subroutine upscaleLengthTests2

!> Testing the upscaling from a cross section to a section. \n
!! Here the part governed by subroutine ComputeBetaSection(). \n
!! BetaCrossSection= 5, sectionLength= 250, breach length= 50, rhoZ= 0.8, corr. length dz= 125. \n
!! The resulting betaSection is compared to a precomputed result.\n
subroutine upscaleLengthTests3

    real( kind= wp)  :: betaCrossSection       !< Reliability index of a cross section
    real( kind= wp)  :: sectionLength          !< Length of the section
    real( kind= wp)  :: breachL                !< Breach Length
    real( kind= wp)  :: rhoZ                   !< Correlation Z-function
    real( kind= wp)  :: dz                     !< Correlation length
    real( kind= wp)  :: deltaL                 !< Delta L (used by Hohenbichler if rhoZ <= 0.001D0)
    real( kind= wp)  :: betaSection            !< Reliability index of the section after upscaling
    real( kind= wp)  :: expectedBetaSection    !< expected reliability index for the combination
    real( kind= wp)  :: myMargin               !< acceptable margin for difference between beta expected and beta computed

    betaCrossSection= 5.0
    sectionLength   = 250.0d0
    breachL         = 50.0d0
    rhoZ            = 0.8d0
    dz              = 125.0d0
!   Compute deltaL  ( only used by Hohenbichler when rhoZ<=0.001)
    deltaL          = min( dz / betaCrossSection * sqrt( pi ) / sqrt( 1.D0 - rhoZ ), sectionLength)
    deltaL          = max( deltaL, 0.01D0 )
    myMargin        = 1.0d-6

!   Compute the beta for the section:
    call ComputeBetaSection( betaCrossSection, sectionLength, breachL, rhoZ, dz, deltaL, betaSection)
!   Set the expected beta to a precomputed value:
    expectedBetaSection= 4.78198440553261d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected' )

end subroutine upscaleLengthTests3

!> Testing the upscaling from a cross section to a section. \n
!! Here the part governed by subroutine ComputeBetaSection(). \n
!! BetaCrossSection= 5, sectionLength= 250, breach length= 0, rhoZ= 0.8, corr. length dz= 125. \n
!! The resulting betaSection is compared to a precomputed result.\n
subroutine upscaleLengthTests4

    real( kind= wp)  :: betaCrossSection       !< Reliability index of a cross section
    real( kind= wp)  :: sectionLength          !< Length of the section
    real( kind= wp)  :: breachL                !< Breach Length
    real( kind= wp)  :: rhoZ                   !< Correlation Z-function
    real( kind= wp)  :: dz                     !< Correlation length
    real( kind= wp)  :: deltaL                 !< Delta L (used by Hohenbichler if rhoZ <= 0.001D0)
    real( kind= wp)  :: betaSection            !< Reliability index of the section after upscaling
    real( kind= wp)  :: expectedBetaSection    !< expected reliability index for the combination
    real( kind= wp)  :: myMargin               !< acceptable margin for difference between beta expected and beta computed

    betaCrossSection= 5.0
    sectionLength   = 250.0d0
    breachL         = 0.0d0
    rhoZ            = 0.8d0
    dz              = 125.0d0
!   Compute deltaL  ( only used by Hohenbichler when rhoZ<=0.001)
    deltaL          = min( dz / betaCrossSection * sqrt( pi ) / sqrt( 1.D0 - rhoZ ), sectionLength)
    deltaL          = max( deltaL, 0.01D0 )
    myMargin        = 1.0d-6

!   Compute the beta for the section:
    call ComputeBetaSection( betaCrossSection, sectionLength, breachL, rhoZ, dz, deltaL, betaSection)
!   Set the expected beta to a precomputed value:
    expectedBetaSection= 4.75188548643029d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected' )

end subroutine upscaleLengthTests4

!> Testing the upscaling from a cross section to a section. \n
!! Here the part governed by subroutine ComputeBetaSection(). \n
!! BetaCrossSection= 5, sectionLength= 250, breach length= 0, rhoZ= 0.00100001, corr. length dz= 125. \n
!! In this setting rhoZ is just above the limit where Hohenbichler is used within upscaleLength(). \n
!! In this case not Hohenbichler is not used but a numerical integration technique. \n
!! The resulting betaSection is compared to a precomputed result.\n
subroutine upscaleLengthTests5

    real( kind= wp)  :: betaCrossSection       !< Reliability index of a cross section
    real( kind= wp)  :: sectionLength          !< Length of the section
    real( kind= wp)  :: breachL                !< Breach Length
    real( kind= wp)  :: rhoZ                   !< Correlation Z-function
    real( kind= wp)  :: dz                     !< Correlation length
    real( kind= wp)  :: deltaL                 !< Delta L (used by Hohenbichler if rhoZ <= 0.001D0)
    real( kind= wp)  :: betaSection            !< Reliability index of the section after upscaling
    real( kind= wp)  :: expectedBetaSection    !< expected reliability index for the combination
    real( kind= wp)  :: myMargin               !< acceptable margin for difference between beta expected and beta computed

    betaCrossSection= 5.0
    sectionLength   = 250.0d0
    breachL         = 0.0d0
    rhoZ            = 0.00100001d0             ! Just above the limit of rhoZ=0.001 where Hohenbichler is used
    dz              = 125.0d0
!   Compute deltaL  ( only used by Hohenbichler when rhoZ<=0.001)
    deltaL          = min( dz / betaCrossSection * sqrt( pi ) / sqrt( 1.D0 - rhoZ ), sectionLength)
    deltaL          = max( deltaL, 0.01D0 )
    myMargin        = 1.0d-6

!   Compute the beta for the section:
    call ComputeBetaSection( betaCrossSection, sectionLength, breachL, rhoZ, dz, deltaL, betaSection)
!   Set the expected beta to a precomputed value:
    expectedBetaSection= 4.61522017604640d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected' )

end subroutine upscaleLengthTests5

!> Testing the upscaling from a cross section to a section. \n
!! Here the part governed by subroutine ComputeBetaSection(). \n
!! BetaCrossSection= 5, sectionLength= 250, breach length= 0, rhoZ= 0.00099999, corr. length dz= 125. \n
!! For this setting Hohenbichler is used within upscaleLength(). \n
!! The resulting betaSection is compared to a precomputed result.\n
subroutine upscaleLengthTests6

    real( kind= wp)  :: betaCrossSection       !< Reliability index of a cross section
    real( kind= wp)  :: sectionLength          !< Length of the section
    real( kind= wp)  :: breachL                !< Breach Length
    real( kind= wp)  :: rhoZ                   !< Correlation Z-function
    real( kind= wp)  :: dz                     !< Correlation length
    real( kind= wp)  :: deltaL                 !< Delta L (used by Hohenbichler if rhoZ <= 0.001D0)
    real( kind= wp)  :: betaSection            !< Reliability index of the section after upscaling
    real( kind= wp)  :: expectedBetaSection    !< expected reliability index for the combination
    real( kind= wp)  :: myMargin               !< acceptable margin for difference between beta expected and beta computed

    betaCrossSection= 5.0
    sectionLength   = 250.0d0
    breachL         = 0.0d0
    rhoZ            = 0.00099999d0             ! Just below limit of rhoZ=0.001 where Hohenbichler is used
    dz              = 125.0d0
!   Compute deltaL  ( is only used by Hohenbichler when rhoZ<=0.001)
    deltaL          = min( dz / betaCrossSection * sqrt( pi ) / sqrt( 1.D0 - rhoZ ), sectionLength)
    deltaL          = max( deltaL, 0.01D0 )
    myMargin        = 1.0d-6

!   Compute the beta for the section:
    call ComputeBetaSection( betaCrossSection, sectionLength, breachL, rhoZ, dz, deltaL, betaSection)
!   Set the expected beta to a precomputed value:
    expectedBetaSection= 4.62169347989363d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected' )

end subroutine upscaleLengthTests6

!> Testing the upscaling from a cross section to a section. \n
!! Here the part governed by subroutine ComputeBetaSection(). \n
!! BetaCrossSection= 5, sectionLength= 250, breach length= 0, rhoZ= 0.00099999, corr. length dz= 125. \n
!! For this setting Hohenbichler is used within upscaleLength to derive betaSection. \n
!! An alternative, quasi analytical method is used for finding the expected beta. \n
subroutine upscaleLengthTests7

    real( kind= wp)  :: betaCrossSection       !< Reliability index of a cross section
    real( kind= wp)  :: sectionLength          !< Length of the section
    real( kind= wp)  :: breachL                !< Breach Length
    real( kind= wp)  :: rhoZ                   !< Correlation Z-function
    real( kind= wp)  :: dz                     !< Correlation length
    real( kind= wp)  :: deltaL                 !< Delta L (used by Hohenbichler if rhoZ <= 0.001D0)
    real( kind= wp)  :: betaSection            !< Reliability index of the section after upscaling
    real( kind= wp)  :: expectedBetaSection    !< expected reliability index for the combination
    real( kind= wp)  :: myMargin               !< acceptable margin for difference between beta expected and beta computed

    betaCrossSection= 5.0
    sectionLength   = 250.0d0
    breachL         = 0.0d0
    rhoZ            = 0.00099999d0             ! Just below limit of rhoZ=0.001 where Hohenbichler is used
    dz              = 125.0d0
!   Compute deltaL  ( is only used by Hohenbichler when rhoZ<=0.001)
    deltaL          = min( dz / betaCrossSection * sqrt( pi ) / sqrt( 1.D0 - rhoZ ), sectionLength)
    deltaL          = max( deltaL, 0.01D0 )

!   Compute the beta for the section. For present rhoZ Hohenbichler will be used within upscaleLength
    call ComputeBetaSection( betaCrossSection, sectionLength, breachL, rhoZ, dz, deltaL, betaSection)
!   Compute the expected beta using a quasi analytical approach
    call ComputeBetaSectionThroughSeriesExpansion( betaCrossSection, sectionLength, breachL, rhoZ, dz, expectedBetaSection)
!   NB. This will yield             : expectedBetaSection = 4.61539109839496
!       while Hohenbichler will give: expectedBetaSection = 4.62158580512824
!       For 0.00100001                expectedBetaSection = 4.61539110017523 (not through Hohenbichler)
    myMargin        = 0.07d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected' )

end subroutine upscaleLengthTests7

!> Testing the upscaling from a cross section to a section. \n
!! Here the part governed by subroutine ComputeBetaSection(). \n
!! BetaCrossSection= 5, sectionLength= 250, breach length= 0, rhoZ= 0.8, corr. length dz= 25. \n
!! An alternative, quasi analytical method is used for finding the expected beta. \n
subroutine upscaleLengthTests8

    real( kind= wp)  :: betaCrossSection       !< Reliability index of a cross section
    real( kind= wp)  :: sectionLength          !< Length of the section
    real( kind= wp)  :: breachL                !< Breach Length
    real( kind= wp)  :: rhoZ                   !< Correlation Z-function
    real( kind= wp)  :: dz                     !< Correlation length
    real( kind= wp)  :: deltaL                 !< Delta L (used by Hohenbichler if rhoZ <= 0.001D0)
    real( kind= wp)  :: betaSection            !< Reliability index of the section after upscaling
    real( kind= wp)  :: expectedBetaSection    !< expected reliability index for the combination
    real( kind= wp)  :: myMargin               !< acceptable margin for difference between beta expected and beta computed

    betaCrossSection= 5.0
    sectionLength   = 250.0d0
    breachL         = 0.0d0
    rhoZ            = 0.8d0
    dz              = 25.0d0
!   Compute deltaL  ( is only used by Hohenbichler when rhoZ<=0.001)
    deltaL          = min( dz / betaCrossSection * sqrt( pi ) / sqrt( 1.D0 - rhoZ ), sectionLength)
    deltaL          = max( deltaL, 0.01D0 )

!   Compute the beta for the section:
    call ComputeBetaSection( betaCrossSection, sectionLength, breachL, rhoZ, dz, deltaL, betaSection)
!   NB: betaSection from this computation should be 4.49621400247016

!   Compute the expected beta using a quasi analytical approach
    call ComputeBetaSectionThroughSeriesExpansion( betaCrossSection, sectionLength, breachL, rhoZ, dz, expectedBetaSection)
!   NB: expectedBetaSection from this computation: 4.49621400247093

    myMargin        = 1.0d-6
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected' )

end subroutine upscaleLengthTests8

!> Testing the upscaling from a cross section to a section. \n
!! Here the part governed by subroutine ComputeBetaSection(). \n
!! BetaCrossSection= 5, sectionLength= 250, breach length= 0, rhoZ= 0.9999, corr. length dz= 125. \n
!! An alternative, quasi analytical method is used for finding the expected beta. \n
subroutine upscaleLengthTests9

    real( kind= wp)  :: betaCrossSection       !< Reliability index of a cross section
    real( kind= wp)  :: sectionLength          !< Length of the section
    real( kind= wp)  :: breachL                !< Breach Length
    real( kind= wp)  :: rhoZ                   !< Correlation Z-function
    real( kind= wp)  :: dz                     !< Correlation length
    real( kind= wp)  :: deltaL                 !< Delta L (used by Hohenbichler if rhoZ <= 0.001D0)
    real( kind= wp)  :: betaSection            !< Reliability index of the section after upscaling
    real( kind= wp)  :: expectedBetaSection    !< expected reliability index for the combination
    real( kind= wp)  :: myMargin               !< acceptable margin for difference between beta expected and beta computed

    betaCrossSection= 5.0
    sectionLength   = 250.0d0
    breachL         = 0.0d0
    rhoZ            = 0.9999d0
    dz              = 125.0d0
!   Compute deltaL  ( is only used by Hohenbichler when rhoZ<=0.001)
    deltaL          = min( dz / betaCrossSection * sqrt( pi ) / sqrt( 1.D0 - rhoZ ), sectionLength)
    deltaL          = max( deltaL, 0.01D0 )

!   Compute the beta for the section:
    call ComputeBetaSection( betaCrossSection, sectionLength, breachL, rhoZ, dz, deltaL, betaSection)
!   NB: betaSection from this computation should be betaSection = 4.99055000083597

!   Compute the expected beta using a quasi analytical approach
    call ComputeBetaSectionThroughSeriesExpansion( betaCrossSection, sectionLength, breachL, rhoZ, dz, expectedBetaSection)
!   NB: expectedBetaSection from this computation: expectedBetaSection = 4.99055000083405

    myMargin        = 1.0d-6
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected' )

end subroutine upscaleLengthTests9

!> Testing the upscaling from a cross section to a section. \n
!! Here test for subroutine upscaleLength(). \n
!! Test with rhoZ= 0.8, and breach length explicitly prescibed as 0. \n
subroutine upscaleLengthTests10

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
    real( kind= wp)      :: breachLength                  !< Breach Length
    real( kind= wp)      :: myMargin                      !< Acceptable margin for difference between expected and computed beta
    character( len= 132) :: errorText                     !< character string for diagnostic message in case of test failure

!   Initialisation section and cross section parameters
    betaCrossSection   = 5.0d0
    alphaCrossSection  = (/ 0.0d0, 0.6d0, 0.0d0, -0.8d0, 0.0d0 /)
    rhoXK              = 0.8d0
    dXK                = 200.0d0
    sectionLength      = 250.0d0
    breachLength       = 0.0d0
    myMargin           = 1.0d-6

    call upscaleLength( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection, breachLength)
!
!   The expected value of beta is taken from a previous calculation; this test must ensure that the code does not change the result
    expectedBetaSection= 4.81288713415857d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected')
!
!   The expected value of alpha() is the same as the one for the cross section because of uniform rhoXK and dXK
    expectedAlphaSection= alphaCrossSection
    do iStoch= 1, nStoch
       write( errorText, '(a, i0, a)')                                           &
             "An unexpected value is found for design point entry alpha(", iStoch, ") after upscaling"
       call assert_comparable( alphaSection( iStoch), expectedAlphaSection( iStoch), myMargin, errorText)
    end do
end subroutine upscaleLengthTests10

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

    call upscaleLength( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection)
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
!! Here test for subroutine upscaleLength(). \n
!! Test with rhoZ= 0.0, and breach length set to 0. \n
subroutine upscaleLengthTests12

    integer, parameter   :: nStoch= 5                     !< Number of stochastic variables
    integer              :: iStoch                        !< Entry for the alpha() vector
    real( kind= wp)      :: betaCrossSection              !< Reliability index of a cross section
    real( kind= wp)      :: betaSection                   !< Reliability index of the section after upscaling
    real( kind= wp)      :: expectedBetaSection           !< Expected reliability index of the section after upscaling
    real( kind= wp)      :: breachLength                  !< Breach Length
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
    rhoXK              = (/ 1.0d0, 0.0d0, 1.0d0,  0.0d0, 1.0d0 /)
    dXK                = 200.0d0
    sectionLength      = 250.0d0
    breachLength       = 0.0d0
    myMargin           = 1.0d-6

    call upscaleLength( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection, breachLength)
!   The expected value of beta is taken from a previous calculation; this test must ensure that the code does not change the result
    expectedBetaSection= 4.7005296730163d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected')
!
!   The expected value of alpha() is the same as the one for the cross section
    expectedAlphaSection= alphaCrossSection
    do iStoch= 1, nStoch
       write( errorText, '(a, i0, a)')                                           &
             "An unexpected value is found for design point entry alpha(", iStoch, ") after upscaling"
       call assert_comparable( alphaSection( iStoch), expectedAlphaSection( iStoch), myMargin, errorText)
    end do
end subroutine upscaleLengthTests12

!> Testing the upscaling from a cross section to a section. \n
!! Here test for subroutine upscaleLength(). \n
!! Test with rhoZ= 1, and breach length set to 0. \n
!! Uniform correlation length dXK(). \n
!! For betaSection the value betaCrossSection is expected. \n
subroutine upscaleLengthTests13

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
    real( kind= wp)      :: breachLength                  !< Breach Length
    real( kind= wp)      :: myMargin                      !< Acceptable margin for difference between expected and computed beta
    character( len= 132) :: errorText                     !< character string for diagnostic message in case of test failure

!   Initialisation section and cross section parameters
    betaCrossSection   = 5.0d0
    alphaCrossSection  = (/ 0.0d0, 0.6d0, 0.0d0, -0.8d0, 0.0d0 /)
    rhoXK              = (/ 0.0d0, 1.0d0, 0.0d0,  1.0d0, 0.0d0 /)
    dXK                = 200.0d0
    sectionLength      = 250.0d0
    breachLength       = 0.0d0
    myMargin           = 1.0d-6

    call upscaleLength( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection, breachLength)
!   For betaSection the same value as for betaCrossSection is expected because in this case rhoZ= 1
    expectedBetaSection= betaCrossSection
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected')
!
!   The expected value of alpha() is the same as the one for the cross section
    expectedAlphaSection= alphaCrossSection
    do iStoch= 1, nStoch
       write( errorText, '(a, i0, a)')                                           &
             "An unexpected value is found for design point entry alpha(", iStoch, ") after upscaling"
       call assert_comparable( alphaSection( iStoch), expectedAlphaSection( iStoch), myMargin, errorText)
    end do
end subroutine upscaleLengthTests13

!> Testing the upscaling from a cross section to a section. \n
!! Here test for subroutine upscaleLength(). \n
!! Test with rhoZ= 0.8, uniform rhoXK() and a small uniform correlation length dXK(). \n
subroutine upscaleLengthTests14

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
    real( kind= wp)      :: breachLength                  !< Breach Length
    real( kind= wp)      :: myMargin                      !< Acceptable margin for difference between expected and computed beta
    character( len= 132) :: errorText                     !< character string for diagnostic message in case of test failure

!   Initialisation section and cross section parameters
    betaCrossSection   = 5.0d0
    alphaCrossSection  = (/ 0.0d0, 0.6d0, 0.0d0, -0.8d0, 0.0d0 /)
    rhoXK              = 0.8d0
    dXK                = 10.0d0
    sectionLength      = 250.0d0
    breachLength       = 0.0d0
    myMargin           = 1.0d-6

    call upscaleLength( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection, breachLength)
!
!   The expected value of beta is taken from a previous calculation; this test must ensure that the code does not change the result
    expectedBetaSection= 4.34229408482452d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected')
!
!   The expected value of alpha() is the same as the one for the cross section because of uniform rhoXK and dXK
    expectedAlphaSection= alphaCrossSection
    do iStoch= 1, nStoch
       write( errorText, '(a, i0, a)')                                           &
             "An unexpected value is found for design point entry alpha(", iStoch, ") after upscaling"
       call assert_comparable( alphaSection( iStoch), expectedAlphaSection( iStoch), myMargin, errorText)
    end do
end subroutine upscaleLengthTests14

!> Testing the upscaling from a cross section to a section. \n
!! Here test for subroutine upscaleLength(). \n
!! Non-uniform alphaCrossSection(), Non-uniform rhoXK() and non-uniform correlation length dXK(). \n
subroutine upscaleLengthTests15
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
    real( kind= wp)      :: breachLength                  !< Breach Length
    real( kind= wp)      :: myMargin                      !< Acceptable margin for difference between expected and computed beta
    character( len= 132) :: errorText                     !< character string for diagnostic message in case of test failure

!   Initialisation section and cross section parameters
    betaCrossSection   = 5.0d0
    alphaCrossSection  = (/   1.0d0,  -2.0d0,  4.0d0,   0.0d0,  -3.0d0 /)
    alphaCrossSection  = normalize( alphaCrossSection)
    rhoXK              = (/   0.8d0,   0.0d0,  1.0d0,   0.5d0,   1.0d0 /)
    dXK                = (/ 100.0d0, 200.0d0, 50.0d0, 150.0d0, 300.0d0 /)
    sectionLength      = 250.0d0
    breachLength       = 0.0d0
    myMargin           = 1.0d-6

    call upscaleLength( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, betaSection, alphaSection, breachLength)
!
!   The expected value of beta is taken from a previous calculation; this test must ensure that the code does not change the result
    expectedBetaSection= 4.82816016866383d0
    call assert_comparable( betaSection, expectedBetaSection, myMargin, 'The upscaled beta is not as expected')
!
!   The expected value of alpha() is taken from a previous calculation; this test must ensure that the code does not change the result
    expectedAlphaSection= (/ 0.223610452110384d0, -0.466115729079332d0, 0.684799295706101d0, 0.0d0,  -0.513599471779576d0 /)
    do iStoch= 1, nStoch
       write( errorText, '(a, i0, a)')                                           &
             "An unexpected value is found for design point entry alpha(", iStoch, ") after upscaling"
       call assert_comparable( alphaSection( iStoch), expectedAlphaSection( iStoch), myMargin, errorText)
    end do
end subroutine upscaleLengthTests15



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
       call upscaleLength( betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLengths(iLength), betaSection, alphaSection)
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
