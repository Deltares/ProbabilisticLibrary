module interface_convert
use interface_gen
use f2c_tools
use precision
implicit none

interface
    function QfromBeta(beta) result(q) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double), intent(in) :: beta
        real(kind=c_double)             :: q
    end function QfromBeta
end interface

interface
    function PfromBeta(beta) result(q) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double), intent(in) :: beta
        real(kind=c_double)             :: q
    end function PfromBeta
end interface

interface
    subroutine PQfromBeta( beta, p, q ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double), intent(in)  :: beta
        real(kind=c_double), intent(out) :: p, q
    end subroutine PQfromBeta
end interface

interface BetaFromQ
    module procedure BetaFromQ1
    module procedure BetaFromQ2
    module procedure BetaFromQ3
end interface

interface
    subroutine BetaFromQC(q, beta, ierr) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        import :: tError
        real(kind=c_double), intent(  out) :: beta
        real(kind=c_double), intent(in   ) :: q
        type(tError),        intent(inout) :: ierr
    end subroutine BetaFromQC
end interface

interface
    subroutine ReturnTimeFromBeta( beta, returnTime ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double), intent(in)  :: beta
        real(kind=c_double), intent(out) :: returnTime
    end subroutine ReturnTimeFromBeta
end interface

interface
    subroutine FreqFromBeta(beta, freq) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double), intent(in)  :: beta
        real(kind=c_double), intent(out) :: freq
    end subroutine FreqFromBeta
end interface

interface
    subroutine LogQFromBeta(beta, logQ) bind(c)
        use, intrinsic :: iso_c_binding, only: c_double
        real(kind=c_double), intent(in)  :: beta
        real(kind=c_double), intent(out) :: logQ
    end subroutine LogQFromBeta
end interface

contains
    subroutine BetaFromQ1(q, beta, ierr)
        real(kind=wp), intent(  out) :: beta
        real(kind=wp), intent(in   ) :: q
        type(tError),  intent(inout) :: ierr

        call BetaFromQC(q, beta, ierr)
    end subroutine BetaFromQ1

    subroutine BetaFromQ2(q, beta, ierr, errorMessage)
        real(kind=wp),    intent(  out) :: beta
        real(kind=wp),    intent(in   ) :: q
        integer,          intent(  out) :: ierr
        character(len=*), intent(inout) :: errorMessage

        type(tError) :: error

        call BetaFromQC(q, beta, error)
        ierr = error%iCode
        if (error%iCode /= 0) then
            call copystrback(errorMessage, error%message)
        end if
    end subroutine BetaFromQ2

    subroutine BetaFromQ3(q, beta)
        use feedback, only : fatalError
        real(kind=wp),    intent(  out) :: beta
        real(kind=wp),    intent(in   ) :: q

        type(tError) :: error
        character(len=128) :: msg

        call BetaFromQC(q, beta, error)
        if (error%iCode /= 0) then
            call copystrback(msg, error%message)
            call fatalError(msg)
        end if
    end subroutine BetaFromQ3

    function FBetaFromQ(q) result(beta)
        real(kind=wp)                   :: beta
        real(kind=wp),    intent(in   ) :: q
        call BetaFromQ(q, beta)
    end function FBetaFromQ

    function FReturnTimeFromBeta( beta ) result( returnTime )
        real(kind=wp), intent(in)  :: beta
        real(kind=wp)  :: returnTime
        call returnTimeFromBeta( beta, returnTime )
    end function FReturnTimeFromBeta

    function FFreqFromBeta( beta ) result (freq)
        real(kind=wp), intent(in)  :: beta
        real(kind=wp)  :: freq
        call FreqFromBeta(beta, freq)
    end function FFreqFromBeta
end module interface_convert
