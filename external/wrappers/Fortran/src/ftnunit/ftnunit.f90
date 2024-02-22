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

! ftnunit.f90 --
!     Module that implements part of the "ftnunit" framework:
!     It is inspired by the well-known JUnit framework for
!     integrating unit tests in a Java application.
!
!     The module offers:
!     - a set of common utilities, such as assertion checking routines
!     - a general routine to run the tests if requested
!     - resources that keep track of the status
!
!     Related files:
!     ftnunit_test.f90 -- deprecated
!     runtests.bat
!     runtests.sh
!     runtests.tcl
!
!     TODO:
!     - Test the various assertion failures
!       - Check the output (especially the array assertions)
!     - Split the source code in several files
!     - Implement "expect_program_test"
!       - current implementation incomplete! Variable
!         "stopped" must be properly used.
!
! ftnunit --
!     Core module for ftnunit
!
module ftnunit
    use ftnunit_hooks

    implicit none
    private

    interface
        subroutine proc 
        end subroutine proc
    end interface
    procedure (proc), public, pointer :: subptrTestProgramInit => null()
    procedure (proc), public, pointer :: subptrTestInit => null()
    
    public :: test, testwithlevel, runtests, runtests_init, runtests_final, setRunTestLevel, setTestTitle, &
              assert, assert_true, assert_false, assert_equal, assert_comparable, assert_inbetween, &
              assert_files_comparable, assert_files_comparable_with_exceptions, ftnunit_write_html_comment, &
              ftnunit_remove_file

    integer, parameter :: mode_all     = 0  ! Run all tests
    integer, parameter :: mode_single  = 1  ! Run a single test
    integer, parameter :: mode_list    = 2  ! Only list the tests

    integer, parameter :: dp = selected_real_kind(15, 307)   ! Kind for double precision
    integer, parameter :: qd = selected_real_kind(33, 4931)  ! Kind for quad precision

    integer, save :: run_test_level = 99      ! Run all tests for level up to this level
    integer, save :: single_test = -1         ! Single test to run (if > 0)
    integer, save :: test_mode = mode_all     ! Mode for testing
    integer, save :: last_test                ! Last test that was started
    integer, save :: testno                   ! Current test number
    integer, save :: nofails                  ! Number of assertions that failed
    integer, save :: nofails_prev             ! Number of assertions that failed before the test
    integer, save :: notests_run              ! Number of tests run
    integer, save :: notests_failed           ! Number of tests failed
    integer, save :: notests_ignored          ! Number of tests ignored
    integer, save :: notests_work_in_progress ! Number of tests work in progress
    integer, save :: noruns                   ! Number of runs so far
    logical, save :: call_final = .true.      ! Call runtests_final implicitly?
    logical, save :: previous   = .false.     ! Previous test run?
    integer, save :: failed_asserts = 0
    logical, save :: has_run        = .false.
    logical, save :: ignore_test    = .false.
    logical, save :: ignore_previous_test    = .false.
    character(len=20), save :: html_file = 'ftnunit.html'
    character(len=:), allocatable, save :: testname
    character(len=80), save :: testTitle = 'Result of unit tests'
    character(len=64), save :: ignore_reason
    character(len=64), save :: ignore_reason_previous_test = ' '

    interface assert_equal
        module procedure assert_equal_int
        module procedure assert_equal_int1d
        module procedure assert_equal_logical
        module procedure assert_equal_logical1d
        module procedure assert_equal_string
        module procedure assert_equal_string1d
    end interface

    interface assert_comparable
        module procedure assert_comparable_real
        module procedure assert_comparable_real1d
        module procedure assert_comparable_real2d
        module procedure assert_comparable_double
        module procedure assert_comparable_double1d
        module procedure assert_comparable_double2d
    end interface
    interface assert_inbetween
        module procedure assert_inbetween_real
        module procedure assert_inbetween_double
        module procedure assert_inbetween_quad
    end interface

contains

!> Set the run test level
!> 0 = fast tests 
!> 1..
!> 2..
!> 3 = slow tests
!!
subroutine setRunTestLevel( runtTestLevel )
    integer, intent(in)             :: runtTestLevel !< set the new run testlevel to this value
    
    run_test_level = runtTestLevel
end subroutine setRunTestLevel

!> Set the title that is displayed in the report of the test run
!!
subroutine setTestTitle(message)
    character(len=*), intent(in) :: message  !< the title to be set
    testTitle = message
end subroutine setTestTitle

! test --
!     Routine to run a unit test
! Arguments:
!     proc          The subroutine implementing the unit test
!     text          Text describing the test
!
subroutine test( proc, text, ignore_reason_in )
    external                               :: proc
    character(len=*)                       :: text
    character(len=*), intent(in), optional :: ignore_reason_in

    call testWithLevel(proc, text, 0, ignore_reason_in)
end subroutine test

! test --
!     Routine to run a unit test up until a level
! Arguments:
!     proc          The subroutine implementing the unit test
!     text          Text describing the test
!
subroutine testWithLevel( proc, text, testlevel, ignore_reason_in )
use precision, only : wp
    external                               :: proc
    character(len=*)                       :: text
    integer, intent(in)                    :: testlevel
    character(len=*), intent(in), optional :: ignore_reason_in

    real(kind=wp)     :: start
    real(kind=wp)     :: finish
    integer(kind=8)   :: cstart
    integer(kind=8)   :: cfinish
    integer           :: lun
    integer           :: ierr

    ignore_test = .false.
    if ( present(ignore_reason_in) ) then
        ignore_test = .true.
    endif

    if ( .not. ignore_test ) then
      if (testlevel > run_test_level) then
        ignore_test = .true.
      endif
    endif

    if ( .not. ignore_test) then
        ignore_reason = ' '
    else if (present(ignore_reason_in)) then
        ignore_reason = ignore_reason_in
    else
        write(ignore_reason, '(a,i1)') 'level = ', testlevel
    endif

    !
    ! Are we running in LIST mode?
    !
    if ( test_mode == mode_list ) then
        write( *, '(a)' ) text
        return
    endif

    !
    ! Check if the test should run
    !
    testno = testno + 1
    if ( testno <= last_test .or. (test_mode == mode_single .and. testno /= single_test) ) then
        return
    endif

    !
    ! Record the fact that we started the test
    !
    notests_run = notests_run + 1

    has_run = .true.
    if ( .not. ignore_test ) then
        open( newunit=lun, file = 'ftnunit.lst' )
        write( lun, * ) testno, nofails, noruns, notests_run, notests_failed, notests_ignored, ' ', .true.
        close( lun )
    endif

    !
    ! Run the test
    !
    testname     = text
    nofails_prev = nofails


    call ftnunit_write_html_test_begin( text )
    if ( .not. ignore_test ) then
        call ftnunit_hook_test_start( text )
    else
        write( *, '(/,2a)' ) 'Test: ', trim(text)
    endif

    if ( .not. ignore_test ) then

        call cpu_time(start)
        call system_clock(cstart)
        call proc
        call cpu_time(finish)
        call system_clock(cfinish)
        call ftnunit_write_html_cpu(finish - start, cfinish - cstart)

        if ( nofails /= nofails_prev ) then
            notests_failed = notests_failed + 1
        endif

        !
        ! No runtime error or premature end of the program ...
        !
        previous = .true.
        open( newunit=lun, file = 'ftnunit.lst' )
        write( lun, * ) testno, nofails, noruns, notests_run, notests_failed, notests_ignored, ' ', .true.
        close( lun )

        if ( nofails /= nofails_prev .and. test_mode == mode_single ) then
            write( *, '(a)' ) 'ASSERTION FAILED'
        endif
    else
        if (present(ignore_reason_in)) then
            notests_work_in_progress = notests_work_in_progress + 1
        else
            notests_ignored = notests_ignored + 1
        endif
        open( newunit=lun, file = 'ftnunit.lst' )
        write( lun, * ) testno, nofails, noruns, notests_run, notests_failed, notests_ignored, ' ', .true.
        close( lun )

    end if

    if ( .not. ignore_test ) then
        call ftnunit_hook_test_stop( text )
    endif

    ignore_previous_test = ignore_test
    ignore_reason_previous_test = ignore_reason

end subroutine testWithLevel

! runtests_init --
!     Subroutine to initialise the ftnunit system
! Arguments:
!     None
! Note:
!     Use in conjunction with runtests_final to enable multiple calls
!     to the runtests subroutine. This makes it easier to run tests
!     from different modules, as you have more than one subroutine to
!     do the actual tests.
!
subroutine runtests_init

    call_final = .false.
    call runtests_init_priv

end subroutine

