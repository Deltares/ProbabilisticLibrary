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

module distributionFunctions
use precision
use interface_distributions
use interface_convert

implicit none

contains

function uniformInverse( p, distParameter1, distParameter2 )
    real(kind = wp), intent(in) :: p               !< Non-exceedance probability
    real(kind = wp), intent(in) :: distParameter1  !< x value where distribution first has a non-zero probability (a in above equation)
    real(kind = wp), intent(in) :: distParameter2  !< x value where distribution last has a non-zero probability (b in above equation)
    real(kind = wp) :: uniformInverse
    real(kind = wp) :: p2(2), u
    type(tError)    :: ierr
    p2 = [distParameter1, distParameter2]
    call BetaFromQ(1.0_wp - p, u)
    call calculateDistributionInverse(u, uniformInverse, distributionUniform, p2, ierr)
end function uniformInverse

function exponentialInverse( distParameter1, distParameter2, q )
    real(kind = wp), intent(in) :: q               !< Non-exceedance probability
    real(kind = wp), intent(in) :: distParameter1  !< x value where distribution first has a non-zero probability (a in above equation)
    real(kind = wp), intent(in) :: distParameter2  !< x value where distribution last has a non-zero probability (b in above equation)
    real(kind = wp) :: exponentialInverse
    real(kind = wp) :: p2(2), u
    type(tError)    :: ierr
    p2 = [distParameter1, distParameter2]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, exponentialInverse, distributionShiftedExponential, p2, ierr)
end function exponentialInverse

function logNormalInverse( u, mu, sig, par3 )
    real(kind = wp), intent(in) :: u, mu, sig, par3
    real(kind = wp) :: logNormalInverse
    real(kind = wp) :: p3(3)
    type(tError)    :: ierr
    p3 = [mu, sig, par3]
    call calculateDistributionInverse(u, logNormalInverse, distributionShiftedLognormal, p3, ierr)
end function logNormalInverse

function logNormalInverseII( u, M, S, par3, ierr, errorMessage )
    real(kind = wp), intent(in) :: u, M, S, par3
    real(kind = wp) :: logNormalInverseII
    integer, intent(out) :: ierr
    character(len=*), intent(inout) :: errorMessage
    call calculateDistributionInverse(u, logNormalInverseII, distributionShiftedLognormal2, M, S, par3, 0.0_wp, ierr, errorMessage)
end function logNormalInverseII

function gumbelInverse( uin, a, b, p, q )
    real(kind = wp), intent(in) :: uin, a, b, p, q
    real(kind = wp) :: gumbelInverse
    real(kind = wp) :: p4(4), u
    type(tError)    :: ierr
    p4 = [a, b, p, q]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, gumbelInverse, distributionGumbelDistribution, p4, ierr)
end function gumbelInverse

function gumbelInverseII( uin, M, S, p, q )
    real(kind = wp), intent(in) :: uin, M, S, p, q
    real(kind = wp) :: gumbelInverseII
    real(kind = wp) :: p4(4), u
    type(tError)    :: ierr
    p4 = [M, S, p, q]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, gumbelInverseII, distributionGumbelDistribution2, p4, ierr)
end function gumbelInverseII

function weibullInverse(a, b, c, q)
    real(kind = wp), intent(in) :: a, b, c, q
    real(kind = wp) :: weibullInverse
    real(kind = wp) :: p3(3),u
    type(tError)    :: ierr
    p3 = [a, b, c]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, weibullInverse, distributionWeibull, p3, ierr)
end function weibullInverse

function paretoInverse( l, s, k, q)
    real(kind = wp), intent(in) :: l, s, k, q
    real(kind = wp) :: paretoInverse
    real(kind = wp) :: p3(3), u
    type(tError)    :: ierr
    p3 = [l, s, k]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, paretoInverse, distributionPareto, p3, ierr)
end function paretoInverse

function  triangularInverse( a, b, c, p, q )
    real(kind = wp), intent(in) :: a, b, c, p, q
    real(kind = wp) :: triangularInverse
    real(kind = wp) :: p4(3), u
    type(tError)    :: ierr
    p4 = [a,b, c]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, triangularInverse, distributionTriangular, p4, ierr)
end function triangularInverse

function conditionalWeibullInverse(distParameter1, distParameter2, distParameter3, distParameter4, p, q)
    real(kind = wp), intent(in) :: distParameter1, distParameter2, distParameter3, distParameter4, p, q
    real(kind = wp) :: conditionalWeibullInverse
    real(kind = wp) :: p4(4), u
    type(tError)    :: ierr
    p4 = [distParameter1, distParameter2, distParameter3, distParameter4]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, conditionalWeibullInverse, distributionConditionalWeibull, p4, ierr)
end function conditionalWeibullInverse

function modifiedGumbelInverse(uin, a, b, c, p, q)
    real(kind = wp), intent(in) :: uin, a, b, c, p, q
    real(kind = wp) :: modifiedGumbelInverse
    real(kind = wp) :: p3(3), u
    type(tError)    :: ierr
    p3 = [a, b, c]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, modifiedGumbelInverse, distributionModifiedGumbel, p3, ierr)
end function modifiedGumbelInverse

function rayleighNInverse(sigma, N, distParameter3, q)
    real(kind = wp), intent(in) :: sigma, N, distParameter3, q
    real(kind = wp) :: rayleighNInverse
    real(kind = wp) :: p3(3), u
    type(tError)    :: ierr
    p3 = [sigma, N, distParameter3]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, rayleighNInverse, distributionRayleighN, p3, ierr)
end function rayleighNInverse

function rayleighInverse( distParameter1, distParameter2, distParameter3, q )
    real(kind = wp), intent(in) :: distParameter1, distParameter2, distParameter3, q
    real(kind = wp) :: rayleighInverse
    real(kind = wp) :: p3(3), u
    type(tError)    :: ierr
    p3 = [distParameter1, distParameter2, distParameter3]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, rayleighInverse, distributionRayleigh, p3, ierr)
end function rayleighInverse

function betaInverse( distParameter1, distParameter2, distParameter3, distParameter4, p, q)
    real(kind = wp), intent(in) :: distParameter1, distParameter2, distParameter3, distParameter4, p, q
    real(kind = wp) :: betaInverse
    real(kind = wp) :: p4(4), u
    type(tError)    :: ierr
    p4 = [distParameter1, distParameter2, distParameter3, distParameter4]
    call betaFromQ(q, u)
    call calculateDistributionInverse(u, betaInverse, distributionBeta, p4, ierr)
end function betaInverse

end module distributionFunctions

