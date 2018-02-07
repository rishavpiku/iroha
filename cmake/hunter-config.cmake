# gtest
hunter_config(GTest
    VERSION
      1.8.0-hunter-p11   # latest hunterized version
    CMAKE_ARGS
      -Dgtest_force_shared_crt=ON
      -Dgtest_disable_pthreads=OFF
    )


# spdlog
hunter_config(spdlog
    VERSION
      0.13.0-p1          # lataest hunterized version
    CMAKE_ARGS
      -DSPDLOG_BUILD_EXAMPLES=OFF
      -DSPDLOG_BUILD_TESTS=OFF
    )

# rapidjson
hunter_config(RapidJSON
    VERSION
      1.1.0
    CMAKE_ARGS
      -DRAPIDJSON_BUILD_DOC=OFF
      -DRAPIDJSON_BUILD_EXAMPLES=OFF
      -DRAPIDJSON_BUILD_TESTS=OFF
      -DRAPIDJSON_BUILD_THIRDPARTY_GTEST=OFF
      -DRAPIDJSON_BUILD_CXX11=ON
      -DRAPIDJSON_BUILD_ASAN=OFF
      -DRAPIDJSON_BUILD_UBSAN=OFF
    )


## Protobuf
## TODO: latest releast is 3.5, we can hunterize it
#hunter_config(Protobuf
#    VERSION
#      3.3.0               # latest hunterized version
#    CMAKE_ARGS
#      -Dprotobuf_BUILD_TESTS=OFF
#      -Dprotobuf_BUILD_SHARED_LIBS=OFF
#    )


## gRPC
#hunter_config(gRPC
#    VERSION
#      1.8.1
#    CMAKE_ARGS
#      -DgRPC_PROTOBUF_PROVIDER=package
#      -DgRPC_PROTOBUF_PACKAGE_TYPE=CONFIG
#      -DProtobuf_DIR=${EP_PREFIX}/src/google_protobuf-build/lib/cmake/protobuf
#      -DgRPC_ZLIB_PROVIDER=package
#      -DBUILD_SHARED_LIBS=ON
#    )

