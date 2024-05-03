module sparseWaartsTestFunctions
    use precision
    use interface_ProbCalc
    use waartsFunctions

    implicit none

    private
    public :: initSparseWaartsTestsFunctions, cleanUpWaartsTestsFunctions, &
                updateCounter, &
                zLimitState25QuadraticTermsSparse, zOblateSpheroid

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


end module sparseWaartsTestFunctions

