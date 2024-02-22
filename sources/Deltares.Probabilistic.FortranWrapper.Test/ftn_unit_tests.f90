program ftn_unit_tests
    use ftnunit
    use testTools
    use ftn_interface_tests

    integer, parameter :: testLevel = 1

    call prepareTests('unitTests')
    call setTestTitle('Fortran-C++ ProbLib interface tests')

    call setRunTestLevel(testLevel)

    call runtests_init

    call testWithLevel(run_all_ftn_interface_tests, "run all Fortran interface tests", testLevel)

    call runtests_final(.true.)

end program ftn_unit_tests

