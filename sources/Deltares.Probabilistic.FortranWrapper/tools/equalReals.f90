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

!> @file
!! This file contains a module with functions to test quality of reals
!<
!
!>
!! Module with functions to test quality of reals
!!   @ingroup General
module equalReals

    use precision

    implicit none
    
    private
    
    public :: equalRealsRelative, equalRealsAbsolute, equalRealsInVector, equalVectorsWithReals
    public :: equalTablesWithReals, equalVectorsWithIntegers, equalVectorsWithRealsWeightedMargin
    
    interface equalRealsRelative
        module procedure equalRealsRelative8, equalRealsRelative16
    end interface equalRealsRelative

contains

!> equalRealsRelative
!! Function to test for equality of two reals with a relative value for the margin
!! equalRealsRelative = |x-y| <= margin * (|x|+|y|)/2
!!   @ingroup General
logical function equalRealsRelative8 (x, y, margin)
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp), intent(in) :: x         !< First real for the comparison
    real(kind=wp), intent(in) :: y         !< Second real for the comparison
    real(kind=wp), intent(in) :: margin    !< Relative value for the margin
!
    equalRealsRelative8 = abs(x-y) <= margin * (abs(x)+abs(y)) * 0.5d0
!
end function equalRealsRelative8

logical function equalRealsRelative16 (x, y, margin)
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=qd), intent(in) :: x         !< First real for the comparison
    real(kind=qd), intent(in) :: y         !< Second real for the comparison
    real(kind=qd), intent(in) :: margin    !< Relative value for the margin
!
    equalRealsRelative16 = abs(x-y) <= margin * (abs(x)+abs(y)) * 0.5_qd
!
end function equalRealsRelative16

!> equalRealsAbsolute
!! Function to test for equality of two reals with an absolute value for the margin
!! equalRealsAbsolute = |x-y| <= margin
!!   @ingroup General
logical function equalRealsAbsolute (x, y, margin)
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp), intent(in) :: x         !< First real for the comparison
    real(kind=wp), intent(in) :: y         !< Second real for the comparison
    real(kind=wp), intent(in) :: margin    !< Absolute value for the margin
!
    equalRealsAbsolute = abs(x-y) <= margin
!
end function equalRealsAbsolute

!>
!! Function to test for equality of reals in a vector with a relative value of the margin
!!   @ingroup General
logical function equalRealsInVector (n, vector, margin)
!
!   INPUT/OUTPUT VARIABLES
!
    integer,        intent(in) :: n            !< Vector length
    real(kind=wp),  intent(in) :: vector(n)    !< Vector with reals
    real(kind=wp),  intent(in) :: margin       !< Relative value for the margin
!
!   SOURCE
!
!   Compute the minimal value of the vector
!   Compute the maximal value of the vector
!   Compare the minimal value with the maximal value with a relative margin
!
    equalRealsInVector = equalRealsRelative(minval(vector), maxval(vector), margin)
!
end function equalRealsInVector

!>
!! Function to test for equality of two vectors with reals
!! Two vectors are equal if all elements in the vector are equal
!!   @ingroup General
logical function equalVectorsWithReals (n, vector1, vector2, margin, equality)
!
!   INPUT/OUTPUT VARIABLES
!
    integer,       intent(in)  :: n             !< Vector length
    real(kind=wp), intent(in)  :: vector1(n)    !< Vector1 with reals (size n)
    real(kind=wp), intent(in)  :: vector2(n)    !< Vector2 with reals (size n)
    real(kind=wp), intent(in)  :: margin        !< Relative value for the margin
    logical,       intent(out) :: equality(n)   !< Vector (size n) with elements which indicates if the
                                                !! corresponding elements of the two input vectors are the same
!
!   LOCAL VARIABLES
!
    integer     :: i                            ! do-loop counter
!
!   SOURCE
!
!   Compare all the elements in the vectors with each other
!
    do i = 1, n
        equality(i) = equalRealsRelative(vector1(i), vector2(i), margin)
    enddo
!
!   The two vectors are equal if all elements in the vector are equal
!
    equalVectorsWithReals = all(equality)

end function equalVectorsWithReals

