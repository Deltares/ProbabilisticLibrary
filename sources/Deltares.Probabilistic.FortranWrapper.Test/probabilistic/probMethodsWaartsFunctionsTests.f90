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
!! Test for the computation of mechanisms
!<
!
module probMethodsWaartsFunctionsTests

    use precision, only : wp
    use interface_probCalc, only : probabilisticDataStructure_data
    use interface_probCalcData, only : initProbabilisticCalculation, setStochasticDistrib

    implicit none

    private

    public :: initializeCalculation, initializeStochast

contains

!> Initializes calculation
subroutine initializeCalculation (probDb, mvar, alfa, x)

    type(probabilisticDataStructure_data)          :: probDb
    integer,       intent(in)   :: mvar
    real (kind = wp), pointer   :: alfa(:)
    real (kind = wp), pointer   :: x(:)

    allocate ( alfa(mvar) )
    allocate ( x(mvar) )

    call initProbabilisticCalculation ( probDb, mvar, .false., .false. )

    !
    ! Seeding option for MonteCarlo and Directional Sampling: use fixed seed
    !
    probDb%method%CMC%seedPRNG = 1234
    probDb%method%DS%seedPRNG = 1234

end subroutine initializeCalculation

!> Initializes stochast
subroutine initializeStochast (probDb, stnr, sttype, par1, par2, par3, par4)

    type(probabilisticDataStructure_data)           :: probDb
    integer,       intent(in)    :: stnr
    integer,       intent(in)    :: sttype
    real(kind=wp), intent(in)    :: par1
    real(kind=wp), intent(in)    :: par2
    real(kind=wp), intent(in)    :: par3
    real(kind=wp), intent(in)    :: par4

    call setStochasticDistrib( probDb, stnr, sttype, par1, par2, par3, par4 )

end subroutine initializeStochast

end module probMethodsWaartsFunctionsTests
