! Copyright (C) Stichting Deltares. All rights reserved.
!
! This file is part of the Probabilistic Library.
!
! The Probabilistic Library is free software: you can redistribute it and/or modify
! it under the terms of the GNU Lesser General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
!
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
! GNU Lesser General Public License for more details.
!
! You should have received a copy of the GNU Lesser General Public License
! along with this program. If not, see <http://www.gnu.org/licenses/>.
!
! All names, logos, and references to "Deltares" are registered trademarks of
! Stichting Deltares and remain full property of Stichting Deltares at all times.
! All rights reserved.
!

!> @file
!! Test for the incomplete gamma function
!<
!
module incompleteGammaFunctionTests
    use ieee_arithmetic
    use precision
    use ftnunit
    use interface_distributions
    use interface_special_funcs

    implicit none

    private

    public :: allIncompleteGammaTests

contains

!> Call all the tests
!!
subroutine allIncompleteGammaTests

    call testWithLevel( incompleteGammaTest, "Test for incomplete gamma function", 1)
    call testWithLevel( incompleteGammaTest2, "Test exceptional cases of incomplete gamma function", 1)

end subroutine allIncompleteGammaTests

!> Test for incomplete gamma function
subroutine incompleteGammaTest

   real(kind=wp) :: gamma, gamma_complement, a, x, ref, table(15,10)
   real(kind=wp), parameter :: tol = 6d-4 ! table is rounded on 3 digits, and our function also has some truncation error
   integer            :: i, j
   character(len=40)  :: msg
   integer            :: ierr
   character(len=100) :: errorMessage

   ! reference values from: http://uicl.iut.nsysu.edu.tw/courses/105-1/probability/tables/Table_A8_IncompleteGamma.pdf
   !
   table( 1,:) = [0.632_wp, 0.264_wp, 0.080_wp, 0.019_wp, 0.004_wp, 0.001_wp, 0.000_wp, 0.000_wp, 0.000_wp, 0.000_wp]
   table( 2,:) = [0.865_wp, 0.594_wp, 0.323_wp, 0.143_wp, 0.053_wp, 0.017_wp, 0.005_wp, 0.001_wp, 0.000_wp, 0.000_wp]
   table( 3,:) = [0.950_wp, 0.801_wp, 0.577_wp, 0.353_wp, 0.185_wp, 0.084_wp, 0.034_wp, 0.012_wp, 0.004_wp, 0.001_wp]
   table( 4,:) = [0.982_wp, 0.908_wp, 0.762_wp, 0.567_wp, 0.371_wp, 0.215_wp, 0.111_wp, 0.051_wp, 0.021_wp, 0.008_wp]
   table( 5,:) = [0.993_wp, 0.960_wp, 0.875_wp, 0.735_wp, 0.560_wp, 0.384_wp, 0.238_wp, 0.133_wp, 0.068_wp, 0.032_wp]
   table( 6,:) = [0.998_wp, 0.983_wp, 0.938_wp, 0.849_wp, 0.715_wp, 0.554_wp, 0.394_wp, 0.256_wp, 0.153_wp, 0.084_wp]
   table( 7,:) = [0.999_wp, 0.993_wp, 0.970_wp, 0.918_wp, 0.827_wp, 0.699_wp, 0.550_wp, 0.401_wp, 0.271_wp, 0.170_wp]
   table( 8,:) = [1.000_wp, 0.997_wp, 0.986_wp, 0.958_wp, 0.900_wp, 0.809_wp, 0.687_wp, 0.547_wp, 0.407_wp, 0.283_wp]
   table( 9,:) = [1.000_wp, 0.999_wp, 0.994_wp, 0.979_wp, 0.945_wp, 0.884_wp, 0.793_wp, 0.676_wp, 0.544_wp, 0.413_wp]
   table(10,:) = [1.000_wp, 1.000_wp, 0.997_wp, 0.990_wp, 0.971_wp, 0.933_wp, 0.870_wp, 0.780_wp, 0.667_wp, 0.542_wp]
   table(11,:) = [1.000_wp, 1.000_wp, 0.999_wp, 0.995_wp, 0.985_wp, 0.962_wp, 0.921_wp, 0.857_wp, 0.768_wp, 0.659_wp]
   table(12,:) = [1.000_wp, 1.000_wp, 1.000_wp, 0.998_wp, 0.992_wp, 0.980_wp, 0.954_wp, 0.911_wp, 0.845_wp, 0.758_wp]
   table(13,:) = [1.000_wp, 1.000_wp, 1.000_wp, 0.999_wp, 0.996_wp, 0.989_wp, 0.974_wp, 0.946_wp, 0.900_wp, 0.834_wp]
   table(14,:) = [1.000_wp, 1.000_wp, 1.000_wp, 1.000_wp, 0.998_wp, 0.994_wp, 0.986_wp, 0.968_wp, 0.938_wp, 0.891_wp]
   table(15,:) = [1.000_wp, 1.000_wp, 1.000_wp, 1.000_wp, 0.999_wp, 0.997_wp, 0.992_wp, 0.982_wp, 0.963_wp, 0.930_wp]

   do i = 1, 10
      a = real(i, wp)
      !call prepareGammaLog(a)
      do j = 1, 15
          x = real(j, wp)
          gamma_complement = Dgammq(a, x, ierr, errorMessage)
          gamma = 1.0_wp - gamma_complement
          ref = table(j,i)
          write(msg,'(a,f4.1,a,f4.1,a)') "difference in gamma(",a,",",x,")"
          call assert_inbetween(gamma, ref - tol, ref + tol, msg)
       enddo
   enddo
