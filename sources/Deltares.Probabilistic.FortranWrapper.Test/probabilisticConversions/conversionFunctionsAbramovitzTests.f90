! Copyright (C) Stichting Deltares 2022. All rights reserved.
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
!! Test inverse distribution functions and associated auxiliary routines
!<
!
! $Id$
!
!> @ingroup ProbabilisticTests
!
!> In the first  part of subroutine allDistributionFunctionsTests several betaFromQ()  tests are executed
!> In the second part of subroutine allDistributionFunctionsTests several PQFromBeta() tests are executed
!
module conversionFunctionsAbramovitzTests
    !
    !------------------------------------------------------------------------
    !   programmer: Kathryn Roscoe
    !               Henk van den Boogaard and Edwin Spee (modifications, upgrading, extensions)
    !------------------------------------------------------------------------

    use ftnunit
    use precision
    use fileUtilities
    use equalReals
    use conversionFunctionsAbramovitz
    use betaFromPQminAbramovitzModule
    use utilities
    use feedback

    implicit none

    real (kind = wp), parameter :: margin = 1.0d-6

    private

    public :: allBetaFromQAbramovitzTests, allPFromBetaAbramovitzTests, allPQFromBetaAbramovitzTests, allQFromBetaAbramovitzTests

contains

! Begin of a series of subroutines for testing betaFromQ()
! -------------------------------------------------------------------
!
!> Call all the betaFromQ() tests
!!
subroutine allBetaFromQAbramovitzTests

    call testWithLevel( testExpectedValueBeta_BetaFromQ, "testExpectedValueBeta from module betaFromQAbramovitzTests", 1)
    call testWithLevel( testProbabilityEqual2, "testProbabilityEqual2 from module betaFromQAbramovitzTests", 1)
    call testWithLevel( testProbabilityEqual0and1, "testProbabilityEqual0and1 from module betaFromQAbramovitzTests", 1)
    call testWithLevel( testProbabilityEqualMinus1, "testProbabilityEqualMinus1 from module betaFromQAbramovitzTests", 1)
    call testWithLevel( testProbabilityEqualNaN, "testProbabilityEqualNaN from module betaFromQAbramovitzTests", 1)
    call testWithLevel( test05precentQuantile_BetaFromQ, "test05precentQuantile from module betaFromQAbramovitzTests", 1)
    call testWithLevel( test95precentQuantile_BetaFromQ, "test95precentQuantile from module betaFromQAbramovitzTests", 1)
    call testWithLevel( testTinyQuantileBetaFromPQMin, "testTinyQuantileBetaFromPQMin from module betaFromQAbramovitzTests", 1)
    call testWithLevel( testTinyQuantileBetaFromQ, "testTinyQuantileBetafromQ from module betaFromQAbramovitzTests", 1)

    call testWithLevel( testWholeRangeQuantileBetaFromQ,"testWholeRangeQuantileBetaFromQ from module betaFromQAbramovitzTests", 1)
    ! also check files testWholeRangeQuantileBetaFromQ*.txt

    call testWithLevel( repetitiveBetaAndQComputation1, "repetitiveBetaAndQComputation1 from module betaFromQAbramovitzTests", 1)
    call testWithLevel( repetitiveBetaAndQComputation2, "repetitiveBetaAndQComputation2 from module betaFromQAbramovitzTests", 1)
    call testWithLevel( repetitiveBetaAndQComputation3, "repetitiveBetaAndQComputation3 from module betaFromQAbramovitzTests", 1)
    call testWithLevel( repetitiveBetaAndQComputation4, "repetitiveBetaAndQComputation4 from module betaFromQAbramovitzTests", 1)
    call testWithLevel( repetitiveBetaAndQTable, "repetitiveBetaAndQTable from module betaFromQAbramovitzTests", 1)

end subroutine allBetaFromQAbramovitzTests

