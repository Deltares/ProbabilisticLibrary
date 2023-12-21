module interface_special_funcs
use, intrinsic :: iso_c_binding, only: c_double
use interface_gen, only : tError
use f2c_tools, only : copystrback

implicit none

private :: c_double, tError, copystrback

interface
    function betainc(x, p, q, beta, ifault) bind(C)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double), intent(in   ), value :: x, p, q, beta
        integer,             intent(  out)        :: ifault
        real(kind=c_double) :: betainc
    end function betainc
end interface

interface
    function dgamln(x) bind(C)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double), intent(in), value :: x
        real(kind=c_double)                    :: dgamln
    end function dgamln
end interface

interface
    function dgammq_c(a, x, ierr) bind(C)
        use, intrinsic :: iso_c_binding, only: c_double
        import tError
        real(kind=c_double), intent(in), value :: a
        real(kind=c_double), intent(in), value :: x
        type(tError),        intent(inout)     :: ierr
        real(kind=c_double)                    :: dgammq_c
    end function dgammq_c
end interface

contains

function betain(x, p, q, beta, ifault)
    real(kind=c_double), intent(in   ) :: x, p, q, beta
    integer,             intent(  out) :: ifault
    real(kind=c_double) :: betain
    betain = betainc(x, p, q, beta, ifault)
end function betain

function dgammq(a, x, ierr, errorMessage)
    real(kind=c_double), intent(in)        :: a
    real(kind=c_double), intent(in)        :: x
    integer,             intent(out)       :: ierr
    character(len=*),    intent(inout)     :: errorMessage
    real(kind=c_double)                    :: dgammq

    type(tError) :: ierror

    dgammq = dgammq_c(a, x, ierror)
    ierr = ierror%iCode
    if (ierr /= 0) then
        call copystrback(errorMessage, ierror%message)
    end if
end function dgammq

end module interface_special_funcs