! runtests_init_priv --
!     Private initialisation routine
! Arguments:
!     None
!
subroutine runtests_init_priv
    character(len=20) :: mode_string
    integer           :: ierr
    integer           :: lun
    logical           :: stopped

    if (associated(subptrTestProgramInit)) then
        call subptrTestProgramInit
    endif
    
    if ( .not. ftnunit_file_exists("ftnunit.lst") ) then
        call ftnunit_write_html_header
    endif

    last_test                = 0
    nofails                  = 0
    noruns                   = 0
    testno                   = 0
    notests_run              = 0
    notests_ignored          = 0
    notests_work_in_progress = 0
    notests_failed           = 0
    previous                 = .false.
    has_run                  = .false.
    
    call SetWorkingDir

    if ( ftnunit_file_exists("ftnunit.run") ) then
        open( newunit=lun, file = "ftnunit.run", iostat = ierr )
        if ( ierr == 0 ) then
            read( lun, '(a)', iostat = ierr ) mode_string
            if ( ierr == 0 ) then
                select case (mode_string)
                    case( 'ALL' )
                        test_mode = mode_all
                    case( 'LIST' )
                        test_mode = mode_list
                    case default
                        test_mode = mode_single
                        read( mode_string, *, iostat = ierr ) single_test
                        if ( ierr /= 0 ) then
                            test_mode = mode_all
                        endif
                end select
            else
                test_mode = mode_all
            endif
        endif

        close( lun )

        if (isSilent() .and. ftnunit_file_exists("ftnunit.lst") ) then
            open( newunit=lun, file = "ftnunit.lst", iostat = ierr )
            if ( ierr == 0 ) then
                read( lun, *, iostat = ierr ) last_test, nofails, noruns, &
                    notests_run, notests_failed, notests_ignored, previous
                if ( ierr /= 0 ) then
                    last_test = 0
                    nofails   = 0
                    noruns    = 0
                    previous  = .false.
                endif
                close( lun )
                notests_failed = notests_failed + 1
            endif
            if (previous ) then
                stopped = .false.
                if ( ftnunit_file_exists("ftnunit.stop") ) then
                    open( newunit=lun, file = "ftnunit.stop", iostat = ierr )
                    if ( ierr == 0 ) then
                        read( lun, *, iostat = ierr ) stopped
                        if ( ierr /= 0 ) then
                            stopped = .false.
                        endif
                        close( lun, status = 'delete' )
                    endif
                endif
                if ( .not. stopped ) then
                    write( *, '(4x,a)' ) 'Note: the program crashed'
                    call ftnunit_write_html_previous_failed
                else
                    write( *, '(4x,a)' ) 'Note: the program correctly stopped'
                    call ftnunit_write_html_previous_stopped
                endif
            endif
        endif

        noruns = noruns + 1
    endif

end subroutine

! runtests_final --
!     Subroutine to report the probabilisticDataStructure statistics
! Arguments:
!     stop              (Optional) stop the program (default) or not
! Note:
!     Use in conjunction with runtests_init to enable multiple calls
!     to the runtests subroutine. This makes it easier to run tests
!     from different modules, as you have more than one subroutine to
!     do the actual tests.
!
subroutine runtests_final( stop )
    logical, optional, intent(in) :: stop
    character(len=16) :: nofails_str

    if ( ftnunit_file_exists("ftnunit.run") ) then
        if ( test_mode == mode_all ) then
            write(*,'(a,i5)') 'Number of tests run:                        ', notests_run
            write(*,'(a,i5)') 'Number of tests ignored:                    ', notests_ignored
            write(*,'(a,i5)') 'Number of tests failed:                     ', notests_failed
            
            write(nofails_str,'(i0)') nofails
            write(*,'(3a)')   'Number of failed assertions:', repeat(' ', 16+5-len_trim(nofails_str)), nofails_str
            write(*,'(a,i5)') 'Number of runs needed to complete the tests:', noruns
        endif
        call ftnunit_remove_file( "ftnunit.lst" )
        call ftnunit_write_html_footer

        if ( test_mode == mode_all .and. .not. isSilent() ) then
            call ftnunit_hook_test_completed
        endif

        if ( .not. present(stop) ) then
            if (nofails > 0) then
                error stop nofails
            else
                stop
            end if
        else
            if ( stop ) then
                if (nofails > 0) then
                    error stop nofails
                else
                    stop
                end if
            endif
        endif
    endif
end subroutine

function isSilent () result (silent)

    logical                 :: silent
    character(len=160)      :: argument

    integer                 :: status, i
    
    silent = .false.
    
    do i = 1, command_argument_count()
        call get_command_argument( i, argument, status = status )
        argument = adjustl(argument)
        if (argument == "-s" .or. argument== "/s" .or. argument == "-silent" .or. argument== "/silent" ) then
            silent = .true.
        end if
    end do
    
end function isSilent

subroutine setWorkingDir

    logical                 :: silent
    character(len=255)      :: argument

    integer                 :: status, i, j
    
    silent = .false.
    
    j = -1
    do i = 1, command_argument_count()
        if (i == j) then
            call get_command_argument( i, argument, status = status )
            argument = adjustl(argument)
            
            call chdir(argument) 
        else
            call get_command_argument( i, argument, status = status )
            argument = adjustl(argument)
            if (argument == "-p" .or. argument== "/p" .or. argument == "-path" .or. argument== "/path" ) then
                j = i + 1
            end if
        end if
    end do
    
end subroutine setWorkingDir


! runtests --
!     Subroutine to run the tests if requested
! Arguments:
!     testproc      The test subroutine that actually runs the unit test
!
subroutine runtests( testproc )
    interface
        subroutine testproc
        end subroutine testproc
    end interface

    integer :: lun
    integer :: ierr

    if (associated(subptrTestInit)) then
        call subptrTestInit
    endif
    
    if ( call_final ) then
        call runtests_init_priv
    endif

    if ( ftnunit_file_exists("ftnunit.run") ) then
        if ( noruns == 1 .and. call_final ) then
            call ftnunit_write_html_header
        endif

        call testproc

        if ( call_final ) then
            call runtests_final
        endif

    endif

end subroutine runtests

! expect_program_stop --
!     Indicate that the program is expected to stop in this test
! Arguments:
!     None
! Side effects:
!     A file "ftnunit.stop" is created to register this expectation
!     and whether it was successful. A file is needed because the
!     program has to be rerun.
!
! Note:
!     It is NOT possible to distinguish an expected stop from
!     a crash in a test that should stop explicitly. Hence the
!     different status (Stopped).
!
subroutine expect_program_stop

    integer :: lun

    write(*,*) 'Warning: the feature "expect_program_stop" is incomplete!'
    open( newunit=lun, file = "ftnunit.stop" )
    write( lun, *) .true.
    close( lun )

end subroutine expect_program_stop

! assert --
!     Subroutine for general failure (e.g. in case default)
! Arguments:
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
subroutine assert( text )
    character(len=*), intent(in) :: text

    nofails = nofails + 1
    write(*,*) '    Error: "',trim(text)
    call ftnunit_write_html_failed_logic( text, .true. )
    call ftnunit_hook_test_assertion_failed( testname, text, "General failure" )
end subroutine assert

! assert_true --
!     Subroutine to check if a condition is true
! Arguments:
!     cond          Condition to be checked
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
subroutine assert_true( cond, text )
    logical, intent(in)          :: cond
    character(len=*), intent(in) :: text

    if ( .not. cond ) then
        nofails = nofails + 1
        write(*,*) '    Condition "',trim(text), '" failed'
        write(*,*) '    It should have been true'
        call ftnunit_write_html_failed_logic( text, .true. )
        call ftnunit_hook_test_assertion_failed( testname, text, "Condition should have been true" )
    endif
end subroutine assert_true

! assert_false --
!     Subroutine to check if a condition is false
! Arguments:
!     cond          Condition to be checked
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
subroutine assert_false( cond, text )
    logical, intent(in)          :: cond
    character(len=*), intent(in) :: text

    if ( cond ) then
        nofails = nofails + 1
        write(*,*) '    Condition "',trim(text), '" failed'
        write(*,*) '    It should have been false'
        call ftnunit_write_html_failed_logic( text, .false. )
        call ftnunit_hook_test_assertion_failed( testname, text, "Condition should have been false" )
    endif
end subroutine assert_false

! assert_equal_logical --
!     Subroutine to check if two logical values are equivalent
! Arguments:
!     value1        First logical value
!     value2        Second logical value
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
subroutine assert_equal_logical( value1, value2, text )
    logical, intent(in)          :: value1
    logical, intent(in)          :: value2
    character(len=*), intent(in) :: text

    if ( value1 .neqv. value2 ) then
        nofails = nofails + 1
        write(*,*) '    Assertion "',trim(text), '" failed'
        write(*,*) '    The two logical values are not the same'
        call ftnunit_write_html_failed_equivalent( text )
        call ftnunit_hook_test_assertion_failed( testname, text, "Logical values are not the same" )
    endif
end subroutine assert_equal_logical