!> Test betaFromQ with the expected value
!!
subroutine testExpectedValueBeta_BetaFromQ

    real (kind = wp)   :: q = 0.5d0
    real (kind = wp)   :: expectedBeta = 0.0d0
    real (kind = wp)   :: margin = 1.0d-6
    real (kind = wp)   :: actualBeta
    integer            :: ierr
    character(len=128) :: errorMessage

    call betaFromQAbramovitz( q, actualBeta, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine testExpectedValueBeta_BetaFromQ

!> Test betaFromQ with impossible probability of 2.0
!!
subroutine testProbabilityEqual2

    real (kind = wp)   :: q
    real (kind = wp)   :: margin = 1.0d-6
    real (kind = wp)   :: expectedBeta
    real (kind = wp)   :: actualBeta
    integer            :: ierr
    character(len=128) :: errorMessage

    q = 2.0d0
    expectedBeta = huge(q)
    call betaFromQAbramovitz( q, actualBeta, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( actualBeta, -expectedBeta, margin, "unexpected return value Beta" )

end subroutine testProbabilityEqual2

!> Test betaFromQ with impossible probability of 0.0 and 1.0
!! q should be 0.0 < q < 1.0 , but for safety reasons values
!! 0.0-EPS < q < 1.0d0 + EPS are allowed.
subroutine testProbabilityEqual0and1

    real (kind = wp)   :: q
    real (kind = wp)   :: beta0
    real (kind = wp)   :: beta1
    real (kind = wp)   :: betaMin
    integer            :: ierr
    character(len=128) :: errorMessage

    q = 0.0d0
    call betaFromQAbramovitz( q, beta0, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    q = 1.0d-300
    call betaFromQAbramovitz( q, betaMin, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( beta0, betaMin, margin, "unexpected return value Beta" )
    q = 1.0d0
    call betaFromQAbramovitz( q, beta1, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( -beta0, beta1, margin, "unexpected return value Beta" )

end subroutine testProbabilityEqual0and1

!> Test betaFromQ with impossible probability of -1.0
!!
subroutine testProbabilityEqualMinus1

    real (kind = wp)   :: q
    real (kind = wp)   :: margin = 1.0d-6
    real (kind = wp)   :: expectedBeta
    real (kind = wp)   :: actualBeta
    integer            :: ierr
    character(len=128) :: errorMessage

    q = -1.0d0
    expectedBeta = huge(q)
    call betaFromQAbramovitz( q, actualBeta, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine testProbabilityEqualMinus1

!> Test betaFromQ with impossible probability of NaN
!!
subroutine testProbabilityEqualNaN

    real (kind = wp)  :: q
    real (kind = wp)  :: actualBeta
    integer           :: ierr
    character(len=64) :: errorMessage

    call set_nan(q)
    call betaFromQAbramovitz( q, actualBeta, ierr, errorMessage )
    call assert_true( isnan(actualBeta), "Beta <> NaN" )

end subroutine testProbabilityEqualNaN

!> Test betaFromQ with the 5%-quantile
!!
subroutine test05precentQuantile_BetaFromQ

    real (kind = wp)  :: q = 0.95d0
    real (kind = wp)  :: expectedBeta = -1.64485362695147d0  ! value generated by using Matlab
    real (kind = wp)  :: margin = 1.0d-6
    real (kind = wp)  :: actualBeta
    integer           :: ierr
    character(len=64) :: errorMessage

    call betaFromQAbramovitz( q, actualBeta, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine test05precentQuantile_BetaFromQ

!> Test betaFromQ with the 95%-quantile
!!
subroutine test95precentQuantile_BetaFromQ

    real (kind = wp)  :: q = 0.05d0
    real (kind = wp)  :: expectedBeta = 1.64485362695147d0  ! value generated by using Matlab
    real (kind = wp)  :: margin = 1.0d-6
    real (kind = wp)  :: actualBeta
    integer           :: ierr
    character(len=64) :: errorMessage

    call betaFromQAbramovitz( q, actualBeta, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine test95precentQuantile_BetaFromQ

!> Test betafromQ with p=1E-16
!>   @ingroup ProbabilisticTests
!

subroutine testTinyQuantileBetaFromPQmin

    real (kind = wp) :: p = 1Q-16
    real (kind = wp) :: qw
    real (kind = wp) :: qExpected, pExpected
    real (kind = wp) :: margin = 1.0d-5
    real (kind = wp) :: actualBeta

    qw = 1.Q0 - p
    call betaFromPQmin( p, qw, actualBeta )
    call PQfromBetaAbramovitz(actualBeta, pExpected, qExpected)

    call assert_comparable( pExpected, dble(p), margin, "unexpected return value p from calculated Beta using betaFromPQmin" )
    call assert_comparable( qExpected, dble(qw), margin, "unexpected return value q from calculated Beta using betaFromPQmin" )


end subroutine testTinyQuantileBetaFromPQmin

!> Test betafromQ with p=1E-16
!>   @ingroup ProbabilisticTests
!
subroutine testTinyQuantileBetaFromQ

    real (kind = wp)  :: p = 1Q-16
    real (kind = wp)  :: qw
    real (kind = wp)  :: qExpected, pExpected
    real (kind = wp)  :: margin = 1.0d-5
    real (kind = wp)  :: actualBeta
    integer           :: ierr
    character(len=64) :: errorMessage

    if (p < 0.5) then
        qw = p
    else
        qw = 1.Q0 - p
    endif
    call betaFromQAbramovitz( dble(qw), actualBeta, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)

    if (p < 0.5) then
        actualbeta = -actualbeta
        qw = 1.Q0 - p
    endif
    call PQfromBetaAbramovitz(actualBeta, pExpected, qExpected)

    call assert_comparable( pExpected, dble(p), margin, "unexpected return value p from calculated Beta using betaFromQ" )
    call assert_comparable( qExpected, dble(qw), margin, "unexpected return value q from calculated Beta using betaFromQ" )

end subroutine testTinyQuantileBetaFromQ


!> Test betaFromQ and PQgromBeta in the half range of quantile (0 - 0.5)
!>   @ingroup ProbabilisticTests
!!
subroutine testWholeRangeQuantileBetaFromQ

    real (kind = wp)              ::  qLow            !<lower limit for u
    real (kind = wp)              ::  qHigh           !<upper limit for u
    real (kind = wp)              ::  qStep           !<increment for increasing u
    real (kind = wp), allocatable ::  beta(:)         !<vector of beta values
    real (kind = wp), allocatable ::  q(:)            !<vector of exceedance probabilities
    real (kind = wp), allocatable ::  qbyBeta(:)      !<vector of exceedance probabilities associated with beta values
    real (kind = wp), allocatable ::  qbyBetaM(:)     !<vector of exceedance probabilities associated with betaM values
    real (kind = wp), allocatable ::  pbyBeta(:)      !<vector of non-exceedance probabilities associated with beta values
    real (kind = wp), allocatable ::  pbyBetaM(:)     !<vector of non-exceedance probabilities associated with betaM values

    real (kind = wp), allocatable ::  betaM(:)        !<vector of beta values by betaFromPQmin
    integer                       ::  dim             !<length of vector q
    integer                       ::  i               ! counter
    real (kind = wp)              :: qQuad
    real (kind = wp)              :: pQuad

    real (kind = wp)              :: margin = 1.0d-6

    integer                       ::  tabL            !<# spaces at beginning of header line
    integer, parameter            ::  TL = 28         !<total length of output numbers
    integer                       ::  ofile           !<output file number
    integer                       ::  distCol         !<number of columns in output file for distribution
    character(len=TL + 1)         ::  heading(26)     !<heading for output files
    integer                       ::  ierr
    character(len=64)             ::  errorMessage

    qLow    = 1D-10 !0.0D0
    qHigh   = 0.50D0
    qStep   = 0.001D0
    dim     = nint((qHigh - qLow)/qStep + 1)
!   Correction of this dim with the following motivation.
!   For this value of dim the LAST q (i.e. q(ndim) ) will be very close to 0.5. The from this value computed beta(ndim) and
!   betaM(ndim) will be very close to zero. In fact: beta(ndim) ~ 10**(-13) and betaM(ndim) ~ 10**(-62).
!   Despite these virtually the same (and correct) values assert_comparble() considers these more different as tolerated
!   within the relative comparison of the values. To avoid a failure of this test the case q=0.5 is excluded through:
    dim     = dim- 1
!
    allocate( beta(dim) )
    allocate( betaM(dim) )
    allocate( q(dim) )
    allocate( qbyBeta(dim) )
    allocate( qbyBetaM(dim) )
    allocate( pbyBeta(dim) )
    allocate( pbyBetaM(dim) )

    q = (/ (qLow + qStep*(i-1),  i = 1,dim)/)

    !>compute u
    do i = 1,dim
        !> compute Beta using exceedance probability q
        call betaFromQAbramovitz( q(i), beta(i), ierr, errorMessage )
        call assert_equal( ierr, 0, errorMessage)

        !>compute back non-exceedance probability pbyBeta and exceedance probability qbyBeta
        call PQfromBetaAbramovitz(Beta(i), pbyBeta(i), qbyBeta(i) )

        !> compute BetaM using exceedance probability Quadq (Quadratic variable) and using routine betaFromPQmin
        qQuad=real(q(i), 16)
        pQuad=1.Q0-qQuad
        call betaFromPQmin( pQuad, qQuad, betaM(i) )

        !>compute back non-exceedance probability pbyBetaM and exceedance probability qbyBetaM
        call PQfromBetaAbramovitz(BetaM(i), pbyBetaM(i), qbyBetaM(i) )
    enddo

    call assert_comparable( beta, BetaM  , margin, "unexpected return value Beta for the range q (1E-10, 0.50)" )
    call assert_comparable( q   , qbyBeta, margin, "unexpected return value q from PQfromBeta calculated by beta from betafrom q for the range q (1E-10, 0.50)" )

    !>Make tables
    !----------------------------------

    tabL = TL - 22      !<defining the tab length for output formatting

    distCol = 4         !<number of columns to write

    heading(1) = 'Original exceed. prob. (q)'
    heading(2) = 'Beta using q (betaFromQ)'
    heading(3) = 'q via PQfromBeta'
    heading(4) = 'Abs. Diff. orig. and cal. q'

    call getFreeLuNumber( ofile )
    open( ofile, file = 'testWholeRangeQuantileBetaFromQ_q2beta_beta2q_Abramovitz.txt' )
    write( ofile, '(t<tabL>,<distCol>a<TL>)' ) (heading(i), i = 1,distCol)
    write( ofile,'(<distCol>es<TL>.16E3)' ) (q(i), Beta(i), qbyBeta(i), abs(q(i)-qbyBeta(i)),i = 1,dim)
    close( ofile )

    heading(1) = 'Original exceed. prob. (q)'
    heading(2) = 'QBeta using q (betaFromPQMin)'
    heading(3) = 'q via PQfromBeta'
    heading(4) = 'Abs. Diff. orig. and cal. q'

    open( ofile, file = 'testWholeRangeQuantileBetaFromQ_q2beta_beta2q_Quad_Abramovitz.txt' )
    write( ofile, '(t<tabL>,<distCol>a<TL>)' ) (heading(i), i = 1,distCol)
    write( ofile,'(<distCol>es<TL>.16E3)' ) (q(i), BetaM(i), qbyBetaM(i), abs(q(i)-qbyBetaM(i)), i = 1,dim)
    close( ofile )


    deallocate( beta )
    deallocate( betaM )
    deallocate( q )
    deallocate( qbyBeta )
    deallocate( qbyBetaM )
    deallocate( pbyBeta )
    deallocate( pbyBetaM )

end subroutine testWholeRangeQuantileBetaFromQ

!> Test betaFromQ and PQFromBeta in a repetitive way with the 5%-quantile
!!
subroutine repetitiveBetaAndQComputation1

    real (kind = wp)   :: expectedBeta = 1.64485362695147d0
    real (kind = wp)   :: margin = 1.0d-6
    real (kind = wp)   :: actualBeta              ! reliability index (beta)
    real (kind = wp)   :: p                       ! probability of non-exceedance
    real (kind = wp)   :: q                       ! probability of exceedance
    integer            :: i                       ! do-loop counter
    integer            :: ierr                    ! error code
    character(len=64)  :: errorMessage            ! error message

    actualBeta = expectedBeta
    do i = 1, 1000
        call PQFromBetaAbramovitz(actualBeta, p, q)
        call betaFromQAbramovitz (q, actualBeta, ierr, errorMessage)
        call assert_equal( ierr, 0, errorMessage)
    enddo
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine repetitiveBetaAndQComputation1

!> Test betaFromQ and PQFromBeta in a repetitive way with a value close to zero
!!
subroutine repetitiveBetaAndQComputation2

    real (kind = wp)  :: expectedBeta = 6.5d-8
    real (kind = wp)  :: margin = 1.0d-6
    real (kind = wp)  :: actualBeta              ! reliability index (beta)
    real (kind = wp)  :: p                       ! probability of non-exceedance
    real (kind = wp)  :: q                       ! probability of exceedance
    integer           :: i                       ! do-loop counter
    integer           :: ierr
    character(len=64) :: errorMessage

    actualBeta = expectedBeta
    do i = 1, 1000
        call PQFromBetaAbramovitz(actualBeta, p, q)
        call betaFromQAbramovitz (q, actualBeta, ierr, errorMessage)
        call assert_equal( ierr, 0, errorMessage)
    enddo
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine repetitiveBetaAndQComputation2

!> Test betaFromQ and PQFromBeta in a repetitive way with the a large value of beta
!!
subroutine repetitiveBetaAndQComputation3

    real (kind = wp)  :: expectedBeta = 7.8d0
    real (kind = wp)  :: margin = 1.0d-6
    real (kind = wp)  :: actualBeta              ! reliability index (beta)
    real (kind = wp)  :: p                       ! probability of non-exceedance
    real (kind = wp)  :: q                       ! probability of exceedance
    integer           :: i                       ! do-loop counter
    integer           :: ierr
    character(len=64) :: errorMessage

    actualBeta = expectedBeta
    do i = 1, 1000
        call PQFromBetaAbramovitz(actualBeta, p, q)
        call betaFromQAbramovitz (q, actualBeta, ierr, errorMessage)
        call assert_equal( ierr, 0, errorMessage)
    enddo
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine repetitiveBetaAndQComputation3

!> Test betaFromQ and PQFromBeta in a repetitive way with the a large negative value of beta
!!
subroutine repetitiveBetaAndQComputation4

    real (kind = wp)  :: expectedBeta = -7.1d0
    real (kind = wp)  :: margin = 1.0d-6
    real (kind = wp)  :: actualBeta              ! reliability index (beta)
    real (kind = wp)  :: p                       ! probability of non-exceedance
    real (kind = wp)  :: q                       ! probability of exceedance
    integer           :: i                       ! do-loop counter
    integer           :: ierr
    character(len=64) :: errorMessage

    actualBeta = expectedBeta
!
!   Repetitive computations from beta to q and vice versa aren't possible
!   for to probabilities of exceedance close to 1. That is for large negative beta's
!   An example is beta = -7.8. It is also inmpossible for beta = -7.2 (it also depends on
!   the margin)
!
    do i = 1, 1000
        call PQFromBetaAbramovitz(actualBeta, p, q)
        call betaFromQAbramovitz (q, actualBeta, ierr, errorMessage)
        call assert_equal( ierr, 0, errorMessage)
    enddo
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine repetitiveBetaAndQComputation4

!> Test betaFromQ and PQFromBeta in with a table
!!
subroutine repetitiveBetaAndQTable

    real (kind = wp)  :: margin = 1.0d-9
    real (kind = wp)  :: maximum
    real (kind = wp)  :: betaIn               ! reliability index before betaFromQ and PQFromBeta
    real (kind = wp)  :: betaOut              ! reliability index after betaFromQ and PQFromBeta
    real (kind = wp)  :: p                    ! probability of non-exceedance
    real (kind = wp)  :: q                    ! probability of exceedance
    integer           :: i                    ! do-loop counter
    integer           :: ierr                 ! error code
    character(len=64) :: errorMessage         ! error message
!
    maximum = 0.0d0
    do i = 1, 741
!
!       The settings of the do-loop are chosen in such a matter that beta is maximal 37.0 in a absolute way
!       Give beta a value
!
        betaIn = -37.0d0 + (i-1)/10.0d0
!
!       Compute the probability of non-exceedance and the probability of exceedance
!
        call PQFromBetaAbramovitz(betaIn, p, q)
        if (q < 0.5) then
            call betaFromQAbramovitz (q, betaOut, ierr, errorMessage)
            call assert_equal( ierr, 0 , errorMessage)
        else
            call betaFromQAbramovitz (p, betaOut, ierr, errorMessage)
            call assert_equal( ierr, 0 , errorMessage)
            betaOut = -betaOut
        endif
        maximum = max(maximum, abs(betaIn-betaOut))
    enddo
    call assert_Inbetween( maximum, 0.0d0, margin, "unexpected return value in table of beta's" )

end subroutine repetitiveBetaAndQTable

! ---------------------------------------------------------
! End   of a series of subroutines for testing betaFromQ()
! ---------------------------------------------------------
! Begin of a series of subroutines for testing PQFromBeta()
! ---------------------------------------------------------
!
!> Call all the PQFromBeta tests
!!
subroutine allPQFromBetaAbramovitzTests

    call testWithLevel( testExpectedValueBeta_PQFromBeta, "testExpectedValueBeta from module PQFromBetaAbramovitzTests", 1)
    call testWithLevel( test05precentQuantile_PQFromBeta, "test05precentQuantile from module PQFromBetaAbramovitzTests", 1)
    call testWithLevel( test95precentQuantile_PQFromBeta, "test95precentQuantile from module PQFromBetaAbramovitzTests", 1)
    call testWithLevel( testPQFromBetaWholeRange, "testPQFromBetaWholeRange from module PQFromBetaAbramovitzTests; also check files testPQFromBetaWholeRange*.txt", 1 )

end subroutine allPQFromBetaAbramovitzTests

!> Test PQFromBeta with expected value
!!
subroutine testExpectedValueBeta_PQFromBeta

    real (kind = wp) :: beta = 0.0d0
    real (kind = wp) :: expectedP = 0.5d0
    real (kind = wp) :: expectedQ = 0.5d0
    real (kind = wp) :: margin = 1.0d-6
    real (kind = wp) :: actualP
    real (kind = wp) :: actualQ
    call PQFromBetaAbramovitz( beta, actualP, actualQ )
    call assert_comparable( actualP, expectedP, margin, "unexpected return value P" )
    call assert_comparable( actualQ, expectedQ, margin, "unexpected return value Q" )

end subroutine testExpectedValueBeta_PQFromBeta

!> Test PQFromBeta with the 5%-quantile
!!
subroutine test05precentQuantile_PQFromBeta

    real (kind = wp) :: beta = -1.64485362695147d0  ! value generated by using Matlab
    real (kind = wp) :: expectedP = 0.05d0
    real (kind = wp) :: expectedQ = 0.95d0
    real (kind = wp) :: margin = 1.0d-6
    real (kind = wp) :: actualP
    real (kind = wp) :: actualQ
    call PQFromBetaAbramovitz( beta, actualP, actualQ )
    call assert_comparable( actualP, expectedP, margin, "unexpected return value P" )
    call assert_comparable( actualQ, expectedQ, margin, "unexpected return value Q" )

end subroutine test05precentQuantile_PQFromBeta

!> Test PQFromBeta with the 95%-quantile
!!
subroutine test95precentQuantile_PQFromBeta

    real (kind = wp) :: beta = 1.64485362695147d0  ! value generated by using Matlab
    real (kind = wp) :: expectedP = 0.95d0
    real (kind = wp) :: expectedQ = 0.05d0
    real (kind = wp) :: margin = 1.0d-6
    real (kind = wp) :: actualP
    real (kind = wp) :: actualQ
    call PQFromBetaAbramovitz( beta, actualP, actualQ )
    call assert_comparable( actualP, expectedP, margin, "unexpected return value P" )
    call assert_comparable( actualQ, expectedQ, margin, "unexpected return value Q" )

end subroutine test95precentQuantile_PQFromBeta

subroutine testPQFromBetaWholeRange

    real (kind = qd)              ::  uLow              !<lower limit for u
    real (kind = qd)              ::  uHigh             !<upper limit for u
    real (kind = qd)              ::  uStep             !<increment for increasing u
    real (kind = qd), allocatable ::  u(:)              !<vector of u-values (beta values)
    real (kind = qd), allocatable ::  ucal(:)           !<vector of calculated u-values (beta values)
    real (kind = qd), allocatable ::  ucalM(:)          !<vector of calculated u-values (beta values)
    real (kind = qd), allocatable ::  p(:)              !<vector of non-exceedance probabilities associated with u values
    real (kind = qd), allocatable ::  q(:)              !<vector of exceedance probabilities associated with u values
    real (kind = qd), allocatable ::  dpdu(:)           !<vector of derivative of non-exceedance probabilities w.r.t. u
    real (kind = qd), allocatable ::  dqdu(:)           !<vector of derivative of exceedance probabilities w.r.t. u
    real (kind = qd)              ::  margin = 1.0d-6   !< margin value for comparison

    real (kind = qd), allocatable ::  uQuad(:)          !<vector of u-values (beta values) defined as Quadrable
    real (kind = qd), allocatable ::  pQuad(:)          !<vector of non-exceedance probabilities associated with u values calculated useing QERF
    real (kind = qd), allocatable ::  qQuad(:)          !<vector of exceedance probabilities associated with u values calculated useing QERF
    real (kind = qd), allocatable ::  dpQuaddu(:)       !<vector of derivative of pQuad w.r.r. u
    real (kind = qd), allocatable ::  dqQuaddu(:)       !<vector of derivative of qQuad w.r.r. u


    integer                       ::  dim               !<length of vector u
    integer                       ::  i
    real (kind = qd)              ::  pQ
    real (kind = qd)              ::  qQ


    integer                       ::  tabL              !<# spaces at beginning of header line
    integer, parameter            ::  TL = 28           !<total length of output numbers
    integer                       ::  ofile             !<output file number
    integer                       ::  distCol           !<number of columns in output file for distribution
    character(len=TL + 1)         ::  heading(15)       !<heading for output files
    integer                       ::  ierr              !<error code
    character(len=128)            ::  errorMessage      !<error message

    uLow    =-40.0q0
    uHigh   = 40.0q0
    uStep   = 0.1q0
    dim     = nint((uHigh - uLow)/uStep + 1)
    allocate( u(dim) )
    allocate( ucal(dim) )
    allocate( ucalM(dim) )
    allocate( p(dim) )
    allocate( q(dim) )
    allocate( dpdu(dim) )
    allocate( dqdu(dim) )
    allocate( pQuad(dim) )
    allocate( qQuad(dim) )
    allocate( dpQuaddu(dim) )
    allocate( dqQuaddu(dim) )
    allocate( uQuad(dim) )

    u = (/ (uLow + uStep*(i-1),  i = 1,dim)/)
    u = nint(u*10.q0)/10.q0
    uQuad =  (/ (uLow + uStep*(i-1),  i = 1,dim)/)
    uQuad = nint(uQuad*10.Q0)/10.Q0
    !>compute non-exceedance probability p and exceedance probability q
    do i = dim,1,-1
        call PQFromBetaAbramovitz( u(i), p(i), q(i) )
        call betaFromQAbramovitz( q(i), ucal(i), ierr, errorMessage )
        call assert_equal( ierr, 0 , errorMessage)
        qQ = q(i)
        pQ = p(i)
        call betaFromPQmin( pQ, qQ, ucalM(i) )
    enddo

    !>compute non-exceedance probability pQuad and exceedance probability qQuad using built-in QERF
    do i = 1,dim
        pQuad(i) = 0.5Q0*(1.Q0+erf(uQuad(i)/sqrt(2.0Q0)))
        qQuad(i) = 0.5Q0 - 0.5Q0 * erf(uQuad(i)/sqrt(2.0Q0))
    enddo

    do i = 1,dim
        call assert_inbetween( p(i), pQuad(i) - margin, pQuad(i) + margin , "unexpected return value P within the whole range of u (-40,40)" )
    enddo

    do i = 1,dim
        call assert_inbetween( q(i), qQuad(i) - margin, qQuad(i) + margin , "unexpected return value P within the whole range of u (-40,40)" )
    enddo

    !>Gradient calculation and comparison
    dpdu(1)=0.q0
    dpdu(dim)=0.q0
    dqdu(1)=0.q0
    dqdu(dim)=0.q0
    do i = 1, dim-1
        dpdu(i)=(p(i+1)-p(i))/uStep
        dqdu(i)=(q(i+1)-q(i))/uStep
    enddo
    call assert_true( minval(dpdu) >= 0, 'Gradient of p from PQfromBeta is a discontinues function' )
    call assert_true( minval(dqdu) <= 0, 'Gradient of q from PQfromBeta is a discontinues function' )

    dpQuaddu(1)=0.Q0
    dpQuaddu(dim)=0.Q0
    dqQuaddu(1)=0.Q0
    dqQuaddu(dim)=0.Q0
    do i = 1, dim-1
        dpQuaddu(i)=(pQuad(i+1)-pQuad(i))/uStep
        dqQuaddu(i)=(qQuad(i+1)-qQuad(i))/uStep
    enddo
    call assert_true( minval(dpQuaddu) >= 0, 'Gradient of p from ERF is a discontinues function' )

    do i = 1,dim-1
        call assert_inbetween( dpQuaddu(i), dpdu(i) - margin, dpdu(i) + margin , "unexpected return value P within the whole range of u (-40,40)" )
    enddo

    !>Make tables
    !----------------------------------

    tabL = TL - 22      !<defining the tab length for output formatting

    !>uniform - write table

    distCol = 5         !<number of columns to write

    heading(1) = 'u (beta)'
    heading(2) = 'Quad. non-exceedance prob'
    heading(3) = 'non-exceedance prob'
    heading(4) = 'Quad. exceedance prob'
    heading(5) = 'exceedance prob'

    call getFreeLuNumber( ofile )
    open( ofile, file = 'testPQFromBetaWholeRange_QuadPQfromBetadUTable_Abramovitz.txt' )
    write( ofile, '(t<tabL>,<distCol>a<TL>)' ) (heading(i), i = 1,distCol)
    write( ofile,'(<distCol>es<TL>.16E3)' ) (u(i), pQuad(i), p(i), qQuad(i), q(i), i = 1,dim)
    close( ofile )

    heading(1) = 'u (beta)'
    heading(2) = 'd(Quad. non-exceedance prob)/du'
    heading(3) = 'd(non-exceedance prob)/du'
    heading(4) = 'd(Quad. exceedance prob)/du'
    heading(5) = 'd(exceedance prob)/du'

    open( ofile, file = 'testPQFromBetaWholeRange_dQuadPQfromBetadUTable_Abramovitz.txt' )
    write( ofile, '(t<tabL>,<distCol>a<TL>)' ) (heading(i), i = 1,distCol)
    write( ofile,'(<distCol>es<TL>.16E3)' ) (u(i), dpQuaddu(i), dpdu(i), dqQuaddu(i), dqdu(i), i = 1,dim)
    close( ofile )

    !> table to compare u and calculate u (comparison between PQfrombeta and betafromq
    distCol = 3         !<number of columns to write

    heading(1) = 'u (beta)'
    heading(2) = 'non-exceedance prob'
    heading(3) = 'Ucal (calculated)'
    heading(4) = 'UcalM (calculated)'

    open( ofile, file = 'testPQFromBetaWholeRange_BetaVcalBetaTable_Abramovitz.txt' )
    write( ofile, '(t<tabL>,<distCol>a<TL>)' ) (heading(i), i = 1,distCol)
    !write( ofile,'(<distCol>F<TL>.20)' ) (u(i), p(i), ucal(i), ucalM(i), i = 1,dim)
    do i=1,dim
      write( ofile, * ) u(i), q(i), ucal(i)
    enddo
    close( ofile )

    heading(1) = 'u (beta)'
    heading(2) = 'non-exceedance prob'
    heading(3) = 'UcalM (calculated)'
    open( ofile, file = 'testPQFromBetaWholeRange_BetaVcalBetaTableM_Abramovitz.txt' )
    write( ofile, '(t<tabL>,<distCol>a<TL>)' ) (heading(i), i = 1,distCol)
    !write( ofile,'(<distCol>F<TL>.20)' ) (u(i), p(i), ucal(i), ucalM(i), i = 1,dim)
    do i=1,dim
      write( ofile, * ) u(i), q(i), ucalM(i)
    enddo
    close( ofile )

    deallocate( u )
    deallocate( ucal )
    deallocate( ucalM )
    deallocate( p )
    deallocate( q )
    deallocate( dpdu )
    deallocate( dqdu )
    deallocate( pQuad )
    deallocate( dpQuaddu )
    deallocate( dqQuaddu )
    deallocate( qQuad )
    deallocate( uQuad )

end subroutine testPQFromBetaWholeRange
! -------------------------------------------------------
! End   of a series of subroutines for testing PQFromBeta
! -------------------------------------------------------
!
!> All tests of the function PFromBeta \n
!! Checking the consistency of PFromBeta and subroutine PQfromBeta \n
!! \n
subroutine allPFromBetaAbramovitzTests

    call testWithLevel( testPFromBeta1, "PFromBetaAbramovitzTests: 1 Consistency with subroutine PQfromBeta for Beta= 3 ", 1)
    call testWithLevel( testPFromBeta2, "PFromBetaAbramovitzTests: 2 Consistency of function PFromBeta and subroutine PQFromBeta", 1)
    call testWithLevel( testPFromBeta3, "PFromBetaAbramovitzTests: 3 Consistency of function PFromBeta and subroutine PQFromBeta", 1)

end subroutine allPFromBetaAbramovitzTests
!
!
!> Test of function PFromBeta \n
!! Testing the consistency with subroutine PQfromBeta for Beta= 3 \n
!! The non-exceedence probabilities should be equal \n
!!
subroutine testPFromBeta1
    real (kind = wp)   :: beta              ! reliability index beta in the present test
    real (kind = wp)   :: p1                ! probability of non-exceedence for present beta, according to PFromBeta
    real (kind = wp)   :: p2                ! probability of non-exceedence for present beta, according to PQfromBeta
    real (kind = wp)   :: q2                ! probability of     exceedence for present beta, according to PQfromBeta
    real (kind = wp)   :: margin = 1.0d-8   ! acceptable margin when comparing the p1 and p2

    beta = 3.0d0
    p1   = PFromBetaAbramovitz( beta)
    call  PQfromBetaAbramovitz( Beta, p2, q2 )

    call assert_comparable( p1, p2, margin, "An unexpected value is found for the non-exceedance probability P computed by PFromBeta" )

end subroutine testPFromBeta1
!
!
!> Test of function PFromBeta \n
!! Testing the consistency with subroutine PQfromBeta for Beta=-3 \n
!! The non-exceedence probabilities should be equal \n
!!
subroutine testPFromBeta2
    real (kind = wp)    :: beta              ! reliability index beta in the present test
    real (kind = wp)    :: p1                ! probability of non-exceedence for present beta, according to PFromBeta
    real (kind = wp)    :: p2                ! probability of non-exceedence for present beta, according to PQfromBeta
    real (kind = wp)    :: q2                ! probability of     exceedence for present beta, according to PQfromBeta
    real (kind = wp)    :: margin = 1.0d-8   ! acceptable margin when comparing the p1 and p2

    beta =-3.0d0
    p1   = PFromBetaAbramovitz( beta)
    call  PQfromBetaAbramovitz( Beta, p2, q2 )

    call assert_comparable( p1, p2, margin, "An unexpected value is found for the non-exceedance probability P computed by PFromBeta" )

end subroutine testPFromBeta2
!
!
!> Test of function PFromBeta \n
!! Testing the consistency with subroutine PQfromBeta for Beta=-3 \n
!! The non-exceedence probabilities should be equal \n
!!
subroutine testPFromBeta3
    real (kind = wp)   :: beta             ! reliability index beta in the present test
    real (kind = wp)   :: p1               ! probability of non-exceedence for present beta, according to PFromBeta
    real (kind = wp)   :: p2               ! probability of non-exceedence for present beta, according to PQfromBeta
    real (kind = wp)   :: q2               ! probability of     exceedence for present beta, according to PQfromBeta
    real (kind = wp)   :: margin = 1.0d-8  ! acceptable margin when comparing the p1 and p2

    beta = 8.0d0
    p1   = PFromBetaAbramovitz( beta)
    call  PQfromBetaAbramovitz( Beta, p2, q2 )

    call assert_comparable( p1, p2, margin, "An unexpected value is found for the non-exceedance probability P computed by PFromBeta" )

end subroutine testPFromBeta3
!
!> All tests of the function QFromBeta \n
!! Checking the consistency of QFromBeta and subroutine PQfromBeta \n
!! \n
subroutine allQFromBetaAbramovitzTests

    call testWithLevel( testQFromBeta1, "QFromBetaAbramovitzTests: 1 Consistency of function QFromBeta and subroutine PQFromBeta", 1)
    call testWithLevel( testQFromBeta2, "QFromBetaAbramovitzTests: 2 Consistency of function QFromBeta and subroutine PQFromBeta", 1)
    call testWithLevel( testQFromBeta3, "QFromBetaAbramovitzTests: 3 Consistency of function QFromBeta and subroutine PQFromBeta", 1)

end subroutine allQFromBetaAbramovitzTests
!
!
!> Test of function QFromBeta \n
!! Testing the consistency with subroutine PQfromBeta for Beta= 3 \n
!! The exceedence probabilities should be equal \n
!!
subroutine testQFromBeta1
    real (kind = wp)   :: beta              ! reliability index beta in the present test
    real (kind = wp)   :: q1                ! probability of     exceedence for present beta, according to QFromBeta
    real (kind = wp)   :: q2                ! probability of     exceedence for present beta, according to PQfromBeta
    real (kind = wp)   :: p                 ! probability of non-exceedence for present beta, according to PQfromBeta
    real (kind = wp)   :: margin = 1.0d-8   ! acceptable margin when comparing the p1 and p2

    beta = 3.0d0
    q1   = QFromBetaAbramovitz( beta)
    call  PQfromBetaAbramovitz( Beta, p , q2 )

    call assert_comparable( q1, q2, margin, "An unexpected value is found for the exceedance probability Q computed by QFromBeta" )

end subroutine testQFromBeta1
!
!
!> Test of function QFromBeta \n
!! Testing the consistency with subroutine PQfromBeta for Beta=-3 \n
!! The exceedence probabilities should be equal \n
!!
subroutine testQFromBeta2
    real (kind = wp)   :: beta              ! reliability index beta in the present test
    real (kind = wp)   :: q1                ! probability of     exceedence for present beta, according to QFromBeta
    real (kind = wp)   :: q2                ! probability of     exceedence for present beta, according to PQfromBeta
    real (kind = wp)   :: p                 ! probability of non-exceedence for present beta, according to PQfromBeta
    real (kind = wp)   :: margin = 1.0d-8   ! acceptable margin when comparing the p1 and p2

    beta =-3.0d0
    q1   = QFromBetaAbramovitz( beta)
    call  PQfromBetaAbramovitz( Beta, p , q2 )

    call assert_comparable( q1, q2, margin, "An unexpected value is found for the exceedance probability Q computed by QFromBeta" )

end subroutine testQFromBeta2
!
!
!> Test of function QFromBeta \n
!! Testing the consistency with subroutine PQfromBeta for Beta= 8 \n
!! The exceedence probabilities should be equal \n
!!
subroutine testQFromBeta3
    real (kind = wp)   :: beta              ! reliability index beta in the present test
    real (kind = wp)   :: q1                ! probability of     exceedence for present beta, according to QFromBeta
    real (kind = wp)   :: q2                ! probability of     exceedence for present beta, according to PQfromBeta
    real (kind = wp)   :: p                 ! probability of non-exceedence for present beta, according to PQfromBeta
    real (kind = wp)   :: margin = 1.0d-8   ! acceptable margin when comparing the p1 and p2

    beta = 8.0d0
    q1   = QFromBetaAbramovitz( beta)
    call  PQfromBetaAbramovitz( Beta, p , q2 )

    call assert_comparable( q1, q2, margin, "An unexpected value is found for the exceedance probability Q computed by QFromBeta" )

end subroutine testQFromBeta3

end module conversionFunctionsAbramovitzTests
