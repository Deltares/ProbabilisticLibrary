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

!> @file
!! Test Direct and inverse distribution functions and associated auxiliary routines
!<
!
!> @ingroup ProbabilisticTests
!> Module holding all distribution functions tests. This includes: \n
!> 1) uniformDistributionTests\n
!> 2) exponentialDistributionTests\n
!> 3) lognormalDistributionChangesTests\n
!> 4) lognormalDistributionTests\n
!> 5) lognormalDistributionTypeIITests\n
!> 6) gumbelDistributionTests\n
!> 7) weibullDistributionTests\n
!> 8) paretoDistributionTests\n
!> 9) triangularDistributionTests\n
!> 10) conditionalWeibullDistributionTests\n
!> 11)modifiedGumbelDistributionTests\n
!> 12)rayleighNDistributionTests
!
module distributionFunctionsTests
    !
    !------------------------------------------------------------------------
    !   programmer: Kathryn Roscoe
    !               Henk van den Boogaard (modifications, upgrading, extensions)
    !   date:       $Date: 2019-02-05 14:53:13 +0100 (Tue, 05 Feb 2019) $
    !   version:    $Revision: 9222 $
    !------------------------------------------------------------------------

    use ftnunit
    use precision
    use distributionFunctions
    use feedback, only : fatalError
    use interface_distributions
    use interface_special_funcs
    use interface_convert
    use fileUtilities
    use equalReals

    implicit none

    private

    public :: allDistributionFunctionsTests, setTestDir2

    character(len=128) :: testDir = '../probabilistic/testData'

    real (kind = wp), parameter :: margin = 1.0d-6

contains

subroutine allDistributionFunctionsTests

    !ToDo
    !Add tests for the interpolation of tables, functionality found in calcHydraulicStat

    call uniformDistributionTests
    call exponentialDistributionTests
    call lognormalDistributionChangesTests
    call lognormalDistributionTests
    call lognormalDistributionTypeIITests
    call gumbelDistributionTests
    call weibullDistributionTests
    call paretoDistributionTests
    call triangularDistributionTests
    call testWithLevel(conditionalWeibInverseExcelTest, &
        "Inverse conditional Weibull: Compare Hydra-Ring output and excel output for Hoek van Holland", 1, "work-in-progress")
    call modifiedGumbelDistributionTests
    call rayleighNDistributionTests
    call testWithLevel(conditionalWeibullTest, "conditional Weibull test", 1)
    call testWithLevel(calculateDistributionTest, "general entry calculate distribution test", 1)
    call testWithLevel(calculateDistributionInverseTest, "general entry inverse calculate distribution test", 1)
    call testWithLevel(truncatedNormalTests, "truncated normal tests", 1)
    call betaDistributionTests
!
end subroutine allDistributionFunctionsTests

subroutine setTestDir2(nwTestDir)
character(len=*), intent(in) :: nwTestDir  !< current test directory to overwrite default value

    testDir = nwTestDir
end subroutine setTestDir2

! -------------------------------------------------------------------
! Begin of a series of subroutines for testing distribution functions
! -------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
!> Inverse uniform tests
!!

subroutine uniformDistributionTests

    call testWithLevel(uniformTestProbUnity,   "InverseUniform: Test for non-exceedance probability equal to one", 1)
    call testWithLevel(uniformTestProbZero,    "InverseUniform: Test for non-exceedance probability equal to zero", 1)
    call testWithLevel(uniformTestProbHalf,    "InverseUniform: Test for non-exceedance probability equal to one-half", 1)

end subroutine uniformDistributionTests

!> Inverse uniform test: This subroutine tests if the second parameter is returned for a probability equal to one
!
subroutine uniformTestProbUnity

    real(kind = wp)::   p = 1
    real(kind = wp)::   a = 2
    real(kind = wp)::   b = 3
    real(kind = wp)::   margin = 0.000001

    real(kind = wp)::   x

    x = uniformInverse(p, a, b)
    call assert_comparable(b, x, margin, "output should be the second parameter for a probability = 1")

end subroutine uniformTestProbUnity

!> Inverse uniform test: This subroutine tests if the first parameter is returned for a probability equal to zero
!
subroutine uniformTestProbZero

    real(kind = wp)::   p = 0
    real(kind = wp)::   a = 2
    real(kind = wp)::   b = 3
    real(kind = wp)::   margin = 0.000001

    real(kind = wp):: x

    x = uniformInverse(p, a, b)

    call assert_comparable(a, x, margin, "output should be the second parameter for a probability = 0")

end subroutine uniformTestProbZero

!> Inverse uniform test: This subroutine tests if the first parameter is returned for a probability equal to 1/2
!
subroutine uniformTestProbHalf

    real(kind = wp)::   p = 0.5
    real(kind = wp)::   a = 2
    real(kind = wp)::   b = 3
    real(kind = wp)::   margin = 0.000001

    real(kind = wp):: x

    x = uniformInverse(p, a, b)

    call assert_comparable(0.5*(a + b), x, margin, "output should be 1/2*(par1+par2) for a probability = 1/2")

end subroutine uniformTestProbHalf

!--------------------------------------------------------------------------------------
!> Inverse exponential tests
!!
subroutine exponentialDistributionTests

  call testWithLevel( exponentialTestProbZero, &
      "InverseExponential: Test for non-exceedance probability equal to zero", 1, "Not implemented yet")
  call testWithLevel( expInverseMatlabTest, &
      "InverseExponential: Compare Hydra-Ring output and Matlab output for u = -8:8", 1, "Not implemented yet")

end subroutine exponentialDistributionTests


!> Inverse exponential test: This subroutine tests if the second parameter is returned for a probability equal to zero
!
subroutine exponentialTestProbZero
    real(kind = wp), parameter  ::   p = 0
    real(kind = wp)             ::   q = 1.0d0 - p
    real(kind = wp)             ::   a = 2
    real(kind = wp)             ::   b = 3
    real(kind = wp)             ::   margin = 0.000001

    real(kind = wp)::   x

    x = exponentialInverse(a, b, q)

    call assert_comparable(b, x, margin, "output should be equal to the second parameter for non-exceedance probability p = 0")

end subroutine


