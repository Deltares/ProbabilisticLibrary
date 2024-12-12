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
module sparseWaartsTestFunctions
    use precision
    use interface_ProbCalc
    use class_ProbCalc
    use waartsFunctions

    implicit none

    private
    public :: initSparseWaartsTestsFunctions, cleanUpWaartsTestsFunctions, &
                updateCounter, &
                probCalc, &
                zLimitState25QuadraticTermsSparse, zOblateSpheroid, &
                zLinearResistanceSolicitation, simpleZ, blighZ

    type(tProbCalc)                    :: probCalc
    integer,       allocatable, target :: counter(:)
    real(kind=wp), allocatable, target :: xFull(:,:)

contains

subroutine initSparseWaartsTestsFunctions(nstoch, nthreads)
    integer, intent(in) :: nstoch, nthreads
    allocate(xFull(nstoch, nthreads), counter(nThreads))
    xFull = 0.0_wp
    counter = 0
end subroutine initSparseWaartsTestsFunctions

subroutine updateCounter(invocationCount)
    integer, intent(inout) :: invocationCount

    invocationCount = invocationCount + sum(counter)
end subroutine updateCounter

subroutine cleanUpWaartsTestsFunctions
    deallocate(xFull, counter)
end subroutine cleanUpWaartsTestsFunctions

!> Limit state function with 25 quadratic terms sparse with generic interface
function zLimitState25QuadraticTermsSparse( xDense, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: xDense(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    real(kind=wp), pointer                  :: x(:)
    integer      , pointer                  :: invocationCount

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    x => xFull(:, compSetting%threadId+1)
    invocationCount => counter(compSetting%threadId+1)
    call probCalc%cpData%copyDense2Full(xDense, x)

    z = limitState25QuadraticTerms( x ( 30 ), x ( 3 : 27) )

    invocationCount = invocationCount + 1

end function zLimitState25QuadraticTermsSparse

!> Oblate spheroid with generic interface
function zOblateSpheroid( xDense, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: xDense(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    real(kind=wp), pointer                  :: x(:)
    integer      , pointer                  :: invocationCount

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    x => xFull(:, compSetting%threadId+1)
    invocationCount => counter(compSetting%threadId+1)

    call probCalc%cpData%copyDense2full(xDense, x)
    z = oblateSpheroid( x ( 1 ), x ( 2 : 11 )  )

    invocationCount = invocationCount + 1

end function zOblateSpheroid

!> Linear resistance solicitation function with generic interface
function zLinearResistanceSolicitation( xDense,  compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: xDense(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    real(kind=wp), pointer                  :: x(:)
    integer      , pointer                  :: invocationCount

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    x => xFull(:, compSetting%threadId+1)
    invocationCount => counter(compSetting%threadId+1)

    call probCalc%cpData%copyDense2Full(xDense, x)
    z = linearResistanceSolicitation( x(1), x(2) )

    invocationCount = invocationCount + 1

end function zLinearResistanceSolicitation

!> Simple z function: valueB - u1 - u2, where u1 and u2 are standard normally distributed, and where beta = 4
function simpleZ( xDense, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: xDense(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    real(kind=wp), pointer                  :: x(:)
    integer      , pointer                  :: invocationCount

    ierr%iCode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    x => xFull(:, compSetting%threadId+1)
    invocationCount => counter(compSetting%threadId+1)

    call probCalc%cpData%copyDense2Full(xDense, x)
    z = simpleSystem ( x(1),  x(2), x(3))

    invocationCount = invocationCount + 1

end function simpleZ

function simpleSystem( x1, x2, valueB )

    real (kind=wp), intent(in)   :: x1               !< input parameter x1
    real (kind=wp), intent(in)   :: x2               !< input parameter x2
    real (kind=wp), intent(in)   :: valueB           !< input parameter valueB
    real (kind=wp)               :: simpleSystem     !< output parameter

    simpleSystem = valueB - x1 - x2

end function simpleSystem

function blighZ( xDense, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: xDense(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    real(kind=wp), pointer                  :: x(:)
    integer      , pointer                  :: invocationCount

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    x => xFull(:, compSetting%threadId+1)
    invocationCount => counter(compSetting%threadId+1)

    call probCalc%cpData%copyDense2Full(xDense, x)
    z = x(1) * x(2) / x (3) - x(4)

    invocationCount = invocationCount + 1

end function blighZ

end module sparseWaartsTestFunctions