end subroutine incompleteGammaTest

!> Test exceptional cases of incomplete gamma function
subroutine incompleteGammaTest2
    real(kind=wp) :: a, x, gamma_complement
    real(kind=wp), parameter :: expected1 = 0.909796_wp
    real(kind=wp), parameter :: expected2 = 0.1358882_wp
    real(kind=wp), parameter :: eps = 1d-6
    integer                  :: ierr
    character(100)           :: errorMessage

    a = 1.0
    !call prepareGammaLog(a)  !< test a <> a given here
    a = 2.0
    x = 0.5
    gamma_complement = Dgammq(a, x, ierr, errorMessage)
    call assert_comparable(expected1, gamma_complement, eps, "Dgammaq(2, 0.5)")

    x = 3.5
    gamma_complement = Dgammq(a, x, ierr, errorMessage)
    call assert_comparable(expected2, gamma_complement, eps, "Dgammaq(2, 1.5)")

    x = -1.0
    gamma_complement = Dgammq(a, x, ierr, errorMessage)
    call assert_true(isNaN(gamma_complement), " x < 0")
    call assert_equal(-1, ierr, "ierr Dgammq")

    x =  1.0
    a = -1.0
    gamma_complement = Dgammq(a, x, ierr, errorMessage)
    call assert_equal(-1, ierr, "ierr Dgammq")

    a = ieee_value(a, ieee_positive_inf)
    x = 1.0
    gamma_complement = Dgammq(a, x, ierr, errorMessage)
    call assert_equal(-1, ierr, "ierr Dgammq")
    call assert_equal('Error in DGser: no convergence', errorMessage, "err msg")

    x = ieee_value(x, ieee_positive_inf)
    a = 1.0
    gamma_complement = Dgammq(a, x, ierr, errorMessage)
    call assert_equal(-1, ierr, "ierr Dgammq")
    call assert_equal('Error in DGcf: no convergence', errorMessage, "err msg")

    x = 0.0
    a = 2.0
    gamma_complement = Dgammq(a, x, ierr, errorMessage)
    call assert_equal(0, ierr, "ierr Dgammq")
    call assert_comparable(1.0_wp, gamma_complement, eps, "gamma from Dgammq")
end subroutine incompleteGammaTest2

end module incompleteGammaFunctionTests