! assert_equal_logical1d --
!     Subroutine to check if two arrays of logical values are equivalent
! Arguments:
!     array1        First logical array
!     array2        Second logical array
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
subroutine assert_equal_logical1d( array1, array2, text )
    logical, dimension(:), intent(in)          :: array1
    logical, dimension(:), intent(in)          :: array2
    character(len=*), intent(in)               :: text

    integer                                    :: i
    integer                                    :: count
    logical                                    :: addtext

    addtext = .false.

    if ( size(array1) /= size(array2) ) then
        nofails = nofails + 1
        write(*,*) '    Arrays have different sizes: "',trim(text), '" - assertion failed'
        call ftnunit_hook_test_assertion_failed( testname, text, "Arrays have different sizes" )
    else
        if ( any( array1 .neqv. array2 ) ) then
            nofails = nofails + 1
            write(*,*) '    One or more logical values different: "',trim(text), '" - assertion failed'
            call ftnunit_hook_test_assertion_failed( testname, text, "One or more logical values are different" )
            count = 0
            do i = 1,size(array1)
                if ( array1(i) .neqv. array2(i) ) then
                    count = count + 1
                    if ( count == 1 ) then
                        write(*,'(3a10)')    '    Index', '     First', '    Second'
                    endif
                    if ( count < 50 ) then
                        write(*,'(i10,2l10)')    i, array1(i), array2(i)
                        call ftnunit_write_html_failed_equivalent1d( &
                            text, i, array1(i), array2(i), addtext )
                        addtext = .false.
                    endif
                    write(*,*) 'Number of differences: ', count
                endif
            enddo
        endif
    endif
end subroutine assert_equal_logical1d

! assert_equal_string --
!     Subroutine to check if two string are equal
! Arguments:
!     value1        First string value
!     value2        Second string value
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
subroutine assert_equal_string( value1, value2, text )
    character(len=*), intent(in) :: value1
    character(len=*), intent(in) :: value2
    character(len=*), intent(in) :: text

    if ( value1 /= value2 ) then
        nofails = nofails + 1
        write(*,*) '    Assertion "',trim(text), '" failed'
        write(*,*) '    The two strings are not the same:'
        write(*,*) '    String 1: ' // trim(value1)
        write(*,*) '    String 2: ' // trim(value2)
        call ftnunit_write_html_failed_string( text, value1, value2 )
        call ftnunit_hook_test_assertion_failed( testname, text, "Strings are not the same" )
    endif
end subroutine assert_equal_string

! assert_equal_string1d --
!     Subroutine to check if two arrays of strings are equal
! Arguments:
!     array1        First string array
!     array2        Second string array
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
subroutine assert_equal_string1d( array1, array2, text )
    character(len=*), dimension(:), intent(in) :: array1
    character(len=*), dimension(:), intent(in) :: array2
    character(len=*), intent(in)               :: text

    integer                                    :: i
    integer                                    :: count
    logical                                    :: addtext

    addtext = .false.

    if ( size(array1) /= size(array2) ) then
        nofails = nofails + 1
        write(*,*) '    Arrays have different sizes: "',trim(text), '" - assertion failed'
        call ftnunit_hook_test_assertion_failed( testname, text, "Arrays have different sizes" )
    else
        if ( any( array1 /= array2 ) ) then
            nofails = nofails + 1
            write(*,*) '    One or more strings different: "',trim(text), '" - assertion failed'
            call ftnunit_hook_test_assertion_failed( testname, text, "One or more strings are different" )
            count = 0
            do i = 1,size(array1)
                if ( array1(i) /= array2(i) ) then
                    count = count + 1
                    write(*,'(a10,i10)')    '    Index:', i
                    if ( count < 50 ) then
                        write(*,'(a10,a)') '        First:  ', array1(i)
                        write(*,'(a10,a)') '        Second: ', array2(i)
                        call ftnunit_write_html_failed_string1d( &
                            text, i, array1(i), array2(i), addtext )
                        addtext = .false.
                    endif
                    write(*,*) 'Number of differences: ', count
                endif
            enddo
        endif
    endif
end subroutine assert_equal_string1d

! assert_equal_int --
!     Subroutine to check if two integers are equal
! Arguments:
!     value1        First value
!     value2        Second value
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
subroutine assert_equal_int( value1, value2, text )
    integer, intent(in)          :: value1
    integer, intent(in)          :: value2
    character(len=*), intent(in) :: text

    if ( value1 /= value2) then
        nofails = nofails + 1
        write(*,*) '    Values not equal: "',trim(text), '" - assertion failed'
        write(*,*) '    Values: ', value1, ' and ', value2
        call ftnunit_write_html_failed_int( text, value1, value2 )
        call ftnunit_hook_test_assertion_failed( testname, text, "Integer values are not the same" )
    endif
end subroutine assert_equal_int

! assert_equal_int1d --
!     Subroutine to check if two integer arrays are equal
! Arguments:
!     array1        First array
!     array2        Second array
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
subroutine assert_equal_int1d( array1, array2, text )
    integer, dimension(:), intent(in) :: array1
    integer, dimension(:), intent(in) :: array2
    character(len=*), intent(in)      :: text

    integer                           :: i
    integer                           :: count
    logical                           :: addtext

    addtext = .false.

    if ( size(array1) /= size(array2) ) then
        nofails = nofails + 1
        write(*,*) '    Arrays have different sizes: "',trim(text), '" - assertion failed'
        call ftnunit_hook_test_assertion_failed( testname, text, "Arrays have different sizes" )
    else
        if ( any( array1 /= array2 ) ) then
            nofails = nofails + 1
            write(*,*) '    One or more values different: "',trim(text), '" - assertion failed'
            call ftnunit_hook_test_assertion_failed( testname, text, "One or more values are different" )
            count = 0
            do i = 1,size(array1)
                if ( array1(i) /= array2(i) ) then
                    count = count + 1
                    if ( count == 1 ) then
                        write(*,'(3a10)')    '    Index', '     First', '    Second'
                    endif
                    if ( count < 50 ) then
                        write(*,'(3i10)')    i, array1(i), array2(i)
                        call ftnunit_write_html_failed_int1d( &
                            text, i, array1(i), array2(i), addtext )
                        addtext = .false.
                    endif
                    write(*,*) 'Number of differences: ', count
                endif
            enddo
        endif
    endif
end subroutine assert_equal_int1d

! assert_comparable_real --
!     Subroutine to check if two reals are approximately equal
! Arguments:
!     value1        First value
!     value2        Second value
!     margin        Allowed margin (relative)
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
! Note:
!     Because of NaNs, the conditions are reversed!
!
subroutine assert_comparable_real( value1, value2, margin, text )
    real, intent(in)             :: value1
    real, intent(in)             :: value2
    real, intent(in)             :: margin
    character(len=*), intent(in) :: text

    if ( abs(value1-value2) <= 0.5 * margin * (abs(value1)+abs(value2)) ) then
        !
        ! Within the margin - no action
        !
    else
        nofails = nofails + 1
        write(*,*) '    Values not comparable: "',trim(text), '" - assertion failed'
        write(*,*) '    Values: ', value1, ' and ', value2
        call ftnunit_hook_test_assertion_failed( testname, text, "One or more values differ more than the margin" )
        call ftnunit_write_html_failed_real( text, value1, value2 )
    endif
end subroutine assert_comparable_real

! assert_comparable_real1d --
!     Subroutine to check if two real arrays are comparable
! Arguments:
!     array1        First array
!     array2        Second array
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
!
! Note:
!     Because of NaNs, the conditions are reversed!
!
subroutine assert_comparable_real1d( array1, array2, margin, text )
    real, dimension(:), intent(in)    :: array1
    real, dimension(:), intent(in)    :: array2
    real, intent(in)                  :: margin
    character(len=*), intent(in)      :: text

    integer                           :: i
    integer                           :: count
    logical                           :: addtext

    addtext = .false.

    if ( size(array1) /= size(array2) ) then
        nofails = nofails + 1
        write(*,*) '    Arrays have different sizes: "',trim(text), '" - assertion failed'
        call ftnunit_hook_test_assertion_failed( testname, text, "Arrays have different sizes" )
    else
        if ( all( abs(array1-array2) <= 0.5 * margin * (abs(array1)+abs(array2)) ) ) then
            !
            ! Within the margin - no action
            !
        else
            nofails = nofails + 1
            write(*,*) '    One or more values different: "',trim(text), '" - assertion failed'
            call ftnunit_hook_test_assertion_failed( testname, text, "One or more values differ more than the margin" )
            count = 0
            do i = 1,size(array1)
                if ( abs(array1(i)-array2(i)) <= &
                         0.5 * margin * (abs(array1(i))+abs(array2(i))) ) then
                    !
                    ! Within the margin - no action
                    !
                else
                    count = count + 1
                    if ( count == 1 ) then
                        write(*,'(a10,2a15)')    '    Index', '          First', '         Second'
                    endif
                    if ( count < 50 ) then
                        write(*,'(i10,2e15.5)')    i, array1(i), array2(i)
                        call ftnunit_write_html_failed_real1d( &
                            text, i, array1(i), array2(i), addtext )
                        addtext = .false.
                    endif
                endif
            enddo
            if ( count > 0 ) then
                write(*,*) 'Number of differences: ', count
            endif
        endif
    endif
end subroutine assert_comparable_real1d

