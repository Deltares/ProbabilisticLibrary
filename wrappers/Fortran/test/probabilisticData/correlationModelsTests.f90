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
!! Tests for correlation models
!<
!
module correlationModelsTests

    use precision
    use ftnunit
    use interface_convert
    use interface_distributions
    use interface_correlation

    implicit none

    private

    public :: allCorrelationModelsTests

contains

!> Call all the tests
!!
subroutine allCorrelationModelsTests

    call testWithLevel( testCorrelationNLmodelIndependent1, &
        "testCorrelationNLmodelIndependent1 from module correlationModelsTests", 1 )
    call testWithLevel( testCorrelationNLmodelIndependent2, &
        "testCorrelationNLmodelIndependent2 from module correlationModelsTests", 1 )
    call testWithLevel( testCorrelationNLmodelPartial1, "testCorrelationNLmodelPartial1 from module correlationModelsTests", 1 )
    call testWithLevel( testCorrelationNLmodelPartial2, "testCorrelationNLmodelPartial2 from module correlationModelsTests", 1 )
    call testWithLevel( testCorrelationNLmodelNearlyPerfect1, &
        "testCorrelationNLmodelNearlyPerfect1 from module correlationModelsTests", 1 )
    call testWithLevel( testCorrelationNLmodelNearlyPerfect2, &
        "testCorrelationNLmodelNearlyPerfect2 from module correlationModelsTests", 1 )
    call testWithLevel( testCorrelationNLmodelTable1, "testCorrelationNLmodelTable1 from module correlationModelsTests", 1 )
    call testWithLevel( testCorrelationNLmodelTable2, "testCorrelationNLmodelTable2 from module correlationModelsTests", 1 )
    call testWithLevel( testCorrelationNLmodelTable3, "testCorrelationNLmodelTable3 from module correlationModelsTests", 1 )

    call testWithLevel( testCorrelationVolker1, "testCorrelationVolker1 from module correlationModelsTests", 1)
    call testWithLevel( testCorrelationVolker2, "testCorrelationVolker2 from module correlationModelsTests", 1)
    call testWithLevel( testCorrelationVolker3, "testCorrelationVolker3 from module correlationModelsTests", 1)
    call testWithLevel( testCorrelationVolker4, "testCorrelationVolker4 from module correlationModelsTests", 1)
    call testWithLevel( testCorrelationVolker5, "testCorrelationVolker5 from module correlationModelsTests", 1)
    call testWithLevel( testCorrelationVolkerNoCorrelation, &
        "testCorrelationVolkerNoCorrelation from module correlationModelsTests", 1)

    call testWithLevel( testCorrelationPCRmodelNearlyPerfect, "testPCRcorrelation, nearly perfect correlation", 1)
    call testWithLevel( testCorrelationPCRmodel1, "testPCRcorrelation, case 1", 1)
    call testWithLevel( testCorrelationPCRmodel2, "testPCRcorrelation, case 2", 1)
    call testWithLevel( testCorrelationPCRmodel3, "testPCRcorrelation, case 3", 1)
    call testWithLevel( testCorrelationPCRmodel4, "testPCRcorrelation, case 4", 1)

    call testWithLevel( testcalculateCorrelationComplete1, "testcalculateCorrelation with complete correlation, case 1", 1)
    call testWithLevel( testcalculateCorrelationComplete2, "testcalculateCorrelation with complete correlation, case 2", 1)
    call testWithLevel( testcalculateCorrelationComplete3, "testcalculateCorrelation with complete correlation, case 3", 1)

    call testWithLevel( testCorrelationGaussianModel1, "testCorrelationGaussianModel, case 1 (rho = -1, 0, 1)", 1 )
    call testWithLevel( testCorrelationGaussianModel2, "testCorrelationGaussianModel, case 2 (other rho values)", 1 )

end subroutine allCorrelationModelsTests

!> Test Correlation NL model with practically no correlation
subroutine testCorrelationNLmodelIndependent1

    real (kind = wp)                 :: u1 = 0.5d0
    real (kind = wp)                 :: u2 = 0.5d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId  = correlationNLmodel
    correlationParameters%distribution   = distributionNormal ! not used
    correlationParameters%distParameter1 = 0.0d0              ! not used
    correlationParameters%distParameter2 = 1.0d4
    correlationParameters%distParameter3 = 0.0d0              ! not used
    correlationParameters%distParameter4 = 0.0d0              ! not used
    correlationParameters%shift          = 0.0d0

    expected_u2 = 0.50001242d0  ! pre-computed with Matlab
    !
    ! Due to limited discretisation in the Hydra-NL correlationmodel the correlated u2 isn't
    ! exactly equal to the independent u2 (0.5)
    !
    call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationNLmodelIndependent1

