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

def replace_one_type(extension, header):
    for name in glob.iglob("**/*."+extension, recursive = True):
        if "ASA" in name:
            continue
        if "framework.h" in name:
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

def replace_all():
    header = read_header("license.txt")
    header_c = update_header(header, "//")
    header_f90 = update_header(header, "!")
    header_py = update_header(header, "#")

    if len(sys.argv) > 1:
        folder = sys.argv[1]
        os.chdir(folder)

    extensions = ("cpp", "h")
    for ext in extensions:
        replace_one_type(ext, header_c)

    replace_one_type("py", header_py)
    replace_one_type("f90", header_f90)

replace_all()
