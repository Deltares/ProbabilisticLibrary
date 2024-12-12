! Copyright (C) Stichting Deltares. All rights reserved.
!
! This file is part of the Probabilistic Library.
!
! The Probabilistic Library is free software: you can redistribute it and/or modify
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
!> In the first  part of subroutine allDistributionFunctionsTests several betaFromQ()  tests are executed
!> In the second part of subroutine allDistributionFunctionsTests several PQFromBeta() tests are executed
!
module conversionFunctionsTests
    !
    !------------------------------------------------------------------------
    !   programmer: Kathryn Roscoe
    !               Henk van den Boogaard and Edwin Spee (modifications, upgrading, extensions)
    !------------------------------------------------------------------------

    use ftnunit
    use precision
    !use fileUtilities
    !use equalReals
    use interface_convert
    use betaFromPQminModule
    use steinbrecherSeries

    implicit none

    ! margin used for comparison of floating point numbers
    real (kind = wp), parameter :: margin = 1.0d-10
    
    private
    
    public :: allBetaFromQTests, allPFromBetaTests, allPQFromBetaTests, allQFromBetaTests, allReturnTimeFreqTests

    contains

!
! Begin of a series of subroutines for testing betaFromQ()
! -------------------------------------------------------------------
!
!> Call all the betaFromQ() tests
!!
subroutine allBetaFromQTests

    call testWithLevel( testExpectedValueBeta_BetaFromQ, "testExpectedValueBeta from module betaFromQTests", 1)
    call testWithLevel( testProbabilityEqual2, "testProbabilityEqual2 from module betaFromQTests", 2)
    call testWithLevel( testProbabilityEqual0and1, "testProbabilityEqual0and1 from module betaFromQTests", 1)
    call testWithLevel( testProbabilityEqualMinus1, "testProbabilityEqualMinus1 from module betaFromQTests", 2)
    call testWithLevel( testProbabilityEqualNaN, "testProbabilityEqualNaN from module betaFromQTests", 1)
    call testWithLevel( test05precentQuantile_BetaFromQ, "test05precentQuantile from module betaFromQTests", 1)
    call testWithLevel( test95precentQuantile_BetaFromQ, "test95precentQuantile from module betaFromQTests", 1)
    call testWithLevel( testTinyQuantileBetaFromPQMin, "testTinyQuantileBetaFromPQMin from module betaFromQTests", 1)
    call testWithLevel( testTinyQuantileBetaFromQ, "testTinyQuantileBetafromQ from module betaFromQTests", 1)
    
    call testWithLevel( testWholeRangeQuantileBetaFromQ,"testWholeRangeQuantileBetaFromQ from module betaFromQTests", 1)
    call testWithLevel( testComputeSteinbrecherSeries, "testComputeSteinbrecherSeries from module betaFromQTests", 3)
    ! also check files testWholeRangeQuantileBetaFromQ*.txt

    call testWithLevel( repetitiveBetaAndQComputation1, "repetitiveBetaAndQComputation1 from module betaFromQTests", 1)
    call testWithLevel( repetitiveBetaAndQComputation2, "repetitiveBetaAndQComputation2 from module betaFromQTests", 1)
    call testWithLevel( repetitiveBetaAndQComputation3, "repetitiveBetaAndQComputation3 from module betaFromQTests", 1)
    call testWithLevel( repetitiveBetaAndQComputation4, "repetitiveBetaAndQComputation4 from module betaFromQTests", 1)
    call testWithLevel( repetitiveBetaAndQTable, "repetitiveBetaAndQTable from module betaFromQTests", 1)
    
end subroutine allBetaFromQTests