! assert_comparable_real2d --
!     Subroutine to check if two two-dimensional real arrays are comparable
! Arguments:
!     array1        First array
!     array2        Second array
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
! Note:
!     Because of NaNs, the conditions are reversed!
!
subroutine assert_comparable_real2d( array1, array2, margin, text )
    real, dimension(:,:), intent(in)    :: array1
    real, dimension(:,:), intent(in)    :: array2
    real, intent(in)                    :: margin
    character(len=*), intent(in)        :: text

    integer                             :: i
    integer                             :: j
    integer                             :: count
    logical                             :: addtext

    addtext = .false.

    if ( any( shape(array1) /= shape(array2) ) ) then
        nofails = nofails + 1
        write(*,*) '    Arrays have different shapes: "',trim(text), '" - assertion failed'
        call ftnunit_hook_test_assertion_failed( testname, text, "Arrays have different shapes" )
    else
        if ( all( abs(array1-array2) <= 0.5 * margin * (abs(array1)+abs(array2)) ) ) then
            !
            ! Within the margin - no action
            !
        else
            nofails = nofails + 1
            write(*,*) '    One or more values different: "',trim(text), '" - assertion failed'
            call ftnunit_hook_test_assertion_failed( testname, text, "One or more values differ more than the margin" )
            count = 0
            do j = 1,size(array1,2)
                do i = 1,size(array1,1)
                    if ( abs(array1(i,j)-array2(i,j)) <= &
                             0.5 * margin * (abs(array1(i,j))+abs(array2(i,j))) ) then
                        !
                        ! Within the margin - no action
                        !
                    else
                        count = count + 1
                        if ( count == 1 ) then
                            write(*,'(a10,2a15)')    '    Index', '          First', '         Second'
                        endif
                        if ( count < 50 ) then
                            write(*,'(2i5,2e15.5)')    i, j, array1(i,j), array2(i,j)
                            call ftnunit_write_html_failed_real2d( &
                                text, i, j, array1(i,j), array2(i,j), addtext )
                            addtext = .false.
                        endif
                    endif
                enddo
            enddo
            if ( count > 0 ) then
                write(*,*) 'Number of differences: ', count
            endif
        endif
    endif
end subroutine assert_comparable_real2d

! assert_comparable_double --
!     Subroutine to check if two double precision reals are approximately equal
! Arguments:
!     value1        First value
!     value2        Second value
!     margin        Allowed margin (relative)
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
! Note:
!     Because of NaNs, the conditions are reversed!
!
subroutine assert_comparable_double( value1, value2, margin, text )
    use, intrinsic :: ieee_arithmetic, only: ieee_is_finite
    real(kind=dp), intent(in)             :: value1
    real(kind=dp), intent(in)             :: value2
    real(kind=dp), intent(in)             :: margin
    character(len=*), intent(in)          :: text

    if ( ieee_is_finite(value1) .and. ieee_is_finite(value2) .and. &
        abs(value1-value2) <= 0.5d0 * margin * (abs(value1)+abs(value2)) ) then
        !
        ! Within the margin - no action
        !
    else
        nofails = nofails + 1
        write(*,*) '    Values not comparable: "',trim(text), '" - assertion failed'
        write(*,*) '    Values: ', value1, ' and ', value2
        call ftnunit_hook_test_assertion_failed( testname, text, "One or more values differ more than the margin" )
        call ftnunit_write_html_failed_double( text, value1, value2 )
    endif
end subroutine assert_comparable_double

! assert_comparable_double1d --
!     Subroutine to check if two double precision real arrays are comparable
! Arguments:
!     array1        First array
!     array2        Second array
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
! Note:
!     Because of NaNs, the conditions are reversed!
!
subroutine assert_comparable_double1d( array1, array2, margin, text )
    real(kind=dp), dimension(:), intent(in)    :: array1
    real(kind=dp), dimension(:), intent(in)    :: array2
    real(kind=dp), intent(in)                  :: margin
    character(len=*), intent(in)               :: text

    integer                           :: i
    integer                           :: count
    logical                           :: addtext

    addtext = .true.

    if ( size(array1) /= size(array2) ) then
        nofails = nofails + 1
        write(*,*) '    Arrays have different sizes: "',trim(text), '" - assertion failed'
        call ftnunit_hook_test_assertion_failed( testname, text, "Arrays have different sizes" )
    else
        if ( all( abs(array1-array2) <= 0.5d0 * margin * (abs(array1)+abs(array2)) ) ) then
            !
            ! Within the margin - no action
            !
        else
            nofails = nofails + 1
            write(*,*) '    One or more values different: "',trim(text), '" - assertion failed'
            call ftnunit_hook_test_assertion_failed( testname, text, "One or more values differ more than the margin" )
            count = 0
            do i = 1,size(array1)
                if ( abs(array1(i)-array2(i)) <= &
                         0.5d0 * margin * (abs(array1(i))+abs(array2(i))) ) then
                    !
                    ! Within the margin - no action
                    !
                else
                    count = count + 1
                    if ( count == 1 ) then
                        write(*,'(a10,2a15)')    '    Index', '          First', '         Second'
                    endif
                    if ( count < 50 ) then
                        write(*,'(i10,2e15.5)')    i, array1(i), array2(i)
                        call ftnunit_write_html_failed_double1d( &
                            text, i, array1(i), array2(i), addtext )
                        addtext = .false.
                    endif
                endif
            enddo
            if ( count > 0 ) then
                write(*,*) 'Number of differences: ', count
            endif
        endif
    endif
end subroutine assert_comparable_double1d

! assert_comparable_double2d --
!     Subroutine to check if two two-dimensional double precision real arrays are comparable
! Arguments:
!     array1        First array
!     array2        Second array
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
! Note:
!     Because of NaNs, the conditions are reversed!
!
subroutine assert_comparable_double2d( array1, array2, margin, text )
    real(kind=dp), dimension(:,:), intent(in)    :: array1
    real(kind=dp), dimension(:,:), intent(in)    :: array2
    real(kind=dp), intent(in)                    :: margin
    character(len=*), intent(in)                 :: text

    integer                             :: i
    integer                             :: j
    integer                             :: count
    logical                             :: addtext

    addtext = .false.

    if ( any( shape(array1) /= shape(array2) ) ) then
        nofails = nofails + 1
        write(*,*) '    Arrays have different shapes: "',trim(text), '" - assertion failed'
        call ftnunit_hook_test_assertion_failed( testname, text, "Arrays have different shapes" )
    else
        if ( all( abs(array1-array2) <= 0.5d0 * margin * (abs(array1)+abs(array2)) ) ) then
            !
            ! Within the margin - no action
            !
        else
            nofails = nofails + 1
            write(*,*) '    One or more values different: "',trim(text), '" - assertion failed'
            call ftnunit_hook_test_assertion_failed( testname, text, "One or more values differ more than the margin" )
            count = 0
            do j = 1,size(array1,2)
                do i = 1,size(array1,1)
                    if ( abs(array1(i,j)-array2(i,j)) <= &
                             0.5d0 * margin * (abs(array1(i,j))+abs(array2(i,j))) ) then
                        !
                        ! Within the margin - no action
                        !
                    else
                        count = count + 1
                        if ( count == 1 ) then
                            write(*,'(a10,2a15)')    '    Index', '          First', '         Second'
                        endif
                        if ( count < 50 ) then
                            write(*,'(2i5,2e15.5)')    i, j, array1(i,j), array2(i,j)
                            call ftnunit_write_html_failed_double2d( &
                                text, i, j, array1(i,j), array2(i,j), addtext )
                            addtext = .false.
                        endif
                    endif
                enddo
            enddo
            if ( count > 0 ) then
                write(*,*) 'Number of differences: ', count
            endif
        endif
    endif
end subroutine assert_comparable_double2d

! assert_inbetween_real --
!     Subroutine to check if a real number is contained in a given interval
! Arguments:
!     value         Value to check
!     vmin          Minimum value
!     vmax          Maximum value
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
! Note:
!     Because of NaNs, the conditions are reversed!
!
subroutine assert_inbetween_real( value, vmin, vmax, text )
    real, intent(in)                    :: value
    real, intent(in)                    :: vmin
    real, intent(in)                    :: vmax
    character(len=*), intent(in)        :: text

    logical                             :: addtext

    addtext = .false.

    if ( value >= vmin .and. value <= vmax ) then
        !
        ! Within the margin - no action
        !
    else
        nofails = nofails + 1
        write(*,'(3a)')      '    Value out of range: "',trim(text), '" - assertion failed'
        write(*,'(a,g15.5)') '        Value:     ', value
        write(*,'(a,g15.5)') '        Minimum:   ', vmin
        write(*,'(a,g15.5)') '        Maximum:   ', vmax
        call ftnunit_write_html_failed_inbetween_real( &
                                text, value, vmin, vmax )
        call ftnunit_hook_test_assertion_failed( testname, text, "Value out of range" )
    endif
end subroutine assert_inbetween_real

! assert_inbetween_double --
!     Subroutine to check if a double precision real number is contained in a given interval
! Arguments:
!     value         Value to check
!     vmin          Minimum value
!     vmax          Maximum value
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
! Note:
!     Because of NaNs, the conditions are reversed!
!
subroutine assert_inbetween_double( value, vmin, vmax, text )
    real(kind=dp), intent(in)           :: value
    real(kind=dp), intent(in)           :: vmin
    real(kind=dp), intent(in)           :: vmax
    character(len=*), intent(in)        :: text

    logical                             :: addtext

    addtext = .false.

    if ( value >= vmin .and. value <= vmax ) then
        !
        ! Within the margin - no action
        !
    else
        nofails = nofails + 1
        write(*,'(3a)')      '    Value out of range: "',trim(text), '" - assertion failed'
        write(*,'(a,g21.12)') '        Value:     ', value
        write(*,'(a,g21.12)') '        Minimum:   ', vmin
        write(*,'(a,g21.12)') '        Maximum:   ', vmax
        call ftnunit_write_html_failed_inbetween_double( &
                                text, value, vmin, vmax )
        call ftnunit_hook_test_assertion_failed( testname, text, "Value out of range" )
    endif
