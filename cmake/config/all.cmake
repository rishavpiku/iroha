SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
SET(CMAKE_POSITION_INDEPENDENT_CODE    TRUE)  # -fPIC
SET(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)     # https://stackoverflow.com/a/40110752/1953079
SET(CMAKE_INSTALL_RPATH                "../lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS      "ON")  # creates compile_database.json

# list of configurations
include(cmake/config/release.cmake)
include(cmake/config/debug.cmake)
