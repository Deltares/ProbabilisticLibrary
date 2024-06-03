module class_probCalc
use interface_probCalc

implicit none

private
public :: tProbCalc

type tProbCalc
    type(TCpData) :: cpData
contains
    procedure :: run => wrapperProbCalc
end type tProbCalc

contains

subroutine wrapperProbCalc(this, probDb, fx, alfaN, beta, x, conv, convCriterium, convergenceData, pc)
    class(tProbCalc), intent(inout)                   :: this
    type(probabilisticDataStructure_data), intent(in) :: probDb           !< Probabilistic data module
    procedure(zfunc)                                  :: fx               !< Function implementing the z-function of the failure mechanism
    real(kind=wp), intent(out)                        :: alfaN(:)         !< Alpha values
    real(kind=wp), intent(out)                        :: beta             !< Reliability index
    real(kind=wp), intent(inout)                      :: x(:)             !< X values of design point
    logical,       intent(out)                        :: conv             !< Convergence indicator
    logical,       intent(out)                        :: convCriterium    !< Convergence criterium indicator
    type(storedConvergenceData), intent(inout)        :: convergenceData  !< struct holding all convergence data
    procedure(progressCancel),    optional            :: pc               !< progress function

    call calculateLimitStateFunction(probDb, fx, alfaN, beta, x, conv, convCriterium, convergenceData, this%cpData, pc)

end subroutine wrapperProbCalc

end module class_probCalc
