! Copyright (C) Stichting Deltares. All rights reserved.
!
! This file is part of Streams.
!
! Streams is free software: you can redistribute it and/or modify
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
!! Test the function Hohenbichler
!<
!
module HohenbichlerTests

    use precision
    use ftnunit
    use interface_combin
    use feedback
    use interface_combin
    use interface_distributions
    use interface_convert

    implicit none

    private

    public :: allHohenbichlerTests

contains

!> Call all the tests
!!
subroutine allHohenbichlerTests

    call testWithLevel( testHohenbichlerNoCorrelation, "testHohenbichlerNoCorrelation from module HohenbichlerTests", 1)
    !AM - failing with code coverage?
    call testWithLevel( testHohenbichlerAlmostFullCorrelation, &
        "testHohenbichlerAlmostFullCorrelation from module HohenbichlerTests", 1)
    call testWithLevel( testHohenbichlerFullCorrelation, "testHohenbichlerFullCorrelation from module HohenbichlerTests", 1)
    call testWithLevel( testHohenbichlerPartialCorrelation, "testHohenbichlerPartialCorrelation from module HohenbichlerTests", 1)
    call testWithLevel( testHohenbichlernegativeCorrelation, "testHohenbichlernegativeCorrelation from module HohenbichlerTests", 1)
    call testWithLevel( testHohenbichlernegativeCorrelationNegativeBeta, &
        "testHohenbichlernegativeCorrelationNegativeBeta from module HohenbichlerTests", 1)
    call testWithLevel( testHohenbichlernegativeCorrelationZeroBeta, &
        "testHohenbichlernegativeCorrelationZeroBeta from module HohenbichlerTests", 1)

!   The following "test" on the effect of parameter rhoLimitHohenbichler in Hohenbichler is formally not a unit test since no computational results are compared to expected results.
!   For further explanation see the legends in the header of the subroutine.
    call testWithLevel( testHohenbichlerRhoLimit, "test Hohenbichler rho limit", 3)

end subroutine allHohenbichlerTests

!> Test Hohenbichler without correlation
!!
subroutine testHohenbichlerNoCorrelation

    real (kind = wp) :: beta2
    real (kind = wp) :: pf1
    real (kind = wp) :: rho
    real (kind = wp) :: pf2pf1
    real (kind = wp) :: margin = 1.0d-12
    real (kind = wp) :: expectedpf2pf1
    real (kind = wp) :: p

    beta2 = 3.0d0
    pf1   = 1.0d-4
    rho   = 0.0d0

    call PQFromBeta(beta2, p, expectedpf2pf1)

    call Hohenbichler( beta2, pf1, rho, pf2pf1 )
!
    call assert_comparable( pf2pf1, expectedpf2pf1, margin, "unexpected return value beta" )

end subroutine testHohenbichlerNoCorrelation

!> FORM does not converge properly if the correlation is 1, so what happens \n
!! when it is close to 1? \n
!! This test can be used to fix against future changes in Hohenbichler \n
!!
subroutine testHohenbichlerAlmostFullCorrelation

    real (kind = wp) :: beta2
    real (kind = wp) :: pf1
    real (kind = wp) :: rho
    real (kind = wp) :: pf2pf1
    real (kind = wp) :: margin = 1.0d-12
    real (kind = wp) :: expectedpf2pf1

    beta2 = 3.0d0
    pf1   = 1.0d-4
    rho   = 0.998d0

    expectedpf2pf1= 0.999983622739076d0
    call Hohenbichler( beta2, pf1, rho, pf2pf1 )

    call assert_comparable( pf2pf1, expectedpf2pf1, margin, "unexpected return value beta" )

end subroutine testHohenbichlerAlmostFullCorrelation

subroutine testHohenbichlerFullCorrelation

    real (kind = wp) :: beta2
    real (kind = wp) :: pf1
    real (kind = wp) :: rho
    real (kind = wp) :: pf2pf1
    real (kind = wp) :: margin = 1.0d-12
    real (kind = wp) :: expectedpf2pf1

    beta2 = 3.0d0
    pf1   = 1.0d-4
    rho   = 1.0d0

    expectedpf2pf1 = 1.0d0

    call Hohenbichler( beta2, pf1, rho, pf2pf1 )
!
    call assert_comparable( pf2pf1, expectedpf2pf1, margin, "unexpected return value beta" )

end subroutine testHohenbichlerFullCorrelation