end subroutine assert_inbetween_double

! assert_inbetween_quad --
!     Subroutine to check if a quad precision real number is contained in a given interval
! Arguments:
!     value         Value to check
!     vmin          Minimum value
!     vmax          Maximum value
!     text          Text describing the assertion
! Side effects:
!     If the assertion fails, this is reported to standard
!     output. Also, nofails is increased by one.
! Note:
!     Because of NaNs, the conditions are reversed!
!
subroutine assert_inbetween_quad( value, vmin, vmax, text )
    real(kind=qd), intent(in)           :: value
    real(kind=qd), intent(in)           :: vmin
    real(kind=qd), intent(in)           :: vmax
    character(len=*), intent(in)        :: text

    logical                             :: addtext

    addtext = .false.

    if ( value >= vmin .and. value <= vmax ) then
        !
        ! Within the margin - no action
        !
    else
        nofails = nofails + 1
        write(*,'(3a)')      '    Value out of range: "',trim(text), '" - assertion failed'
        write(*,'(a,g21.12)') '        Value:     ', value
        write(*,'(a,g21.12)') '        Minimum:   ', vmin
        write(*,'(a,g21.12)') '        Maximum:   ', vmax
        call ftnunit_write_html_failed_inbetween_quad( text, value, vmin, vmax )
        call ftnunit_hook_test_assertion_failed( testname, text, "Value out of range" )
    endif
end subroutine assert_inbetween_quad

! lineInExceptionList --
!     Look if one of the items in exceptionList is part of a line
!
! Arguments:
!     line              The line to be evaluated
!     exceptionList     The list of exception items
!
! Return: true if one of the exception items is part of the line
function lineInExceptionList(line, exceptionList)
    character(len=*)                :: line
    character(len=*), dimension(:)  :: exceptionList
    
    integer                         :: j
    integer                         :: exceptionListCount
    logical                         :: lineInExceptionList
    logical                         :: skipLine
    
    skipLine = .false.
    exceptionListCount = size(exceptionList)
    do j = 1, exceptionListCount
        skipLine = index(line, trim(exceptionList(j))) > 0 
        if (skipLine) then 
            exit
        endif
    enddo
    lineInExceptionList = skipLine
end function lineInExceptionList

! assert_files_comparable --
!     Compare two files and establish whether they are equal or not
!
! Arguments:
!     filename1       Name of the first file
!     filename2       Name of the second file
!     text            Text describing the assertion
!     tolerance       Relative tolerance for numbers
!                     (optional; defaults to 1.0e-5)
!
! Note:
!     The files are compared line by line and the items
!     within each line are compared too for numbers that
!     are almost equal.
!     The lines are read into a variable of length line_length
!     Each line may contain up to max_items items (character
!     strings) of length item_length.
!
subroutine assert_files_comparable( filename1, filename2, text, tolerance )
    character(len=*), intent(in) :: filename1
    character(len=*), intent(in) :: filename2
    character(len=*), intent(in) :: text
    real, intent(in), optional   :: tolerance
    character(len=1), dimension(0)  :: exceptionList
    
    call assert_files_comparable_with_exceptions(filename1, filename2, text, exceptionList, tolerance)
end subroutine assert_files_comparable

! assert_files_comparable --
!     Compare two files and establish whether they are equal or not
!
! Arguments:
!     filename1       Name of the first file
!     filename2       Name of the second file
!     text            Text describing the assertion
!     exceptionList   List of of exception items
!     tolerance       Relative tolerance for numbers
!                     (optional; defaults to 1.0e-5)
!
! Note:
!     The files are compared line by line and the items
!     within each line are compared too for numbers that
!     are almost equal.
!     The lines are read into a variable of length line_length
!     Each line may contain up to max_items items (character
!     strings) of length item_length.
!     If a line contains one of the items in the exceptionlist, the line will be skipped for the comparison
!
subroutine assert_files_comparable_with_exceptions(filename1, filename2, text, exceptionList, tolerance)

    integer, parameter           :: line_length = 200
    integer, parameter           :: item_length = 20
    integer, parameter           :: max_items   = 100

    character(len=*), intent(in) :: filename1
    character(len=*), intent(in) :: filename2
    character(len=*), intent(in) :: text
    real, intent(in), optional   :: tolerance
    character(len=*), dimension(:)  :: exceptionList

    real                         :: tol
    real                         :: value1
    real                         :: value2
    integer                      :: i
    integer                      :: lun1
    integer                      :: lun2
    logical                      :: skipLine
    integer                      :: differences
    integer                      :: ierr1
    integer                      :: ierr2
    logical                      :: header
    character(len=line_length)   :: line1
    character(len=line_length)   :: line2
    character(len=line_length)   :: elapsedTimeline
    character(len=item_length), dimension(max_items) :: item1
    character(len=item_length), dimension(max_items) :: item2

    header      = .true.
    differences = 0
    tol         = 1.0e-3
    if ( present(tolerance) ) tol = tolerance

    open( newunit=lun1, file = filename1, status = 'old', iostat = ierr1 )

    open( newunit=lun2, file = filename2, status = 'old', iostat = ierr2 )

    if ( ierr1 /= 0 .or. ierr2 /= 0 ) then
        nofails = nofails + 1
        call write_header

        if ( ierr1 /= 0 ) then
            write(*, '(a)' ) 'Input file '// trim(filename1) // ' could not be opened'
        endif

        if ( ierr2 /= 0 ) then
            write(*, '(a)' ) 'Input file '// trim(filename2) // ' could not be opened'
        endif

        write(*, '(a)' ) 'Comparison failed'
        call ftnunit_write_html_failed_files( text, filename1, filename2, &
            'One or both files could not be opened' )
        call ftnunit_hook_test_assertion_failed( testname, text, "One or both files could not be opened" )

        return
    endif

    do
        skipLine = .true.
        do while (skipLine)
            read( lun1, '(a)', iostat = ierr1 ) line1
            skipLine = lineInExceptionList(line1, exceptionList)
        enddo
        skipLine = .true.
        do while (skipLine)
            read( lun2, '(a)', iostat = ierr2 ) line2
            skipLine = lineInExceptionList(line2, exceptionList)
        enddo
        !
        ! End of file for both?
        !
        if ( ierr1 < 0 .and. ierr2 < 0 ) then
            exit
        endif

        !
        ! End of file or error for only one?
        !
        if ( ierr1 /= 0 .or. ierr2 /= 0 ) then
            call write_header

            if ( ierr1 /= 0 ) then
                write(*, '(a)' ) 'Error/end-of-file reading file '// trim(filename1)
            endif

            if ( ierr2 /= 0 ) then
                write(*, '(a)' ) 'Error/end-of-file reading file '// trim(filename2)
            endif

            write(*, '(a)' ) 'Comparison failed'
            call ftnunit_write_html_failed_files( text, filename1, filename2, &
                'Error/premature end-of-file reading the files' )
            call ftnunit_hook_test_assertion_failed( testname, text, "Error/premature end-of-file reading the files" )

            exit
        endif

        !
        ! If the lines are equal, there is nothing to do
        !
        if ( line1 == line2 ) then
            cycle
        endif

        !
        ! don't compare exact warning message
        !
        if ( line1(1:7) == 'WARNING' .and. line2(1:7) == 'WARNING' ) then
            cycle
        endif

        !
        ! don't compare computation times: Elapsed time
        !
        write( elapsedTimeline, '(A40)') ' Elapsed time'
        if (( line1(1:40) == elapsedTimeline(1:40) ) .and. ( line2(1:40) == elapsedTimeline(1:40) )) then
            cycle
        endif

        !
        ! The lines are different - but sufficiently different?
        !
        item1 = '?'
        item2 = '?'

        read( line1, *, iostat = ierr1 ) item1
        read( line2, *, iostat = ierr2 ) item2

        !
        ! We are not interested in the error codes, but in
        ! the contents of the items
        !
        do i = 1,max_items
            if ( item1(i) /= item2(i) ) then
                read( item1(i), *, iostat = ierr1 ) value1
                read( item2(i), *, iostat = ierr2 ) value2

                if ( ierr1 == 0 .and. ierr2 == 0 ) then
                    if ( abs(value1-value2) <= 0.5 * tol * (abs(value1)+abs(value2)) ) then
                        !
                        ! Within the margin - no action
                        !
                    else
                        nofails = nofails + 1
                        call write_header

                        write(*, '(4x,a)' ) 'File 1: ' // trim(line1)
                        write(*, '(4x,a)' ) 'File 2: ' // trim(line2)
                        write(*, '(4x,a,i5,a,e12.4)' ) &
                            'Difference in item ', i, ': ', abs(value1-value2)
                        write(*, '(4x,a,e12.4)' ) &
                            'Mean absolute value       : ', 0.5 * (abs(value1)+abs(value2))

                        differences = differences + 1
                        if ( differences == 1 ) then
                            call ftnunit_write_html_failed_files( text, line1, line2, &
                                'Differences in one or more numerical items' )
                            call ftnunit_hook_test_assertion_failed( testname, text, "Differences in one or more numerical items" )
                        endif
                        exit
                    endif

                elseif ( ierr1 == 0 .neqv. ierr2 == 0 ) then
                    !
                    ! Only one is a number?
                    !
                    nofails = nofails + 1
                    call write_header

                    write(*, '(4x,a)' ) 'File 1: ' // trim(line1)
                    write(*, '(4x,a)' ) 'File 2: ' // trim(line2)
                    write(*, '(4x,a,i5,a,e12.4)' ) &
                        'Item ', i, ' is a number in one file and not in the other'

                    differences = differences + 1
                    if ( differences == 1 ) then
                        call ftnunit_write_html_failed_files( text, line1, line2, &
                             'Corresponding items not both numerical' )
                        call ftnunit_hook_test_assertion_failed( testname, text, "Corresponding items not both numerical" )
                    endif
                    exit
                else
                    !
                    ! Unequal substrings
                    !
                    nofails = nofails + 1
                    call write_header

                    write(*, '(4x,a)' ) 'File 1: ' // trim(line1)
                    write(*, '(4x,a)' ) 'File 2: ' // trim(line2)
                    write(*, '(4x,a,i5,a,e12.4)' ) &
                        'Item ', i, ' differs in the two lines'
                    differences = differences + 1
                    if ( differences == 1 ) then
                        call ftnunit_write_html_failed_files( text, line1, line2, &
                            'Differences in one or more string items' )
                        call ftnunit_hook_test_assertion_failed( testname, text, "Differences in one or more string items" )
                    endif
                    exit
                endif
            endif
        enddo
    enddo

    close( lun1 )
    close( lun2 )
