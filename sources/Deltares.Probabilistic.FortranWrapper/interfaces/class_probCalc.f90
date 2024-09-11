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
module class_probCalc
use interface_probCalc

implicit none

private
public :: tProbCalc

type tProbCalc
    type(TCpData) :: cpData
contains
    procedure :: run => wrapperProbCalc
end type tProbCalc

contains

subroutine wrapperProbCalc(this, probDb, fx, alfaN, beta, x, conv, convCriterium, convergenceData, pc)
    class(tProbCalc), intent(inout)                   :: this
    type(probabilisticDataStructure_data), intent(in) :: probDb           !< Probabilistic data module
    procedure(zfunc)                                  :: fx               !< Function implementing the z-function of the failure mechanism
    real(kind=wp), intent(out)                        :: alfaN(:)         !< Alpha values
    real(kind=wp), intent(out)                        :: beta             !< Reliability index
    real(kind=wp), intent(inout)                      :: x(:)             !< X values of design point
    logical,       intent(out)                        :: conv             !< Convergence indicator
    logical,       intent(out)                        :: convCriterium    !< Convergence criterium indicator
    type(storedConvergenceData), intent(inout)        :: convergenceData  !< struct holding all convergence data
    procedure(progressCancel),    optional            :: pc               !< progress function

    call calculateLimitStateFunction(probDb, fx, alfaN, beta, x, conv, convCriterium, convergenceData, this%cpData, pc)

end subroutine wrapperProbCalc

end module class_probCalc