subroutine testHohenbichlerPartialCorrelation

    real (kind = wp) :: beta2
    real (kind = wp) :: pf1
    real (kind = wp) :: rho
    real (kind = wp) :: pf2pf1
    real (kind = wp) :: margin = 2.0d-7
    real (kind = wp) :: expectedpf2pf1

    beta2 = 3.0d0
    pf1   = 1.0d-4
    rho   = 0.5d0

    expectedpf2pf1 = 0.11323892d0

    call Hohenbichler( beta2, pf1, rho, pf2pf1 )
!
    call assert_comparable( pf2pf1, expectedpf2pf1, margin, "unexpected return value beta" )

    !!second set of values:
    beta2 = 2.5d0
    pf1   = 6.209679853494862d-3
    rho   = 0.95d0

    expectedpf2pf1 = 0.600507845d0

    call Hohenbichler( beta2, pf1, rho, pf2pf1 )
!
    call assert_comparable( pf2pf1, expectedpf2pf1, margin, "unexpected return 2nd value beta" )

end subroutine testHohenbichlerPartialCorrelation

subroutine testHohenbichlernegativeCorrelation

    real (kind = wp) :: beta2
    real (kind = wp) :: pf1
    real (kind = wp) :: rho
    real (kind = wp) :: pf2pf1
    real (kind = wp) :: margin = 1.0d-12
    real (kind = wp) :: expectedpf2pf1

    beta2 = 3.0d0
    pf1   = 1.0d-4
    rho   = -1.0d0

    expectedpf2pf1 = 0.0d0

    call Hohenbichler( beta2, pf1, rho, pf2pf1 )

    call assert_inbetween( pf2pf1 - expectedpf2pf1, -margin, margin, "unexpected return value beta" )

end subroutine testHohenbichlernegativeCorrelation

subroutine testHohenbichlernegativeCorrelationNegativeBeta

    real (kind = wp) :: beta2
    real (kind = wp) :: pf1
    real (kind = wp) :: rho
    real (kind = wp) :: pf2pf1
    real (kind = wp) :: margin = 1.0d-8
    real (kind = wp) :: expectedpf2pf1

    beta2 = -3.0d0
    pf1   = 0.8d0
    rho   = -1.0d0

    expectedpf2pf1 = 0.998312630153354d0

    call Hohenbichler( beta2, pf1, rho, pf2pf1 )
!
    call assert_comparable( pf2pf1, expectedpf2pf1, margin, "unexpected return value beta" )

end subroutine testHohenbichlernegativeCorrelationNegativeBeta

!> Test on the margin of what can be handled (hence a fairly large margin):
!> The two stochasts are perfectly negatively correlated so they fail in
!> separate half-planes, the area of common failure is actually the _line_
!> u1 = 0 (since both probabilities of failure are 0.5).
subroutine testHohenbichlernegativeCorrelationZeroBeta

    real (kind = wp) :: beta2
    real (kind = wp) :: pf1
    real (kind = wp) :: rho
    real (kind = wp) :: pf2pf1
    real (kind = wp) :: margin = 1.0d-3
    real (kind = wp) :: expectedpf2pf1

    beta2 = 0.0d0
    pf1   = 0.5d0
    rho   = -1.0d0

    expectedpf2pf1 = 1.052261624672d-9

    call Hohenbichler( beta2, pf1, rho, pf2pf1 )
!
    call assert_inbetween( pf2pf1 - expectedpf2pf1, -margin, margin, "unexpected return value beta" )

end subroutine testHohenbichlernegativeCorrelationZeroBeta