!> Test betaFromQ with the expected value
!!
subroutine testExpectedValueBeta_BetaFromQ

    real (kind = wp)   :: q = 0.5d0
    real (kind = wp)   :: expectedBeta = 0.0d0
    real (kind = wp)   :: actualBeta
    integer            :: ierr
    character(len=128) :: errorMessage

    call betaFromQ( q, actualBeta, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine testExpectedValueBeta_BetaFromQ

!> Test betaFromQ with impossible probability of 2.0
!!
subroutine testProbabilityEqual2

    real (kind = wp)   :: q
    
    real (kind = wp)   :: expectedBeta
    real (kind = wp)   :: actualBeta
    integer            :: ierr
    character(len=128) :: errorMessage

    q = 2.0d0
    expectedBeta = huge(q)
    call betaFromQ( q, actualBeta, ierr, errorMessage )
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
    call betaFromQ( q, beta0, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    q = 1.0d-300
    call betaFromQ( q, betaMin, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_true( beta0 > betaMin - margin, "unexpected return value Beta" )
    q = 1.0d0
    call betaFromQ( q, beta1, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( -beta0, beta1, margin, "unexpected return value Beta" )

end subroutine testProbabilityEqual0and1


!> Test betaFromQ with impossible probability of -1.0
!!
subroutine testProbabilityEqualMinus1

    real (kind = wp)   :: q
    real (kind = wp)   :: expectedBeta
    real (kind = wp)   :: actualBeta
    integer            :: ierr
    character(len=128) :: errorMessage

    q = -1.0d0
    expectedBeta = huge(q)
    call betaFromQ( q, actualBeta, ierr, errorMessage )
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
    call betaFromQ( q, actualBeta, ierr, errorMessage )
    call assert_true( isnan(actualBeta), "Beta <> NaN" )

end subroutine testProbabilityEqualNaN

!> Test betaFromQ with the 5%-quantile
!!
subroutine test05precentQuantile_BetaFromQ

    real (kind = wp)  :: q = 0.95d0
    real (kind = wp)  :: expectedBeta = -1.64485362695147d0  ! value generated by using Matlab
    real (kind = wp)  :: actualBeta
    integer           :: ierr
    character(len=64) :: errorMessage

    call betaFromQ( q, actualBeta, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine test05precentQuantile_BetaFromQ

!> Test betaFromQ with the 95%-quantile
!!
subroutine test95precentQuantile_BetaFromQ

    real (kind = wp)  :: q = 0.05d0
    real (kind = wp)  :: expectedBeta = 1.64485362695147d0  ! value generated by using Matlab
    real (kind = wp)  :: actualBeta
    integer           :: ierr
    character(len=64) :: errorMessage

    call betaFromQ( q, actualBeta, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine test95precentQuantile_BetaFromQ

!> Test betafromQ with p=1E-16
!>   @ingroup ProbabilisticTests
!

subroutine testTinyQuantileBetaFromPQmin

    real (kind = wp) :: p = 1.E-16_wp
    real (kind = wp) :: qw
    real (kind = wp) :: qExpected, pExpected
    real (kind = wp) :: actualBeta
    real( kind = wp) :: eps = EPSILON(p)
    
    qw = 1._wp - p
    call betaFromPQmin( p, qw, actualBeta )
    call PQfromBeta(actualBeta, pExpected, qExpected)
    
    call assert_inbetween( abs(pExpected - p) , 0.0_wp, eps, "unexpected return value p from calculated Beta using betaFromPQmin" )
    call assert_inbetween( abs(qExpected - qw), 0.0_wp, eps, "unexpected return value q from calculated Beta using betaFromPQmin" )
    
end subroutine testTinyQuantileBetaFromPQmin

!> Test betafromQ with p=1E-16
!>   @ingroup ProbabilisticTests
!
subroutine testTinyQuantileBetaFromQ

    real (kind = wp)  :: p = 1.E-16_wp
    real (kind = wp)  :: qw
    real (kind = wp)  :: qExpected, pExpected    
    real (kind = wp)  :: actualBeta
    real( kind = wp) :: eps = EPSILON(p)
    integer           :: ierr
    character(len=64) :: errorMessage

    if (p < 0.5) then
        qw = p
    else
        qw = 1.0_wp - p
    endif
    call betaFromQ( qw, actualBeta, ierr, errorMessage )
    call assert_equal( ierr, 0, errorMessage)

    if (p < 0.5) then
        actualbeta = -actualbeta
        qw = 1.0_wp - p
    endif
    
    call PQfromBeta(actualBeta, pExpected, qExpected)
    
    call assert_inbetween( abs(pExpected - p ), 0.0_wp, eps, "unexpected return value p from calculated Beta using betaFromQ" )
    call assert_inbetween( abs(qExpected - qw), 0.0_wp, eps, "unexpected return value q from calculated Beta using betaFromQ" )
   
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
    real (kind = wp)              ::  pdble
    real (kind = wp)              ::  qdble
    integer                       ::  dim             !<length of vector q
    integer                       ::  i               ! counter

    real (kind = wp)              ::  margin = 1.0d-6

    integer                       ::  tabL            !<# spaces at beginning of header line
    integer, parameter            ::  TL = 28         !<total length of output numbers
    integer                       ::  ofile           !<output file number
    integer                       ::  distCol         !<number of columns in output file for distribution
    character(len=TL + 1)         ::  heading(26)     !<heading for output files
    integer                       ::  ierr
    character(len=64)             ::  errorMessage

    qLow    = 1D-10 !0.0D0
    qHigh   = 0.5_wp
    qStep   = 0.001_wp
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
        call betaFromQ( q(i), beta(i), ierr, errorMessage )
        call assert_equal( ierr, 0, errorMessage)

        !>compute back non-exceedance probability pbyBeta and exceedance probability qbyBeta
        call PQfromBeta(beta(i), pbyBeta(i), qbyBeta(i) )
        
        !> compute BetaM using routine betaFromPQmin
        qdble = q(i)
        pdble = 1.0_wp - qdble
        call betaFromPQmin( pdble, qdble, betaM(i) )
        
        !>compute back non-exceedance probability pbyBetaM and exceedance probability qbyBetaM
        call PQfromBeta(BetaM(i), pbyBetaM(i), qbyBetaM(i) )
    enddo
   
    call assert_comparable( beta, dble(betaM)  , margin, "unexpected return value Beta for the range q (1E-10, 0.50)" )
    call assert_comparable( q   , qbyBeta, margin, &
            "unexpected return value q from PQfromBeta calculated by beta from betafrom q for the range q (1E-10, 0.50)" )
    
    !>Make tables
    !----------------------------------   
    
    tabL = TL - 22      !<defining the tab length for output formatting
    
    distCol = 4         !<number of columns to write
    
    heading(1) = 'Original exceed. prob. (q)'
    heading(2) = 'Beta using q (betaFromQ)'
    heading(3) = 'q via PQfromBeta'
    heading(4) = 'Abs. Diff. orig. and cal. q'

    open( newunit=ofile, file = 'testWholeRangeQuantileBetaFromQ_q2beta_beta2q_Wichura.txt' )
    write( ofile, * ) (heading(i), i = 1,distCol)
    write( ofile, * ) (q(i), Beta(i), qbyBeta(i), abs(q(i)-qbyBeta(i)),i = 1,dim)
    close( ofile )

    heading(1) = 'Original exceed. prob. (q)'
    heading(2) = 'QBeta using q (betaFromPQMin)'
    heading(3) = 'q via PQfromBeta'
    heading(4) = 'Abs. Diff. orig. and cal. q'

    open( newunit=ofile, file = 'testWholeRangeQuantileBetaFromQ_q2beta_beta2q_Quad_Wichura.txt' )
    write( ofile, * ) (heading(i), i = 1,distCol)
    write( ofile, * ) (q(i), BetaM(i), qbyBetaM(i), abs(q(i)-qbyBetaM(i)), i = 1,dim)
    close( ofile )

    deallocate( beta )
    deallocate( betaM )
    deallocate( q )
    deallocate( qbyBeta )
    deallocate( qbyBetaM )
    deallocate( pbyBeta )
    deallocate( pbyBetaM )

end subroutine testWholeRangeQuantileBetaFromQ

subroutine testComputeSteinbrecherSeries

    real(kind=qd), allocatable, dimension(:) :: q
    real(kind=dp), allocatable, dimension(:) :: res_series, res_wichura
    real (kind = wp)              ::  qLow            !<lower limit for u
    real (kind = wp)              ::  qHigh           !<upper limit for u
    real (kind = wp)              ::  qStep           !<increment for increasing u

    integer            :: i
    integer            :: ierr
    integer            :: dim
    character(len=128) :: errorMessage
    character(len=30)  :: heading(6)       !<heading for output files
    integer            :: ofile            !<output file number
    integer            :: tabL             !<# spaces at beginning of header line

    real(kind=qd), parameter, dimension(20) :: q2 = &
          (/1.E-16_wp, 1.E-15_wp, 1.E-14_wp, 1.E-13_wp, 1.E-12_wp, 1.E-11_wp, 1.E-10_wp, 1.E-9_wp, &
            1.E-8_wp, 1.E-7_wp,  1.E-6_wp,  1.E-5_wp,  1.E-4_wp,  1.E-3_wp,  1.E-2_wp,  0.1_wp, 0.2_wp, 0.3_wp, 0.4_wp, 0.499_wp/)
    ! values computed in R, with function qnorm (method also uses Wichura approximation) 
    real(kind=wp), parameter, dimension(20) :: res_R = (/-8.222082216130437_wp, & 
                                                           -7.941345326170996_wp, &
                                                           -7.650628092935268_wp, &
                                                           -7.348796102800677_wp, &
                                                           -7.034483825301132_wp, & 
                                                           -6.706023155495136_wp, &
                                                           -6.361340902404057_wp, & 
                                                           -5.997807015007686_wp, & 
                                                           -5.612001244174788_wp, & 
                                                           -5.199337582192818_wp, & 
                                                           -4.753424308822898_wp, & 
                                                           -4.264890793922824_wp, & 
                                                           -3.719016485455680_wp, & 
                                                           -3.090232306167814_wp, &
                                                           -2.326347874040841_wp, &
                                                           -1.2815515655446_wp, &
                                                           -0.8416212335729143_wp, &
                                                           -0.5244005127080409_wp, &
                                                           -0.2533471031357998_wp, &
                                                           -0.002506630899571766_wp/)

    qLow    = 0.001D0 !0.0D0
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
    allocate( q(dim) )
    allocate( res_series(dim) )
    allocate( res_wichura(dim) )
    
    q = (/ (qLow + qStep*(i-1),  i = 1,dim)/)

    ! initialize terms in Steinbrecher series
    call init_series(7500)

    ierr = 0
    do i=1,dim
        res_series(i) = - RecursiveQuantile(q(i),30000)
        call betaFromQ( dble(q(i)), res_wichura(i), ierr, errorMessage )        
    end do


    tabL = 6      !<defining the tab length for output formatting

    open( newunit=ofile, file = 'testComputeSteinbrecherSeries_betacompare.txt' )

    heading(1) = 'q'
    heading(2) = 'Beta'
    heading(3) = ''
    write( ofile, * ) (heading(i), i = 1,3)

    heading(1) = ''
    heading(2) = 'Power Series (PS)'
    heading(3) = 'abs(Wichura - PS)'
    write( ofile, * ) (heading(i), i = 1,3)

    write( ofile,'(3es28.16E3)' ) (q(i), res_series(i), abs(res_wichura(i)-res_series(i)), i=1,dim)
    
    close( ofile )

    if (size(res_R,1) < dim) then
        ierr = 0
        do i=1,size(res_R,1)
            ! qLow = 1.6E-4 smallest value possible with power series computation for quad numbers
            if (q2(i) > 1.6E-4) then  
                res_series(i) = RecursiveQuantile(q2(i),30000)
            else
                res_series(i) = res_R(i)
            endif
            
            call betaFromQ( dble(q2(i)), res_wichura(i), ierr, errorMessage )    
        end do

        open( newunit=ofile, file = 'testCompareWithRValues_betacompare.txt' )

        heading(1) = 'q'
        heading(2) = 'Beta'
        heading(3) = ''         
        write( ofile, * ) (heading(i), i = 1,6)
    
        heading(1) = ''
        heading(2) = 'Wichura'
        heading(3) = 'abs(PS - R)'
        write( ofile, * ) (heading(i), i = 1,6)

        write( ofile,'(3es28.16E3)' ) (q2(i), res_wichura(i), abs(res_series(i) - res_R(i)), i=1,size(res_R,1))
    
        close( ofile )
    endif
    
    call finish_series()

    deallocate( q )
    deallocate( res_series )
    deallocate( res_wichura )

end subroutine testComputeSteinbrecherSeries

!> Test betaFromQ and PQFromBeta in a repetitive way with the 5%-quantile
!!
subroutine repetitiveBetaAndQComputation1

    real (kind = wp)   :: expectedBeta = 1.64485362695147d0
    real (kind = wp)   :: actualBeta              ! reliability index (beta)
    real (kind = wp)   :: p                       ! probability of non-exceedance
    real (kind = wp)   :: q                       ! probability of exceedance
    integer            :: i                       ! do-loop counter
    integer            :: ierr                    ! error code
    character(len=64)  :: errorMessage            ! error message

    actualBeta = expectedBeta
    do i = 1, 1000
        call PQFromBeta(actualBeta, p, q)
        call betaFromQ (q, actualBeta, ierr, errorMessage)
        call assert_equal( ierr, 0, errorMessage)
    enddo
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine repetitiveBetaAndQComputation1

!> Test betaFromQ and PQFromBeta in a repetitive way with a value close to zero
!!
subroutine repetitiveBetaAndQComputation2

    real (kind = wp)  :: expectedBeta = 6.499999994529636D-008
    real (kind = wp)  :: actualBeta              ! reliability index (beta)
    real (kind = wp)  :: p                       ! probability of non-exceedance
    real (kind = wp)  :: q                       ! probability of exceedance
    integer           :: i                       ! do-loop counter
    integer           :: ierr
    character(len=64) :: errorMessage

    actualBeta = expectedBeta
    do i = 1, 1000
        call PQFromBeta(actualBeta, p, q)
        call betaFromQ (q, actualBeta, ierr, errorMessage)
        call assert_equal( ierr, 0, errorMessage)
    enddo
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine repetitiveBetaAndQComputation2

!> Test betaFromQ and PQFromBeta in a repetitive way with the a large value of beta
!!
subroutine repetitiveBetaAndQComputation3

    real (kind = wp)  :: expectedBeta = 7.8_wp
    real (kind = wp)  :: margin = 1.0d-6
    real (kind = wp)  :: actualBeta              ! reliability index (beta)
    real (kind = wp)  :: p                       ! probability of non-exceedance
    real (kind = wp)  :: q                       ! probability of exceedance
    integer           :: i                       ! do-loop counter
    integer           :: ierr
    character(len=64) :: errorMessage
    
    actualBeta = expectedBeta
    do i = 1, 1000
        call PQFromBeta(actualBeta, p, q)
        call betaFromQ (q, actualBeta, ierr, errorMessage)
        call assert_equal( ierr, 0, errorMessage)
    enddo
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine repetitiveBetaAndQComputation3

!> Test betaFromQ and PQFromBeta in a repetitive way with the a large negative value of beta
!!
subroutine repetitiveBetaAndQComputation4

    real (kind = wp)  :: expectedBeta
    real (kind = wp)  :: actualBeta              ! reliability index (beta)
    real (kind = wp)  :: p                       ! probability of non-exceedance
    real (kind = wp)  :: q                       ! probability of exceedance
    integer           :: i                       ! do-loop counter
    integer           :: ierr
    character(len=64) :: errorMessage

    p = 6.2E-13
    q = 1.0_wp - p
    call betaFromQ(q, expectedBeta,ierr,errorMessage)
    actualBeta = expectedBeta
!
!   Repetitive computations from beta to q and vice versa aren't possible
!   for to probabilities of exceedance close to 1. That is for large negative beta's
!   An example is beta = -7.8. It is also inmpossible for beta = -7.2 (it also depends on
!   the margin)
!
    do i = 1, 1000
        call PQFromBeta(actualBeta, p, q)
        call betaFromQ (q, actualBeta, ierr, errorMessage)
        call assert_equal( ierr, 0, errorMessage)
    enddo
    call assert_comparable( actualBeta, expectedBeta, margin, "unexpected return value Beta" )

end subroutine repetitiveBetaAndQComputation4

!> Test betaFromQ and PQFromBeta in with a table
!!
subroutine repetitiveBetaAndQTable

    real (kind = wp)  :: margin = 2.0E-9_wp
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
        call PQFromBeta(betaIn, p, q)
        if (q < 0.5) then
            call betaFromQ (q, betaOut, ierr, errorMessage)
            call assert_equal( ierr, 0 , errorMessage)
        else
            call betaFromQ (p, betaOut, ierr, errorMessage)
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
subroutine allPQFromBetaTests

    call testWithLevel( testExpectedValueBeta_PQFromBeta, "testExpectedValueBeta from module PQFromBetaTests", 1)
    call testWithLevel( test05precentQuantile_PQFromBeta, "test05precentQuantile from module PQFromBetaTests", 1)
    call testWithLevel( test95precentQuantile_PQFromBeta, "test95precentQuantile from module PQFromBetaTests", 1)
    call testWithLevel( testPQFromBetaWholeRange, &
            "testPQFromBetaWholeRange from module PQFromBetaTests; also check files testPQFromBetaWholeRange*.txt", 1 )

end subroutine allPQFromBetaTests

!> Test PQFromBeta with expected value
!!
subroutine testExpectedValueBeta_PQFromBeta

    real (kind = wp) :: beta = 0.0d0
    real (kind = wp) :: expectedP = 0.5d0
    real (kind = wp) :: expectedQ = 0.5d0
    real (kind = wp) :: actualP
    real (kind = wp) :: actualQ
    call PQFromBeta( beta, actualP, actualQ )
    call assert_comparable( actualP, expectedP, margin, "unexpected return value P" )
    call assert_comparable( actualQ, expectedQ, margin, "unexpected return value Q" )

end subroutine testExpectedValueBeta_PQFromBeta

!> Test PQFromBeta with the 5%-quantile
!!
subroutine test05precentQuantile_PQFromBeta

    real (kind = wp) :: beta = -1.64485362695147d0  ! value generated by using Matlab
    real (kind = wp) :: expectedP = 0.05d0
    real (kind = wp) :: expectedQ = 0.95d0
    real (kind = wp) :: actualP
    real (kind = wp) :: actualQ
    call PQFromBeta( beta, actualP, actualQ )
    call assert_comparable( actualP, expectedP, margin, "unexpected return value P" )
    call assert_comparable( actualQ, expectedQ, margin, "unexpected return value Q" )

end subroutine test05precentQuantile_PQFromBeta

!> Test PQFromBeta with the 95%-quantile
!!
subroutine test95precentQuantile_PQFromBeta

    real (kind = wp) :: beta = 1.64485362695147d0  ! value generated by using Matlab
    real (kind = wp) :: expectedP = 0.95d0
    real (kind = wp) :: expectedQ = 0.05d0
    real (kind = wp) :: actualP
    real (kind = wp) :: actualQ
    call PQFromBeta( beta, actualP, actualQ )
    call assert_comparable( actualP, expectedP, margin, "unexpected return value P" )
    call assert_comparable( actualQ, expectedQ, margin, "unexpected return value Q" )

end subroutine test95precentQuantile_PQFromBeta

subroutine testPQFromBetaWholeRange

    real (kind = qd)              ::  uLow              !<lower limit for u
    real (kind = qd)              ::  uHigh             !<upper limit for u
    real (kind = qd)              ::  uStep             !<increment for increasing u
    real (kind = qd), allocatable ::  u(:)              !<vector of u-values (beta values)
    real (kind = wp), allocatable ::  ucal(:)           !<vector of calculated u-values (beta values)
    real (kind = wp), allocatable ::  ucalM(:)          !<vector of calculated u-values (beta values)
    real (kind = wp), allocatable ::  p(:)              !<vector of non-exceedance probabilities associated with u values
    real (kind = wp), allocatable ::  q(:)              !<vector of exceedance probabilities associated with u values
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
        call PQFromBeta( dble(u(i)), p(i), q(i) )
        call betaFromQ( q(i), ucal(i), ierr, errorMessage )
        call assert_equal( ierr, 0 , errorMessage)
        call betaFromPQmin( p(i), q(i), ucalM(i) )
    enddo

    !>compute non-exceedance probability pQuad and exceedance probability qQuad using built-in QERF
    do i = 1,dim
        pQuad(i) = 0.5Q0*(1.Q0+erf(uQuad(i)/sqrt(2.0Q0)))
        qQuad(i) = 0.5Q0 - 0.5Q0 * erf(uQuad(i)/sqrt(2.0Q0))
    enddo
    
    do i = 1,dim
        call assert_inbetween( real(p(i),qd), pQuad(i) - margin, pQuad(i) + margin , &
            "unexpected return value P within the whole range of u (-40,40)" )
    enddo
    
    do i = 1,dim
        call assert_inbetween( real(q(i),qd), qQuad(i) - margin, qQuad(i) + margin , &
            "unexpected return value P within the whole range of u (-40,40)" )
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
    call assert_true( minval(dpdu) >= 0, 'Gradient of p from PQfromBeta is a discontinuous function' )
    call assert_true( minval(dqdu) <= 0, 'Gradient of q from PQfromBeta is a discontinuous function' )
    
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
        call assert_inbetween( dpQuaddu(i), dpdu(i) - margin, dpdu(i) + margin , &
            "unexpected return value P within the whole range of u (-40,40)" )
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

    open( newunit=ofile, file = 'testPQFromBetaWholeRange_QuadPQfromBetadUTable_Wichura.txt' )
    write( ofile, * ) (heading(i), i = 1,distCol)
    write( ofile, * ) (u(i), pQuad(i), p(i), qQuad(i), q(i), i = 1,dim)
    close( ofile )

    heading(1) = 'u (beta)'
    heading(2) = 'd(Quad. non-exceedance prob)/du'
    heading(3) = 'd(non-exceedance prob)/du'
    heading(4) = 'd(Quad. exceedance prob)/du'
    heading(5) = 'd(exceedance prob)/du'

    open( newunit=ofile, file = 'testPQFromBetaWholeRange_dQuadPQfromBetadUTable_Wichura.txt' )
    write( ofile, * ) (heading(i), i = 1,distCol)
    write( ofile, * ) (u(i), dpQuaddu(i), dpdu(i), dqQuaddu(i), dqdu(i), i = 1,dim)
    close( ofile )

    !> table to compare u and calculate u (comparison between PQfrombeta and betafromq
    distCol = 3         !<number of columns to write
    
    heading(1) = 'u (beta)'
    heading(2) = 'non-exceedance prob'
    heading(3) = 'Ucal (calculated)'
    heading(4) = 'UcalM (calculated)'
    
    open( newunit=ofile, file = 'testPQFromBetaWholeRange_BetaVcalBetaTable_Wichura.txt' )
    write( ofile, * ) (heading(i), i = 1,distCol)
    do i=1,dim
      write( ofile, * ) u(i), real(q(i),qd), real(ucal(i),qd)
    enddo
    close( ofile )

    heading(1) = 'u (beta)'
    heading(2) = 'non-exceedance prob'
    heading(3) = 'UcalM (calculated)'
    open( newunit=ofile, file = 'testPQFromBetaWholeRange_BetaVcalBetaTableM_Wichura.txt' )
    write( ofile, * ) (heading(i), i = 1,distCol)
    do i=1,dim
      write( ofile, * ) u(i), real(q(i),qd), real(ucalM(i),qd)
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
subroutine allPFromBetaTests

    call testWithLevel( testPFromBeta1, "PFromBetaTests: 1 Consistency with subroutine PQfromBeta for Beta= 3 ", 1)
    call testWithLevel( testPFromBeta2, "PFromBetaTests: 2 Consistency of function PFromBeta and subroutine PQFromBeta", 1)
    call testWithLevel( testPFromBeta3, "PFromBetaTests: 3 Consistency of function PFromBeta and subroutine PQFromBeta", 1)

end subroutine allPFromBetaTests
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
    p1   = PFromBeta( beta)
    call  PQfromBeta( Beta, p2, q2 )

    call assert_comparable( p1, p2, margin, &
            "An unexpected value is found for the non-exceedance probability P computed by PFromBeta" )

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
    p1   = PFromBeta( beta)
    call  PQfromBeta( Beta, p2, q2 )

    call assert_comparable( p1, p2, margin, &
            "An unexpected value is found for the non-exceedance probability P computed by PFromBeta" )

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
    p1   = PFromBeta( beta)
    call  PQfromBeta( Beta, p2, q2 )

    call assert_comparable( p1, p2, margin, &
        "An unexpected value is found for the non-exceedance probability P computed by PFromBeta" )

end subroutine testPFromBeta3
!
!> All tests of the function QFromBeta \n
!! Checking the consistency of QFromBeta and subroutine PQfromBeta \n
!! \n
subroutine allQFromBetaTests

    call testWithLevel( testQFromBeta1, "QFromBetaTests: 1 Consistency of function QFromBeta and subroutine PQFromBeta", 1)
    call testWithLevel( testQFromBeta2, "QFromBetaTests: 2 Consistency of function QFromBeta and subroutine PQFromBeta", 1)
    call testWithLevel( testQFromBeta3, "QFromBetaTests: 3 Consistency of function QFromBeta and subroutine PQFromBeta", 1)

end subroutine allQFromBetaTests
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
    q1   = QFromBeta( beta)
    call  PQfromBeta( Beta, p , q2 )

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
    q1   = QFromBeta( beta)
    call  PQfromBeta( Beta, p , q2 )

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
    q1   = QFromBeta( beta)
    call  PQfromBeta( Beta, p , q2 )

    call assert_comparable( q1, q2, margin, "An unexpected value is found for the exceedance probability Q computed by QFromBeta" )

end subroutine testQFromBeta3
!
!> All tests of ReturnTimeFromBeta, FreqFromBeta and LogQFromBeta \n
!! Comparison with results per-computed with Matlab \n
!!
subroutine allReturnTimeFreqTests

    call testWithLevel( testReturnTimeFromBeta, "ReturnTimeFromBeta: check return period for different beta values", 1)
    call testWithLevel( testFreqFromBeta, "FreqFromBeta: check frequency for different beta values", 1)
    call testWithLevel( testLogQFromBeta, "LogQFromBeta: check -log(Q) for different beta values", 1)

end subroutine allReturnTimeFreqTests
!
!
!> Test of function ReturnTimeFromBeta \n
!! Comparison with results per-computed with Matlab \n
!!
subroutine testReturnTimeFromBeta
    integer, parameter  :: dim = 8                  ! dimension, number of comparisons
    real (kind = wp)    :: beta                     ! reliability index beta in the present test
    real (kind = wp)    :: returnTime               ! return period in the present test
    real (kind = wp)    :: returnTime_expected(dim) ! expected return period
    real (kind = wp)    :: margin = 1.0d-8          ! acceptable margin 
    integer             :: i                        ! index

    ! expected return period, pre-computed with Matlab
    returnTime_expected = (/ 1.44269504088896d0, 5.78858538234500d0, 43.4538712904684d0, 740.296582122419d0, &
                            31573.8855320086d0, 3488555.28669823d0, 0d0, 0d0/)

    ! expected return period, per-computed with Hydra-Ring (too big diviation from Matlab)
    returnTime_expected(7) = 1013594691.25190d0
    returnTime_expected(8) = 781364428834.289d0

    do i = 1,dim
        beta = i-1.0d0
        call ReturnTimeFromBeta( beta, returnTime )
        call assert_comparable( returnTime, returnTime_expected(i), margin, &
            "An unexpected value is found for the return period computed by ReturnTimeFromBeta" )
    end do

end subroutine testReturnTimeFromBeta
!
!
!> Test of function FreqFromBeta \n
!! Comparison with results per-computed with Matlab \n
!!
subroutine testFreqFromBeta
    integer, parameter  :: dim = 8            ! dimension, number of comparisons 
    real (kind = wp)    :: beta               ! reliability index beta in the present test
    real (kind = wp)    :: freq               ! frequency in the present test
    real (kind = wp)    :: freq_expected(dim) ! expected frequency
    real (kind = wp)    :: margin = 1.0d-6    ! acceptable margin 
    integer             :: i                  ! index

    ! expected frequency, pre-computed with Matlab
    freq_expected = (/ 0.693147180559945d0, 0.172753779023450d0, 0.0230129093289635d0, 0.00135080996474820d0, &
                        3.16717433774893d-05, 2.86651613008105d-07, 9.86587645037701d-10, 1.27981254388584d-12/)

    do i = 1,dim
        beta = i-1.0d0
        call FreqFromBeta( beta, freq )
        call assert_comparable( freq, freq_expected(i), margin, &
            "An unexpected value is found for the frequency computed by FreqFromBeta" )
    end do

end subroutine testFreqFromBeta
!
!
!> Test of function LogQFromBeta \n
!! Comparison with results per-computed with Matlab \n
!!
subroutine testLogQFromBeta
    integer, parameter  :: dim = 8            ! dimension, number of comparisons
    real (kind = wp)    :: beta               ! reliability index beta in the present test
    real (kind = wp)    :: logQ               ! -log(Q) in the present test
    real (kind = wp)    :: logQ_expected(dim) ! expected -log(Q)
    real (kind = wp)    :: margin = 1.0d-6    ! acceptable margin 
    integer             :: i                  ! index

    ! expected -log(Q), pre-computed with Matlab
    logQ_expected = (/ 0.366512920581664d0, 1.75588794089519d0, 3.77169994533666d0, 6.60705089255662d0, &
                       10.3600856506974d0, 15.0649982505082d0, 20.7367689499747d0, 27.3843074988111d0/)

    do i = 1,dim
        beta = i-1.0d0
        call LogQFromBeta( beta, logQ )
        call assert_comparable( logQ, logQ_expected(i), margin, &
            "An unexpected value is found for the -log(Q) computed by LogQFromBeta" )
    end do

end subroutine testLogQFromBeta

end module conversionFunctionsTests