contains
subroutine write_header
    if ( header ) then
        header = .false.
        write(*, '(a)' ) 'Comparing files:'
        write(*, '(a)' ) '    File 1:' // trim(filename1)
        write(*, '(a)' ) '    File 2:' // trim(filename2)
    endif
end subroutine write_header
end subroutine assert_files_comparable_with_exceptions

! ftnunit_file_exists --
!     Auxiliary function to see if a file exists
! Arguments:
!     filename      Name of the file to check
! Returns:
!     .true. if the file exists, .false. otherwise
!
logical function ftnunit_file_exists( filename )
    character(len=*), intent(in) :: filename

    inquire( file = filename, exist = ftnunit_file_exists )
end function ftnunit_file_exists

! ftnunit_remove_file --
!     Auxiliary subroutine to remove a file
! Arguments:
!     filename      Name of the file to be removed
!
subroutine ftnunit_remove_file( filename )
    character(len=*), intent(in) :: filename

    integer                      :: lun
    integer                      :: ierr

    open( newunit=lun, file = filename, iostat = ierr )
    if ( ierr /= 0 ) then
        write(*,'(10a)') '    Could not open file for removal: ', trim(filename)
        nofails = nofails + 1
    else
        close( lun, status = 'delete' )
        if ( ftnunit_file_exists( filename ) ) then
            write(*,'(10a)') '    Removal of file unsuccessful: ', trim(filename)
            nofails = nofails + 1
        endif
    endif

end subroutine ftnunit_remove_file

! ftnunit_make_empty_file --
!     Auxiliary subroutine to make an empty file
! Arguments:
!     filename      Name of the file to be created
!
subroutine ftnunit_make_empty_file( filename )
    character(len=*), intent(in) :: filename

    integer                      :: lun
    integer                      :: ierr

    if ( ftnunit_file_exists( filename ) ) then
        call ftnunit_remove_file( filename )
    endif
    open( newunit=lun, file = filename, iostat = ierr, status = 'new' )
    if ( ierr /= 0 ) then
        write(*,'(10a)') '    Failed to create empty file: ', trim(filename)
        nofails = nofails + 1
    else
        close( lun )
    endif

end subroutine ftnunit_make_empty_file

! ftnunit_write_html_header --
!     Auxiliary subroutine to write the header of the HTML file
! Arguments:
!     None
!
subroutine ftnunit_write_html_header
    integer :: lun

    open( newunit=lun, file = html_file )
    write( lun, '(a)' ) &
        '<html>', &
        '<header>', &
        '<title>' // trim(TestTitle) // '</title>', &
        '<style type="text/css">', &
        'span.indent {', &
        '   text-indent: 30;', &
        '}', &
        'span.red {', &
        '   background-color: #FF4040;', &
        '}', &
        'span.green {', &
        '   background-color: #00DD00;', &
        '}', &
        'span.yellow {', &
        '   background: yellow;', &
        '}', &
        'span.orange {', &
        '   background: orange;', &
        '}', &
        '</style>', &
        '</header>', &
        '<body>', &
        '<h3>' // trim(TestTitle) // '</h3>', &
        '<table>'
    close( lun )

end subroutine ftnunit_write_html_header

! ftnunit_write_html_footer --
!     Auxiliary subroutine to write the footer of the HTML file
! Arguments:
!     None
!
subroutine ftnunit_write_html_footer
    integer :: lun
    character(len=64) :: coloured


    if ( .not. has_run ) then
        call ftnunit_write_html_previous_failed
    endif

    open( newunit=lun, file = html_file, position = 'append' )

    if ( (has_run .or. ignore_test) .and. failed_asserts == 0 ) then
        call ftnunit_write_html_close_row( lun )
    endif

    write( lun, '(a)' ) '</tr>', '</table>'
    write( lun,'(a,i5)') '<h3>Summary:</h3>'
    write( lun,'(a,i5,a)' ) '<table cellspacing="0"> <tr> <td>Number of tests run: </td> <td>', notests_run, '</td></tr>'
    
    call colour_number(notests_ignored, coloured, 'yellow')
    write( lun,'(3a)') '<tr><td>Number of tests ignored: </td> <td>', trim(coloured), '</td></tr>'
    
    if (notests_work_in_progress > 0) then
        call colour_number(notests_work_in_progress, coloured, 'orange')
        write( lun,'(3a)') '<tr><td>Number of tests work-in-progress: </td> <td>', trim(coloured), '</td></tr>'
    endif

    call colour_number(notests_failed, coloured, 'red', 'green')
    write( lun,'(3a)') '<tr><td>Number of tests failed: </td> <td>', trim(coloured), '</td></tr>'

    call colour_number(nofails, coloured, 'red', 'green')
    write( lun,'(3a)') '<tr><td>Number of failed assertions: </td> <td>', trim(coloured), '</td></tr>'

    call colour_number(noruns, coloured, 'red', 'green', 1)
    write( lun,'(3a)') '<tr><td>Number of runs needed to complete the tests: &nbsp;</td> <td>', trim(coloured), '</td></tr></table>'

    write( lun, '(a)' ) '</body>', '</html>'
    close( lun )

end subroutine ftnunit_write_html_footer

subroutine colour_number(number, coloured, colour1, colour2, allowed)
integer, intent(in) :: number
integer, intent(in), optional :: allowed
character(len=*), intent(out) :: coloured
character(len=*), intent(in) :: colour1
character(len=*), intent(in), optional :: colour2

integer :: cp_allowed

cp_allowed = 0
if (present(allowed)) cp_allowed = allowed

if (number > cp_allowed ) then
    write(coloured, '(3a,i0,a)') '<span class="', colour1, '">', number, '</span>'
else if (present(colour2)) then
    write(coloured, '(3a,i0,a)') '<span class="', colour2, '">', number, '</span>'
else
    write(coloured, '(i0)') number
endif
end subroutine colour_number

! ftnunit_write_html_test_begin --
!     Auxiliary subroutine to write the test text to the HTML file
! Arguments:
!     text         Description of the test
!
subroutine ftnunit_write_html_test_begin( text )
    character(len=*)  :: text

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    if ( previous ) then
        if ( ignore_previous_test ) then
            ignore_previous_test = .false.
            call ftnunit_write_html_ignored( lun )
        else if ( failed_asserts == 0 ) then
            write( lun, '(a)' ) '<td><span class="green">OK</span></td>'
        endif
        write( lun, '(a)' ) '</tr>'
    endif

    failed_asserts = 0

    write( lun, '(4a)' ) '<tr>', '<td><b>', trim(text), '</b></td>'
    close( lun )

    previous = .true.

end subroutine ftnunit_write_html_test_begin

subroutine ftnunit_write_html_ignored(lun)
    integer, intent(in) :: lun

    character(len=6)  :: color

    if (index(ignore_reason_previous_test, 'level =') /= 1) then
        color = 'orange'
    else
        color = 'yellow'
    endif
    write( lun, '(5a)' ) '<td> <span class="', color, '"> Ignored: ' , trim(ignore_reason_previous_test), ' </span> </td>'
end subroutine ftnunit_write_html_ignored

!> write cpu and wall time as html comment to ftnunit.html
subroutine ftnunit_write_html_cpu ( cpu, wall )
    use precision, only : wp
    real(kind=wp), intent(in)   :: cpu   !< cpu time used for current test
    integer(kind=8), intent(in) :: wall  !< wall time used for current test
