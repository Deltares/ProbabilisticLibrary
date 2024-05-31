module class_probCalc
use interface_probCalc

implicit none

private
public :: tProcCalc

type tProcCalc
    type(TCpData) :: cpData
contains
    procedure :: run => wrapperProbCalc
    procedure :: copyDense2Full => cpDense2Full
end type tProcCalc

contains

subroutine wrapperProbCalc(this, probDb, fx, alfaN, beta, x, conv, convCriterium, convergenceData, pc)
    class(tProcCalc), intent(inout)                   :: this
    type(probabilisticDataStructure_data), intent(in) :: probDb           !< Probabilistic data module
    procedure(zfunc)                                  :: fx               !< Function implementing the z-function of the failure mechanism
    real(kind=wp), intent(out)                        :: alfaN(:)         !< Alpha values
    real(kind=wp), intent(out)                        :: beta             !< Reliability index
    real(kind=wp), intent(inout)                      :: x(:)             !< X values of design point
    logical,       intent(out)                        :: conv             !< Convergence indicator
    logical,       intent(out)                        :: convCriterium    !< Convergence criterium indicator
    type(storedConvergenceData), intent(inout)        :: convergenceData  !< struct holding all convergence data
    procedure(progressCancel),    optional            :: pc               !< progress function

    call calculateLimitStateFunction(probDb, fx, alfaN, beta, x, conv, convCriterium, convergenceData, pc, this%cpData)

end subroutine wrapperProbCalc

!> copy x-vector from Hydra-Ring to problib
subroutine cpFull2Dense(this, xFull, xDense)
    class(tProcCalc), intent(inout) :: this
    real(kind=wp),    intent(in   ) :: xFull(:)
    real(kind=wp),    intent(inout) :: xDense(*)

    integer :: i

    do i = 1, this%cpData%nStochActive
        xDense(i) = xFull(this%cpData%iPoint(i))
    end do
end subroutine cpFull2Dense

!> copy x-vector from problib to Hydra-Ring
subroutine cpDense2Full(this, xDense, xFull)
    class(tProcCalc), intent(inout) :: this
    real(kind=wp),    intent(in)    :: xDense(*)
    real(kind=wp),    intent(inout) :: xFull(:)

    integer :: i

    xFull = this%cpData%xHR
    do i = 1, this%cpData%nStochActive
        xFull(this%cpData%iPoint(i)) = xDense(i)
    end do
end subroutine cpDense2Full


end module class_probCalc