!
!> Test for verifying effects of the setting of the parameter rhoLimitHohenbichler in subroutine Hohenbichler( betaV, pfU, rhoInput, pfVpfU )  \n
!! NOTE: Formally this is not a unit test is strict sense since no computational results are compared to expected results \n
!! \n
!! In this test the output parameter pfVpfU of Hohenbichler() is computed for variations of its three input parameters {betaV, pfU, rhoInput} \n
!! rhoInput is varied on a dense partition of [0,1] \n
!! The betaV variations consist of the following four values {-3, 0, 3, 6, 9} \n
!! Similarly three variations are applied for pfU \n
!! For all rhoInput * betaV * pfU combinations subroutine Hohenbichler( ) is called to obtain the corresponding pfVpfU \n
!! The results are stored in a plain ascii file ..\HydraRing\trunk\test\unitTests\Test_RhoLimit.tek \n
!! On the basis of this file plots can be made of pfVpfU as function of rhoInput (for fixed betaV and pfU) \n
!! In particular (possibly undesired) effects of the restriction rho = max( min( rhoInput, rhoLimitHohenbichler ) can be verified. \n
!! At this moment rhoLimitHohenbichler=0.99d0 as set in subroutine Hohenbichler() \n
subroutine testHohenbichlerRhoLimit
    integer, parameter :: npfUvar= 3                 ! total number of variations of the failure probability pfU
    integer, parameter :: lWork  = 2 + 2 * npfUvar   ! dimension of locally used work space array work()
    integer            :: lunOut                     ! logical unit number for output data of the present test
    integer            :: nRhoInput                  ! total number of variations of the input correlation rhoInput
    integer            :: iRhoInput                  ! number of a rhoInput variation
    integer            :: nBeta                      ! total number of variations of the reliability index beta
    integer            :: iBeta                      ! number of a beta variation
    integer            :: ipfUvar                    ! number of pfU variations
    integer            :: iWork                      ! entry to work array work()a
    real (kind = wp)   :: betaV                      ! Smallest reliability index of two stochastic parameters
    real (kind = wp)   :: betaMin                    ! minimum in variations of reliability index beta
    real (kind = wp)   :: betaMax                    ! maximum in variations of reliability index beta
    real (kind = wp)   :: dBeta                      ! increment in variations of reliability index beta
    real (kind = wp)   :: pfU                        ! Smallest probability of failure of two stochastic parameters
    real (kind = wp)   :: pfUvar                     ! failure probability in the variational analysis
    real (kind = wp)   :: p                          ! probability of non-exceendence for a given beta
    real (kind = wp)   :: pfVpfU                     ! output of the Hohenbichler subroutine for given beta, pU and rhoInput
!                                                    ! Failure probability P[Z2 < 0|Z1 < 0]
    real (kind = wp)   :: rhoInput                   ! Correlation coefficient between Z1 and Z2
    real (kind = wp)   :: rhoInput_Min               ! minimum in variations of the rhoInput
    real (kind = wp)   :: rhoInput_Max               ! maximum in variations of the rhoInput
    real (kind = wp)   :: rhoInput_Delta             ! increment in variations of rhoInput
    real (kind = wp)   :: pfUfactor( npfUvar)        ! array with the variations of the pfU
    real (kind = wp)   :: work( lWork)               ! locally defined and used array for work space within the computations

!   Range and increment for the rhoInput variations
    rhoInput_Min  = 0.0d0
    rhoInput_Max  = 1.0d0
!   rhoInput_Delta= 0.00001d0   ! Reference setting for high resolution of rho-variations rhoLimitHohenbichler. Requires much computation time
!   rhoInput_Delta= 0.00010d0   ! for less dense variations of rhoLimit
    rhoInput_Delta= 0.00100d0   ! for less dense variations of rhoLimit
!
!   Number of gridpoints for equidistant rhoInput variations:
    nRhoInput     = nint ( 1.0d0+ (rhoInput_Max  - rhoInput_Min) / rhoInput_Delta )
    rhoInput_Max  = rhoInput_Min+  rhoInput_Delta* dble ( nRhoInput- 1 )

!   Range and increment for the Beta variations
    betaMin= -3.0d0
    betaMax=  9.0d0
    dBeta  =  3.0d0
!   Number of gridpoints for equidistant Beta variations:
    nBeta  =  nint   ( 1.0d0+ ( betaMax - betaMin) / dBeta )

!   Initialisation of the pfU factors
    pfUfactor= (/ 1.0d0, 1.0d-3, 1.0d-5 /)

!   Open a data file for saving the result of the (beta, pfU, rhoInput)-variations:
    open( NewUnit=lunOut, file='Test_RhoLimit.tek', status='unknown' )
!   NB. This file can be found in directory: ..\HydraRing\trunk\test\unitTests\
!       Visualisation of the results (pfVpfU as function of rhoInput) must be done with some plotting program
!   Generate header/legends in the output file for the applied variations:
    write ( lunOut, '(a, i0, a, /, a)' )                                               &
           '* Listing/Legends for the ', nBeta, ' data blocks in present file:',       &
           '*'
    do iBeta = 1      , nBeta
       betaV = betaMin+ dBeta * dble( iBeta- 1 )
       call PQFromBeta( betaV, p, pfU)
       write ( lunOut, '( a, /, 2(a, i0), /, a, /, a, 1Pg15.8, /, 2(a, g15.8, /), a)') &
              '* ---------------------------------------------',                       &
              '* Block of (beta, pfU, rhoInput) variations Nr. ', iBeta, '/', nBeta,   &
              '* ---------------------------------------------',                       &
              '* Beta             = ', BetaV, &
              '* ==> p(Beta)      = ', p    , &
              '* ==> pfU(Beta)    = ', pfU  , &
              '*'
       do ipfUvar= 1     , npfUvar
          pfUvar = pfU   * pfUfactor( ipfUvar)
          write  ( lunOut,'(a, i0, a, 1Pg15.8)')                              &
                 '* Variation pfU(', ipfUvar, ') = ', pfU * pfUfactor( ipfUvar)
       enddo
       write ( lunOut, '(a, /, a, 1Pg15.8, /, 2(a, g15.8, /), a, i0, /, a)' ) &
              '*'    ,                                                        &
              '* rhoInput_Min     = ', rhoInput_Min  ,                        &
              '* rhoInput_Max     = ', rhoInput_Max  ,                        &
              '* rhoInput_Delta   = ', rhoInput_Delta,                        &
              '* nRhoInput        = ', nRhoInput     ,                        &
              '*'
    enddo

!   Entry to the computation and saving results for plotting pfVpfU as function of rhoInput, for given variations of beta, and pfU:
    do iBeta = 1      , nBeta
       betaV = betaMin+ dBeta * dble( iBeta- 1 )
       call PQFromBeta( betaV, p, pfU)
       work  ( 1 ) = betaV
       write ( lunOut, '( a, /, 2(a, i0), /, a, /, a, 1Pg15.8, /, 2(a, g15.8, /), a)') &
              '* ---------------------------------------' ,                            &
              '* Entry to (pfU, rhoInput) variations Nr. ', iBeta, '/', nBeta,         &
              '* ---------------------------------------' ,                            &
              '* Beta             = ', BetaV,                                          &
              '* ==> p(Beta)      = ', p    ,                                          &
              '* ==> pfU(Beta)    = ', pfU  ,                                          &
              '*'
       do ipfUvar= 1     , npfUvar
          pfUvar = pfU   * pfUfactor( ipfUvar)
          write  ( lunOut,'(a, i0, a, 1Pg15.8)')                                &
                 '* Variation pfU(', ipfUvar, ') = ', pfU * pfUfactor( ipfUvar)
       enddo
       write ( lunOut, '(a, /, a, 1Pg15.8, /, 2(a, g15.8, /), a, i0, /, a)' )   &
              '*'    ,                                                          &
              '* rhoInput_Min     = ', rhoInput_Min  ,                          &
              '* rhoInput_Max     = ', rhoInput_Max  ,                          &
              '* rhoInput_Delta   = ', rhoInput_Delta,                          &
              '* nRhoInput        = ', nRhoInput     ,                          &
              '*'
       write ( lunOut,'(a,/, a)')                                               &
               '*  1       2          3               4               5             6               7             8'    ,  &
               '*Beta   rhoInput    pfU(1)        pfVpfU(1)         pfU(2)      pfVpfU(2)         pfU(3)      pfVpfU(3)'
       write ( lunOut,'(a, i2.2, /, i0, i3)') &
              'BL'   , iBeta , nRhoInput, lWork
       do iRhoInput= 1, nRhoInput
          rhoInput    = rhoInput_Min + rhoInput_Delta* dble ( iRhoInput- 1 )
          work  ( 2 ) = rhoInput
          iwork       = 2
          do ipfUvar  = 1, npfUvar
             pfUvar   = pfU * pfUfactor( ipfUvar)
             call Hohenbichler( betaV, pfUvar, rhoInput, pfVpfU )
             iWork          = iWork + 1
             work( iWork )  = pfUvar
             iWork          = iWork + 1
             work( iWork )  = pfVpfU
          enddo
          write ( lunOut,'(f5.1, f12.8, 1Pg15.7, 5g15.7)' ) &
                ( work  ( iWork), iWork= 1, lWork )
       enddo
       write ( lunOut, '(a)' ) '*'
    enddo

    close( Unit= lunOut)

end subroutine testHohenbichlerRhoLimit

end module HohenbichlerTests
