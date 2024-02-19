module interface_gen
  integer, parameter :: ErrMsgLength = 256
  type, public, bind(c) :: tError
    character(len=1) :: message(ErrMsgLength)
    integer          :: icode
  end type tError
end module interface_gen
