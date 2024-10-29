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
module interface_enums

  integer, parameter :: stochastNotActive                             = -1
  integer, parameter :: stochastDeterminist                           =  0
  integer, parameter :: stochastActive                                =  1

  integer, parameter :: methodFORM                                       =  1
  integer, parameter :: methodCrudeMonteCarlo                            =  3
  integer, parameter :: methodDirectionalSampling                        =  4
  integer, parameter :: methodNumericalIntegration                       =  5
  integer, parameter :: methodImportanceSampling                         =  6
  integer, parameter :: methodAdaptiveImportanceSampling                 =  7
  integer, parameter :: methodFORMandDirSampling                         = 11
  integer, parameter :: methodDirSamplingWithFORMiterations              = 12
  integer, parameter :: methodCrudeMonteCarloWithFORMiterations          = 13
  integer, parameter :: methodImportanceSamplingWithFORMiterations       = 14
  integer, parameter :: methodFORMandDirSamplingWithFORMiterations       = 15
  integer, parameter :: methodDirSamplingWithFORMiterationsStartU        = 16
  integer, parameter :: methodFORMandDirSamplingWithFORMiterationsStartU = 17
  integer, parameter :: methodLatinHyperCube                             = 18
  integer, parameter :: methodNumericalBisection                         = 19
  integer, parameter :: methodCobylaReliability                          = 20
  integer, parameter :: methodFORMstart                                  = 1234

  integer, parameter :: GeorgeMarsaglia = 1
  integer, parameter :: MersenneTwister = 2

  integer, parameter :: designPointNone                               =  0
  integer, parameter :: designPointXfromU                             =  1
  integer, parameter :: designPointXCorrelatedFromU                   =  2
  integer, parameter :: designPointRMin                               =  3
  integer, parameter :: designPointRMinZFunc                          =  4
  integer, parameter :: designPointXCorrelatedFromUCompatible         =  5
  integer, parameter :: designPointRMinZFuncCompatible                =  6
  integer, parameter :: designPointMultiplyInvCholesky                =  7
  integer, parameter :: designPointCenterOfGravity                    =  8
  integer, parameter :: designPointCenterOfAngles                     =  9

  integer, parameter :: fORMStartZero                                 =  1
  integer, parameter :: fORMStartOne                                  =  2
  integer, parameter :: fORMStartGivenVector                          =  3
  integer, parameter :: fORMStartRaySearch                            =  4
  integer, parameter :: fORMStartSphereSearch                         =  5
  integer, parameter :: fORMStartRaySearchNewDeprecated               =  6
  integer, parameter :: fORMStartRaySearchWind                        =  7
  integer, parameter :: fORMStartRaySearchVector                      =  8
  integer, parameter :: fORMStartRaySearchVectorScaled                =  9
  integer, parameter :: fORMStartSphereSearchKW                       =  10
  integer, parameter :: fORMStartSensitivity                          =  11

  integer, parameter :: DirSamplingIterMethodRobust                   =  1

  integer, parameter :: designPointOutputFALSE                        =  0
  integer, parameter :: designPointOutputTRUE                         =  1
  integer, parameter :: designPointOutputPrintAll                     =  3

end module interface_enums
