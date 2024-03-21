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

!> @file
!! Main program for running the unit tests for probabilistic functions
!<
!
module probabilisticTests

    use ftnunit
    use distributionFunctionsTests
    use probMethodsWaartsFunctionsTests
    use randomGeneratorTests
    use waartsFunctionsTests
    use performImportanceSamplingTests
    use performAdaptiveMCIStests
    use HohenbichlerTests
    use combineElementsTests
    use performDSTests
    use upscalingTests
    use upscaleLengthTests
    use initProbabilisticTests
    use incompleteGammaFunctionTests
    use basicCorrelationTests
    implicit none

    private

    public :: allProbabilisticTests, allProbabilisticIntegrationTests

contains

!> Routine to start the testing
!! Note:
!!     This routine merely takes care that the unit tests
!!     are indeed run
subroutine allProbabilisticTests

    call allInitProbabilisticTests
    call allRandomGeneratorTests
    !call allDistributionFunctionsTests
    call allWaartsFunctionsTests
    call allProbMethodsWaartsFunctionsNoIterationTest
    call allHohenbichlerTests
    call allCombineElementsTests
    !call allPerformImportanceSamplingTests
    !call allPerformAdaptiveMCIStests
    call performAllDSTests
    call allUpscalingTests
    call allUpscaleLengthTests
    !call allIncompleteGammaTests
    !call allBasicCorrelationTests

end subroutine allProbabilisticTests


subroutine allProbabilisticIntegrationTests

    methodDS = 1
    call runtests( allProbMethodsWaartsFunctionsIterationTest )

    methodDS = 2
    call runtests( allProbMethodsWaartsFunctionsIterationTest)

    methodDS = 3
    call runtests( allProbMethodsWaartsFunctionsIterationTest)

    methodDS = 1
end subroutine allProbabilisticIntegrationTests

end module probabilisticTests
