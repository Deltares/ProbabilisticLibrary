enable_language(C)
include(FortranCInterface)
FortranCInterface_VERIFY(CXX)

add_library(pb_wrapper_c STATIC "")
target_sources(pb_wrapper_c
  PRIVATE
    stringHelper.h
    stringHelper.cpp
    probCalcF2C.cpp
    randomgen.cpp
    fortranZfunc.h
    fortranZfunc.cpp
    calcdistributionsinv.cpp
    convert.cpp
    combin.cpp
    correlation.cpp
    special_funcs.cpp
  )

if ( TARGET tools )
else()
    if(WIN32)
        add_library(pb_wrapper_dll SHARED
            stringHelper.h
            stringHelper.cpp
            probCalcF2C.cpp
            randomgen.cpp
            fortranZfunc.h
            fortranZfunc.cpp
            distributions.cpp
            convert.cpp
            combin.cpp
            correlation.cpp
            special_funcs.cpp
        )
        target_link_libraries(pb_wrapper_dll probLib)
        target_link_options(pb_wrapper_dll PRIVATE "$<$<CONFIG:Debug>:/Debug>")

    endif(WIN32)
endif()

