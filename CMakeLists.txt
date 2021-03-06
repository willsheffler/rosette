cmake_minimum_required(VERSION 3.0)
project(rosette)

# message("============== CMAKE ===========================================")

function(main)
    # message("-------------- CMAKE main --------------------------------------")
    print_config()
    add_subdirectory(external/pybind11)
    # execute_process(COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/util/_rosette.gen.cpp)
    build_gtests()
    build_python_bindings("")
    # summary()
endfunction()


if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    message("CMAKE_LIBRARY_OUTPUT_DIRECTORY not defined, setting to .")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY .)
endif()

if(DEFINED BOOST_ROOT)
    message("cmake: using BOOST_ROOT " ${BOOST_ROOT})
    include_directories(${BOOST_ROOT}/include)
    link_directories(${BOOST_ROOT}/lib)
else()
    message("cmake: NOT using BOOST_ROOT")
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message("cmake: using ccache ${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
else()
    message("cmake: not using ccache")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXXFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCXX11 -DCXX14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} $ENV{LDFLAGS}")

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math") # eigen xform perf worse!!
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

include_directories("external/include")
include_directories("src/rosette")
include_directories("src")
include_directories("/usr/local/include")
include_directories("/opt/local/include")
link_directories("/usr/local/lib")
#link_directories("/opt/local/lib")
# list(APPEND extra_libs boost_system)
# list(APPEND extra_libs boost_iostreams)

function(print_config)
    # message("-------------- CMAKE print_config -------------------------------------------------")
    message("cmake: CC:  " $ENV{CC})
    message("cmake: CXX: " $ENV{CXX})
    message("cmake: CXXFLAGS: " $ENV{CXXFLAGS})
    message("cmake: CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
    message("cmake: CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
    message("cmake: CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
    message("cmake: extra_libs: " ${extra_libs})
    message("cmake: CMAKE_OPTIONS: " $ENV{CMAKE_OPTIONS})
    message("cmake: BOOST_ROOT: " $ENV{BOOST_ROOT})
    message("PYTHON_EXECUTABLE: " ${PYTHON_EXECUTABLE})
endfunction()

function(copy_py_to_liboutdir)
    # message("-------------- CMAKE copy_py_to_liboutdir ----------")
    execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/tools/cp_filtered.py
        ${CMAKE_SOURCE_DIR}/src/rosette ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/rosette .py .gz .dat .csv .txt .pdb .pdf .pkl .h5
        RESULT_VARIABLE STATUS)
    if(NOT "${STATUS}" STREQUAL "0")
        message(FATAL_ERROR "cp_filtered.py failed: ${STATUS}")
    endif()
endfunction()

function(build_python_bindings)
    # message("-------------- CMAKE build_python_bindings ----------------------------------------")
    set(SRCGEN ${CMAKE_SOURCE_DIR}/tools/pybind_source_gen.py)
    FILE(GLOB_RECURSE PY_CPP "./src/rosette/[0-9a-zA-Z_]*.pybind.cpp")
    copy_py_to_liboutdir()
    # execute_process(COMMAND python ${SRCGEN} ${CMAKE_SOURCE_DIR}/src/rosette ${CMAKE_CURRENT_BINARY_DIR} )
    # if(NOT "${STATUS}" STREQUAL "0")
        # message(FATAL_ERROR "${SRCGEN} failed: ${STATUS}")
    # endif()
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/_rosette.gen.cpp
        COMMAND python ${SRCGEN} ${CMAKE_SOURCE_DIR}/src/rosette ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS src/rosette/util/_rosette.gen.cpp.jinja ${PY_CPP} ${SRCGEN})
    pybind11_add_module(_rosette ${CMAKE_CURRENT_BINARY_DIR}/_rosette.gen.cpp ${PY_CPP} ${SRCGEN} NO_EXTRAS)
    target_link_libraries( _rosette PRIVATE ${extra_libs} )

endfunction()


function(build_gtests)
    # message("-------------- CMAKE build_gtests -------------------------------------------------")
    add_subdirectory(external/googletest)
    include_directories("external/googletest/include")
    list(APPEND extra_libs gtest)

    file(GLOB_RECURSE TEST_CPP "src/rosette/[0-9a-zA-Z_]*.gtest.cpp")
    add_executable(gtest_all src/test/gtest_all.cpp ${TEST_CPP})
    target_link_libraries(gtest_all ${extra_libs})

    if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/gtest_auto.gen.cpp)
        message("!!!!! creating gtest_auto file")
        execute_process(COMMAND cp ${CMAKE_SOURCE_DIR}/tools/empty.cpp.tplt ${CMAKE_CURRENT_BINARY_DIR}/gtest_auto.gen.cpp)
    endif()
    add_executable(gtest_auto ${CMAKE_CURRENT_BINARY_DIR}/gtest_auto.gen.cpp)
    target_link_libraries(gtest_auto ${extra_libs})
endfunction()

function(summary)
    # message("-------------- CMAKE summary ------------------------------------------------------")
    if(CCACHE_PROGRAM)
        message("cmake: add_custom_target: ccache -s")
        add_custom_target(ccache_stats ALL
            COMMAND ${CCACHE_PROGRAM} -s
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "!!!!!!!!!!!!!!!!!!!! getting ccache stats !!!!!!!!!!!!!!!!!!!!!!!!!!"
            DEPENDS _rosette gtest_all
        )
    endif()
endfunction()

main()


# message("============== CMAKE DONE =========================================================")