!
!   locals
!
    character(len=32) :: cpu_as_string
    character(len=32) :: wall_as_string
    real(kind=wp)     :: rate

    call system_clock(count_rate = rate)

    write( cpu_as_string, '(f20.3)' ) cpu
    write( wall_as_string, '(f20.3)' ) real(wall, 8) / rate
    call ftnunit_write_html_comment( "cpu time: " // trim(adjustl(cpu_as_string)) // " s. " // &
                       " wall time: " // trim(adjustl(wall_as_string)) // " s.")

end subroutine ftnunit_write_html_cpu

!> write string as html comment to ftnunit.html
subroutine ftnunit_write_html_comment(comment)    !< string to be printed
    character(len=*), intent(in) :: comment
!
!   locals
!
    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    write (lun, '(3a)') "<!-- " , trim(comment), " -->"

    close ( lun )
end subroutine ftnunit_write_html_comment

! ftnunit_write_html_previous_failed --
!     Auxiliary subroutine to write the closing of the failed test
!     from the previous run to the HTML file
! Arguments:
!     None
!
! Note:
!     Apparently the test caused a run-time error, so most probably no
!     assertions were checked.
!
subroutine ftnunit_write_html_previous_failed
    integer                     :: lun
    logical                     :: exist
    character(len=255)          :: msg
    character(len=16)           :: testnr

    character(len=*), parameter :: fileLastError = 'last_error.txt'

    inquire(file = fileLastError, exist=exist)
    if (exist) then
        open(newunit=lun, file = fileLastError)
        read(lun, '(a)') msg
        close(lun, status='delete')
    else
        msg = '-'
    endif

    open( newunit=lun, file = html_file, position = 'append' )

    write( lun, '(a)' ) '<td><span class="red">Crashed</span></td></tr>'
    write( lun, '(a)' ) '<tr><td><span class="indent">Possible run-time failure: '
    if (exist) then
        write ( lun, '(a)' ) trim(msg)
    else
        write ( lun, '(a)' ) 'check the log file'
    endif
    write( lun, '(a)' ) '</span></td></tr>'
    write(testnr, '(a,i0)') 'Testnr ', last_test
    call ftnunit_hook_test_assertion_failed( trim(testnr), "Crashed", trim(msg) )

    close( lun )

    failed_asserts = 1 ! Implicit assertion failed that the test will complete

end subroutine ftnunit_write_html_previous_failed

! ftnunit_write_html_previous_stopped --
!     Auxiliary subroutine to write the closing of a test that was expected to stop
!     from the previous run to the HTML file
! Arguments:
!     None
!
! Note:
!     There is no distinction possible between a deliberate stop and a run-time error
!
subroutine ftnunit_write_html_previous_stopped

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    write( lun, '(a)' ) &
        '<td><span class="green">Stopped</span></td></tr>', &
        '<tr><td><span class="indent">Deliberate stop, but run-time failure is still possible: check the log file</span></td></tr>'

    close( lun )

end subroutine ftnunit_write_html_previous_stopped

! ftnunit_write_html_close_row --
!     Auxiliary subroutine to write the closing of a row to the HTML file
! Arguments:
!     lun          LU-number for the HTML file
!
subroutine ftnunit_write_html_close_row( lun )
    integer           :: lun

    if ( ignore_previous_test ) then
        call ftnunit_write_html_ignored( lun )
    else if ( failed_asserts > 0 ) then
        if ( failed_asserts == 1 ) then
            write( lun, '(a)' ) &
                '<td><span class="red">Failed</span></td>'
        endif
        write( lun, '(a)' ) &
            '</tr><tr>'
    else
        write( lun, '(a)' ) &
            '<td><span class="green">OK</span></td>'
    endif

end subroutine ftnunit_write_html_close_row

! ftnunit_write_html_failed_logic --
!     Auxiliary subroutine to write a failed logic assertion to the HTML file
! Arguments:
!     text         Description of the test
!     expected     Expected value
!
subroutine ftnunit_write_html_failed_logic( text, expected )
    character(len=*)  :: text
    logical           :: expected

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    write( lun, '(a)' ) &
        '<td><span class="indent">', trim(text), '</span></td>', &
        '<td>Value should have been "' // trim(merge("true ", "false", expected)) // '"</td>'
    close( lun )

end subroutine ftnunit_write_html_failed_logic

! ftnunit_write_html_failed_equivalent --
!     Auxiliary subroutine to write a failed equivalent assertion to the HTML file
! Arguments:
!     text         Description of the test
!     expected     Expected value
!
subroutine ftnunit_write_html_failed_equivalent( text )
    character(len=*)  :: text
    logical           :: expected

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    write( lun, '(a)' ) &
        '<td><span class="indent">', trim(text), '</span></td>', &
        '<td>Logical values should have been equal</td>'
    close( lun )

end subroutine ftnunit_write_html_failed_equivalent

! ftnunit_write_html_failed_equivalent1d --
!     Auxiliary subroutine to write a failed logical array assertion to the HTML file
! Arguments:
!     text         Description of the test
!     idx          Index
!     value1       First value
!     value2       Second value
!     addtext      Add the text or not
!
subroutine ftnunit_write_html_failed_equivalent1d( text, idx, value1, value2, addtext )
    character(len=*)  :: text
    integer           :: idx
    logical           :: value1
    logical           :: value2
    logical           :: addtext

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    if ( addtext ) then
        write( lun, '(a)' ) &
            '<td><span class="indent">', trim(text), '</span></td>'
    else
        write( lun, '(a)' ) &
            '<td></td>'
    endif

    write( lun, '(a)' ) &
        '<td>Values at index: '
    write( lun, '(i0,a,l5,a,l5,a)' ) &
        idx, ':', &
        value1, ' -- ', value2, '</td>'
    close( lun )

end subroutine ftnunit_write_html_failed_equivalent1d

! ftnunit_write_html_failed_string --
!     Auxiliary subroutine to write a failed string assertion to the HTML file
! Arguments:
!     text         Description of the test
!     expected     Expected value
!
subroutine ftnunit_write_html_failed_string( text, value1, value2 )
    character(len=*)  :: text
    character(len=*)  :: value1
    character(len=*)  :: value2

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    write( lun, '(a)' ) &
        '<td><span class="indent">', trim(text), '</span></td>', &
        '<td>Strings should have been equal:<br>', &
        'String 1: ', value1, '<br>', &
        'String 2: ', value2, '</td>'
    close( lun )

end subroutine ftnunit_write_html_failed_string

! ftnunit_write_html_failed_string1d --
!     Auxiliary subroutine to write a failed string array assertion to the HTML file
! Arguments:
!     text         Description of the test
!     idx          Index
!     value1       First value
!     value2       Second value
!     addtext      Add the text or not
!
subroutine ftnunit_write_html_failed_string1d( text, idx, value1, value2, addtext )
    character(len=*)  :: text
    integer           :: idx
    character(len=*)  :: value1
    character(len=*)  :: value2
    logical           :: addtext

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    if ( addtext ) then
        write( lun, '(a)' ) &
            '<td><span class="indent">', trim(text), '</span></td>'
    else
        write( lun, '(a)' ) &
            '<td></td>'
    endif

    write( lun, '(a)' ) &
        '<td>Values at index: '
    write( lun, '(i0,a,a,a,a,a)' ) &
        idx, ':', &
        value1, ' -- ', value2, '</td>'
    close( lun )

end subroutine ftnunit_write_html_failed_string1d

! ftnunit_write_html_failed_int --
!     Auxiliary subroutine to write a failed integer assertion to the HTML file
! Arguments:
!     text         Description of the test
!     value1       First value
!     value2       Second value
!
subroutine ftnunit_write_html_failed_int( text, value1, value2 )
    character(len=*)  :: text
    integer           :: value1
    integer           :: value2

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    write( lun, '(a)' ) &
        '<td><span class="indent">', trim(text), '</span></td>', &
        '<td>Values: '
    write( lun, '(i0,a,i0,a)' ) &
        value1, ' -- ', value2, '</td>'
    close( lun )


end subroutine ftnunit_write_html_failed_int

! ftnunit_write_html_failed_int1d --
!     Auxiliary subroutine to write a failed integer assertion to the HTML file
! Arguments:
!     text         Description of the test
!     idx          Index
!     value1       First value
!     value2       Second value
!     addtext      Add the text or not
!
subroutine ftnunit_write_html_failed_int1d( text, idx, value1, value2, addtext )
    character(len=*)  :: text
    integer           :: idx
    integer           :: value1
    integer           :: value2
    logical           :: addtext

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    if ( addtext ) then
        write( lun, '(a)' ) &
            '<td><span class="indent">', trim(text), '</span></td>'
    else
        write( lun, '(a)' ) &
            '<td></td>'
    endif

    write( lun, '(a)' ) &
        '<td>Values at index: '
    write( lun, '(i0,a,i0,a,i0,a)' ) &
        idx, ':', &
        value1, ' -- ', value2, '</td>'
    close( lun )

end subroutine ftnunit_write_html_failed_int1d

! ftnunit_write_html_failed_real --
!     Auxiliary subroutine to write a failed real assertion to the HTML file
! Arguments:
!     text         Description of the test
!     value1       First value
!     value2       Second value
!
subroutine ftnunit_write_html_failed_real( text, value1, value2 )
    character(len=*)  :: text
    real              :: value1
    real              :: value2

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    write( lun, '(a)' ) &
        '<td><span class="indent">', trim(text), '</span></td>', &
        '<td>Values: '
    write( lun, '(e15.7,a,e15.7,a)' ) &
        value1, ' -- ', value2, '</td>'
    close( lun )

end subroutine ftnunit_write_html_failed_real

! ftnunit_write_html_failed_real1d --
!     Auxiliary subroutine to write a failed real assertion to the HTML file
! Arguments:
!     text         Description of the test
!     idx          Index
!     value1       First value
!     value2       Second value
!     addtext      Add the text or not
!
subroutine ftnunit_write_html_failed_real1d( text, idx, value1, value2, addtext )
    character(len=*)  :: text
    integer           :: idx
    real              :: value1
    real              :: value2
    logical           :: addtext

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    if ( addtext ) then
        write( lun, '(a)' ) &
            '<td><span class="indent">', trim(text), '</span></td>'
    else
        write( lun, '(a)' ) &
            '<td></td>'
    endif

    write( lun, '(a)' ) &
        '<td>Values at index: '
    write( lun, '(i0,a,g15.5,a,g15.5,a)' ) &
        idx, ':', &
        value1, ' -- ', value2, '</td>'
    close( lun )
end subroutine ftnunit_write_html_failed_real1d

! ftnunit_write_html_failed_real2d --
!     Auxiliary subroutine to write a failed real assertion to the HTML file
! Arguments:
!     text         Description of the test
!     idx1         Index 1
!     idx2         Index 2
!     value1       First value
!     value2       Second value
!     addtext      Add the text or not
!
subroutine ftnunit_write_html_failed_real2d( text, idx1, idx2, value1, value2, addtext )
    character(len=*)  :: text
    integer           :: idx1
    integer           :: idx2
    real              :: value1
    real              :: value2
    logical           :: addtext

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    if ( addtext ) then
        write( lun, '(a)' ) &
            '<td><span class="indent">', trim(text), '</span></td>'
    else
        write( lun, '(a)' ) &
            '<td></td>'
    endif

    write( lun, '(a)' ) &
        '<td>Values at index: '
    write( lun, '(i0,a,i0,a,g15.5,a,g15.5,a)' ) &
        idx1, ',', idx2, ':', &
        value1, ' -- ', value2, '</td>'
    close( lun )
end subroutine ftnunit_write_html_failed_real2d

! ftnunit_write_html_failed_double --
!     Auxiliary subroutine to write a failed doublde precision real assertion to the HTML file
! Arguments:
!     text         Description of the test
!     value1       First value
!     value2       Second value
!
subroutine ftnunit_write_html_failed_double( text, value1, value2 )
    character(len=*)  :: text
    real(kind=dp)     :: value1
    real(kind=dp)     :: value2

    integer           :: lun
    character(len=22) :: cvalue(2)                     ! temp storage for converted doubles
    character(len=*), parameter  :: cfmt = '(e22.15)'  ! format for dbl2str conversion

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    write( cvalue(1), cfmt) value1
    write( cvalue(2), cfmt) value2

    write( lun, '(a)' ) '<td><span class="indent">', trim(text), '</span></td>', '<td>Values: '
    write( lun, '(4a)' ) trim(adjustl(cvalue(1))), ' -- ', trim(adjustl(cvalue(2))), '</td>'
    close( lun )

end subroutine ftnunit_write_html_failed_double

! ftnunit_write_html_failed_double1d --
!     Auxiliary subroutine to write a failed double precision real assertion to the HTML file
! Arguments:
!     text         Description of the test
!     idx          Index
!     value1       First value
!     value2       Second value
!     addtext      Add the text or not
!
subroutine ftnunit_write_html_failed_double1d( text, idx, value1, value2, addtext )
    character(len=*)  :: text
    integer           :: idx
    real(kind=dp)     :: value1
    real(kind=dp)     :: value2
    logical           :: addtext

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    if ( addtext ) then
        write( lun, '(a)' ) &
            '<td><span class="indent">', trim(text), '</span></td>'
    else
        write( lun, '(a)' ) &
            '<td></td>'
    endif

    write( lun, '(a)' ) &
        '<td>Values at index: '
    write( lun, '(i0,a,g21.12,a,g21.12,a)' ) &
        idx, ':', &
        value1, ' -- ', value2, '</td>'
    close( lun )
end subroutine ftnunit_write_html_failed_double1d

! ftnunit_write_html_failed_double2d --
!     Auxiliary subroutine to write a failed double precision real assertion to the HTML file
! Arguments:
!     text         Description of the test
!     idx1         Index 1
!     idx2         Index 2
!     value1       First value
!     value2       Second value
!     addtext      Add the text or not
!
subroutine ftnunit_write_html_failed_double2d( text, idx1, idx2, value1, value2, addtext )
    character(len=*)  :: text
    integer           :: idx1
    integer           :: idx2
    real(kind=dp)     :: value1
    real(kind=dp)     :: value2
    logical           :: addtext

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    if ( addtext ) then
        write( lun, '(a)' ) &
            '<td><span class="indent">', trim(text), '</span></td>'
    else
        write( lun, '(a)' ) &
            '<td></td>'
    endif

    write( lun, '(a)' ) &
        '<td>Values at index: '
    write( lun, '(i0,a,i0,a,g21.12,a,g21.12,a)' ) &
        idx1, ',', idx2, ':', &
        value1, ' -- ', value2, '</td>'
    close( lun )
end subroutine ftnunit_write_html_failed_double2d

! ftnunit_write_html_failed_inbetween_real --
!     Auxiliary subroutine to write an out-of-range real assertion to the HTML file
! Arguments:
!     text         Description of the test
!     value        Value to be checked
!     vmin         Minimum value
!     vmax         Maximum value
!     addtext      Add the text or not
!
subroutine ftnunit_write_html_failed_inbetween_real( text, value, vmin, vmax )
    character(len=*)  :: text
    real              :: value
    real              :: vmin
    real              :: vmax

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    write( lun, '(3a)' ) &
        '<td><span class="indent">', trim(text), '</span></td>'
    write( lun, '(a,g15.5,a,g15.5,a,g15.5)' ) &
        '<td>Value: ', value, ' - expected range: ',vmin, ' -- ', vmax
    close( lun )

end subroutine ftnunit_write_html_failed_inbetween_real

! ftnunit_write_html_failed_inbetween_double --
!     Auxiliary subroutine to write an out-of-range real assertion to the HTML file
! Arguments:
!     text         Description of the test
!     value        Value to be checked
!     vmin         Minimum value
!     vmax         Maximum value
!     addtext      Add the text or not
!
subroutine ftnunit_write_html_failed_inbetween_double( text, value, vmin, vmax )
    character(len=*)  :: text
    real(kind=dp)     :: value
    real(kind=dp)     :: vmin
    real(kind=dp)     :: vmax

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    write( lun, '(3a)' ) &
        '<td><span class="indent">', trim(text), '</span></td>'
    write( lun, '(a,g21.12,a,g21.12,a,g21.12)' ) &
        '<td>Value: ', value, ' - expected range: ',vmin, ' -- ', vmax
    close( lun )

end subroutine ftnunit_write_html_failed_inbetween_double

! ftnunit_write_html_failed_inbetween_quad --
!     Auxiliary subroutine to write an out-of-range real assertion to the HTML file
! Arguments:
!     text         Description of the test
!     value        Value to be checked
!     vmin         Minimum value
!     vmax         Maximum value
!     addtext      Add the text or not
!
subroutine ftnunit_write_html_failed_inbetween_quad( text, value, vmin, vmax )
    character(len=*)  :: text
    real(kind=qd)     :: value
    real(kind=qd)     :: vmin
    real(kind=qd)     :: vmax

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    write( lun, '(3a)' ) &
        '<td><span class="indent">', trim(text), '</span></td>'
    write( lun, '(a,g21.12,a,g21.12,a,g21.12)' ) &
        '<td>Value: ', value, ' - expected range: ',vmin, ' -- ', vmax
    close( lun )

end subroutine ftnunit_write_html_failed_inbetween_quad

! ftnunit_write_html_failed_files --
!     Auxiliary subroutine to write information about file differences to the HTML file
! Arguments:
!     text         Description of the test
!     string1      First string
!     string2      Second string
!     string3      Descriptive string
!
subroutine ftnunit_write_html_failed_files( text, string1, string2, string3 )
    character(len=*) :: text
    integer          :: type
    character(len=*) :: string1
    character(len=*) :: string2
    character(len=*) :: string3

    integer           :: lun

    open( newunit=lun, file = html_file, position = 'append' )

    failed_asserts = failed_asserts + 1

    call ftnunit_write_html_close_row( lun )

    write( lun, '(a)' ) &
        '<td><span class="indent">', trim(text), '</span></td>', &
        '<td>Failure: '

    write( lun, '(3a)' ) string3, '<br>'
    write( lun, '(3a)' ) 'File 1: ', trim(string1), '<br>'
    write( lun, '(3a)' ) 'File 2: ', trim(string2), '</td>'

    close( lun )
end subroutine ftnunit_write_html_failed_files

end module ftnunit
