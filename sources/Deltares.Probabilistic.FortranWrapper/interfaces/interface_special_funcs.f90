module interface_special_funcs
use, intrinsic :: iso_c_binding, only: c_double
use interface_gen, only : tError
use f2c_tools, only : copystrback

implicit none

private :: c_double, tError, copystrback

contains

function betain(x, p, q, beta, ifault)
    real(kind=c_double), intent(in   ) :: x, p, q, beta
    integer,             intent(  out) :: ifault
    real(kind=c_double) :: betain
    betain = -999.0;
end function betain

function dgammq(a, x, ierr, errorMessage)
    real(kind=c_double), intent(in)        :: a
    real(kind=c_double), intent(in)        :: x
    integer,             intent(out)       :: ierr
    character(len=*),    intent(inout)     :: errorMessage
    real(kind=c_double)                    :: dgammq

    type(tError) :: ierror

    dgammq = -999;
    ierr = -999;
end function dgammq

end module interface_special_funcs
