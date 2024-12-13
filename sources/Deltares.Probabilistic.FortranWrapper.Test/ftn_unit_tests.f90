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
program ftn_unit_tests
    use ftnunit
    use testTools
    use ftn_interface_tests

    integer, parameter :: testLevel = 1

    call prepareTests('unitTests')
    call setTestTitle('Fortran-C++ ProbLib interface tests')

    call setRunTestLevel(testLevel)

    call runtests_init

    call testWithLevel(run_all_ftn_interface_tests, "run all Fortran interface tests", testLevel)

    call runtests_final(.true.)

end program ftn_unit_tests

