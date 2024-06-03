module mTpXData
use precision, only : wp
implicit none

private

type, public :: tCpData
    integer,      allocatable  :: iPoint(:)    ! Pointer to active variables used in the limit state function
    integer                    :: nStochActive
    real(kind=wp), allocatable :: xHR(:)
contains
    procedure :: copyDense2Full => cpDense2Full
    procedure :: copyFull2Dense => cpFull2Dense
end type tCpData

contains

!> copy x-vector from Hydra-Ring to problib
subroutine cpFull2Dense(this, xFull, xDense)
    class(tCpData),  intent(inout) :: this
    real(kind=wp),   intent(in   ) :: xFull(:)
    real(kind=wp),   intent(inout) :: xDense(this%nStochActive)

    integer :: i

    do i = 1, this%nStochActive
        xDense(i) = xFull(this%iPoint(i))
    end do
end subroutine cpFull2Dense

!> copy x-vector from problib to Hydra-Ring
subroutine cpDense2Full(this, xDense, xFull)
    class(tCpData),  intent(inout) :: this
    real(kind=wp),   intent(in)    :: xDense(this%nStochActive)
    real(kind=wp),   intent(inout) :: xFull(:)

    integer :: i

    xFull = this%xHR
    do i = 1, this%nStochActive
        xFull(this%iPoint(i)) = xDense(i)
    end do
end subroutine cpDense2Full


end module mTpXData