!>
!! Function to test for equality of two vectors with reals
!! Two vectors are equal if all elements in the vector are equal
!!   @ingroup General
logical function equalTablesWithReals (matrix1, matrix2, margin)
!
!   INPUT/OUTPUT VARIABLES
!
    real(kind=wp), intent(in)  :: matrix1(:,:)    !< Matrix 1 with reals
    real(kind=wp), intent(in)  :: matrix2(:,:)    !< Matrix 2 with reals
    real(kind=wp), intent(in)  :: margin          !< Relative value for the margin
!
!   LOCAL VARIABLES
!
    integer              :: matrix1_n             ! number of rows of matrix1
    integer              :: matrix1_m             ! number of columns of matrix1
    integer              :: matrix2_n             ! number of rows of matrix2
    integer              :: matrix2_m             ! number of columns of matrix2
    logical, allocatable :: equality(:)           ! Vector with elements which indicates if the corresponding
                                                  ! elements of the two input vectors are the same
    integer              :: i                     ! do-loop counter
!
!   SOURCE
!
!   Compare all the elements in the matrices with each other
!
    matrix1_n = size(matrix1(:,1))
    matrix1_m = size(matrix1(1,:))

    matrix2_n = size(matrix2(:,1))
    matrix2_m = size(matrix2(1,:))

    equalTablesWithReals = .true.
    if ((matrix1_n /= matrix2_n) .or. (matrix1_m /= matrix2_m)) then
        equalTablesWithReals = .false.
    else
        allocate (equality(matrix1_n))
        do i = 1, matrix1_m
            if (.not. equalVectorsWithReals(matrix1_n, matrix1(:,i), matrix2(:,i), margin, equality)) then
                equalTablesWithReals = .false.
                exit
            endif
        enddo
        deallocate (equality)
    endif

end function equalTablesWithReals

!>
!! Function to test for equality of two vectors with reals.
!! Two vectors are equal if all elements in the vector are almost equal
!! Here, margin is weighted according to tested numbers.
!!   @ingroup General
logical function equalVectorsWithRealsWeightedMargin (n, vector1, vector2, margin, equality)
!
!   INPUT/OUTPUT VARIABLES
!
    integer,       intent(in)  :: n             !< Vector length
    real(kind=wp), intent(in)  :: vector1(n)    !< Vector1 with reals (size n)
    real(kind=wp), intent(in)  :: vector2(n)    !< Vector2 with reals (size n)
    real(kind=wp), intent(in)  :: margin        !< Relative value for the margin
    logical,       intent(out) :: equality(n)   !< Vector (size n) with elements which indicates if the
                                                !! corresponding elements of the two input vectors are the same
!
!   LOCAL VARIABLES
!
    real(kind=wp)  :: Wmargin       ! weighted margin on values of vector1
    real(kind=wp)  :: factor        ! weight factor for margin
    integer        :: i             ! do-loop counter
!
!   SOURCE
!
!   Compare all the elements in the vectors with each other
!
    do i = 1, n
        if (vector1(i)==0.D0) then
           Wmargin = margin
        else
          factor = ( 10.D0**NINT(log10( abs( vector1(i) ) ) ) )
          Wmargin = margin / factor
        endif
        equality(i) = equalRealsRelative(vector1(i), vector2(i), Wmargin)
    enddo
!
!   The two vectors are equal if all elements in the vector are equal
!
    equalVectorsWithRealsWeightedMargin = all(equality)

end function equalVectorsWithRealsWeightedMargin


!! Function to test for equality of two vectors with reals
!! Two vectors are equal if all elements in the vector are equal
!!   @ingroup General
logical function equalVectorsWithIntegers (n, vector1, vector2, equality)
!
!   INPUT/OUTPUT VARIABLES
!
    integer,       intent(in)  :: n             !< Vector length
    integer,       intent(in)  :: vector1(n)    !< Vector1 with reals (size n)
    integer,       intent(in)  :: vector2(n)    !< Vector2 with reals (size n)
    logical,       intent(out) :: equality(n)   !< Vector (size n) with elements which indicates if the
                                                !! corresponding elements of the two input vectors are the same
!
!   LOCAL VARIABLES
!
    integer     :: i                            ! do-loop counter
!
!   SOURCE
!
!   Compare all the elements in the vectors with each other
!
    do i = 1, n
        equality(i) = ( vector1(i) == vector2(i) )
    enddo
!
!   The two vectors are equal if all elements in the vector are equal
!
    equalVectorsWithIntegers = all(equality)

end function equalVectorsWithIntegers

end module equalReals
