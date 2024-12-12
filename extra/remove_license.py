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

"""_summary_
This script removes the header on top of source files.
So that in the next step a new/updated header can be added.
It handles all files recursive in the given folder.

"""

import glob
import sys
import os
from license_helper import is_excluded

def remove_header(filename):
    first_and_last_line = "All rights reserved."
    count = 0
    contents = []
    skip_extra = False
    with open(filename, "r") as f:
        for line in f:
            if first_and_last_line in line:
                count += 1
            elif count == 2:
                if not skip_extra:
                    skip_extra = True
                else:
                    contents.append(line)

    if count == 2:
        with open(filename, "w") as f:
            f.writelines(contents)

def replace_one_type(extension):
    for name in glob.iglob("**/*."+extension, recursive = True):
        if is_excluded(name):
            continue
        print("removing header in file: ", name)
        remove_header(name)

def replace_all(folder):
    os.chdir(folder)

    extensions = ("cpp", "h", "cs", "py", "f90")
    for ext in extensions:
        replace_one_type(ext)

def start_replace():
    print("removes (c) header from all files (recursive) in given folder")
    if len(sys.argv) == 1:
        print("missing comment line argument")
        print("usage: python remove_license.py <folder>")
    else:
        folder = sys.argv[1]
        replace_all(folder)

start_replace()
