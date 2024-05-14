module interface_randomgen

  implicit none

  interface
    subroutine initRandomGenerator (ij, kl) bind(c)
      integer, value, intent(in)  :: ij  !< Seed for i and j
      integer, value, intent(in)  :: kl  !< Seed for k and l
    end subroutine initRandomGenerator
  end interface

  interface
    subroutine cleanupRandomGenerator () bind(c)
    end subroutine cleanupRandomGenerator
  end interface

  !! This subroutine returns a uniform random deviate between 0.0 and 1.0
  interface
    subroutine getRandomValue (randomValue) bind(c)
      use, intrinsic :: iso_c_binding, only: c_int, c_double
      real(kind=c_double), intent(out) :: randomValue  !< Uniform random deviate between 0.0 and 1.0
    end subroutine getRandomValue
  end interface

end module interface_randomgen
