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
!! Test for the random number generator
!<
!
module randomGeneratorTests

    use precision
    use ftnunit
    use fileUtilities
    use interface_randomgen

    implicit none

    private

    public :: allRandomGeneratorTests

contains

!> Call all the tests
!!
subroutine allRandomGeneratorTests

    call testWithLevel( testFirstRandomValue,                "testFirstRandomValue from module randomGeneratorTests", 1)
    call testWithLevel( testGenerateRandomValues,            "testGenerateRandomValues from module randomGeneratorTests", 1)

end subroutine allRandomGeneratorTests

!> Test getRandomValue for one value with ij and kl both equal to 10000
!!
subroutine testFirstRandomValue

    real(kind=wp)                    :: margin = 10d-12
    integer                          :: ij = 10000
    integer                          :: kl = 10000
    real(kind=wp)                    :: randomValue

    call initRandomGenerator( ij, kl )

    call getRandomValue( randomValue )

    call assert_comparable( randomValue, 0.755450129508972d0 , margin, "unexpected return value randomValue" )

end subroutine testFirstRandomValue

subroutine testGenerateRandomValues

    real(kind=wp)                    :: margin = 10d-12
    integer                          :: ij = 177
    integer                          :: kl = 177
    real(kind=wp)                    :: randomValue

    integer, parameter               :: n = 100000
    integer                          :: i
    integer                          :: fileId               ! file handle
    integer                          :: ierr

    call initRandomGenerator( ij, kl )

    open( newunit=fileId, file = 'trekking.txt', iostat = ierr )
    do i = 1, n
        call getRandomValue( randomValue )
        write( fileId, '(F8.5)') randomValue
    enddo

    close( fileId )

    call assert_comparable( randomValue, 0.7785768508911d0 , margin, "unexpected return value randomValue" )

    call removeFile('trekking.txt')
end subroutine testGenerateRandomValues

end module randomGeneratorTests