!> Test Correlation NL model with practically no correlation
!!
subroutine testCorrelationNLmodelIndependent2

    real (kind = wp)                 :: u1 = 0.2d0
    real (kind = wp)                 :: u2 = 0.9d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId  = correlationNLmodel
    correlationParameters%distribution   = distributionNormal ! not used
    correlationParameters%distParameter1 = 0.0d0              ! not used
    correlationParameters%distParameter2 = 1.0d4
    correlationParameters%distParameter3 = 0.0d0              ! not used
    correlationParameters%distParameter4 = 0.0d0              ! not used
    correlationParameters%shift          = 0.0d0

    expected_u2 = 0.8999825d0  ! pre-computed with Matlab
    !
    ! Due to limited discretisation in the Hydra-NL correlationmodel the correlated u2 isn't
    ! exactly equal to the independent u2 (0.9)
    !
    call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationNLmodelIndependent2

!> Test Correlation NL model with Partial correlation
!!
subroutine testCorrelationNLmodelPartial1

    real (kind = wp)                 :: u1 = 0.1d0
    real (kind = wp)                 :: u2 = 0.9d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId  = correlationNLmodel
    correlationParameters%distribution   = distributionNormal ! not used
    correlationParameters%distParameter1 = 0.0d0              ! not used
    correlationParameters%distParameter2 = 1.0d0
    correlationParameters%distParameter3 = 0.0d0              ! not used
    correlationParameters%distParameter4 = 0.0d0              ! not used
    correlationParameters%shift          = 0.0d0

    expected_u2 = 0.58770755d0  ! pre-computed with Matlab

    call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationNLmodelPartial1

!> Test Correlation NL model with Partial correlation with a shift
!!
subroutine testCorrelationNLmodelPartial2

    real (kind = wp)                 :: u1 = 0.1d0
    real (kind = wp)                 :: u2 = 0.9d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId  = correlationNLmodel
    correlationParameters%distribution   = distributionNormal ! not used
    correlationParameters%distParameter1 = 0.0d0              ! not used
    correlationParameters%distParameter2 = 1.0d0
    correlationParameters%distParameter3 = 0.0d0              ! not used
    correlationParameters%distParameter4 = 0.0d0              ! not used
    correlationParameters%shift          = 4.0d0

    expected_u2 = 0.587707550d0  ! pre-computed with Matlab

    call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationNLmodelPartial2

!> Test Correlation NL model with nearly perfect correlation
!!
subroutine testCorrelationNLmodelNearlyPerfect1

    real (kind = wp)                 :: u1 = 0.1d0
    real (kind = wp)                 :: u2 = 0.9d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId  = correlationNLmodel
    correlationParameters%distribution   = distributionNormal ! not used
    correlationParameters%distParameter1 = 0.0d0              ! not used
    correlationParameters%distParameter2 = 1.0d-2
    correlationParameters%distParameter3 = 0.0d0              ! not used
    correlationParameters%distParameter4 = 0.0d0              ! not used
    correlationParameters%shift          = 0.0d0

    expected_u2 = 0.11033332d0  ! pre-computed with Matlab

   call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationNLmodelNearlyPerfect1

!> Test Correlation NL model with nearly perfect correlation with a shift
!!
subroutine testCorrelationNLmodelNearlyPerfect2

    real (kind = wp)                 :: u1 = 0.1d0
    real (kind = wp)                 :: u2 = 0.9d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId  = correlationNLmodel
    correlationParameters%distribution   = distributionNormal ! not used
    correlationParameters%distParameter1 = 0.0d0              ! not used
    correlationParameters%distParameter2 = 1.0d-2
    correlationParameters%distParameter3 = 0.0d0              ! not used
    correlationParameters%distParameter4 = 0.0d0              ! not used
    correlationParameters%shift          = 4.0d0

    expected_u2 = 0.11033332d0  ! pre-computed with Matlab

   call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationNLmodelNearlyPerfect2

