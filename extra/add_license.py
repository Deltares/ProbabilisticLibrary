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
