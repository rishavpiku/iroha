# TODO: we need to look at this script, maybe it can be useful
# https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake


# append coverage flags
set(CMAKE_CXX_FLAGS           "--coverage ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS             "--coverage ${CMAKE_C_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "--coverage ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS    "--coverage ${CMAKE_EXE_LINKER_FLAGS}")

# gcovr performs coverage analysis and saves result in build/gcovr.xml
# expects to receive array of paths to analyzed directories relative to project root
#
# specify GCOVR_BIN variable to set custom path to gcovr
# variable REPORT_DIR must be specified, otherwise default is used
if(NOT GCOVR_BIN)
  find_program(GCOVR_BIN gcovr)
endif()

if(NOT GCOVR_BIN)
  message(STATUS "Target gcovr enabled")

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(GCOV_BACKEND "llvm-cov gcov")
  else()
    set(GCOV_BACKEND "gcov")
  endif()

  add_custom_target(gcovr
      COMMAND ${GCOVR_BIN} -s -x -r '${CMAKE_SOURCE_DIR}'
      -e '${CMAKE_SOURCE_DIR}/external/*'
      -e '${CMAKE_SOURCE_DIR}/schema/*'
      -e '${CMAKE_BINARY_DIR}/*'
      --gcov-executable='${GCOV_BACKEND}'
      -o ${REPORT_DIR}/gcovr.xml
      COMMENT "Collecting coverage data with gcovr"
      )
endif()
