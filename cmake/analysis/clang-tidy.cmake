if(NOT CLANG_TIDY_BIN)
  find_program(CLANG_TIDY_BIN "clang-tidy")
endif()

if(CLANG_TIDY_BIN)
  set(CMAKE_CXX_CLANG_TIDY
      ${CLANG_TIDY_BIN}

      CACHE STRING ""
      )
  message(STATUS "clang-tidy enabled")
endif()
