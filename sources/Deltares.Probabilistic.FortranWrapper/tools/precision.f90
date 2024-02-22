module precision
    integer, parameter :: sp = 4
    integer, parameter :: dp = 8
    integer, parameter :: wp = 8
    integer, parameter :: qd = selected_real_kind(33, 4931)  !< Quadruple precision

    real(kind=wp), parameter :: almostzero = 1.0d-30           !< Definition of zero for the machine

    contains

    !> set_nan
    !! set number to NaN, double precision version
    subroutine set_nan(value)
    use, intrinsic :: iso_fortran_env
    use, intrinsic :: ieee_arithmetic
    real(kind=dp), intent(out) :: value   !< double to be set to NaN

    value = ieee_value(value, ieee_quiet_nan)

    end subroutine set_nan

end module precision

