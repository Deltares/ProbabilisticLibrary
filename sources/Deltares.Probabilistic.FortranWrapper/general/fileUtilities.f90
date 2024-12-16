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
!! Module for reading information from files
!<
!
module fileUtilities

    use precision
    use utilities
    use errorMessages

    implicit none

    private
    public :: readTable, removeFile, copy_File, writeTableValues
    public :: tMessage

contains

subroutine readTable( filename, nColumns, table )
    character(len=*), intent(in)               :: filename     !< Name of the file
    integer,          intent(in)               :: nColumns     !< Number of colums
    real(kind=wp), dimension(:,:), allocatable :: table        !< Table with contents of file

    integer                                 :: nRecords
    integer                                 :: i
    integer                                 :: j
    integer                                 :: ierr
    integer                                 :: inputFileID

    open( newunit=inputFileID, file = trim(filename) )
    nRecords = 0
    do
        read( inputFileID, *, iostat = ierr )
        if ( ierr /= 0 ) then
            exit
        endif
        nRecords = nRecords + 1
    enddo
    close( inputFileID )

    allocate( table(nRecords,nColumns) )

    open( inputFileID, file = trim(filename) )
    read( inputFileID, * ) ( ( table(i,j), j=1,nColumns ), i=1,nRecords )
    close( inputFileID )

end subroutine readTable

subroutine writeTableValues( filename, table, error )
    character(len=*), intent(in)    :: filename     !< Name of the file to write
    real(kind=wp),    intent(in)    :: table(:,:)   !< Table with contents to write in file
    type(tMessage),   intent(out)   :: error        !< error code/message

    integer                      :: nRecords
    integer                      :: nColumns
    integer                      :: i
    integer                      :: j
    integer                      :: ierr
    integer                      :: outputFileID
    character(len=20)            :: stringFormat

    nRecords = size(table,1)
    nColumns = size(table,2)
    open( newunit=outputFileID, file = trim(filename), iostat = ierr )
    if ( ierr /= 0 ) then
        error%errorCode = ierr
        error%message   = "Impossible to open file: " // filename
    else
        write( outputFileID, * ) ( ( table(i,j), j=1,nColumns ), i=1,nRecords )
        close( outputFileID )
    endif

end subroutine writeTableValues

!> Routine to throw away a file
subroutine removeFile(filename)
    character(len=*), intent(in)            :: filename             !< file to delete
    logical                                 :: file_exists          !< boolean for check on file exists
    integer                                 :: fileId               !< free file number to use for file I/O
    integer                                 :: ierr                 !< error code of open

    ! make sure file does not exist
    inquire(file=filename, exist=file_exists)    ! file_exists will be TRUE if the file exists
    if (file_exists) then
        open (newunit=fileId, file=filename, iostat = ierr, status='old', action='write')   ! Current directory
        if (ierr == 0) close (fileId, status="delete")
    endif
end subroutine removeFile

! copy one file
subroutine copy_File (file_name, file_name_new, error)
    ! copies a file file_name to file_name_new
    ! file_name and file_name_new must include the path information and may include wildcard characters

    character(len=*), intent(in)    :: file_name     !< filename to copy from
    character(len=*), intent(in)    :: file_name_new !< filename to copy to
    type(tMessage),   intent(out)   :: error         !< error code/message

    character(len=:), allocatable :: command           !< string with copy command to execute
    integer                       :: iresult           !< result from system call to copy a file
    logical                       :: fExists           !< input file exists
    logical                       :: has_spaces        !< (at least) one of the filename has spaces

    error = errorNone

    inquire ( file=file_name, exist=fExists )
    if (.not. fExists) then
        error%errorCode = -1
        error%message   = trim(file_name) // ' not found.'
        return
    endif

    has_spaces = (index(trim(file_name), ' ') > 0 .or. index(trim(file_name_new), ' ') > 0)
    if (has_spaces) then
        command = 'copy/y "' // trim(file_name) // '" "' // trim(file_name_new) // '"'
    else
        command = 'copy/y ' // trim(file_name) // ' ' // trim(file_name_new)
    endif

    command = command // ' > nul 2>&1'

    call system( command, iresult )

    if (iresult /= 0) then
        error%errorCode = -2
        error%message   = 'Copying of ' // trim(file_name) // ' failed '
    endif

end subroutine copy_File

end module fileUtilities

