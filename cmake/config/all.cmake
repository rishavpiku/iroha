SET(CMAKE_POSITION_INDEPENDENT_CODE   TRUE)
SET(CMAKE_CXX_FLAGS     "-std=c++1y -Wall")
SET(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE   1)
SET(CMAKE_INSTALL_RPATH           "../lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS     "ON")

include(cmake/config/release.cmake)
include(cmake/config/debug.cmake)
