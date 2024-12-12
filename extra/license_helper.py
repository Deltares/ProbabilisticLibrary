# Copyright (C) Stichting Deltares. All rights reserved.
#
# This file is part of the Probabilistic Library.
#
# The Probabilistic Library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# All names, logos, and references to "Deltares" are registered trademarks of
# Stichting Deltares and remain full property of Stichting Deltares at all times.
# All rights reserved.
#

import os

# function for both add_license and remove_license

def is_excluded(filename):
    # list of files that come from a third party
    excluded_files_third_party = [os.sep+"Cobyla.h", os.sep+"Cobyla.cpp", "stop.cpp", "timer.cpp", "rescale.cpp", "nlopt", "ASA", "framework.h"]

    # list of files that are not handled correctly by this script
    # (and must be updated by hand)
    excluded_files_other = ["TestDistributions.cs"]

    all_excluded_files = excluded_files_third_party + excluded_files_other
    for excluded in all_excluded_files:
        if  excluded in filename:
            return True
    return False

