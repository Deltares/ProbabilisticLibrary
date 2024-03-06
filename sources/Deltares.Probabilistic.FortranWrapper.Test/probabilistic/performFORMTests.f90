! Copyright (C) Stichting Deltares 2023. All rights reserved.
!
! This file is part of the Hydra Ring Application.
!
! The Hydra Ring Application is free software: you can redistribute it and/or modify
! it under the terms of the GNU Affero General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
!
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
! GNU Affero General Public License for more details.
!
! You should have received a copy of the GNU Affero General Public License
! along with this program. If not, see <http://www.gnu.org/licenses/>.
!
! All names, logos, and references to "Deltares" are registered trademarks of
! Stichting Deltares and remain full property of Stichting Deltares at all times.
! All rights reserved.
!

!> @file performFORMTests.f90
!! Test the initialisation and/or other methods for FORM
!!
!
module performFORMTests
    use precision
    use feedback
    use feedback_parameters, only : verboseDebugging
    use interface_probcalc
    use interface_probcalcdata
    use ftnunit
    use omp_lib

    implicit none

    private
    public :: allPerformFORMTests, init_probdb_x, cleanup_probdb_x

    integer, parameter :: histU = 10

contains

subroutine allPerformFORMTests

!   Entry to all tests originally defined in previous version of FORM-tests within subroutine allInitializeFORMTests()
    call testWithLevel( testRespectLengths,     &
        "Test FORM: check that differing array lengths are correctly handled (module performFORMTests.f90)", 1)
    call testWithLevel( testSimpleMethods,      "Test FORM: simple initialization methods (module performFORMTests.f90)", 1, "work-in-progress")
    call testWithLevel( testRaySearchMethod,    "Test FORM: the ray search method (module performFORMTests.f90)", 1, "work-in-progress")
    call testWithLevel( testSphereSearchMethod, "Test FORM: the sphere search method (module performFORMTests.f90)", 1, "work-in-progress")
   !call testWithLevel( testParameterCheck,     "Test FORM: the parameter check", 1)
   !call testWithLevel( testHelpersNonConv1,    "Test FORM functions used in case of non convergence (1)", 1)

end subroutine allPerformFORMTests

!> Respect array lengths: the array u may be longer than the number of stochasts
!!
subroutine testRespectLengths

    integer, parameter                     :: nStochasts = 3
    integer                                :: i
    integer, dimension(nStochasts)         :: iPoint = (/ 1, 4, 5 /)
    real(kind=wp), dimension(2*nStochasts) :: u
    real(kind=wp), pointer                 :: x(:)
    real(kind=wp)                          :: margin = 1.0d-06
    real(kind=wp)                          :: maxBetaExpected
    type(probabilisticDataStructure_data)  :: probDb
    character(len=100)                     :: msg

    call init_probdb_x(probDb, x, iPoint, nStochasts)

    u                        = -999.0d0

    probDb%method%maxParallelThreads = 4
    probDb%method%calcMethod = 1
    probDb%method%FORM%maxIterations = 0  ! stop after finding the start vector
    do i = 1,9
        if (i == fORMStartRaySearchNewDeprecated) cycle
        probDb%method%FORM%startMethod = i

        ! The values 1 represent the axes of the hypersphere/spheroid to be searched
        if ( i == fORMStartSphereSearch ) then
            probDb%method%FORM%startValue = 1.0
        endif

        call initializeFORM( probDb, zfuncSimple3, u, nStochasts, x, iPoint, maxBetaExpected )

        write(msg,*) "Expected three times -999; i = ", i
        call assert_comparable( u(nStochasts+1:2*nStochasts), (/  -999.0d0, -999.0d0, -999.0d0 /), margin, msg )
    enddo

    call cleanup_probdb_x(probDb, x)

end subroutine testRespectLengths


!> Simple methods are: all zeroes, all ones, given starting vector
!! Complications: select a small number of stochasts
!!
subroutine testSimpleMethods

    integer, parameter                    :: nStochasts = 3
    integer, dimension(nStochasts)        :: iPoint = (/ 1, 4, 5 /)
    real(kind=wp), dimension(nStochasts)  :: u
    real(kind=wp), pointer                :: x(:)
    real(kind=wp), dimension(nStochasts)  :: expected
    real(kind=wp)                         :: margin = 1.0d-06
    real(kind=wp)                         :: maxBetaExpected
    type(probabilisticDataStructure_data) :: probDb

    call init_probdb_x(probDb, x, iPoint, nStochasts)

    expected                 = (/ 1.0d0, 4.0d0, 5.0d0 /)

    !
    ! Method 1
    !
    probDb%method%FORM%startMethod = fORMStartZero
    probDb%method%calcMethod = 1
    call initializeFORM( probDb, zfuncSimple, u, nStochasts, x, iPoint, maxBetaExpected )

    call assert_comparable( u, (/ 0.0d0, 0.0d0, 0.0d0 /), margin, "Expected three zeroes" )

    !
    ! Method 2
    !
    probDb%method%FORM%startMethod = fORMStartOne
    call initializeFORM( probDb, zfuncSimple, u, nStochasts, x, iPoint, maxBetaExpected )

    call assert_comparable( u, (/ 1.0d0, 1.0d0, 1.0d0 /), margin, "Expected three ones" )

    !
    ! Method 3
    !
    probDb%method%FORM%startMethod = fORMStartGivenVector
    call initializeFORM( probDb, zfuncSimple, u, nStochasts, x, iPoint, maxBetaExpected )

    call assert_comparable( u, expected, margin, "Expected (1,4,5)" )

    call cleanup_probdb_x(probDb, x)

