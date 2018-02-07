find_package(PackageHandleStandardArgs)

include(ExternalProject)
set(EP_PREFIX "${PROJECT_SOURCE_DIR}/external")
set_directory_properties(PROPERTIES
    EP_PREFIX ${EP_PREFIX}
    )


###################################################################
##### START OF PROJECT DEPENDENCIES

find_package(Threads REQUIRED)

##########################
#         gtest          #
##########################
# testing is an option. Look at the main CMakeLists.txt for details.
if (TESTING)
  hunter_add_package(GTest)

  find_package(GTest 1.8.0 CONFIG REQUIRED)
  find_package(GMock 1.8.0 CONFIG REQUIRED)
endif ()

#############################
#         speedlog          #
#############################
hunter_add_package(spdlog)
# TODO: I can not specify version, consider creating own package
find_package(spdlog CONFIG REQUIRED)

################################
#           protobuf           #
################################
option(FIND_PROTOBUF "Try to find protobuf in system" ON)
find_package(protobuf)

#########################
#         gRPC          #
#########################
option(FIND_GRPC "Try to find gRPC in system" ON)
find_package(grpc)

################################
#          rapidjson           #
################################
hunter_add_package(RapidJSON)
find_package(RapidJSON 1.1.0 CONFIG REQUIRED)

#############################
#         optional          #
#############################
find_package(optional)

##########################
#           pq           #
##########################
find_package(pq)

##########################a
#          pqxx          #
##########################
find_package(pqxx)

################################
#            gflags            #
################################
find_package(gflags)

##########################
#        rx c++          #
##########################
find_package(rxcpp)

##########################
#          TBB           #
##########################
find_package(tbb)

##########################
#         boost          #
##########################
find_package(Boost 1.65.0 REQUIRED
    COMPONENTS
    filesystem
    system
    )
add_library(boost INTERFACE IMPORTED)
set_target_properties(boost PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS}
    INTERFACE_LINK_LIBRARIES "${Boost_LIBRARIES}"
    )

if(ENABLE_LIBS_PACKAGING)
  foreach (library ${Boost_LIBRARIES})
    add_install_step_for_lib(${library})
  endforeach(library)
endif()

##########################
#       benchmark        #
##########################
if(BENCHMARKING)
  find_package(benchmark)
endif()

###################################
#          ed25519/sha3           #
###################################
find_package(ed25519)
