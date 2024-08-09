# macro set_version_numbers
# sets: ver_git_commit_hash
#       ver_major
#       ver_minor
#       ver_patch
#
# the git hash is obtained using a git log call
# the other version numbers are read from an ini-file

macro(set_version_numbers filename)

execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE ver_git_commit_hash
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

file(READ ${filename} file_content)

string(REGEX MATCH "major *= *([0-9]*)" _ ${file_content})
set(ver_major ${CMAKE_MATCH_1})

string(REGEX MATCH "minor *= *([0-9]*)" _ ${file_content})
set(ver_minor ${CMAKE_MATCH_1})

string(REGEX MATCH "patch *= *([0-9]*)" _ ${file_content})
set(ver_patch ${CMAKE_MATCH_1})

unset(file_content)

endmacro()