end subroutine testSimpleMethods


!> Ray search method: the vector should have the same value
!! for all stochasts that participate and zero for the others
!!
subroutine testRaySearchMethod

    integer, parameter                    :: nStochasts = 5
    integer, dimension(nStochasts)        :: iPoint = (/ 1, 4, 5, 6, 9 /)
    real(kind=wp), dimension(nStochasts)  :: u
    real(kind=wp), pointer                :: x(:)
    real(kind=wp)                         :: margin = 1.0d-06
    real(kind=wp)                         :: maxBetaExpected
    type(probabilisticDataStructure_data) :: probDb

    call init_probdb_x(probDb, x, iPoint, nStochasts)

    probDb%method%FORM%startValue(4) = 0.0d0

    probDb%method%FORM%startMethod = fORMStartRaySearch
    call initializeFORM( probDb, zfuncSimple2, u, nStochasts, x, iPoint, maxBetaExpected )

    call assert_comparable( u(1), u(3),  margin, "Expected u(1) and u(3) to be equal" )
    call assert_comparable( u(1), u(4),  margin, "Expected u(1) and u(4) to be equal" )
    call assert_comparable( u(1), u(5),  margin, "Expected u(1) and u(5) to be equal" )
    call assert_comparable( u(2), 0.0d0, margin, "Expected u(2) to be zero" )

    call cleanup_probdb_x(probDb, x)

end subroutine testRaySearchMethod


!> Sphere search method:
!! \li The components of the vector should be non-zero only for those \
!! stochasts that participate
!! \li All vectors should lie on the sphere with the radius
!! \li All components should be positive
!!
!! <b>Note:</b> These conditions are built into the search method itself!
!!
subroutine testSphereSearchMethod

    integer, parameter                    :: nStochasts = 5
    integer                               :: i
    integer, dimension(nStochasts)        :: iPoint = (/ 1, 4, 5, 6, 9 /)
    real(kind=wp), dimension(nStochasts)  :: u
    real(kind=wp), pointer                :: x(:)
    real(kind=wp)                         :: margin = 1.0d-06
    real(kind=wp)                         :: maxBetaExpected
    type(probabilisticDataStructure_data) :: probDb

    call init_probdb_x(probDb, x, iPoint, nStochasts)

    probDb%method%FORM%startValue(4) = 0.0d0

    probDb%method%FORM%startMethod = fORMStartSphereSearch
    call omp_set_num_threads( 4 )
    call initializeFORM( probDb, zfuncSimple2, u, nStochasts, x, iPoint, maxBetaExpected )

    ! fortran did not find change in z; now we have
    do i = 1, 4
        call assert_true( abs(u(i)) < margin, "Expected u(1-4) to be zero" )
    enddo
    call assert_comparable( u(5), 0.3763089_wp, margin, "Expected u(5) to be 0.376" )

    call cleanup_probdb_x(probDb, x)

end subroutine testSphereSearchMethod

