cmake_minimum_required(VERSION 3.5.1)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
SET(HUNTER_STATUS_PRINT ON CACHE BOOL "Verbose info")
SET(HUNTER_STATUS_DEBUG ON CACHE BOOL "Debug info")

# hunter binary cache server
set(
    HUNTER_CACHE_SERVERS
    "https://github.com/soramitsu/hunter-binary-cache;https://github.com/elucideye/hunter-cache;https://github.com/ingenue/hunter-cache"
    CACHE
    STRING
    "Hunter cache servers"
)

# enable this option to use binary cache
option(HUNTER_RUN_UPLOAD "Upload cache binaries" OFF)

set(
    HUNTER_PASSWORDS_PATH
    "${CMAKE_SOURCE_DIR}/cmake/hunter-binary-cache.cmake"
    CACHE
    FILEPATH
    "Hunter passwords"
)

include(HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.20.0.tar.gz"
    SHA1 "e94556ed41e5432997450bca7232db72a3b0d5ef"
    FILEPATH ${CMAKE_SOURCE_DIR}/cmake/hunter-config.cmake
)

PROJECT(
    iroha
    LANGUAGES C CXX
)


option(BENCHMARKING "Build benchmarks"               OFF)
option(TESTING      "Build tests"                    ON )
option(COVERAGE     "Enable coverage"                OFF)
option(FUZZING      "Build fuzzing binaries"         OFF)
option(PACKAGE_ZIP  "Create ZIP package"             OFF)
option(PACKAGE_TGZ  "Create TGZ package"             OFF)
option(PACKAGE_RPM  "Create RPM package"             OFF)
option(PACKAGE_DEB  "Create DEB package"             OFF)
option(ENABLE_LIBS_PACKAGING "Enable libs packaging" ON)
option(SWIG_PYTHON  "Generate Swig Python bindings"  OFF)
option(SWIG_JAVA    "Generate Swig Java bindings"    OFF)
option(SWIG_CSHARP  "Generate Swig C# bindings"      OFF)
option(SHARED_MODEL_DISABLE_COMPATIBILITY "Disable backward compatibility in shared model" OFF)

if (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
  message(STATUS "Default build type is Debug")
endif()

if(PACKAGE_TGZ OR PACKAGE_ZIP OR PACKAGE_RPM OR PACKAGE_DEB)
  message(STATUS "Packaging enabled: forcing non-packaging options to OFF")
  set(BENCHMARKING OFF)
  set(TESTING OFF)
  set(COVERAGE OFF)
  set(FUZZING OFF)
  set(SWIG_PYTHON OFF)
  set(SWIG_JAVA OFF)
endif()

message(STATUS "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "-DTESTING=${TESTING}")
message(STATUS "-DBENCHMARKING=${BENCHMARKING}")
message(STATUS "-DFUZZING=${FUZZING}")
message(STATUS "-DCOVERAGE=${COVERAGE}")
message(STATUS "-DPACKAGE_ZIP=${PACKAGE_ZIP}")
message(STATUS "-DPACKAGE_TGZ=${PACKAGE_TGZ}")
message(STATUS "-DPACKAGE_RPM=${PACKAGE_RPM}")
message(STATUS "-DPACKAGE_DEB=${PACKAGE_DEB}")
message(STATUS "-DENABLE_LIBS_PACKAGING=${ENABLE_LIBS_PACKAGING}")
message(STATUS "-DSWIG_PYTHON=${SWIG_PYTHON}")
message(STATUS "-DSWIG_JAVA=${SWIG_JAVA}")
message(STATUS "-DSWIG_CSHARP=${SWIG_CSHARP}")
message(STATUS "-DSHARED_MODEL_DISABLE_COMPATIBILITY=${SHARED_MODEL_DISABLE_COMPATIBILITY}")

SET(IROHA_SCHEMA_DIR "${PROJECT_SOURCE_DIR}/schema")
include_directories(
  ${PROJECT_SOURCE_DIR}/irohad
  ${PROJECT_SOURCE_DIR}/shared_model
  ${PROJECT_SOURCE_DIR}/libs
  ${IROHA_SCHEMA_DIR}
)

# must be here
include(cmake/setup.cmake)

if(PACKAGE_TGZ OR PACKAGE_ZIP OR PACKAGE_RPM OR PACKAGE_DEB)
  include(cmake/release.cmake)
endif()

if(SHARED_MODEL_DISABLE_COMPATIBILITY)
    add_definitions(-DDISABLE_BACKWARD)
endif()

add_subdirectory(schema)
add_subdirectory(libs)
add_subdirectory(irohad)
add_subdirectory(iroha-cli)
add_subdirectory(shared_model)

if(TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

