module interface_correlation
use precision
use interface_gen
use f2c_tools
use interface_distributions
!> Independent variable
integer, parameter :: correlationIndependent = 0

!> Perfect correlation
integer, parameter :: correlationComplete = 1

!> NL correlation model
!! One parameter: standard deviation of independent component
integer, parameter :: correlationNLmodel = 2

!> NL-dunes correlation model
!! TODO
integer, parameter :: correlationNLdunes = 3

!> PCR correlation model
integer, parameter :: correlationPCRmodel = 4

!> Volker correlation model
integer, parameter :: correlationVolker = 5

!> Gaussian correlation model
integer, parameter :: correlationGaussianModel = 6

type, public, bind(c) :: basicCorrelation
    integer       :: first                !< Index of the first stochastic variable
    integer       :: second               !< Index of the second stochastic variable
    real(kind=wp) :: correlation          !< Correlation coefficient
end type basicCorrelation

!! TODO: type is not a enumeration
type, public, bind(c) :: correlationModelParameters
    integer        :: independentId            !< ID of independent u component
    integer        :: correlationId            !< ID of selected correlation model
    integer        :: correlationParameterId   !< Identifier in database of the correlation parameters
    integer        :: distribution             !< ID of the distribution model of independent component
    real(kind=wp)  :: distParameter1           !< Parameter 1 of distribution type
    real(kind=wp)  :: distParameter2           !< Parameter 2 of distribution type
    real(kind=wp)  :: distParameter3           !< Parameter 3 of distribution type
    real(kind=wp)  :: distParameter4           !< Parameter 4 of distribution type
    real(kind=wp)  :: correlationParameter1    !< Parameter 1 of correlation model
    real(kind=wp)  :: correlationParameter2    !< Parameter 2 of correlation model
    real(kind=wp)  :: correlationParameter3    !< Parameter 3 of correlation model
    real(kind=wp)  :: correlationParameter4    !< Parameter 4 of correlation model
    real(kind=wp)  :: correlationParameter5    !< Parameter 5 of correlation model
    real(kind=wp)  :: correlationParameter6    !< Parameter 6 of correlation model
    real(kind=wp)  :: correlationParameter7    !< Parameter 7 of correlation model
    real(kind=wp)  :: correlationParameter8    !< Parameter 8 of correlation model
    real(kind=wp)  :: shift                    !< Parameter to shift the distribution (in case of CorrelationNL)
end type correlationModelParameters

interface
    subroutine calculateCorrelationC(u1, u2, correlationParameters, typeUCorrelation, error) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        import correlationModelParameters, tError
        real (kind = c_double), intent(in)           :: u1                     !< Independent variable
        real (kind = c_double), intent(inout)        :: u2                     !< Dependent variable
        type(correlationModelParameters), intent(in) :: correlationParameters  !< Parameters for the correlation
        logical(kind=1), intent(in)                  :: typeUCorrelation       !< u-space (true) or x-space (false)
        type(tError), intent(inout)                  :: error                  !< error struct
    end subroutine calculateCorrelationC
end interface

interface
    subroutine correlationUpdate_c(correlations, alpha_in, alpha_out, nCorrelations, vectorSize) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        import basicCorrelation
        type(basicCorrelation), intent(in)    :: correlations(*)
        real(kind=c_double),    intent(in)    :: alpha_in(*)
        real(kind=c_double),    intent(  out) :: alpha_out(*)
        integer,        value,  intent(in   ) :: nCorrelations
        integer,        value,  intent(in   ) :: vectorSize
    end subroutine correlationUpdate_c
end interface

type tpBasicCorrelation
    integer       :: number_correlations    = 0
    type(basicCorrelation), dimension(:), allocatable :: basic_correlation
end type tpBasicCorrelation

contains
subroutine calculateCorrelation(u1, u2, correlationParameters, typeUCorrelation) bind(c)
    use, intrinsic :: iso_c_binding, only: c_double
    use feedback
    real (kind = c_double), intent(in)           :: u1                     !< Independent variable
    real (kind = c_double), intent(inout)        :: u2                     !< Dependent variable
    type(correlationModelParameters), intent(in) :: correlationParameters  !< Parameters for the correlation
    logical, optional, intent(in)                :: typeUCorrelation       !< u-space (true) or x-space (false)

    type(tError) :: error
    logical(kind=1) :: typeU
    character(len=ErrMsgLength) :: msg

    if (present(typeUCorrelation)) then
        typeU = typeUCorrelation
    else
        typeU = .true.
    end if

    call calculateCorrelationC(u1, u2, correlationParameters, typeU, error)
    if (error%iCode /= 0) then
        call copystrback(msg, error%message)
        call fatalError(msg)
    end if
end subroutine calculateCorrelation

subroutine correlationUpdate(correlations, alpha_in, alpha_out)
    type(basicCorrelation), intent(in)    :: correlations(:)
    real(kind=wp),          intent(in)    :: alpha_in(:)
    real(kind=wp),          intent(  out) :: alpha_out(:)

    call correlationUpdate_c(correlations, alpha_in, alpha_out, size(correlations), size(alpha_in))
end subroutine correlationUpdate

end module interface_correlation