!> Create table from nearly perfect correlation to nearly independent for normal distribution
!!
subroutine testCorrelationNLmodelTable1

    real (kind = wp)                 :: u1 = 1.0d0
    real (kind = wp)                 :: u2 = 5.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2
    real (kind = wp)                 :: actual_u2

    integer, parameter               :: linelength = 120
    character(len=linelength)        :: line                 ! text to report
    character(len=80)                :: reportFilename       ! File name of file to write to
    integer                          :: fileId               ! file handle
    integer                          :: ierr                 ! return error code
    integer                          :: i                    ! do-loop counter

    correlationParameters%correlationId  = correlationNLmodel
    correlationParameters%distribution   = distributionNormal ! not used
    correlationParameters%distParameter1 = 0.0d0              ! not used
    correlationParameters%distParameter3 = 0.0d0              ! not used
    correlationParameters%distParameter4 = 0.0d0              ! not used
    correlationParameters%shift          = 0.0d0

    write(reportFilename, '(A,I1,A,I1,A)') 'CorrelationTable_u1_is_', int(u1), '_en_u2_is_', int(u2), '.txt'
    open( newunit=fileId, file = reportFilename, iostat = ierr )

    if ( ierr /= 0 ) then
        call assert( "Could not open report file - " // trim(reportFilename) )
        return
    endif

    write(line, '(A,F5.2,A,F5.2,A)') 'Input: u1 =', u1, &
        ' (answer for nearly perfect correlated) and u2 =', u2, ' (answer for nearly independent)'
    write(fileId,*) line
    write(fileId,*) 'Table from nearly perfect correlated to nearly independent'
    write(fileId,*) repeat('-', linelength)
    write(fileId,*) 'sigma        u2'
    write(fileId,*) repeat('-', linelength)
    do i = 1, 81
        actual_u2 = u2
        correlationParameters%distParameter2 = 10.0d0 ** ((dble(i-1)/10.0d0) - 4.0d0)
        call calculateCorrelation( u1, actual_u2, correlationParameters, .true. )
        write(line, '(ES10.3,F7.2)') correlationParameters%distParameter2, actual_u2
        write(fileId,*) line
    enddo
    write(fileId,*) repeat( '-', linelength)
    close( fileId )
    !
    ! The calculated table with values to the correlated u2 is not only enlarging from 1 to 5. (One may expect this.)
    ! At some point the correlated u2 is larger then 5. The largest value is 5.05.
    ! The different result is due to the correlation model: it's an artefact/characteristic of the method. One of the
    ! distributions used in this method is the exponential distribution. That distribution is not symmetric. Changing parameters
    ! in the method (for instance a higher upper bound or more integration steps) doesn't change the results (much). The artefact
    ! is shown in all cases. (It's even worse in the PC-Ring correlation model.)

    !
    ! Expected value for the largest value of sigma
    expected_u2 = 5.00008294d0  ! pre-computed with Matlab
    !
    ! Due to limited discretisation in the Hydra-NL correlationmodel the correlated u2 isn't
    ! exactly equal to the independent u2 (5.0)
    !
    call assert_comparable( actual_u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationNLmodelTable1

!> Create table from nearly perfect correlation to nearly independent for normal distribution
!!
subroutine testCorrelationNLmodelTable2

    real (kind = wp)                 :: u1 = 0.0d0
    real (kind = wp)                 :: u2 = 0.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-4
    real (kind = wp)                 :: expected_u2
    real (kind = wp)                 :: actual_u2

    integer, parameter               :: linelength = 120
    character(len=linelength)        :: line                 ! text to report
    character(len=80)                :: reportFilename       ! File name of file to write to
    integer                          :: fileId               ! file handle
    integer                          :: ierr                 ! return error code
    integer                          :: i                    ! do-loop counter

    correlationParameters%correlationId  = correlationNLmodel
    correlationParameters%distribution   = distributionNormal ! not used
    correlationParameters%distParameter1 = 0.0d0              ! not used
    correlationParameters%distParameter3 = 0.0d0              ! not used
    correlationParameters%distParameter4 = 0.0d0              ! not used
    correlationParameters%shift          = 0.0d0

    write(reportFilename, '(A,I1,A,I1,A)') 'CorrelationTable_u1_is_', int(u1), '_en_u2_is_', int(u2), '.txt'
    open( newunit=fileId, file = reportFilename, iostat = ierr )

    if ( ierr /= 0 ) then
        call assert( "Could not open report file - " // trim(reportFilename) )
        return
    endif

    write(line, '(A,F5.2,A,F5.2,A)') 'Input: u1 =', u1, &
        ' (answer for nearly perfect correlated) and u2 =', u2, ' (answer for nearly independent)'
    write(fileId,*) line
    write(fileId,*) 'Table from nearly perfect correlated to nearly independent'
    write(fileId,*) repeat('-', linelength)
    write(fileId,*) 'sigma        u2'
    write(fileId,*) repeat('-', linelength)
    do i = 1, 81
        actual_u2 = u2
        correlationParameters%distParameter2 = 10.0d0 ** ((dble(i-1)/10.0d0) - 4.0d0)
        call calculateCorrelation( u1, actual_u2, correlationParameters, .true. )
        write(fileId, '(ES10.3,F7.2)') correlationParameters%distParameter2, actual_u2
    enddo
    write(fileId,*) repeat('-', linelength)
    close( fileId )
    !
    ! The table with calculated values of the correlated u2 does not show only values equal to zero. (One may expect this.)
    ! The different result is due to the correlation model: it's an artefact/characteristic of the method. One of the
    ! distributions used in this method is the exponential distribution. That distribution is not symmetric. Changing parameters
    ! in the method (for instance a higher upper bound or more integration steps) doesn't change the results (much). The artefact
    ! is shown in all cases. (It's even worse in the PC-Ring correlation model.) The largest value is -0.14. Quite a deviation from 0.

    !
    ! Expected value for the largest value of sigma
    expected_u2 = -3.80764272d-05  ! pre-computed with Matlab
    !
    ! Due to limited discretisation in the Hydra-NL correlationmodel the correlated u2 isn't
    ! exactly equal to the independent u2 (0.0)
    !
    ! The deviation from the Matlab answer is due to a slightly other implementation of the normal distribution.

    call assert_inbetween( actual_u2 - expected_u2, -margin, margin, "unexpected return value u2" )

end subroutine testCorrelationNLmodelTable2

!> Create table from nearly perfect correlation to nearly independent for normal distribution
!!
subroutine testCorrelationNLmodelTable3

    real (kind = wp)                 :: u1 = -1.0d0
    real (kind = wp)                 :: u2 = -5.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2
    real (kind = wp)                 :: actual_u2

    integer, parameter               :: linelength = 120
    character(len=linelength)        :: line                 ! text to report
    character(len=80)                :: reportFilename       ! File name of file to write to
    integer                          :: fileId               ! file handle
    integer                          :: ierr                 ! return error code
    integer                          :: i                    ! do-loop counter

    correlationParameters%correlationId  = correlationNLmodel
    correlationParameters%distribution   = distributionNormal ! not used
    correlationParameters%distParameter1 = 0.0d0              ! not used
    correlationParameters%distParameter3 = 0.0d0              ! not used
    correlationParameters%distParameter4 = 0.0d0              ! not used
    correlationParameters%shift          = 0.0d0

    write(reportFilename, '(A,I2,A,I2,A)') 'CorrelationTable_u1_is_', int(u1), '_en_u2_is_', int(u2), '.txt'
    open( newunit=fileId, file = reportFilename, iostat = ierr )

    if ( ierr /= 0 ) then
        call assert( "Could not open report file - " // trim(reportFilename) )
        return
    endif

    write(fileId, '(A,F5.2,A,F5.2,A)') 'Input: u1 =', u1, &
        ' (answer for nearly perfect correlated) and u2 =', u2, ' (answer for nearly independent)'
    write(fileId, *) 'Table from nearly perfect correlated to nearly independent'
    write(fileId, *) repeat('-', linelength)
    write(fileId, *) 'sigma        u2'
    write(fileId, *) repeat('-', linelength)
    do i = 1, 81
        actual_u2 = u2
        correlationParameters%distParameter2 = 10.0d0 ** ((dble(i-1)/10.0d0) - 4.0d0)
        call calculateCorrelation( u1, actual_u2, correlationParameters, .true. )
        write(fileId, '(ES10.3,F7.2)') correlationParameters%distParameter2, actual_u2
    enddo
    write(fileId,*) repeat( '-', linelength)
    close( fileId )
    !
    ! The calculated table with values to the correlated u2 is not only descending from -1 to -5. (One may expect this.)
    ! At some point the correlated u2 is lower then 5. The lowest value is -5.18.
    ! The different result is due to the correlation model: it's an artefact/characteristic of the method. One of the
    ! distributions used in this method is the exponential distribution. That distribution is not symmetric. Changing parameters
    ! in the method (for instance a higher upper bound or more integration steps) doesn't change the results (much). The artefact
    ! is shown in all cases. (It's even worse in the PC-Ring correlation model.)

    !
    ! Expected value for the largest value of sigma
    expected_u2 = -5.00008156d0  ! pre-computed with Matlab
    !
    ! Due to limited discretisation in the Hydra-NL correlationmodel the correlated u2 isn't
    ! exactly equal to the independent u2 (-5.0)

    call assert_comparable( actual_u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationNLmodelTable3

!> Auxiliary routine for Volker model tests: fill in the default values

subroutine FillInCorrelationParametersVolker( correlationParameters )
    type(correlationModelParameters), intent(out) :: correlationParameters

    !
    ! Volker model correlation parameters - For Schiphol without volker factor,
    !                                       wind direction north (0 degrees)
    !                                       - from file WenWTzV.dat
    !
    correlationParameters%correlationParameter1 = 0.873d0      !A
    correlationParameters%correlationParameter2 = 0.236d0      !B
    correlationParameters%correlationParameter3 = 0.356d0      !rho
    correlationParameters%correlationParameter4 = 0.67d0       !M
    correlationParameters%correlationParameter5 = 0.0010d0     !aK
    correlationParameters%correlationParameter6 = 0.2347d0     !bK
    correlationParameters%correlationParameter7 = -0.5771d0    !cK
    correlationParameters%correlationParameter8 = 0.02d0       !d

    !
    ! Conditional Weibull water level statistics - Hoek van Holland parameters
    !
    correlationParameters%correlationId  = correlationVolker
    correlationParameters%distribution   = distributionConditionalWeibull
    correlationParameters%distParameter1 = 0.0157d0 !scale
    correlationParameters%distParameter2 = 0.57d0   !shape
    correlationParameters%distParameter3 = 1.97d0   !thresh
    correlationParameters%distParameter4 = 7.237d0  !lambda

end subroutine FillInCorrelationParametersVolker

!> Test Volker model
!! This test compares results from an Excel file with the program in Hydra-Ring
!! The Excel file is located under subversion: test\probabilisticData\Testdata\Berekeningen voor Volker.xls
!!
subroutine testCorrelationVolker1

    real (kind = wp)                 :: u1 = -8.0d0
    real (kind = wp)                 :: u2 = 0.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-5
    real (kind = wp)                 :: expected_u2

    !
    ! Volker model correlation parameters: all defaults
    !
    call FillInCorrelationParametersVolker( correlationParameters )

    !
    ! Expected value computed in Excel
    !
    expected_u2 = 8.053466492d0
    !
    ! Value computed by Hydra-Ring
    !
    call calculateCorrelation( u1, u2, correlationParameters, .false. )
    !
    ! Compare Excel and Hydra-Ring
    !
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationVolker1

!> Test Volker model
!! This test compares results from an Excel file with the program in Hydra-Ring
!! The Excel file is located under subversion: test\probabilisticData\Testdata\Berekeningen voor Volker.xls
!!
subroutine testCorrelationVolker2

    real (kind = wp)                 :: u1 = -8.0d0
    real (kind = wp)                 :: u2 = 0.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-5
    real (kind = wp)                 :: expected_u2

    !
    ! Volker model correlation parameters: aK value modified
    !
    call FillInCorrelationParametersVolker( correlationParameters )

    correlationParameters%correlationParameter5 = -0.0080d0    !aK

    !
    ! Expected value computed in Excel
    !
    expected_u2 = 14.66875d0
    !
    ! Value computed by Hydra-Ring
    !
    call calculateCorrelation( u1, u2, correlationParameters, .false. )
    !
    ! Compare Excel and Hydra-Ring
    !
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationVolker2

!> Test Volker model - Test 3
!! This test compares results from an Excel file with the program in Hydra-Ring
!! The Excel file is located under subversion: test\probabilisticData\Testdata\Berekeningen voor Volker.xls
!!
subroutine testCorrelationVolker3

    real (kind = wp)                 :: u1 = 0.0d0
    real (kind = wp)                 :: u2 = 0.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-5
    real (kind = wp)                 :: expected_u2

    !
    ! Volker model correlation parameters: all defaults
    !
    call FillInCorrelationParametersVolker( correlationParameters )

    !
    ! Expected value computed in Excel
    !
    expected_u2 = 13.22946826d0
    !
    ! Value computed by Hydra-Ring
    !
    call calculateCorrelation( u1, u2, correlationParameters, .false. )
    !
    ! Compare Excel and Hydra-Ring
    !
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationVolker3

!> Test Volker model - Test 4
!! This test compares results from an Excel file with the program in Hydra-Ring
!! The Excel file is located under subversion: test\probabilisticData\Testdata\Berekeningen voor Volker.xls
!!
subroutine testCorrelationVolker4

    real (kind = wp)                 :: u1 = 2.0d0
    real (kind = wp)                 :: u2 = 2.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-5
    real (kind = wp)                 :: expected_u2

    !
    ! Volker model correlation parameters: all defaults
    !
    call FillInCorrelationParametersVolker( correlationParameters )

    !
    ! Expected value computed in Excel
    !
    expected_u2 = 25.03191215d0
    !
    ! Value computed by Hydra-Ring
    !
    call calculateCorrelation( u1, u2, correlationParameters, .false. )
    !
    ! Compare Excel and Hydra-Ring
    !
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationVolker4

!> Test Volker model - Test 5
!! special cases: d = (near) 0 and/or u2 > ulimit (=5.6)
!! references with this library, so this is only a regression test
!!
subroutine testCorrelationVolker5

    real (kind = wp)                 :: u1
    real (kind = wp)                 :: u2
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp), parameter      :: margin = 1.0d-10
    real (kind = wp)                 :: expected_u2A(8), expected_u2B(5), expected_u2C(5), expected_u2D(5)
    integer                          :: i

    !
    ! Volker model correlation parameters: all defaults
    !
    call FillInCorrelationParametersVolker( correlationParameters )

    !
    expected_u2A = [21.4668319943414_wp, 25.0319121448744_wp, 26.6593853001100_wp, 26.8062261070168_wp, &
                    26.8098333593279_wp, 26.8098662176131_wp, 26.8098663309477_wp, 26.8098663310949_wp ]
    !
    ! loop for u2
    !
    u1 = 2.0_wp
    do i = 1, 8
        u2 = real(i, wp)
        call calculateCorrelation( u1, u2, correlationParameters, .false. )
        !
        call assert_comparable( u2, expected_u2A(i), margin, "unexpected return value u2, set A" )
    end do

    !
    expected_u2B = [26.5068980729034_wp, 25.6605635628498_wp, 25.0319121448744_wp, 24.5299731143154_wp, 24.1111010641045_wp]
    !
    ! loop for d, u2 = 2.0
    !
    u1 = 2.0_wp
    do i = 0, 4
        u2 = 2.0_wp
        correlationParameters%correlationParameter8 = real(i, wp) * 1e-2_wp

        call calculateCorrelation( u1, u2, correlationParameters, .false. )
        !
        call assert_comparable( u2, expected_u2B(i+1), margin, "unexpected return value u2, set B" )
    end do

    !
    expected_u2C = [61.7069835440849_wp, 28.4233448746792_wp, 26.8098662176131_wp, 25.8525977230744_wp, 25.1659152472382_wp]
    !
    ! loop for d, u2 = 6.0 ( > uLimit)
    !
    u1 = 2.0_wp
    do i = 0, 4
        u2 = 6.0_wp
        correlationParameters%correlationParameter8 = real(i, wp) * 1e-2_wp

        call calculateCorrelation( u1, u2, correlationParameters, .false. )
        !
        call assert_comparable( u2, expected_u2C(i+1), margin, "unexpected return value u2, set C" )
    end do

    !
    expected_u2D = [61.7069835440849_wp, 33.6322121852445_wp, 32.0852190634610_wp, 31.1723467521617_wp, 30.5209050865614_wp]
    !
    ! loop for d (smaller steps), u2 = 6.0 ( > uLimit)
    !
    u1 = 2.0_wp
    do i = 0, 4
        u2 = 6.0_wp
        correlationParameters%correlationParameter8 = real(i, wp) * 1e-3_wp

        call calculateCorrelation( u1, u2, correlationParameters, .false. )
        !
        call assert_comparable( u2, expected_u2D(i+1), margin, "unexpected return value u2, set D" )
    end do

end subroutine testCorrelationVolker5


!> Test Volker model - Test 6
!! This test compares the wind speed value computed with modified gumbel and computed with the Volker correlation model.
!! For the uncorrelated case, they should be the same. To make the wind and water level uncorrelated, the following values must be
!! set for the correlation model parameters: rho = 0, A = 0, B = 1, M = 1.
!!
subroutine testCorrelationVolkerNoCorrelation

    real (kind = wp)                 :: u1 = 1.0d0      !water level
    real (kind = wp)                 :: u2 = 2.0d0      !wind
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-5
    real (kind = wp)                 :: expectedVal
    real (kind = wp)                 :: abc(3)
    real (kind = wp)                 :: p
    real (kind = wp)                 :: q
    type(tError)                     :: ierr
    !
    ! Volker model correlation parameters: uncorrelated (reduces to modified Gumbel)
    !
    call FillInCorrelationParametersVolker( correlationParameters )

    correlationParameters%correlationParameter1 = 0.0d0      !A
    correlationParameters%correlationParameter2 = 1.0d0      !B
    correlationParameters%correlationParameter3 = 0.0d0      !rho
    correlationParameters%correlationParameter4 = 1.0d0      !M
    correlationParameters%correlationParameter5 = 0.0207d0   !aK
    correlationParameters%correlationParameter6 = 0.0d0      !bK
    correlationParameters%correlationParameter7 = -0.1d0     !cK
    correlationParameters%correlationParameter8 = 0.0d0      !d

    !
    ! Compute (expected) x value for the wind with modified gumbel
    !
    abc(1) = correlationParameters%correlationParameter5
    abc(2) = correlationParameters%correlationParameter6
    abc(3) = correlationParameters%correlationParameter7
    call PQFromBeta( u2, p, q )

    call calculateDistributionInverse(u2, expectedVal, distributionModifiedGumbel, abc, ierr)
    call assert_equal(ierr%iCode, 0, "mod. Gumbel calculation")
    !
    ! Compute x value for the wind with volker correlation model
    !
    call calculateCorrelation( u1, u2, correlationParameters, .false. )
    !
    ! Compare x value of wind from Modified Gumbel and from Volker correlation model
    !
    call assert_comparable( u2, expectedVal, margin, "unexpected return value u2" )

end subroutine testCorrelationVolkerNoCorrelation

!> Test Correlation PCR model with nearly perfect correlation
!!
subroutine testCorrelationPCRmodelNearlyPerfect

    real (kind = wp)                 :: u1 = 0.1d0
    real (kind = wp)                 :: u2 = 0.9d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId  = correlationPCRmodel
    correlationParameters%distribution   = distributionNormal
    correlationParameters%distParameter1 = 0.0d0

    expected_u2 = 0.1d0

    call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationPCRmodelNearlyPerfect

!> test PCR correlation model
subroutine testCorrelationPCRmodel1

    real (kind = wp)                 :: u1 = 1.0d0
    real (kind = wp)                 :: u2 = 8.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId         = correlationPCRmodel
    correlationParameters%distribution          = distributionNormal
    correlationParameters%correlationParameter1 = 0.0d0

    expected_u2 = u1

    call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationPCRmodel1

!> test PCR correlation model
subroutine testCorrelationPCRmodel2

    real (kind = wp)                 :: u1 = 1.0d0
    real (kind = wp)                 :: u2 = 8.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId         = correlationPCRmodel
    correlationParameters%distribution          = distributionNormal
    correlationParameters%correlationParameter1 = 0.5d0

    expected_u2 = 2.717105083391611d0 ! pre-computed with Matlab

    call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationPCRmodel2

!> test PCR correlation model
subroutine testCorrelationPCRmodel3

    real (kind = wp)                 :: u1 = 1.0d0
    real (kind = wp)                 :: u2 = 8.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId         = correlationPCRmodel
    correlationParameters%distribution          = distributionNormal
    correlationParameters%correlationParameter1 = 1.0d0

    expected_u2 = 3.751899981684130d0 ! pre-computed with Matlab

    call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationPCRmodel3

!> test PCR correlation model
subroutine testCorrelationPCRmodel4

    real (kind = wp)                 :: u1 = 1.0d0
    real (kind = wp)                 :: u2 = 8.0d0
    type(correlationModelParameters) :: correlationParameters
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId         = correlationPCRmodel
    correlationParameters%distribution          = distributionNormal
    correlationParameters%correlationParameter1 = 2.0d0

    expected_u2 = 5.147591151114541d0 ! pre-computed with Matlab

    call calculateCorrelation( u1, u2, correlationParameters, .true. )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testCorrelationPCRmodel4

! > test complete correlation model
subroutine testcalculateCorrelationComplete1

    real (kind = wp)                 :: u1 = 2.0d0             !< Independent variable
    real (kind = wp)                 :: u2 = 4.0d0             !< Dependent variable
    type(correlationModelParameters) :: correlationParameters  !< Parameters for the correlation
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId = correlationComplete
    expected_u2 = u1

    call calculateCorrelation( u1, u2, correlationParameters )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testcalculateCorrelationComplete1

! > test complete correlation model
subroutine testcalculateCorrelationComplete2

    real (kind = wp)                 :: u1 = 1.0d0             !< Independent variable
    real (kind = wp)                 :: u2 = 8.0d0             !< Dependent variable
    type(correlationModelParameters) :: correlationParameters  !< Parameters for the correlation
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId = correlationComplete
    expected_u2 = u1

    call calculateCorrelation( u1, u2, correlationParameters )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testcalculateCorrelationComplete2

! > test complete correlation model
subroutine testcalculateCorrelationComplete3

    real (kind = wp)                 :: u1 = 8.0d0             !< Independent variable
    real (kind = wp)                 :: u2 = 1.0d0             !< Dependent variable
    type(correlationModelParameters) :: correlationParameters  !< Parameters for the correlation
    real (kind = wp)                 :: margin = 1.0d-6
    real (kind = wp)                 :: expected_u2

    correlationParameters%correlationId = correlationComplete
    expected_u2 = u1

    call calculateCorrelation( u1, u2, correlationParameters )
    call assert_comparable( u2, expected_u2, margin, "unexpected return value u2" )

end subroutine testcalculateCorrelationComplete3

! > test Gaussian correlation model (rho = -1.0, 0.0, 1.0)
subroutine testCorrelationGaussianModel1

    real (kind = wp)                 :: u1                     !< Independent variable
    real (kind = wp)                 :: u2                     !< Dependent variable
    type(correlationModelParameters) :: correlationParameters  !< Parameters for the correlation
    integer, parameter               :: nSize = 3              ! number of considered rho values
    real (kind=wp)                   :: rho(nSize)             ! vector of correlation coefficients
    real (kind=wp)                   :: expected_u2(nSize)     ! vector of expected u2 values
    integer                          :: i                      ! do-loop counter
    real (kind = wp)                 :: margin = 1.0d-6

    correlationParameters%correlationId = correlationGaussianModel

    rho = (/ -1.0d0, 0.0d0, 1.0d0 /)
    expected_u2 = (/ -8.0d0, 1.0d0, 8.0d0 /) ! trivial results based on values of u1 = 8.0 and u2 = 1.0

    do i = 1, nSize

        u1 = 8.0d0
        u2 = 1.0d0
        correlationParameters%correlationParameter1 = rho(i)

        call calculateCorrelation( u1, u2, correlationParameters, .true. )
        call assert_comparable( u2, expected_u2(i), margin, "unexpected return value u2" )

    enddo

end subroutine testCorrelationGaussianModel1

! > test Gaussian correlation model (other rho values)
subroutine testCorrelationGaussianModel2

    real (kind = wp)                 :: u1                     !< Independent variable
    real (kind = wp)                 :: u2                     !< Dependent variable
    type(correlationModelParameters) :: correlationParameters  !< Parameters for the correlation
    integer, parameter               :: nSize = 5              ! number of considered rho values
    real (kind=wp)                   :: rho(nSize)             ! vector of correlation coefficients
    real (kind=wp)                   :: expected_u2(nSize)     ! vector of expected u2 values
    integer                          :: i                      ! do-loop counter
    real (kind = wp)                 :: margin = 1.0d-6

    correlationParameters%correlationId = correlationGaussianModel

    rho = (/ -0.999d0, -0.1d0, 0.37d0, 0.50d0, 0.999d0 /)
    expected_u2 = (/ -7.947290d0, 0.1949874d0, 3.889032d0, 4.866025d0, 8.036710d0 /) ! pre-computed in Excel for u1 = 8.0 and u2 = 1.0

    do i = 1, nSize

        u1 = 8.0d0
        u2 = 1.0d0
        correlationParameters%correlationParameter1 = rho(i)

        call calculateCorrelation( u1, u2, correlationParameters, .true. )
        call assert_comparable( u2, expected_u2(i), margin, "unexpected return value u2" )

    enddo

end subroutine testCorrelationGaussianModel2

end module correlationModelsTests
