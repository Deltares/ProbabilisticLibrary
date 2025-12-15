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
!! Main program for running the unit tests for probabilistic functions
!<
!
module probabilisticTests

    use ftnunit
    use distributionFunctionsTests
    use probMethodsWaartsFunctionsTests
    use performImportanceSamplingTests
    use performAdaptiveMCIStests
    use performLatinHyperCubeTests
    use performNumericalBisectionTests
    use performCobylaReliabilityTests
    use performSubSetSimulationReliabilityTests
    use performDSTests
    use upscaleLengthTests
    use initProbabilisticTests
    use incompleteGammaFunctionTests
    use basicCorrelationTests
    implicit none

    private

    public :: allProbabilisticTests

contains

!> Routine to start the testing
!! Note:
!!     This routine merely takes care that the unit tests
!!     are indeed run
subroutine allProbabilisticTests

    call allInitProbabilisticTests
    call allDistributionFunctionsTests
    call allPerformImportanceSamplingTests
    call allPerformAdaptiveMCIStests
    call allPerformLatinHyperCubeTests
    call allPerformNumericalBisectionTests
    call allPerformCobylaReliabilityTests
    call allPerformSubSetSimulationReliabilityTests
    call performAllDSTests
    call allUpscaleLengthTests

end subroutine allProbabilisticTests



end module probabilisticTests