!> Test expInverse by comparing with Matlab output for u values ranging from -8 to +8
!> MATLAB results are read from file '../probabilistic/testData/expinvMLTest.dat'
!
subroutine expInverseMatlabTest

    real(kind = wp)             :: margin

    real(kind=wp),  allocatable :: u(:)
    real(kind=wp),  allocatable :: p(:)
    real(kind=wp),  allocatable :: q(:)
    real(kind=wp),  allocatable :: a(:)
    real(kind=wp),  allocatable :: b(:)
    real(kind=wp),  allocatable :: ML(:)

    real(kind=wp),  allocatable :: x(:)

    integer                     :: i
    integer                     :: ifile
    integer                     :: fileLength
    character(len=100)          :: regel
    integer                     :: ios

    open( newunit = ifile, file = trim(testDir) // '/expinvMLTest.dat' )

    !determine length of file
    fileLength = 0
    ios = 0

    do while( ios == 0)
        read( ifile,'(a)', iostat=ios ) regel
        fileLength = fileLength+1
    enddo

    fileLength = fileLength - 1

    allocate( u(fileLength) )
    allocate( p(fileLength) )
    allocate( q(fileLength) )
    allocate( a(fileLength) )
    allocate( b(fileLength) )
    allocate( ML(fileLength) )
    allocate( x(fileLength) )

    close( ifile )

    !read in u-values and matlab output
    open( newunit = ifile, file = trim(testDir) // '/expinvMLTest.dat' )

    read( ifile,* ) ( u(i), p(i), a(i), ML(i), i = 1,fileLength )

    !Run weibull inverse comparison code for all input, compares matlab with hydra-ring
    b       = 0
    margin  = 0.001D0
    q = 1.0d0 - p

    do i=1,fileLength
        x(i) = exponentialInverse(a(i), b(i), q(i))
    end do

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( x, ML, margin, "Matlab output and Hydra-Ring output differ" )

    close( ifile )

    deallocate( u, p, a, b, ML, x )

end subroutine expInverseMatlabTest



!--------------------------------------------------------------------------------------
!> CHANGES to lognormal distribution tests
!> Test for differences with original code.
!!
subroutine lognormalDistributionChangesTests

    call testWithLevel(logNormalInverseChangesTest,   "Changes to lognormal code: Test for differences with original code", 1)

end subroutine lognormalDistributionChangesTests


!> Test logNormalInverse change to original code, compare revised with original
subroutine logNormalInverseChangesTest

    real(kind=wp),   dimension(1000):: u, mu, sig
    real(kind=wp),   dimension(1000):: par3 = 0
    real(kind=wp),   dimension(1000):: y
    logical,         dimension(1000):: z
    integer                         :: i
    integer                         :: ifile

    real(kind=wp),   dimension(1000):: x

    ! Read in a file with 1000 inputs of probability, mu, and sigma
    open( newunit = ifile,file = trim(testDir) // '/longinvTest.dat')
    read( ifile,* ) ( u(i), mu(i), sig(i), i = 1,1000 )
    close ( ifile )

    ! Run lognormal inverse comparison code for all 1000 input, compares old code and new code
    do i=1,1000
        x(i) = logNormalInverse_compare( u(i), mu(i), sig(i), par3(i), y(i), z(i) )
    end do

    ! If there are any differences, the test will fail and a warning will be triggered
    call assert_true( all(z),"WARNING: changes to logNormalInverse caused changes in the output!" )

end subroutine logNormalInverseChangesTest


!--------------------------------------------------------------------------------------
!> Inverse lognormal tests
!!
subroutine lognormalDistributionTests

    call testWithLevel( logNormalInverseTestProbOneHalf, &
        "InverseLognormal: Test for non-exceedance probability equal to one-half", 1, "Not implemented yet")
    call testWithLevel( logNormalInverseMatlabTest, &
        "InverseLognormal: Compare Hydra-Ring output and Matlab output for u = -8:8", 1, "Not implemented yet")

end subroutine lognormalDistributionTests



!> Test logNormalInverse where a known value should be returned
!
subroutine logNormalInverseTestProbOneHalf

    real(kind=wp)   :: u, mu, sig
    real(kind=wp)   :: par3
    real(kind=wp)   :: margin

    real(kind=wp)   :: x

    margin  = 0.000001
    mu      = 2.D0
    sig     = 1.D0
    par3    = 0
    u       = 0.D0

    x = logNormalInverse( u, mu, sig, par3 )

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( exp(mu), x, margin, "output should be equal to exp(par1) for non-exceedance probability p = 0.5" )

end subroutine logNormalInverseTestProbOneHalf


!> Test logNormalInverse by comparing with Matlab output for u values ranging from -8 to +8
!> MATLAB results are read from '../probabilistic/testData/logninvMLTest.dat'.
!
subroutine logNormalInverseMatlabTest

    real(kind=wp)               :: mu, sig
    real(kind=wp)               :: par3
    real(kind = wp)             :: margin

    real(kind=wp),  allocatable :: u(:)
    real(kind=wp),  allocatable :: ML(:)
    real(kind=wp),  allocatable :: x(:)

    integer                     :: i
    integer                     :: ifile
    integer                     :: fileLength
    character(len=100)          :: regel
    integer                     :: ios

    open( newunit=ifile, file = trim(testDir) // '/logninvMLTest.dat' )
    !determine length of file
    fileLength = 0
    ios = 0

    do while( ios == 0)
    read( ifile,'(a)', iostat=ios ) regel
        fileLength = fileLength+1
    enddo

    fileLength = fileLength - 1

    allocate( u(fileLength) )
    allocate( ML(fileLength) )
    allocate( x(fileLength) )

    close( ifile )

    open( newunit= ifile, file = trim(testDir) // '/logninvMLTest.dat' )

    !read in u-values and matlab output
    read( ifile,* ) ( u(i), ML(i), i = 1, fileLength )

    !Run lognormal inverse comparison code for all 1000 input, compares old code and new code
    mu      = 2.D0      !par1, mean of log-transformed variable
    sig     = 1.D0      !par2, standard deviation of log-transformed variable
    par3    = 0         !third parameter, horizontal shift
    margin  = 0.001     !margin for comparing Matlab and Hydra-Ring output

    do i=1,fileLength
        x(i) = logNormalInverse( u(i), mu, sig, par3 )
    end do

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( x, ML, margin, "Matlab output and Hydra-Ring output differ" )

    close( ifile )

    deallocate( u, ML, x )

end subroutine logNormalInverseMatlabTest


!--------------------------------------------------------------------------------------
!> Inverse lognormal tests, Type II
!
!!
subroutine lognormalDistributionTypeIITests

    call testWithLevel(logNormalInverseTypeIITestProbOneHalf, &
        "InverseLognormalII: Test for non-exceedance probability equal to one-half", 1)
    call testWithLevel(logNormalInverseIIMatlabTest, &
        "InverseLognormalII: Compare Hydra-Ring output and Matlab output for u = -8:8", 1)
    call testWithLevel(logNormalTypeIITest, "InverseLogNomalTypeII test", 1)

end subroutine lognormalDistributionTypeIITests


!> Test logNormalInverse Type II where a known value should be returned
!> output should be equal to exp(par1) for non-exceedance probability p = 0.5
!
subroutine logNormalInverseTypeIITestProbOneHalf

    real(kind=wp)     :: u, mu, sig, M, S
    real(kind=wp)     :: par3
    real(kind=wp)     :: margin
    real(kind=wp)     :: x
    integer           :: ierr
    character(len=80) :: errorMessage

    margin  = 0.000001                                  !margin for comparing output with known value
    mu      = 2.D0                                      !mu, mean of log-transformed variable
    sig     = 1.D0                                      !sig, standard deviation of log-transformed variable
    M       = exp(mu+sig**2/2)                          !par1, mean of actual variable
    S       = sqrt((exp(sig**2)-1)*exp(2*mu+sig**2))    !par2, standard deviation of actual variable
    par3    = 0                                         !par3, horizontal shift
    u       = 0.D0                                      !u value, equivalent to a probability of 0.5

    x = logNormalInverseII( u, M, S, par3, ierr, errorMessage )
    call assert_equal(ierr, 0, errorMessage)

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( exp(mu), x, margin, "output should be equal to exp(par1) for non-exceedance probability p = 0.5" )

end subroutine logNormalInverseTypeIITestProbOneHalf


!> Test logNormalInverse Type II by comparing with Matlab output for u values ranging from -8 to +8
!> MATLAB results are read from '../probabilistic/testData/logninvMLTest.dat'
!
subroutine logNormalInverseIIMatlabTest

    real(kind=wp)               :: mu, sig, M, S
    real(kind=wp)               :: par3
    real(kind = wp)             :: margin

    real(kind=wp),  allocatable :: u(:)
    real(kind=wp),  allocatable :: ML(:)
    real(kind=wp),  allocatable :: x(:)

    integer                     :: i
    integer                     :: ifile
    integer                     :: fileLength
    character(len=100)          :: regel
    integer                     :: ios
    integer                     :: ierr
    character(len=80)           :: errorMessage

    open( newunit=ifile, file = trim(testDir) // '/logninvMLTest.dat' )
    !determine length of file
    fileLength = 0
    ios = 0

    do while( ios == 0)
        read( ifile,'(a)', iostat=ios ) regel
        fileLength = fileLength+1
    enddo

    fileLength = fileLength - 1

    allocate( u(fileLength) )
    allocate( ML(fileLength) )
    allocate( x(fileLength) )

    close( ifile )

    open( newunit = ifile, file = trim(testDir) // '/logninvMLTest.dat' )

    !read in u-values and matlab output
    read( ifile,* ) ( u(i), ML(i), i = 1, fileLength )

    !Run lognormal inverse comparison code for all 1000 input, compares old code and new code
    mu      = 2.D0                                      !mu, mean of log-transformed variable
    sig     = 1.D0                                      !sig, standard deviation of log-transformed variable
    M       = exp(mu+sig**2/2)                          !par1, mean of actual variable
    S       = sqrt((exp(sig**2)-1)*exp(2*mu+sig**2))    !par2, standard deviation of actual variable
    par3    = 0                                         !par3, horizontal shift
    margin  = 0.001                                     !margin for comparing Matlab and Hydra-Ring output

    do i=1,fileLength
        x(i) = logNormalInverseII( u(i), M, S, par3, ierr, errorMessage )
        call assert_equal(ierr, 0, errorMessage)
    end do

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( x, ML, margin, "Matlab output and Hydra-Ring output differ" )

    close( ifile )

    deallocate( u, ML, x )

end subroutine logNormalInverseIIMatlabTest

!> Test logNormal Type II where a known value should be returned
!> output is fixed with results from the code itself
subroutine logNormalTypeIITest

    real(kind=wp)      :: u, mu, sig, M, S
    real(kind=wp)      :: par3
    real(kind=wp)      :: margin
    real(kind=wp)      :: x(10), expected(10)
    integer            :: i, ierr
    character(len=128) :: errorMessage

    expected(1)  = -4.30258509299405_wp
	expected(2)  = -3.60943791243410_wp
	expected(3)  = -3.20397280432594_wp
	expected(4)  = -2.91629073187416_wp
	expected(5)  = -2.69314718055995_wp
	expected(6)  = -2.51082562376599_wp
	expected(7)  = -2.35667494393873_wp
	expected(8)  = -2.22314355131421_wp
	expected(9)  = -2.10536051565783_wp
	expected(10) = -2.00000000000000_wp

    margin  = 0.000001                                  !margin for comparing output with known value
    mu      = 2.D0                                      !mu, mean of log-transformed variable
    sig     = 1.D0                                      !sig, standard deviation of log-transformed variable
    M       = exp(mu+sig**2/2)                          !par1, mean of actual variable
    S       = sqrt((exp(sig**2)-1)*exp(2*mu+sig**2))    !par2, standard deviation of actual variable
    par3    = 0                                         !par3, horizontal shift

    do i = 1, 10
        u = 0.1_wp * real(i, wp)
        call calculateDistribution( u, x(i), distributionShiftedLognormal2, M, S, par3, 0d0, ierr, errorMessage)
        call assert_equal(ierr, 0, errorMessage)
    enddo

    !If there are any differences, the test will fail and a warning will be triggered
    do i = 1, 10
        call assert_comparable( expected(i), x(i), margin, &
            "output should be equal to exp(par1) for non-exceedance probability p = 0.5" )
    enddo

end subroutine logNormalTypeIITest


!!--------------------------------------------------------------------------------------
!> Inverse Gumbel tests
!!
subroutine gumbelDistributionTests

    call testWithLevel( gumbelInverseTest, "Inverse Gumbel: Test for non-exceedance probability equal to 1/exp(1)", 1, &
        "Not implemented yet")
    call testWithLevel( gumbelInverseMatlabTestA, &
        "Inverse Gumbel (1st method): Compare Hydra-Ring routine with Matlab for u = -8:8", 1, "Not implemented yet")
    call testWithLevel( gumbelInverseMatlabTestB, &
        "Inverse Gumbel (2nd method): compare alternate hydra-ring routine with principal routine", 1, "Not implemented yet")
    call testWithLevel( gumbelInverseInfinityTest, "Inverse Gumbel: is infinity avoided?", 1, "Not implemented yet")

end subroutine gumbelDistributionTests


!> Test gumbelInverse where a known value should be returned
!> output should be equal to rate parameter (par1) for p = 1/exp(1)
!
subroutine gumbelInverseTest

    real(kind=wp)   :: u, a, b, q, p
    real(kind=wp)   :: margin

    real(kind=wp)   :: x

    margin  = 0.000001      !margin for comparing output with known value
    b       = 2.D0          !par1, rate parameter (1/scale)
    a       = 1.D0          !par2, location parameter
    u       = -.34D0        !u value, associated with p = 1/exp(1) (doesn't need to be exact, only used conditionally for 'order of magnitude' in code)
    p       = 1.D0/exp(1.)  !non-exceedance probability
    q       = 1-p           !exceedance probability

    x = gumbelInverse( u, a, b, p, q )

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( a, x, margin, "output should be equal to rate parameter (par1) for p = 1/exp(1)" )

end subroutine gumbelInverseTest


!> Test gumbelInverse by comparing with Matlab output for u values ranging from -8 to +8
!> MATLAB results are read from file '../probabilistic/testData/gumbelinvMLTest.dat'
subroutine gumbelInverseMatlabTestA
    call gumbelInverseMatlabTest('A')
end subroutine gumbelInverseMatlabTestA

!> Test gumbelInverse by compare alternate hydra-ring routine with principal routine"
subroutine gumbelInverseMatlabTestB
    call gumbelInverseMatlabTest('B')
end subroutine gumbelInverseMatlabTestB

!> test either with matlab results or with alternate hydra-ring routine
subroutine gumbelInverseMatlabTest(typeTest)
    character, intent(in) :: typeTest
    real(kind = wp) :: margin

    real(kind=wp),  allocatable :: u(:)
    real(kind=wp),  allocatable :: p(:)
    real(kind=wp),  allocatable :: q(:)
    real(kind=wp),  allocatable :: a(:)
    real(kind=wp),  allocatable :: b(:)
    real(kind=wp),  allocatable :: r(:)
    real(kind=wp),  allocatable :: M(:)
    real(kind=wp),  allocatable :: S(:)
    real(kind=wp),  allocatable :: ML(:)

    real(kind=wp),  allocatable :: x(:)
    real(kind=wp),  allocatable :: x2(:)

    integer             :: i
    integer             :: ifile
    integer             :: fileLength
    character(len=100)  :: regel
    integer             :: ios, ierr

    open( newunit = ifile, file = trim(testDir) // '/gumbelinvMLTest.dat' )
    !determine length of file
    fileLength = 0
    ios = 0

    do while( ios == 0)
        read( ifile,'(a)', iostat=ios ) regel
        fileLength = fileLength+1
    enddo

    fileLength = fileLength - 1

    allocate( u(fileLength), p(fileLength), q(fileLength), a(fileLength), &
     b(fileLength), r(fileLength), M(fileLength), S(fileLength), ML(fileLength), &
     x(fileLength), x2(fileLength), stat = ierr)
    call assert_equal(ierr, 0, "allocate error")
    if (ierr /= 0) return

    close( ifile )

    !read in u-values and matlab output
    open( newunit = ifile, file = trim(testDir) // '/gumbelinvMLTest.dat' )

    read( ifile,* ) ( u(i), p(i), a(i), b(i), r(i), M(i), S(i), ML(i), i = 1,fileLength )

    !Run weibull inverse comparison code for all input, compares matlab with hydra-ring
    margin  = 0.001D0
    q       = 1.D0 - p

    do i=1,fileLength
        x(i) = gumbelInverse( u(i), a(i), r(i), p(i), q(i) )
    end do
    if (typeTest == 'A') then

        !If there are any differences, the test will fail and a warning will be triggered
        call assert_comparable( x, ML, margin, "Matlab output and Hydra-Ring output differ" )

    else

        !check alternate Gumbel code
        do i=1,fileLength
            x2(i) = gumbelInverseII( u(i), M(i), S(i), p(i), q(i) )
        end do

        !If there are any differences, the test will fail and a warning will be triggered
        call assert_comparable( x, x2, margin, "Hydra-Ring gumbel inverse and alternate gumbel inverse code output differ" )
    endif

    close( ifile )

    deallocate( u, a, b, r, M, S, ML, x )

end subroutine gumbelInverseMatlabTest


!> Test gumbelInverse in the tail
subroutine gumbelInverseInfinityTest

    real(kind=wp) :: u
    real(kind=wp) :: p
    real(kind=wp) :: q
    real(kind=wp) :: a
    real(kind=wp) :: r
    real(kind=wp) :: x


    !Run weibull inverse comparison code for all input, compares matlab with hydra-ring
    p       = 0.0_wp
    q       = 1.D0 - p
    a       = 0.0d0
    r       = 1.0d0 / 3.0d0

    u = -40.0
    call PQfromBeta( u, p, q )
    x = gumbelInverse( u, a, r, p, q )
    call assert_true( abs(2.0*x) > abs(x), "Result at u = -40 should be finite" )

    u = +40.0
    call PQfromBeta( u, p, q )
    x = gumbelInverse( u, a, r, p, q )
    call assert_true( abs(2.0*x) > abs(x), "Result at u = +40 should be finite" )

end subroutine gumbelInverseInfinityTest


!!--------------------------------------------------------------------------------------
!> Inverse Weibull tests
!!
subroutine weibullDistributionTests

    call testWithLevel( weibInverseMatlabTest, "Inverse Weibull: Compare Hydra-Ring output and Matlab output for u = -8:8", 1)

end subroutine weibullDistributionTests


!> Test weibInverse by comparing with Matlab output for u values ranging from -8 to +8
subroutine weibInverseMatlabTest

    real(kind = wp)             :: margin

    real(kind=wp),  allocatable :: u(:)
    real(kind=wp),  allocatable :: p(:)
    real(kind=wp),  allocatable :: q(:)
    real(kind=wp),  allocatable :: a(:)
    real(kind=wp),  allocatable :: b(:)
    real(kind=wp),  allocatable :: c(:)
    real(kind=wp),  allocatable :: ML(:)

    real(kind=wp),  allocatable :: x(:)

    integer                     :: i
    integer                     :: ifile
    integer                     :: fileLength
    character(len=100)          :: regel
    integer                     :: ios

    open( newunit = ifile, file = trim(testDir) // '/weibinvMLTest.dat' )
    !determine length of file
    fileLength = 0
    ios = 0

    do while( ios == 0)
        read( ifile,'(a)', iostat=ios ) regel
        fileLength = fileLength+1
    enddo

    fileLength = fileLength - 1

    allocate( u(fileLength) )
    allocate( p(fileLength) )
    allocate( q(fileLength) )
    allocate( a(fileLength) )
    allocate( b(fileLength) )
    allocate( c(fileLength) )
    allocate( ML(fileLength) )
    allocate( x(fileLength) )

    !read in u-values and matlab output
    rewind ( ifile )
    read( ifile,* ) ( u(i), p(i), a(i), b(i), ML(i), i = 1,fileLength )
    close( ifile )

    !Run weibull inverse comparison code for all input, compares matlab with hydra-ring
    c       = 0
    margin  = 0.001D0
    q       = 1.D0 - p

    do i=1,fileLength
        x(i) = weibullInverse(a(i), b(i), c(i), q(i))
    end do

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( x, ML, margin, "Matlab output and Hydra-Ring output differ" )

    deallocate( u, p, a, b, c, ML, x )

end subroutine weibInverseMatlabTest


!!--------------------------------------------------------------------------------------
!> Inverse Pareto tests
!!
subroutine paretoDistributionTests

    call testWithLevel( paretoInverseMatlabTest, &
        "Inverse Generalized Pareto Distribution: Compare Hydra-Ring and Matlab output", 1, "Not implemented yet")

end subroutine paretoDistributionTests


!> Test paretoInverse by comparing with Matlab output for u values ranging from -8 to +8
subroutine paretoInverseMatlabTest

    real(kind = wp)             :: margin

    real(kind=wp),  allocatable :: u(:)
    real(kind=wp),  allocatable :: p(:)
    real(kind=wp),  allocatable :: q(:)
    real(kind=wp),  allocatable :: k(:)
    real(kind=wp),  allocatable :: k2(:)
    real(kind=wp),  allocatable :: s(:)
    real(kind=wp),  allocatable :: l(:)
    real(kind=wp),  allocatable :: ML(:)
    real(kind=wp),  allocatable :: MLzero(:)

    real(kind=wp),  allocatable :: x(:)
    real(kind=wp),  allocatable :: x2(:)

    integer                     :: i
    integer                     :: ifile
    integer                     :: fileLength
    character(len=100)          :: regel
    integer                     :: ios

    open( newunit = ifile, file = trim(testDir) // '/gpinvMLTest.dat' )
    !determine length of file
    fileLength= 0
    ios       = 0

    do while( ios == 0)
        read( ifile,'(a)', iostat=ios ) regel
        fileLength = fileLength+1
    enddo

    fileLength= fileLength - 1

    allocate( u(fileLength) )
    allocate( p(fileLength) )
    allocate( q(fileLength) )
    allocate( k(fileLength) )
    allocate( k2(fileLength) )
    allocate( s(fileLength) )
    allocate( l(fileLength) )
    allocate( ML(fileLength) )
    allocate( MLzero(fileLength) )
    allocate( x(fileLength) )
    allocate( x2(fileLength) )

    close( ifile )

    !read in u-values and matlab output
    open( newunit = ifile, file = trim(testDir) // '/gpinvMLTest.dat' )

    read( ifile,* ) ( u(i), p(i), k(i), k2(i), s(i), l(i), ML(i), MLzero(i), i = 1,fileLength )

    !Run pareto inverse comparison code for all input, compares matlab with hydra-ring
    margin  = 0.001D0
    q       = 1.D0 - p

    do i= 1, fileLength
        x(i) = paretoInverse( l(i), s(i), k(i), q(i))
    end do
!   If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( x, ML, margin, "Matlab output and Hydra-Ring output differ" )

    do i=1,fileLength
        x2(i) = paretoInverse( l(i), s(i), k2(i), q(i))
    end do

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( x2, MLzero, margin, "Matlab output and Hydra-Ring output differ" )

    close( ifile )

    deallocate( u, p, q, k, s, l, ML, x )

end subroutine paretoInverseMatlabTest


!!--------------------------------------------------------------------------------------
!> Inverse triangular distribution tests
!!
subroutine triangularDistributionTests

    call testWithLevel(TriangularInverseTestProbZero, &
        "Triangular inverse test: Test for probability equal to zero", 1, "Not implemented yet")
    call testWithLevel(TriangularInverseTestProbOne, &
        "Triangular inverse test: Test for probability equal to one", 1, "Not implemented yet")
    call testWithLevel(TriangularInverseTestProbContinuous, &
        "Triangular inverse test: Test for continuity around x = b", 1, "Not implemented yet")

end subroutine triangularDistributionTests


!> inverse triangular test: tests the value of x for a probability equal to zero
subroutine TriangularInverseTestProbZero

    real(kind=wp)   :: a, b, c  !<  parameters of triangular distribution
    real(kind=wp)   :: p, q     !<  non-exceedance and exceedance probabilities
    real(kind=wp)   :: margin   !<  margin for comparing output
    real(kind=wp)   :: x        !<  output of triangular inverse code

    margin  = 0.000001
    a       = 2.D0
    b       = 5.D0
    c       = 10.D0
    p       = 0.D0
    q       = 1.D0 - p

    x = triangularInverse( a, b, c, p, q )

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( a, x, margin, "output should be equal to par1 for non-exceedance probability p = 0" )

end subroutine TriangularInverseTestProbZero


!> inverse triangular test: tests the value of x for a probability equal to one
subroutine TriangularInverseTestProbOne

    real(kind=wp)   :: a, b, c  !<  parameters of triangular distribution
    real(kind=wp)   :: p, q     !<  non-exceedance and exceedance probabilities
    real(kind=wp)   :: margin   !<  margin for comparing output
    real(kind=wp)   :: x        !<  output of triangular inverse code

    margin  = 0.000001
    a       = 2.D0
    b       = 5.D0
    c       = 10.D0
    p       = 1.D0
    q       = 1.D0 - p

    x = triangularInverse( a, b, c, p, q )

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( c, x, margin, "output should be equal to par3 for non-exceedance probability p = 0" )

end subroutine TriangularInverseTestProbOne


!> inverse triangular test: tests the continuity around the value x = b
subroutine TriangularInverseTestProbContinuous

    real(kind=wp)   :: a, b, c  !<  parameters of triangular distribution
    real(kind=wp)   :: p, q     !<  non-exceedance and exceedance probabilities
    real(kind=wp)   :: margin   !<  margin for comparing output
    real(kind=wp)   :: x1, x2   !<  output of triangular inverse code

    margin  = 0.0000001
    a       = 2.D0
    b       = 5.D0
    c       = 10.D0

    !test with probability for x slightly greater than b
    p   = (b - a)/(c - a) + 10E-10
    q   = 1.D0 - p
    x1  = triangularInverse( a, b, c, p, q )

    !test with probability for x slightly less than than b
    p   = (b - a)/(c - a) - 10E-10
    q   = 1.D0 - p
    x2  = triangularInverse( a, b, c, p, q )

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( x1, x2, margin, "output should be equal at x = b" )

end subroutine TriangularInverseTestProbContinuous


!> inverse shifted lognormal distribution - changed code
!!
!! Tests to confirm that changes in the code have no effect on the results!
!! Original code and modifications are both included in this code and the
!! outputs are compared to see if they agree within a specified margin.
!!
!! Lognormal distribution:
!!
!! PDF:      f(x) = 1/[(x-eps)*sqrt(2*pi*sigma^2)] * exp[-(ln(x-eps)-mu)^2/(2*sigma^2)]
!!
!! Inverse:  no explicit form
!!   @ingroup Probabilistic
function logNormalInverse_compare(u, par1, par2, par3, logNormalInverse_revised, check)

    real(kind = wp), intent(in)     :: u        !< standard normally distributed variable u
    real(kind = wp), intent(in)     :: par1     !< mu, lognormal parameter
    real(kind = wp), intent(in)     :: par2     !< sigma, lognormal parameter
    real(kind = wp), intent(in)     :: par3     !< eps, horizontal shifting

    real(kind = wp), intent(out)    :: logNormalInverse_revised
    logical        , intent(out)    :: check

    real(kind = wp), parameter      :: qMin = 1.0d-300
    real(kind = wp), parameter      :: upperLog  = 700.0d0
    real(kind = wp)                 :: tmpM
    real(kind = wp)                 :: tmpS
    real(kind = wp)                 :: sigXtr
    real(kind = wp)                 :: gemXtr
    real(kind = wp)                 :: logNormalInverse_compare

    real(kind = wp), parameter      :: margin    =   0.001d0

    ! Original code
    !
    ! Compute the mean value
    tmpM = par3 + exp(par1 + par2 * par2 / 2.D0)
    ! Compute the standard deviation
    tmpS = exp(par1 + par2 * par2 / 2.D0) * sqrt(exp(par2 * par2) - 1.D0)

    ! Constrain the mean not to be smaller than 10^-300 (numerical problems)
    if ( tmpM < qmin ) then
        tmpM = qmin
    end if

    ! Computing the sigma and mean of the log-transformed variable (not necessary - this is the input!)
    sigXtr = sqrt(log(1.0D0 + (tmpS / (tmpM - par3))**2))
    gemXtr = log(tmpM - par3) - 0.5D0 * sigXtr * sigXtr

    logNormalInverse_compare = gemXtr + u * sigXtr

    if (logNormalInverse_compare > upperLog) then
        logNormalInverse_compare = exp(upperLog) + par3
    else
        logNormalInverse_compare = exp(logNormalInverse_compare) + par3
    end if

    ! modified code

    lognormalInverse_revised = par1 + u*par2

    if (lognormalInverse_revised > upperlog) then
        lognormalInverse_revised = exp(upperlog) + par3
    else
        lognormalInverse_revised = exp(lognormalInverse_revised) + par3
    end if

    ! compare code

    check = equalRealsRelative(lognormalInverse_compare,lognormalInverse_revised,margin)

end function lognormalInverse_compare

subroutine conditionalWeibInverseExcelTest

    real(kind = wp) :: margin
    real(kind=wp)   :: actual
    real(kind=wp)   :: expected
    real(kind=wp)   :: distParameter1, distParameter2, distParameter3, distParameter4, p, q, f
    margin = 1e-6

    !Conditional weibull distribution parameters from (ADD FILENAME)
    distParameter1 = 0.0157d0
    distParameter2 = 0.57d0
    distParameter3 = 1.97d0
    distParameter4 = 7.237d0

    !Test 1: frequency = 0.00025
    !-----------------------------
    f = 0.00025

    !get p and q from frequency
    p = exp(-f)
    q = 1 - p

    !Expected value from Excel code:
     expected = 4.762394681d0

    !Compute conditional Weibull inverse with Hydra Ring
    actual = conditionalWeibullInverse(distParameter1, distParameter2, distParameter3, distParameter4, p, q)

    !Compare Hydra Ring results with Excel code
    call assert_comparable( actual, expected, margin, "Excel output and Hydra-Ring output differ" )

    !Test 2: frequency = 0.0001
    !-----------------------------
    f = 0.0001

    !get p and q from frequency
    p = exp(-f)
    q = 1 - p

    !Expected value from Excel code:
     expected = 5.060938814d0

    !Compute conditional Weibull inverse with Hydra Ring
    actual = conditionalWeibullInverse(distParameter1, distParameter2, distParameter3, distParameter4, p, q)

    !Compare Hydra Ring results with Excel code
    call assert_comparable( actual, expected, margin, "Excel output and Hydra-Ring output differ" )

    !Test 3: frequency = 0.0005
    !-----------------------------

    f = 0.0005

    !get p and q from frequency
    p = exp(-f)
    q = 1 - p

    !Expected value from Excel code:
     expected = 4.541758606d0

    !Compute conditional Weibull inverse with Hydra Ring
    actual = conditionalWeibullInverse(distParameter1, distParameter2, distParameter3, distParameter4, p, q)

    !Compare Hydra Ring results with Excel code
    call assert_comparable( actual, expected, margin, "Excel output and Hydra-Ring output differ" )

    !Test 4: frequency = 0.01
    !-----------------------------

    f = 0.01

    !get p and q from frequency
    p = exp(-f)
    q = 1 - p

    !Expected value from Excel code:
     expected = 3.640516417d0

    !Compute conditional Weibull inverse with Hydra Ring
    actual = conditionalWeibullInverse(distParameter1, distParameter2, distParameter3, distParameter4, p, q)

    !Compare Hydra Ring results with Excel code
    call assert_comparable( actual, expected, margin, "Excel output and Hydra-Ring output differ" )

end subroutine conditionalWeibInverseExcelTest

!!-----------------------------------------------------------------------------------------------------------------------------------------
!> Test modified GumbelInverse by comparing with excel results \n
!! \todo find a location where the test can be implemented

subroutine modifiedGumbelInverseExcelTest

    real(kind = wp)  :: margin
    real(kind=wp)   :: actual
    real(kind=wp)   :: expected
    real(kind=wp)   :: a, b, c, p, q
    margin = 1e-6

    !If there are any differences, the test will fail and a warning will be triggered
    !WHERE DO THESE NUMBERS COME FROM???
    a = 7.292d-04
    b = 2.536d-01
    c = -6.351d-01

    !test 1
    q = 0.0001d0
    p = 1 - q

    actual = modifiedGumbelInverse(0.0D0 , a, b, c, p, q)
    expected = 35.2497173680827d0 !CHANGE THIS TO THE EXCEL VALUE - CURRENTLY EXCEL VALUE (-6) IS WRONG!!
    call assert_comparable( actual, expected, margin, "Excel output and Hydra-Ring output differ" )

    !test 2
    p = 0.005d0
    p = 1 - q
    actual = modifiedGumbelInverse(0.0D0, 5.896d-04, 0.3107d0, -1.786d0, p, q)
    expected = 33.2890811405951 !CHANGE THIS TO THE EXCEL VALUE - CURRENTLY EXCEL VALUE (-6) IS WRONG!!
    call assert_comparable( actual, expected, margin, "Excel output and Hydra-Ring output differ" )

end subroutine modifiedGumbelInverseExcelTest

!!-----------------------------------------------------------------------------------------------------------------------------------------
!> Test modified GumbelInverse by comparing with excel results \n

subroutine modifiedGumbelDistributionTests

    call testWithLevel( modifiedGumbelInverseExcelTest, &
        "Inverse modified Gumbel: Compare Hydra-Ring output and excel output", 1, "Not implemented yet")

end subroutine modifiedGumbelDistributionTests

!!-----------------------------------------------------------------------------------------------------------------------------------------
!> Test RayleighNInverse by comparing with Matlab results \n

subroutine rayleighNDistributionTests

    call testWithLevel( rayleighNDistributionTest, &
        "RayleighNInverse: Compare Hydra-Ring output and Matlab output for N = 1 and u = -8:8", 1)
    call testWithLevel( rayleigh5000DistributionTest, "RayleighNInverse: for N = 5000 and u = -8:8", 1)

end subroutine rayleighNDistributionTests

!!-----------------------------------------------------------------------------------------------------------------------------------------
!> Test modified Inverse Rayleigh N distribution by comparing with Matlab results \n

subroutine rayleighNDistributionTest

    real(kind = wp)             :: margin

    real(kind=wp),  allocatable :: u(:)
    real(kind=wp),  allocatable :: p(:)
    real(kind=wp),  allocatable :: q(:)
    real(kind=wp),  allocatable :: sigma(:)
    real(kind=wp),  allocatable :: N(:)
    real(kind=wp),  allocatable :: ML(:)

    real(kind=wp),  allocatable :: x(:)

    integer                     :: i
    integer                     :: ifile
    integer                     :: fileLength
    character(len=100)          :: regel
    integer                     :: ios

    open( newunit=ifile, file = trim(testDir) // '/raylNinvMLTest.dat' )

    !determine length of file
    fileLength = 0
    ios = 0

    do while( ios == 0)
        read( ifile,'(a)', iostat=ios ) regel
        fileLength = fileLength+1
    enddo

    fileLength = fileLength - 1

    allocate( u(fileLength) )
    allocate( p(fileLength) )
    allocate( q(fileLength) )
    allocate( sigma(fileLength) )
    allocate( N(fileLength) )
    allocate( ML(fileLength) )
    allocate( x(fileLength) )

    !Read in u-values and Matlab output
    rewind(ifile)

    read( ifile,* ) ( u(i), p(i), sigma(i), ML(i), i = 1,fileLength )

    !Run Rayleigh N inverse in Hydra-Ring with N = 1
    N       = 1
    margin  = 0.001D0
    q       = 1.0d0 - p

    do i=1,fileLength
        x(i) = rayleighNInverse(sigma(i), N(i), 0.0_wp, q(i))
    end do

    !If there are any differences, the test will fail and a warning will be triggered
    call assert_comparable( x, ML, margin, "Matlab output and Hydra-Ring output differ" )

    close( ifile )

    deallocate( u, p, sigma, N, ML, x )

end subroutine rayleighNDistributionTest

!> check that rayleighNInverse is always finite for large u and N = 5000
!! (as used for parameter 'Ratio of H_max,0 to H_S for N waves m_Ray' in structural failure)
subroutine rayleigh5000DistributionTest
    integer, parameter          :: N = 5000
    real(kind = wp), parameter  :: margin = 0.001D0
    real(kind=wp)               :: x
    integer                     :: i
    integer, parameter          :: umax = 8
    integer, parameter          :: nn = 10   ! steps of 0.1, nn = 1 / 0.1
    integer, parameter          :: length = nn * umax
    real(kind=wp), parameter    :: sigma = 0.5_wp
    real(kind=wp)               :: u
    real(kind=wp)               :: p
    real(kind=wp)               :: q
    real(kind=wp)               :: distParameter2
    real(kind=wp), parameter    :: expected(-length:length) = [ &
       1.57589_wp, 1.57943_wp, 1.58300_wp, 1.58672_wp, 1.59051_wp, 1.59434_wp, 1.59820_wp, 1.60209_wp, &
       1.60602_wp, 1.60999_wp, 1.61399_wp, 1.61804_wp, 1.62213_wp, 1.62625_wp, 1.63042_wp, 1.63465_wp, &
       1.63890_wp, 1.64321_wp, 1.64756_wp, 1.65196_wp, 1.65641_wp, 1.66092_wp, 1.66547_wp, 1.67007_wp, &
       1.67471_wp, 1.67944_wp, 1.68419_wp, 1.68903_wp, 1.69392_wp, 1.69887_wp, 1.70388_wp, 1.70895_wp, &
       1.71410_wp, 1.71931_wp, 1.72459_wp, 1.72994_wp, 1.73537_wp, 1.74088_wp, 1.74646_wp, 1.75213_wp, &
       1.75788_wp, 1.76372_wp, 1.76964_wp, 1.77566_wp, 1.78178_wp, 1.78799_wp, 1.79431_wp, 1.80073_wp, &
       1.80726_wp, 1.81390_wp, 1.82066_wp, 1.82754_wp, 1.83454_wp, 1.84167_wp, 1.84894_wp, 1.85634_wp, &
       1.86389_wp, 1.87159_wp, 1.87945_wp, 1.88746_wp, 1.89565_wp, 1.90401_wp, 1.91255_wp, 1.92127_wp, &
       1.93020_wp, 1.93933_wp, 1.94867_wp, 1.95823_wp, 1.96802_wp, 1.97806_wp, 1.98834_wp, 1.99889_wp, &
       2.00971_wp, 2.02081_wp, 2.03221_wp, 2.04392_wp, 2.05595_wp, 2.06832_wp, 2.08103_wp, 2.09411_wp, &
       2.10757_wp, 2.12143_wp, 2.13569_wp, 2.15038_wp, 2.16550_wp, 2.18109_wp, 2.19714_wp, 2.21369_wp, &
       2.23073_wp, 2.24830_wp, 2.26639_wp, 2.28503_wp, 2.30422_wp, 2.32398_wp, 2.34432_wp, 2.36524_wp, &
       2.38675_wp, 2.40886_wp, 2.43157_wp, 2.45488_wp, 2.47879_wp, 2.50331_wp, 2.52842_wp, 2.55412_wp, &
       2.58040_wp, 2.60727_wp, 2.63470_wp, 2.66269_wp, 2.69123_wp, 2.72032_wp, 2.74992_wp, 2.78004_wp, &
       2.81067_wp, 2.84178_wp, 2.87336_wp, 2.90542_wp, 2.93792_wp, 2.97089_wp, 3.00426_wp, 3.03805_wp, &
       3.07224_wp, 3.10682_wp, 3.14177_wp, 3.17710_wp, 3.21279_wp, 3.24882_wp, 3.28519_wp, 3.32186_wp, &
       3.35888_wp, 3.39620_wp, 3.43382_wp, 3.47172_wp, 3.50990_wp, 3.54836_wp, 3.58708_wp, 3.62606_wp, &
       3.66528_wp, 3.70475_wp, 3.74445_wp, 3.78436_wp, 3.82452_wp, 3.86489_wp, 3.90546_wp, 3.94623_wp, &
       3.98720_wp, 4.02836_wp, 4.06970_wp, 4.11123_wp, 4.15293_wp, 4.19480_wp, 4.23683_wp, 4.27903_wp, &
       4.32138_wp, 4.36388_wp, 4.40654_wp, 4.44933_wp, 4.49227_wp, 4.53534_wp, 4.57854_wp, 4.62187_wp, &
       4.66533_wp]

    distParameter2 = real(N, wp)
    do i= -length, length
        u = i / real(nn, wp)
        call pQFromBeta ( u, p, q )
        x = rayleighNInverse(sigma, distParameter2, 0.0_wp, q)
        call assert_comparable(x, expected(i), 1d-5, "test on return value of rayleighNInverse compared to previous computation")
    end do

end subroutine rayleigh5000DistributionTest

!> test for distribution function conditionalWeibull
subroutine conditionalWeibullTest
    real(kind=wp) :: x
    real(kind=wp) :: y
    real(kind=wp) :: expected
    real(kind=wp), parameter :: distParameter1 = 1.0_wp
    real(kind=wp), parameter :: distParameter2 = 1.0_wp
    real(kind=wp), parameter :: distParameter3 = 1.0_wp
    real(kind=wp), parameter :: distParameter4 = 1.0_wp
    integer :: i
    
    do i = -5, 5
        x = real(i, wp)
        y = conditionalWeibull(distParameter1, distParameter2, distParameter3, distParameter4, x)
        expected = exp(1.0_wp - x)
    
        call assert_comparable(y, expected, margin, 'difference in conditionalWeibullTest too big')
    enddo

end subroutine conditionalWeibullTest

!> test for general entry of distribution functions
subroutine calculateDistributionTest
    real(kind=wp) :: x
    real(kind=wp) :: u
    real(kind=wp) :: expected, minimum, maximum
    real(kind=wp) :: logNormalIIresults(11)
    real(kind=wp), parameter :: distParameter1 = 2.0_wp
    real(kind=wp), parameter :: distParameter2 = 1.0_wp
    real(kind=wp), parameter :: distParameter3 = 1.0_wp
    real(kind=wp), parameter :: distParameter4 = 1.0_wp
    integer :: i, ierr
    character(len=128) :: message
    
    logNormalIIresults = [-0.416277305578849_wp, 0.416277305578849_wp, 0.903290532908586_wp, &
                           1.24883191673655_wp, 1.51685463659623_wp, 1.73584514406628_wp, &
                           1.92099897993647_wp, 2.08138652789424_wp, 2.22285837139602_wp, &
                           2.34940924775393_wp, 2.46388844050237_wp]

    do i = -5, 5
        x = real(i, wp)
        
        !
        ! test distributionNormal
        !
        call calculateDistribution(x, u, distributionNormal, distParameter1, distParameter2, distParameter3, distParameter4, &
            ierr, message)
        call assert_equal(ierr, 0, message)
        expected = (x - distParameter1) / distParameter2
        call assert_comparable(u, expected, margin, 'difference in distributionNormal too big')

        !
        ! test distributionShiftedLognormal2
        !
        x = real(i+8, wp) * 0.5_wp
        call calculateDistribution(x, u, distributionShiftedLognormal2, distParameter1, distParameter2, distParameter3, &
            distParameter4, ierr, message)
        call assert_equal(ierr, 0, message)
        expected = logNormalIIresults( i+6 )
        call assert_comparable(u, expected, margin, 'difference in distributionShiftedLognormal2 too big')

    enddo
    
    ! special cases truncated Normal
    minimum = -3.0_wp
    maximum =  3.0_wp
    x = minimum
    call calculateDistribution(x, u, distributionTruncatedNormal, distParameter1, distParameter2, minimum, maximum, ierr, message)
    call assert_equal(ierr, 0, message)
    x = maximum
    call calculateDistribution(x, u, distributionTruncatedNormal, distParameter1, distParameter2, minimum, maximum, ierr, message)
    call assert_equal(ierr, 0, message)

    ! error handling truncated Normal: check x is in range [minimum - maximum]
    minimum = -3.0_wp
    maximum =  3.0_wp
    x = -3.1_wp
    call calculateDistribution(x, u, distributionTruncatedNormal, distParameter1, distParameter2, minimum, maximum, ierr, message)
    call assert_true(ierr == 0, message)
    
    ! test not implemented distributions
    !
    call calculateDistribution(x, u, distributionUniform, distParameter1, distParameter2, distParameter3, distParameter4, &
        ierr, message)
    !call assert_equal(message, "Not yet implemented", "test distributionUniform")
    !call calculateDistribution(x, u, distributionShiftedLognormal, distParameter1, distParameter2, distParameter3, distParameter4,&
    !    ierr, message)
    !call calculateDistribution(x, u, distributionShiftedExponential, distParameter1, distParameter2, distParameter3, &
    !    distParameter4, ierr, message)
    !call calculateDistribution(x, u, distributionGumbelDistribution, distParameter1, distParameter2, distParameter3, &
    !    distParameter4, ierr, message)
    !call calculateDistribution(x, u, distributionGumbelDistribution2, distParameter1, distParameter2, distParameter3, &
    !    distParameter4, ierr, message)
    call calculateDistribution(x, u, distributionWeibull, distParameter1, distParameter2, distParameter3, distParameter4, &
        ierr, message)
    call calculateDistribution(x, u, distributionRayleigh, distParameter1, distParameter2, distParameter3, distParameter4, &
        ierr, message)
    !call calculateDistribution(x, u, distributionPareto, distParameter1, distParameter2, distParameter3, distParameter4, &
    !    ierr, message)
    !call calculateDistribution(x, u, distributionTriangular, distParameter1, distParameter2, distParameter3, distParameter4, &
    !    ierr, message)
    !call calculateDistribution(x, u, distributionLogLinearInterpolation, distParameter1, distParameter2, distParameter3, &
    !    distParameter4, ierr, message)
    !call calculateDistribution(x, u, distributionConditionalWeibull, distParameter1, distParameter2, distParameter3, &
    !    distParameter4, ierr, message)
    call calculateDistribution(x, u, -999, distParameter1, distParameter2, distParameter3, distParameter4, ierr, message)
    call assert_equal(message, "Unknown distribution function - code: -999", "test wrong input")
end subroutine calculateDistributionTest

!> test for general entry of inverse distribution functions
subroutine calculateDistributionInverseTest
    real(kind=wp) :: x
    real(kind=wp) :: u
    real(kind=wp) :: p
    real(kind=wp) :: q
    real(kind=wp) :: expected
    !real(kind=wp) :: logNormalIIresults(11)
    real(kind=wp), parameter :: distParameter1 = 2.0_wp
    real(kind=wp), parameter :: distParameter2 = 1.0_wp
    real(kind=wp), parameter :: distParameter3 = 1.0_wp
    real(kind=wp), parameter :: distParameter4 = 1.0_wp
!   Previous parameter settings are not convenient for the beta distribution. Use alternative setting
    real(kind=wp), parameter :: distBetaParameter1 =-2.0_wp
    real(kind=wp), parameter :: distBetaParameter2 = 8.0_wp
    real(kind=wp), parameter :: distBetaParameter3 = 3.0_wp
    real(kind=wp), parameter :: distBetaParameter4 = 6.0_wp
    integer :: i
    integer :: ierr
    character(len=128) :: message
    
    do i = 0, 10
        u = real(i, wp) * 0.1_wp
        
        !
        ! test distributionDeterministic
        !
        call calculateDistributionInverse(u, x, distributionDeterministic, distParameter1, distParameter2, &
            distParameter3, distParameter4, ierr, message)
        expected = distParameter1
        call assert_comparable(x, expected, margin, 'difference in distributionDeterministic too big')

        !
        ! test distributionUniform
        !
        call calculateDistributionInverse(u, x, distributionUniform, distParameter1, distParameter2, distParameter3, &
            distParameter4, ierr, message)
        if (ierr == 0) then
            call pQFromBeta ( u, p, q )
            expected = uniformInverse( p, distParameter1, distParameter2 )
            call assert_comparable(x, expected, margin, 'difference in distributionUniform too big')
        end if

        !
        ! test distributionNormal
        !
        call calculateDistributionInverse(u, x, distributionNormal, distParameter1, distParameter2, distParameter3, &
            distParameter4, ierr, message)
        expected = distParameter1 + u * distParameter2
        call assert_comparable(x, expected, margin, 'difference in distributionNormal too big')
        
        !
        ! test distributionShiftedLognormal
        !
        !call calculateDistributionInverse(u, x, distributionShiftedLognormal, distParameter1, distParameter2, &
        !    distParameter3, distParameter4, ierr, message)
        !expected = logNormalInverse(u, distParameter1, distParameter2, distParameter3)
        !call assert_comparable(x, expected, margin, 'difference in distributionShiftedLognormal too big')

        !
        ! test distributionShiftedLognormal2
        !
        call calculateDistributionInverse(u, x, distributionShiftedLognormal2, distParameter1, distParameter2, &
            distParameter3, distParameter4, ierr, message)
        expected = logNormalInverseII(u, distParameter1, distParameter2, distParameter3, ierr, message )
        call assert_equal(ierr, 0, message)
        call assert_comparable(x, expected, margin, 'difference in distributionShiftedLognormal2 too big')

        !
        ! test distributionShiftedExponential
        !
        !call calculateDistributionInverse(u, x, distributionShiftedExponential, distParameter1, distParameter2, &
        !    distParameter3, distParameter4, ierr, message)
        !call pQFromBeta ( u, p, q )
        !expected = exponentialInverse(distParameter1, distParameter2, q)
        !call assert_comparable(x, expected, margin, 'difference in distributionShiftedExponential too big')

        !
        ! test distributionGumbelDistribution
        !
        !call calculateDistributionInverse(u, x, distributionGumbelDistribution, distParameter1, distParameter2, &
        !    distParameter3, distParameter4, ierr, message)
        !call pQFromBeta ( u, p, q )
        !expected = gumbelInverse( u, distParameter1, distParameter2, p, q )
        !call assert_comparable(x, expected, margin, 'difference in distributionGumbelDistribution too big')

        !
        ! test distributionGumbelDistribution2
        !
        !call calculateDistributionInverse(u, x, distributionGumbelDistribution2, distParameter1, distParameter2, &
        !    distParameter3, distParameter4, ierr, message)
        !call pQFromBeta ( u, p, q )
        !expected = gumbelInverseII( u, distParameter1, distParameter2, p, q )
        !call assert_comparable(x, expected, margin, 'difference in distributionGumbelDistribution2 too big')

        !
        ! test distributionWeibull
        !
        call calculateDistributionInverse(u, x, distributionWeibull, distParameter1, distParameter2, distParameter3, &
            distParameter4, ierr, message)
        call pQFromBeta ( u, p, q )
        expected = weibullInverse( distParameter1, distParameter2, distParameter3, q )
        call assert_comparable(x, expected, margin, 'difference in distributionWeibull too big')

        !
        ! test distributionRayleigh
        !
        call calculateDistributionInverse(u, x, distributionRayleigh, distParameter1, distParameter2, distParameter3, &
            distParameter4, ierr, message)
        call pQFromBeta ( u, p, q )
        expected = rayleighInverse( distParameter1, distParameter2, distParameter3, q )
        call assert_comparable(x, expected, margin, 'difference in distributionRayleigh too big')

        !
        ! test distributionRayleighN
        !
        call calculateDistributionInverse(u, x, distributionRayleighN, distParameter1, distParameter2, distParameter3, &
            distParameter4, ierr, message)
        call pQFromBeta ( u, p, q )
        expected = rayleighNInverse( distParameter1, distParameter2, distParameter3, q )
        call assert_comparable(x, expected, margin, 'difference in distributionRayleighN too big')

        !
        ! test distributionPareto
        !
        !call calculateDistributionInverse(u, x, distributionPareto, distParameter1, distParameter2, distParameter3, &
        !    distParameter4, ierr, message)
        !call pQFromBeta ( u, p, q )
        !expected = paretoInverse( distParameter1, distParameter2, distParameter3, q )
        !call assert_comparable(x, expected, margin, 'difference in distributionPareto too big')

        !
        ! test distributionTriangular
        !
        !call calculateDistributionInverse(u, x, distributionTriangular, distParameter1, distParameter2, distParameter3, &
        !    distParameter4, ierr, message)
        !if (ierr == 0) then
        !    call pQFromBeta ( u, p, q )
        !    expected = triangularInverse( distParameter1, distParameter2, distParameter3, p, q )
        !    call assert_comparable(x, expected, margin, 'difference in distributionTriangular too big')
        !end if

        !
        ! test distributionConditionalWeibull
        !
        !call calculateDistributionInverse(u, x, distributionConditionalWeibull, distParameter1, distParameter2, distParameter3, &
        !    distParameter4, ierr, message)
        !call pQFromBeta ( u, p, q )
        !expected = conditionalWeibullInverse(distParameter1, distParameter2, distParameter3, distParameter4, p, q)
        !call assert_comparable(x, expected, margin, 'difference in distributionConditionalWeibull too big')

        !
        ! test distributionModifiedGumbel
        !
        !call calculateDistributionInverse(u, x, distributionModifiedGumbel, distParameter1, distParameter2, distParameter3, &
        !    distParameter4, ierr, message)
        !call pQFromBeta ( u, p, q )
        !expected = modifiedGumbelInverse(u, distParameter1, distParameter2, distParameter3, p, q)
        !call assert_comparable(x, expected, margin, 'difference in distributionModifiedGumbel too big')

        !
        ! test distributionTruncatedModifiedGumbel
        !
        !call calculateDistributionInverse(u, x, distributionTruncatedModifiedGumbel, distParameter1, distParameter2, &
        !    distParameter3, distParameter4, ierr, message)
        !expected = modifiedGumbelInverse(u, distParameter1, distParameter2, distParameter3, 0.0_wp, 1.0_wp)
        !call assert_comparable(x, expected, margin, 'difference in distributionTruncatedModifiedGumbel too big')
        !
        ! test distributionBeta
        !
        !call calculateDistributionInverse( u, x, distributionBeta, distBetaParameter1, distBetaParameter2, distBetaParameter3, &
        !   distBetaParameter4, ierr, message)
        !call pQFromBeta ( u, p, q )
        !xpected = betaInverse( distBetaParameter1, distBetaParameter2, distBetaParameter3, distBetaParameter4, p, q)
        !call assert_comparable( x, expected, margin, 'difference in beta distribution is too big')
    
    enddo
    
    ! test default case
    !
    call calculateDistributionInverse(u, x, -999, distParameter1, distParameter2, distParameter3, distParameter4, ierr, message)
    call assert_equal(message, "Unknown distribution function - code: -999", "test wrong input")
end subroutine calculateDistributionInverseTest

!> test for distributed type truncated normal
subroutine TruncatedNormalTests
    integer, parameter :: distType = distributionTruncatedNormal
    real(kind=wp) :: mean, deviation, minimum, maximum, u, x, expected
    integer :: ierr, i
    character(len=128) :: errorMessage

    !
    ! for negative large u, minimum is expected
    !
    u = -9999.0_wp
    mean = 1.0_wp
    deviation = 1.0_wp
    minimum = 0.1_wp
    maximum = 9.9_wp
    call calculateDistributionInverse( u, x, distType, mean, deviation, minimum, maximum, ierr, errorMessage )
    call assert_equal(ierr, 0, errorMessage)
    call assert_comparable(x, minimum, 1d-5, "diff in x for u=-9999; truncated normal")

    !
    ! for positive large u, maximum is expected
    !
    u = 9999.0_wp
    call calculateDistributionInverse( u, x, distType, mean, deviation, minimum, maximum, ierr, errorMessage )
    call assert_equal(ierr, 0, errorMessage)
    call assert_comparable(x, maximum, 1d-5, "diff in x for u=9999; truncated normal")
    
    !
    ! with minimum and maximum resp. -huge, +huge, distribution is equal to normal distribution
    !
    minimum = -huge(minimum)
    maximum = huge(maximum)
    do i = -25, 25
        u = 0.1_wp * real(i, wp)
        call calculateDistributionInverse( u, x, distType, mean, deviation, minimum, maximum, ierr, errorMessage )
        call assert_equal(ierr, 0, errorMessage)
        expected = mean + u * deviation
        call assert_Inbetween( abs(x-expected), 0.0d0, 1d-5, "diff in x; truncated normal" )
    enddo

    !
    ! proper error handling: check minimum < maximum
    !
    u = 0.0_wp
    mean = 1.0_wp
    deviation = 1.0_wp
    minimum = 0.1_wp
    maximum = -9.9_wp
    call calculateDistributionInverse( u, x, distType, mean, deviation, minimum, maximum, ierr, errorMessage )
    call assert_true(ierr /= 0, errorMessage)

    !
    ! proper error handling: check deviation > 0
    !
    u = 0.0_wp
    mean = 1.0_wp
    deviation = -1.0_wp
    minimum = 0.1_wp
    maximum = 9.9_wp
    ! TODO
    !call calculateDistributionInverse( u, x, distType, mean, deviation, minimum, maximum, ierr, errorMessage )
    !call assert_true(ierr /= 0, errorMessage)
end subroutine TruncatedNormalTests

!!--------------------------------------------------------------------------------------
!> Inverse beta distribution tests
!!
subroutine betaDistributionTests
!
!   write( *, '(/, a)' ) "Entry to beta tests"     ! Echo on entry to beta-distribution tests. Maintain while developments and testing, and remove later
    call testWithLevel( betaInverseTest_Basic1 , "Inverse beta distribution: Basic Test Nr. 1", 1, "Not implemented yet")
    call testWithLevel( betaInverseTest_Basic2 , &
        "Forward and Inverse Test beta distribution: Basic Test Nr. 2" , 1, "Not implemented yet")
    call testWithLevel( betaInverseTest_Basic3 , &
        "Forward and Inverse Test beta distribution: Basic Test Nr. 3" , 1, "Not implemented yet")
    call testWithLevel( betaInverseTest_Basic4 , &
        "Forward and Inverse Test beta distribution: Basic Test Nr. 4" , 1, "Not implemented yet")
    call testWithLevel( betaInverseTest_Basic5 , &
        "Forward and Inverse Test beta distribution: Basic Test Nr. 5" , 1, "Not implemented yet")
    call testWithLevel( betaInverseTest_Basic6 , &
        "Forward and Inverse Test beta distribution: Basic Test Nr. 6" , 1, "Not implemented yet")
    call testWithLevel( betaInverseTest_Basic7 , &
        "Forward and Inverse Test beta distribution: Basic Test Nr. 7" , 1, "Not implemented yet")
    call testWithLevel( betaInverseTestActualData, "Test beta distribution with actual data", 1, "Not implemented yet")
!   In the event, create more beta-tests
end subroutine betaDistributionTests
!
!
!> Basic test Nr. 1 for the inverse beta distribution
! Symmetric parabolic density function.
subroutine betaInverseTest_Basic1
!
    real( kind= wp) :: a             ! left  location parameter of the beta distribution in this test
    real( kind= wp) :: b             ! right location parameter of the beta distribution in this test
    real( kind= wp) :: alpha         ! left  shape    parameter of the beta distribution in this test
    real( kind= wp) :: beta          ! right shape    parameter of the beta distribution in this test
    real( kind= wp) :: p             ! prescribed probability of non-exceedence in this test
    real( kind= wp) :: q             ! prescribed probability of     exceedence in this test
    real( kind= wp) :: expectedValue ! expected value from inversion of beta distribution
    real( kind= wp) :: computedValue ! computed value in   inversion of beta distribution
    real( kind= wp) :: tolDiff       ! tolerance in difference computed value and expected value

!   Location parameters in present beta distribution:
    a            =-2.0_wp
    b            = 6.0_wp
!   Shape parameters in present beta distribution:
    alpha        = 3.0_wp
    beta         = alpha            ! symmetric distribution
    p            = 0.5_wp           ! Probability of non-exceedence
    q            = 1.0_wp- p        ! Corresponding Probability of exceedence
    tolDiff      = 1.0e-7_wp
    expectedValue= 0.5_wp*( a+ b)   ! because of symmetry due to alpha= beta
    computedValue= betaInverse( a, b, alpha, beta, p, q)
!   Compare computed and expected result
    call assert_comparable( computedValue, expectedValue, tolDiff, "too large diff. in x in inversion of beta distribution")
!    
end subroutine betaInverseTest_Basic1
!
!
!> Basic test Nr. 2 for the inverse beta distribution, here being a symmetric parabolic distribution centered around x= 0
subroutine betaInverseTest_Basic2
!
    integer         :: ifault          ! Error status on exit from ASA-functions for standard beta distribution
    real( kind= wp) :: a               ! left  location parameter of the beta distribution in this test
    real( kind= wp) :: b               ! right location parameter of the beta distribution in this test
    real( kind= wp) :: alpha           ! left  shape    parameter of the beta distribution in this test
    real( kind= wp) :: beta            ! right shape    parameter of the beta distribution in this test
    real( kind= wp) :: p               ! prescribed probability of non-exceedence in this test
    real( kind= wp) :: q               ! prescribed probability of     exceedence in this test
    real( kind= wp) :: x               ! x-argument for the present beta-distribution
    real( kind= wp) :: expectedPValue  ! expected value of CDF for present beta distribution for an argument x
    real( kind= wp) :: computedPValue  ! computed value of CDF for present beta distribution for an argument x
    real( kind= wp) :: expectedXValue  ! expected value of X from inversion of beta distribution
    real( kind= wp) :: computedXValue  ! computed value of X in   inversion of beta distribution
    real( kind= wp) :: lnComplBetaFunc ! value of standard complete beta function for current alpha and beta
    real( kind= wp) :: tolDiff         ! tolerance in difference computed value and expected value
    intrinsic       :: log
!
!   Location parameters in present beta distribution:
    b             = 2.0_wp
    a             =-b
!   Shape parameters in present beta distribution:
    alpha         = 2.0_wp
    beta          = alpha            ! alpha= beta= 2 ==> symmetric parabolic density function f(x|b)= 3/(4b) * ( 1 - (x/b)^2)
!   For this setting: CDF F(x|b)= 1/2 + 3/4*(x/b) - 1/4*(x/b)^3 for -b<x<b
!   Chose expected X-value for the inversion of beta distribution
    expectedXValue= 0.25* b
!   Computed corresponding and analytically exact probability of non exceedence p
    expectedPValue= 0.5_wp+ 0.75_wp* expectedXValue/ b - 0.25_wp* (expectedXValue/ b)**3
!   ----------------------------------------------------------------------------
!   Entry to the tests using the expected X and P-values
    tolDiff       = 1.0e-7_wp
!   ----------------------------------------------------------------------------
!   Test on the CDF of the beta distribution. This is thus a test for function betain(), that provides the CDF of the standard beta-distribution.
    x              = (expectedXValue- a)/( b- a)     ! Normalisation of X to standard beta distribution
    lnComplBetaFunc=-log( 6.0)                       ! Complete beta-function is 6 for alpha= beta= 2
    computedPValue = betain( x, alpha, beta, lnComplBetaFunc, ifault)
!   Compare computed and expected result
    call assert_comparable( computedPValue, expectedPValue, tolDiff, "too large diff. in CDF-values of beta distribution")
!   ----------------------------------------------------------------------------
!   Test on the inversion of the CDF of the beta distribution
    p             = expectedPValue
    q             = 1.0_wp- p        ! Corresponding probability of exceedence
    computedXValue= betaInverse( a, b, alpha, beta, p, q)
!   Compare computed and expected result
    call assert_comparable( computedXValue, expectedXValue, tolDiff, "too large diff. in x in inversion of beta distribution")
!    
end subroutine betaInverseTest_Basic2
!
!
!> Basic test Nr. 3 for the inverse beta distribution, here being a symmetric parabolic distribution centered around x= 0
!  For this purpose the shape parameters must be set to alpha= 2 and beta= 2
!  Repeat of the same test as already done before, but now a very small probability of non-exceedence is adopted.
!  Analytically computed expected value is compared to the value of betaInverse()
subroutine betaInverseTest_Basic3
!
    integer         :: ifault             ! Error status on exit from ASA-functions for standard beta distribution
    real( kind= wp) :: a                  ! left  location parameter of the beta distribution in this test
    real( kind= wp) :: b                  ! right location parameter of the beta distribution in this test
    real( kind= wp) :: alpha              ! left  shape    parameter of the beta distribution in this test
    real( kind= wp) :: beta               ! right shape    parameter of the beta distribution in this test
    real( kind= wp) :: p                  ! prescribed probability of non-exceedence in this test
    real( kind= wp) :: q                  ! prescribed probability of     exceedence in this test
    real( kind= wp) :: x                  ! x-argument for the standard beta-distribution (a'= 0, b'= 1)
    real( kind= wp) :: expectedPValue     ! expected value of CDF for present beta distribution for an argument x
    real( kind= wp) :: expectedLogPValue  ! expected value of the log(CDF) for present beta distribution for an argument x
    real( kind= wp) :: computedPValue     ! computed value of CDF for present beta distribution for an argument x
    real( kind= wp) :: computedLogPValue  ! computed value of the log(CDF) for present beta distribution for an argument x
    real( kind= wp) :: expectedXValue     ! expected value of X from inversion of beta distribution
    real( kind= wp) :: computedXValue     ! computed value of X in   inversion of beta distribution
    real( kind= wp) :: lnComplBetaFunc    ! value of standard complete beta function for current alpha and beta
    real( kind= wp) :: tolDiff            ! tolerance in difference of computed value and expected value
    intrinsic       :: log
!
!   Location parameters in present beta distribution:
    b              = 2.0_wp
    a              =-b
!   Shape parameters in present beta distribution:
    alpha          = 2.0_wp
    beta           = alpha            ! alpha= beta= 2 ==> symmetric parabolic density function f(x|b)= 3/(4b) * ( 1 - (x/b)^2)
!   For this setting: CDF F(x|a=-b, b)= 1/2 + 3/4*(x/b) - 1/4*(x/b)^3 for -b<x<b
!   Chose expected X-value for the inversion of beta distribution
    expectedXValue = a+ 1.0e-6_wp     ! Very close to right boundary
!   Computed corresponding and analytically exact probability of non exceedence p
    expectedPValue = 0.5_wp+ 0.75_wp* expectedXValue/ b - 0.25_wp* (expectedXValue/ b)**3
!   This p-value will be 1.874611577079577D-013 and thus very small
!   ----------------------------------------------------------------------------
!   Entry to the tests using the expected X and P-values
!   ----------------------------------------------------------------------------
!   Test on the CDF of the beta distribution. This is thus a test for function betain(), that provides the CDF of the standard beta-distribution.
    x              = (expectedXValue- a)/( b- a)     ! Normalisation of expectedXValue to argument for the standard beta distribution
    lnComplBetaFunc=-log( 6.0_wp)                       ! Complete beta-function B(alpha, beta) is 6 for alpha= beta= 2
    computedPValue = betain( x, alpha, beta, lnComplBetaFunc, ifault)
!   This computed p-value will be 1.874999727951636D-013 and is also very small
!   ----------------------------------------------------------------------------
!   Compare computed and expected result for the probability of non-exceedence.
!   Relative comparison is not meaningful because of their very small values.
!   Use their logarithms instead.
    computedLogPValue= log( computedPValue)
    expectedLogPValue= log( expectedPValue)
    tolDiff          = 7.5e-6_wp             ! Maximum accepted relative difference
    call assert_comparable( computedLogPValue, expectedLogPValue, tolDiff, "too large diff. in CDF-values of beta distribution")
!   ----------------------------------------------------------------------------
!   Test on the inversion of the CDF of the beta distribution
    p              = expectedPValue
    q              = 1.0_wp- p        ! Corresponding probability of exceedence
    computedXValue= betaInverse( a, b, alpha, beta, p, q)
!   Compare computed and expected result for the x-value found in the beta-inversion:
    tolDiff= 1.0e-7_wp               ! Maximum accepted relative difference
    call assert_comparable( computedXValue, expectedXValue, tolDiff, "too large diff. in x in inversion of beta distribution")
!    
end subroutine betaInverseTest_Basic3
!
!
!> Basic test Nr. 4 for the inverse beta distribution, here being a symmetric parabolic distribution centered around x= 0
!  For this purpose the shape parameters must be set to alpha= 2 and beta= 2
!  Repeat of the same test as already done before, but now a very small probability of exceedence is adopted.
!  Analytically computed expected value is compared to the value of betaInverse()
subroutine betaInverseTest_Basic4
!
    integer         :: ifault          ! Error status on exit from ASA-functions for standard beta distribution
    real( kind= wp) :: a               ! left  location parameter of the beta distribution in this test
    real( kind= wp) :: b               ! right location parameter of the beta distribution in this test
    real( kind= wp) :: alpha           ! left  shape    parameter of the beta distribution in this test
    real( kind= wp) :: beta            ! right shape    parameter of the beta distribution in this test
    real( kind= wp) :: p               ! prescribed probability of non-exceedence in this test
    real( kind= wp) :: q               ! prescribed probability of     exceedence in this test
    real( kind= wp) :: x               ! x-argument for the standard beta-distribution (a'= 0, b'= 1)
    real( kind= wp) :: expectedPValue  ! expected value of CDF for present beta distribution for an argument x
    real( kind= wp) :: computedPValue  ! computed value of CDF for present beta distribution for an argument x
    real( kind= wp) :: expectedXValue  ! expected value of X from inversion of beta distribution
    real( kind= wp) :: computedXValue  ! computed value of X in   inversion of beta distribution
    real( kind= wp) :: lnComplBetaFunc ! value of standard complete beta function for current alpha and beta
    real( kind= wp) :: tolDiff         ! tolerance in difference of computed value and expected value
    intrinsic       :: log
!
!   Location parameters in present beta distribution:
    b              = 2.0_wp
    a              =-b
!   Shape parameters in present beta distribution:
    alpha          = 2.0_wp
    beta           = alpha            ! alpha= beta= 2 ==> symmetric parabolic density function f(x|b)= 3/(4b) * ( 1 - (x/b)^2)
!   For this setting: CDF F(x|a=-b, b)= 1/2 + 3/4*(x/b) - 1/4*(x/b)^3 for -b<x<b
!   Chose expected X-value for the inversion of beta distribution
    expectedXValue = b- 1.0e-6_wp
!   Computed corresponding and analytically exact probability of non exceedence p
    expectedPValue = 0.5_wp+ 0.75_wp* expectedXValue/ b - 0.25_wp* (expectedXValue/ b)**3
!   ----------------------------------------------------------------------------
!   Entry to the tests using the expected X and P-values
    tolDiff        = 1.0e-7_wp      ! Maximum accepted difference
!   ----------------------------------------------------------------------------
!   Test on the CDF of the beta distribution. This is thus a test for function betain(), that provides the CDF of the standard beta-distribution.
    x              = (expectedXValue- a)/( b- a)     ! Normalisation of expectedXValue to argument for the standard beta distribution
    lnComplBetaFunc=-log( 6.0)                       ! Complete beta-function B(alpha, beta) is 6 for alpha= beta= 2
    computedPValue = betain( x, alpha, beta, lnComplBetaFunc, ifault)
!   Compare computed and expected result for the probability of non-exceedence
    call assert_comparable( computedPValue, expectedPValue, tolDiff, "too large diff. in CDF-values of beta distribution")
!   ----------------------------------------------------------------------------
!   Test on the inversion of the CDF of the beta distribution
    p              = expectedPValue
    q              = 1.0_wp- p        ! Corresponding probability of exceedence
    computedXValue = betaInverse( a, b, alpha, beta, p, q)
!   Compare computed and expected result for the x-value found in the beta-inversion:
    call assert_comparable( computedXValue, expectedXValue, tolDiff, "too large diff. in x in inversion of beta distribution")
!    
end subroutine betaInverseTest_Basic4
!
!
!> Basic test Nr. 5 for the inverse beta distribution, here for the case that the right shape parameter beta= 1.
!  In this case the beta distribution the PDF, CDF, and inverse of the CDF are all analytically available.
!  In fact, for the standard beta distribution we then have pdf: f(x|alpha, beta=1)= alpha * x^(alpha-1), cdf: F(x|alpha, beta=1)= x^alpha, and inverse cdf: Finv(P|alpha, beta=1)= P^(1/alpha).
!  For the general beta distribution f(x|a, b, alpha, beta=1) these functions must be horizontally scaled with b-a.
!  For example: cdf: F(x|a, b, alpha, beta=1)= [(x-a)/(x-b)]^alpha
!  This setting with beta= 1 provides thus a convenient means for analytical tests. Here this is done for a non-extreme value for the left shape parameter alpha.
!  Analytically computed expected values are compared to the value of betain() and betaInverse()
subroutine betaInverseTest_Basic5
!
    integer         :: ifault          ! Error status on exit from ASA-functions for standard beta distribution
    real( kind= wp) :: a               ! left  location parameter of the beta distribution in this test
    real( kind= wp) :: b               ! right location parameter of the beta distribution in this test
    real( kind= wp) :: alpha           ! left  shape    parameter of the beta distribution in this test
    real( kind= wp) :: beta            ! right shape    parameter of the beta distribution in this test
    real( kind= wp) :: p               ! prescribed probability of non-exceedence in this test
    real( kind= wp) :: q               ! prescribed probability of     exceedence in this test
    real( kind= wp) :: x               ! x-argument for the standard beta-distribution (a'= 0, b'= 1)
    real( kind= wp) :: expectedPValue  ! expected value of CDF for present beta distribution for an argument x
    real( kind= wp) :: computedPValue  ! computed value of CDF for present beta distribution for an argument x
    real( kind= wp) :: expectedXValue  ! expected value of X from inversion of beta distribution
    real( kind= wp) :: computedXValue  ! computed value of X in   inversion of beta distribution
    real( kind= wp) :: lnComplBetaFunc ! value of standard complete beta function for current alpha and beta
    real( kind= wp) :: tolDiff         ! tolerance in difference of computed value and expected value
    intrinsic       :: log
!
!   Location parameters in present beta distribution:
    a              = 2.0_wp    
    b              = 6.0_wp
!   Shape parameters in present beta distribution:
    alpha          = 4.0_wp
    beta           = 1.0_wp
!   For this setting: CDF F(x|a, b, alpha, beta=1)= [(x-a)/(x-b)]^alpha for a<x<b
!   Chose expected X-value for the inversion of beta distribution. This value must be within present [a, b]
    expectedXValue = 5.0_wp
!   Computed corresponding and analytically exact probability of non exceedence p
    expectedPValue =((expectedXValue- a)/( b- a))**alpha
!   NB. expectedPValue= 0.316406250000000 for alpha= 4, and beta= 1
!   ----------------------------------------------------------------------------
!   Entry to the tests using the expected X and P-values
    tolDiff        = 1.0e-7_wp      ! Maximum accepted difference
!   ----------------------------------------------------------------------------
!   Test on the CDF of the beta distribution. This is thus a test for function betain(), that provides the CDF of the standard beta-distribution.
    x              = (expectedXValue- a)/( b- a)    ! Normalisation of expectedXValue to argument for the standard beta distribution
    lnComplBetaFunc=-log( alpha)                    ! Complete beta-function B(alpha, beta=1) is alpha for beta= 1
    computedPValue = betain( x, alpha, beta, lnComplBetaFunc, ifault)
!   Compare computed and expected result for the probability of non-exceedence
    call assert_comparable( computedPValue, expectedPValue, tolDiff, "too large diff. in CDF-values of beta distribution")
!   ----------------------------------------------------------------------------
!   Test on the inversion of the CDF of the beta distribution
    p              = expectedPValue
    q              = 1.0_wp- p        ! Corresponding probability of exceedence
    computedXValue = betaInverse( a, b, alpha, beta, p, q)
!   Compare computed and expected result for the x-value found in the beta-inversion:
    call assert_comparable( computedXValue, expectedXValue, tolDiff, "too large diff. in x in inversion of beta distribution")
!    
end subroutine betaInverseTest_Basic5
!
!
!> Basic test Nr. 6 for the inverse beta distribution, here for the case that the right shape parameter beta= 1.
!  In this case the beta distribution the PDF, CDF, and inverse of the CDF are all analytically available.
!  In fact, for the standard beta distribution we then have pdf: f(x|alpha, beta=1)= alpha * x^(alpha-1), cdf: F(x|alpha, beta=1)= x^alpha, and inverse cdf: Finv(P|alpha, beta=1)= P^(1/alpha).
!  For the general beta distribution f(x|a, b, alpha, beta=1) these functions must be horizontally scaled with b-a.
!  For example: F(x|a, b, alpha, beta=1)= [(x-a)/(x-b)]^alpha
!  This setting with beta= 1 provides thus a convenient means for analytical tests. Here this is done for a very large value for the left shape parameter alpha.
!  Analytically computed expected values are compared to the value of betain() and betaInverse()
subroutine betaInverseTest_Basic6
!
    integer         :: ifault          ! Error status on exit from ASA-functions for standard beta distribution
    real( kind= wp) :: a               ! left  location parameter of the beta distribution in this test
    real( kind= wp) :: b               ! right location parameter of the beta distribution in this test
    real( kind= wp) :: alpha           ! left  shape    parameter of the beta distribution in this test
    real( kind= wp) :: beta            ! right shape    parameter of the beta distribution in this test
    real( kind= wp) :: p               ! prescribed probability of non-exceedence in this test
    real( kind= wp) :: q               ! prescribed probability of     exceedence in this test
    real( kind= wp) :: x               ! x-argument for the standard beta-distribution (a'= 0, b'= 1)
    real( kind= wp) :: expectedPValue  ! expected value of CDF for present beta distribution for an argument x
    real( kind= wp) :: computedPValue  ! computed value of CDF for present beta distribution for an argument x
    real( kind= wp) :: expectedXValue  ! expected value of X from inversion of beta distribution
    real( kind= wp) :: computedXValue  ! computed value of X in   inversion of beta distribution
    real( kind= wp) :: lnComplBetaFunc ! value of standard complete beta function for current alpha and beta
    real( kind= wp) :: tolDiff         ! tolerance in difference of computed value and expected value
    intrinsic       :: log
!
!   Location parameters in present beta distribution:
    a              = 2.0_wp    
    b              = 6.0_wp
!   Shape parameters in present beta distribution:
    alpha          = 100.0_wp
    beta           = 1.0_wp
!   For this setting: CDF F(x|a, b, alpha, beta=1)= [(x-a)/(x-b)]^alpha for a<x<b
!   Chose expected X-value for the inversion of beta distribution. This value must be within present [a, b]
    expectedXValue = 5.0_wp
!   Computed corresponding and analytically exact probability of non exceedence p
    expectedPValue =((expectedXValue- a)/( b- a))**alpha
!   NB. expectedPValue= 3.207202185381520D-013 for alpha= 100, and beta= 1
!   ----------------------------------------------------------------------------
!   Entry to the tests using the expected X and P-values
    tolDiff        = 1.0e-6_wp      ! Maximum accepted difference
!   ----------------------------------------------------------------------------
!   Test on the CDF of the beta distribution. This is thus a test for function betain(), that provides the CDF of the standard beta-distribution.
    x              = (expectedXValue- a)/( b- a)    ! Normalisation of expectedXValue to argument for the standard beta distribution
    lnComplBetaFunc=-log( alpha)                    ! Complete beta-function B(alpha, beta=1) is alpha for beta= 1
    computedPValue = betain( x, alpha, beta, lnComplBetaFunc, ifault)
!   Compare computed and expected result for the probability of non-exceedence
    call assert_comparable( computedPValue, expectedPValue, tolDiff, "too large diff. in CDF-values of beta distribution")
!   ----------------------------------------------------------------------------
!   Test on the inversion of the CDF of the beta distribution
    p              = expectedPValue
    q              = 1.0_wp- p        ! Corresponding probability of exceedence
    computedXValue = betaInverse( a, b, alpha, beta, p, q)
!   Compare computed and expected result for the x-value found in the beta-inversion:
    call assert_comparable( computedXValue, expectedXValue, tolDiff, "too large diff. in x in inversion of beta distribution")
!    
end subroutine betaInverseTest_Basic6
!
!
!> Basic test Nr. 7 for the inverse beta distribution, here for the case that the right shape parameter beta= 1.
!  In this case the beta distribution the PDF, CDF, and inverse of the CDF are all analytically available.
!  In fact, for the standard beta distribution we then have pdf: f(x|alpha, beta=1)= alpha * x^(alpha-1), cdf: F(x|alpha, beta=1)= x^alpha, and inverse cdf: Finv(P|alpha, beta=1)= P^(1/alpha).
!  For the general beta distribution f(x|a, b, alpha, beta=1) these functions must be horizontally scaled with b-a.
!  For example: F(x|a, b, alpha, beta=1)= [(x-a)/(x-b)]^alpha
!  This setting with beta= 1 provides thus a convenient means for analytical tests. Here this is done for a small value for the left shape parameter alpha.
!  In this case we have then a J-shaped distribution with a pole near x=a
!  Analytically computed expected values are compared to the value of betain() and betaInverse()
subroutine betaInverseTest_Basic7
!
    integer         :: ifault          ! Error status on exit from ASA-functions for standard beta distribution
    real( kind= wp) :: a               ! left  location parameter of the beta distribution in this test
    real( kind= wp) :: b               ! right location parameter of the beta distribution in this test
    real( kind= wp) :: alpha           ! left  shape    parameter of the beta distribution in this test
    real( kind= wp) :: beta            ! right shape    parameter of the beta distribution in this test
    real( kind= wp) :: p               ! prescribed probability of non-exceedence in this test
    real( kind= wp) :: q               ! prescribed probability of     exceedence in this test
    real( kind= wp) :: x               ! x-argument for the standard beta-distribution (a'= 0, b'= 1)
    real( kind= wp) :: expectedPValue  ! expected value of CDF for present beta distribution for an argument x
    real( kind= wp) :: computedPValue  ! computed value of CDF for present beta distribution for an argument x
    real( kind= wp) :: expectedXValue  ! expected value of X from inversion of beta distribution
    real( kind= wp) :: computedXValue  ! computed value of X in   inversion of beta distribution
    real( kind= wp) :: lnComplBetaFunc ! value of standard complete beta function for current alpha and beta
    real( kind= wp) :: tolDiff         ! tolerance in difference of computed value and expected value
    intrinsic       :: log
!
!   Location parameters in present beta distribution:
    a              = 2.0_wp    
    b              = 6.0_wp
!   Shape parameters in present beta distribution:
    alpha          = 0.4_wp
    beta           = 1.0_wp
!   For this setting: CDF F(x|a, b, alpha, beta=1)= [(x-a)/(x-b)]^alpha for a<x<b
!   Chose expected X-value for the inversion of beta distribution. This value must be within present [a, b]
    expectedXValue = 5.0_wp
!   Computed corresponding and analytically exact probability of non exceedence p
    expectedPValue =((expectedXValue- a)/( b- a))**alpha
!   NB. expectedPValue= 0.891301228983002 for alpha= 0.4, and beta= 1
!   ----------------------------------------------------------------------------
!   Entry to the tests using the expected X and P-values
    tolDiff        = 1.0e-7_wp      ! Maximum accepted difference
!   ----------------------------------------------------------------------------
!   Test on the CDF of the beta distribution. This is thus a test for function betain(), that provides the CDF of the standard beta-distribution.
    x              = (expectedXValue- a)/( b- a)    ! Normalisation of expectedXValue to argument for the standard beta distribution
    lnComplBetaFunc=-log( alpha)                    ! Complete beta-function B(alpha, beta=1) is alpha for beta= 1
    computedPValue = betain( x, alpha, beta, lnComplBetaFunc, ifault)
!   Compare computed and expected result for the probability of non-exceedence
    call assert_comparable( computedPValue, expectedPValue, tolDiff, "too large diff. in CDF-values of beta distribution")
!   ----------------------------------------------------------------------------
!   Test on the inversion of the CDF of the beta distribution
    p              = expectedPValue
    q              = 1.0_wp- p        ! Corresponding probability of exceedence
    computedXValue = betaInverse( a, b, alpha, beta, p, q)
!   Compare computed and expected result for the x-value found in the beta-inversion:
!   While testing:
    a= expectedXValue- computedXValue
    b= expectedPValue- computedPValue
    call assert_comparable( computedXValue, expectedXValue, tolDiff, "too large diff. in x in inversion of beta distribution")
!    
end subroutine betaInverseTest_Basic7
!

subroutine betaInverseTestActualData
    real(kind=wp) :: dischargeTableT(5*16) =  (/ &
         5577.57_wp,  -5578.569_wp, 14421.431_wp, 209.669_wp, 540.254_wp, &
         5578.57_wp,  -5578.569_wp, 14421.431_wp, 209.669_wp, 540.254_wp, &
         6615.12_wp,  -6615.121_wp, 13384.879_wp, 217.477_wp, 437.634_wp, &
         7647.69_wp,  -7647.688_wp, 12352.312_wp, 198.653_wp, 317.880_wp, &
         8707.18_wp,  -8707.180_wp, 11292.820_wp, 170.436_wp, 217.541_wp, &
         9732.93_wp,  -9732.933_wp, 10267.067_wp, 158.539_wp, 163.641_wp, &
        11185.32_wp, -11185.315_wp,  8814.685_wp, 118.133_wp,  91.791_wp, &
        13223.49_wp, -13223.494_wp,  6776.506_wp, 101.546_wp,  53.837_wp, &
        14114.88_wp, -14114.883_wp,  5885.117_wp, 105.346_wp,  45.485_wp, &
        14642.09_wp, -14642.090_wp,  5357.910_wp,  73.522_wp,  27.733_wp, &
        15354.04_wp, -15354.039_wp,  4645.961_wp,  43.874_wp,  13.857_wp, &
        16243.82_wp, -16243.818_wp,  3756.182_wp,  33.427_wp,   8.744_wp, &
        16876.69_wp, -16876.695_wp,  3123.305_wp,  30.004_wp,   6.689_wp, &
        17230.18_wp, -17230.178_wp,  2769.822_wp,  28.992_wp,   5.706_wp, &
        25000.0_wp,  -25000.0_wp  , -5000.0_wp  ,   1.000_wp,   0.0001_wp, &
        25001.0_wp,  -25001.0_wp  , -5001.0_wp  ,   1.000_wp,   0.0001_wp /)
    real(kind=wp) :: dischargeTable(16,5)
    integer :: i, iunit, j
    real(kind=wp) :: a, b, alpha, beta, p, q, y(-1:4)

    dischargeTable = transpose(reshape(dischargeTableT, [5,16]))
    open(newunit = iunit, file="q_beta.out")
    do i = 1, 16
        a = dischargeTable(i,2)
        b = dischargeTable(i,3)
        alpha = dischargeTable(i,4)
        beta  = dischargeTable(i,5)
        do j = -1, 4
            call pQFromBeta (real(j,wp), p, q)
            y(j) = betaInverse( a, b, alpha, beta, p, q)
        end do
        write(iunit,'(7f11.3)') dischargeTable(i,1), y(-1:4)
    end do
    close(iunit)
    call assert_files_comparable("q_beta.out", "../probabilisticData/Testdata/q_beta.out", "compare table beta dist.", 1e-3)
end subroutine betaInverseTestActualData
!
! -------------------------------------------------------------------
! End   of a series of subroutines for testing distribution functions
! -------------------------------------------------------------------

end module distributionFunctionsTests

