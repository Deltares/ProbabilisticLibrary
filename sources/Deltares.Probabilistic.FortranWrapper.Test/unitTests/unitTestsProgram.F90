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
!! Main program for running the unit tests for the Probabilistic library
!<
!
program unitTestsProgram

    use ftnunit
    use initTestSubs
    use conversionFunctionsTests
    use probabilisticTests
    use interface_probCalcData

    use, intrinsic :: ieee_exceptions

    implicit none

    integer :: maxNumberIterations ! not used any more, but needed to keep interface the same
    integer :: testLevel
    character(len=16) :: testSet

    ! Enable following line to catch division by zero
    ! call ieee_set_halting_mode( IEEE_DIVIDE_BY_ZERO, .true. )

#ifdef _WIN32
    call setTestTitle("Fortran tests using c++ ProbLib on Windows")
#else
    call setTestTitle("Fortran tests using c++ ProbLib on Linux")
#endif
    call prepareTests

    call runtests_init
    testLevel = 3 ! should be 3
    testSet = 'all'
    call readLevel(testset, testLevel, maxNumberIterations, curParallelThreads)
    call setRunTestLevel(testLevel)

    if (testSet == 'all' .or. testSet == 'conv') then
        call allBetaFromQTests

        call allPFromBetaTests

        call allPQFromBetaTests

        call allQFromBetaTests

        !call allReturnTimeFreqTests
    end if

    if (testSet == 'all' .or. testSet == 'prob') then
        call allProbabilisticTests
    end if

    call runtests_final(.true.)

contains

subroutine readLevel(testSet, testLevel, repeat, numcores)
    character(len=*), intent(inout) :: testSet
    integer, intent(inout) :: testLevel, repeat, numcores

    integer :: i, ierr, args(3), status
    character(len=128) :: arg, value

    if ( command_argument_count() > 0 ) then
        args = [testLevel, repeat, 0]
        do i = 1, command_argument_count()
            call get_command_argument( i, arg, status = ierr )
            if (ierr /= 0) exit
            if (arg == '/s') cycle
            if (i == 1) testSet = arg
            if (i >  1) read(arg, *, iostat=ierr) args(i-1)  ! try to parse argument as number
        enddo
        testLevel = args(1)
        repeat    = args(2)
        if (args(3) > 0) then
            numcores  = args(3)
        else
            call get_environment_variable("OMP_NUM_THREADS", value=value, status=status)
            if (status == 0) then
                read(value,*) numcores
            end if
        end if
    endif
end subroutine readLevel

end program unitTestsProgram
