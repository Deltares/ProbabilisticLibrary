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
This script adds a header on top of source files.
The license text is "license.txt" in the current directory.
It handles all files recursive in the given folder.

It works for c++, c#, python and Fortran
"""

import glob
import sys
import os

def read_header(filename):
    with open(filename, "r") as f:
        contents = f.readlines()
    return contents

def check_already_with_header(filename):
    with open(filename, "r") as f:
        contents = f.readlines()

    for line in contents:
        if "Copyright (C) Stichting Deltares" in line:
            return True

    return False

def add_header(filename, header):
    with open(filename, "r") as f:
        contents = f.readlines()

    for h in reversed(header):
        contents.insert(0, h)

    with open(filename, "w") as f:
        f.writelines(contents)

def is_excluded(filename):
    excluded_files = ["Cobyla.h", "Cobyla.cpp", "stop.cpp", "timer.cpp", "rescale.cpp", "nlopt", "ASA", "framework.h", "TestDistributions.cs"]
    for excluded in excluded_files:
        if filename in excluded:
            return True
    return False

def replace_one_type(extension, header):
    for name in glob.iglob("**/*."+extension, recursive = True):
        if is_excluded(name):
            continue
        print("add header to file: ", name)
        if not check_already_with_header(name):
            add_header(name, header)
        else:
            print("already with header: ", name)

def update_header(header, comment_char):
    new_header = []
    for line in header:
        if line.isspace() > 0:
            with_comment = comment_char + line
        else:
            with_comment = comment_char + " " + line
        new_header.append(with_comment.strip(" "))

    return new_header

def replace_all(folder):
    header = read_header("license.txt")
    header_c = update_header(header, "//")
    header_f90 = update_header(header, "!")
    header_py = update_header(header, "#")

    os.chdir(folder)

    extensions = ("cpp", "h", "cs")
    for ext in extensions:
        replace_one_type(ext, header_c)

    replace_one_type("py", header_py)
    replace_one_type("f90", header_f90)

def start_replace():
    print("add (c) header to all files (recursive) in given folder")
    print("template header is in current folder")
    if len(sys.argv) == 1:
        print("missing comment line argument")
        print("usage: python add_license.py <folder>")
    else:
        folder = sys.argv[1]
        replace_all(folder)

start_replace()
