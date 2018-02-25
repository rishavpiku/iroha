if(NOT CLANG_TIDY_BIN)
  find_program(CLANG_TIDY_BIN "clang-tidy")
endif()

if(CLANG_TIDY_BIN)
  set(TIDY_CHECKS
      -*                    # disable all checks
      cppcoreguidelines-*   # enable these
      boost-*
      google-*
      modernize-*
      performance-*

      CACHE STRING "Clang tidy checks"
      )
  JOIN("${TIDY_CHECKS}" "," "TIDY_CHECKS_JOINED")
  set(CMAKE_CXX_CLANG_TIDY
      ${CLANG_TIDY_BIN};
      -checks=${TIDY_CHECKS_JOINED}

      CACHE STRING ""
      )
  message(STATUS "clang-tidy enabled")
endif()