function zFuncSimple( u, designPointOutput, ierr ) result(z) bind(c)

    real(kind=wp), intent(inout) :: u(*)
    real(kind=wp)                :: z
    integer,       intent(in)    :: designPointOutput(*)    !< Switch for extra design point output
    type(tError),  intent(inout) :: ierr

    ierr%iCode = 0
    if (designPointOutput(1) == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = 0.5 - sum(u(1:6)**2)

end function zFuncSimple

function zFuncSimple2( u, designPointOutput, ierr ) result(z) bind(c)

    real(kind=wp), intent(inout) :: u(*)
    real(kind=wp)                :: z
    integer,       intent(in)    :: designPointOutput(*)    !< Switch for extra design point output
    type(tError),  intent(inout) :: ierr

    integer, parameter              :: nStochasts = 5
    integer                         :: i
    integer, dimension(nStochasts)  :: iPoint = (/ 1, 4, 5, 6, 9 /)

    ierr%iCode = 0
    if (designPointOutput(1) == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = 0.5
    do i = 1, nStochasts
        z = z - u(iPoint(i))**2
    enddo

end function zFuncSimple2


function zFuncSimple3( u, designPointOutput, ierr ) result(z) bind(c)

    real(kind=wp), intent(inout) :: u(*)
    real(kind=wp)                :: z
    integer,       intent(in)    :: designPointOutput(*)    !< Switch for extra design point output
    type(tError),  intent(inout) :: ierr

    ierr%iCode = 0
    if (designPointOutput(1) == designPointOutputTRUE) ierr%Message = ' '  ! avoid not used warning

    z = 0.5 - ( u(1)**2 + u(4)**2 + u(5)**2 )

end function zFuncSimple3

subroutine init_probdb_x(probDb, x, iPoint, nStochasts)
type(probabilisticDataStructure_data), intent(out) :: probDb
real(kind=wp), pointer                             :: x(:)
integer, intent(in)                                :: nStochasts
integer, intent(in)                                :: iPoint(nStochasts)

integer :: maxIpoint, i

maxIpoint = maxval(iPoint)

call initProbabilisticCalculation ( probDb, maxIpoint, .false., .false. )

allocate(probDb%method%FORM%startValue(10))
probDb%method%FORM%startValue    = (/ ( real(i,wp) ,i=1,10 ) /)

allocate(x(maxIpoint))

end subroutine init_probdb_x

subroutine cleanup_probdb_x(probDb, x)
type(probabilisticDataStructure_data), intent(inout) :: probDb
real(kind=wp), pointer                               :: x(:)

deallocate(probDb%method%FORM%startValue)
deallocate(probDb%stoVar%distTypeX)
deallocate(probDb%stoVar%distParameterX)
!deallocate(probDb%stoVar%table)
deallocate(x)

end subroutine cleanup_probdb_x

subroutine fill_convData_lastU(convergenceData , lastU, withNoise, signBeta)
    type(storedConvergenceDataFORM), intent(out) :: convergenceData
    real(kind=wp),                   intent(out) :: lastU(:,:)
    logical,                         intent(in) :: withNoise
    real(kind=wp),                   intent(in) :: signBeta

    integer :: i
    real(kind=wp) :: noise

    do i = 1, histU
        noise = merge(0.01_wp * real(mod(i,3),wp), 0.0_wp, withNoise)
        lastU(i,1) = (1.1_wp + noise) * signBeta
        lastU(i,2) = (2.2_wp + noise) * signBeta
        lastU(i,3) = (3.3_wp + noise) * signBeta
    enddo

    convergenceData%numberIterations = 2 * histU
    allocate(convergenceData%beta(convergenceData%numberIterations))
    convergenceData%beta = 4.116_wp * signBeta
    
    allocate(convergenceData%scaledZ(convergenceData%numberIterations))
    do i = 1, convergenceData%numberIterations
        convergenceData%scaledZ(i) = 2.0_wp / real(i, wp)
    enddo
end subroutine fill_convData_lastU

subroutine check_alpha_normalized(alpha)
    real(kind=wp), intent(in)       :: alpha(:)

    real(kind=wp)                   :: sumAlpha2
    integer                         :: i, nStochasts

    nStochasts = size(alpha)
    sumAlpha2 = 0.0_wp
    do i = 1, nStochasts
        sumAlpha2 = sumAlpha2 + alpha(i)**2
    enddo
    call assert_comparable(sumAlpha2, 1.0_wp, 1d-9, "returned alpha must be normalized")
end subroutine check_alpha_normalized

subroutine initializeForm( probDb, fx, u, nStochasts, x, iPoint, beta)
    type(probabilisticDataStructure_data)      :: probDb           !< Probabilistic data module
    procedure(zfunc)                           :: fx               !< Function implementing the z-function of the failure mechanism
    real(kind=wp), intent(out)                 :: beta             !< Reliability index
    real(kind=wp), intent(inout)               :: u(:), x(:)       !< U and X values of design point
    integer,       intent(in)                  :: iPoint(:)
    integer,       intent(in)                  :: nStochasts

    type(storedConvergenceData) :: allConvergenceData  !< struct holding all convergence data
    integer :: conv, i, keep
    real(kind=wp) :: alfa(maxval(iPoint))
    logical :: convCriterium

    do i = 1, nStochasts
        call setStochasticDistrib (probDb, iPoint(i), distributionNormal, 0.0d0, 1.0D0, 0.0d0, 0.0d0)
    end do
    call calculateLimitStateFunction( probDb, fx, alfa, beta, u, conv, convCriterium, allConvergenceData)

end subroutine initializeForm

end module performFORMTests
