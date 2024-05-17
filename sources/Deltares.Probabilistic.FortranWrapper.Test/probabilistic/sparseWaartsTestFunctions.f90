module sparseWaartsTestFunctions
    use precision
    use interface_ProbCalc
    use waartsFunctions

    implicit none

    private
    public :: initSparseWaartsTestsFunctions, cleanUpWaartsTestsFunctions, &
                updateCounter, &
                zLimitState25QuadraticTermsSparse, zOblateSpheroid, &
                zLinearResistanceSolicitation, simpleZ

    integer,       allocatable, target :: counter(:)
    real(kind=wp), allocatable, target :: xFull(:,:)

contains

subroutine initSparseWaartsTestsFunctions(nstoch, nthreads)
    integer, intent(in) :: nstoch, nthreads
    allocate(xFull(nstoch, nthreads), counter(nThreads))
    xFull = 0.0_wp
    counter = 0
end subroutine initSparseWaartsTestsFunctions

subroutine updateCounter(invocationCount)
    integer, intent(inout) :: invocationCount

    invocationCount = invocationCount + sum(counter)
end subroutine updateCounter

subroutine cleanUpWaartsTestsFunctions
    deallocate(xFull, counter)
end subroutine cleanUpWaartsTestsFunctions

!> Limit state function with 25 quadratic terms sparse with generic interface
function zLimitState25QuadraticTermsSparse( xDense, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: xDense(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    real(kind=wp), pointer                  :: x(:)
    integer      , pointer                  :: invocationCount

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    x => xFull(:, compSetting%threadId+1)
    invocationCount => counter(compSetting%threadId+1)
    call copyDense2Full(xDense, x)

    z = limitState25QuadraticTerms( x ( 30 ), x ( 3 : 27) )

    invocationCount = invocationCount + 1

end function zLimitState25QuadraticTermsSparse

!> Oblate spheroid with generic interface
function zOblateSpheroid( xDense, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: xDense(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    real(kind=wp), pointer                  :: x(:)
    integer      , pointer                  :: invocationCount

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    x => xFull(:, compSetting%threadId+1)
    invocationCount => counter(compSetting%threadId+1)

    call copyDense2full(xDense, x)
    z = oblateSpheroid( x ( 1 ), x ( 2 : 11 )  )

    invocationCount = invocationCount + 1

end function zOblateSpheroid

!> Linear resistance solicitation function with generic interface
function zLinearResistanceSolicitation( xDense,  compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: xDense(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    real(kind=wp), pointer                  :: x(:)
    integer      , pointer                  :: invocationCount

    ierr%icode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    x => xFull(:, compSetting%threadId+1)
    invocationCount => counter(compSetting%threadId+1)

    call copyDense2Full(xDense, x)
    z = linearResistanceSolicitation( x(1), x(2) )

    invocationCount = invocationCount + 1

end function zLinearResistanceSolicitation

!> Simple z function: valueB - u1 - u2, where u1 and u2 are standard normally distributed, and where beta = 4
function simpleZ( xDense, compSetting, ierr ) result(z) bind(c)

    real(kind=wp),            intent(inout) :: xDense(*)
    type(computationSetting), intent(in   ) :: compSetting
    type(tError),             intent(inout) :: ierr
    real(kind=wp)                           :: z

    real(kind=wp), pointer                  :: x(:)
    integer      , pointer                  :: invocationCount

    ierr%iCode = 0
    if (compSetting%designPointSetting == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    x => xFull(:, compSetting%threadId+1)
    invocationCount => counter(compSetting%threadId+1)

    call copyDense2Full(xDense, x)
    z = simpleSystem ( x(1),  x(2), x(3))

end function simpleZ

function simpleSystem( x1, x2, valueB )

    real (kind=wp), intent(in)   :: x1               !< input parameter x1
    real (kind=wp), intent(in)   :: x2               !< input parameter x2
    real (kind=wp), intent(in)   :: valueB           !< input parameter valueB
    real (kind=wp)               :: simpleSystem     !< output parameter

    simpleSystem = valueB - x1 - x2

end function simpleSystem

end module sparseWaartsTestFunctions

