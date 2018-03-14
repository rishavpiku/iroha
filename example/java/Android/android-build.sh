#!/bin/sh
PLATFORM=$1
NDK_PATH=$2
PACKAGE=$3
if [ "$#" -ne 3 ]; then
    echo "Illegal number of parameters"
    echo "Usage: ./android-build.sh <PLATFORM> <NDK_PATH> <PACKAGE>"
    echo "Example: ./android-build.sh arm64-v8a /Users/me/Downloads/android-ndk-r16b jp.co.soramitsu.yuna.bindings"
    exit 1
fi
ANDROID_TOOLCHAIN_ARGS="-DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=26 -DCMAKE_ANDROID_ARCH_ABI=${PLATFORM} -DANDROID_NDK=${NDK_PATH} -DCMAKE_ANDROID_STL_TYPE=c++_static"
DEPS_DIR="$(pwd)/iroha/dependencies"
INSTALL_ARGS="-DCMAKE_INSTALL_PREFIX=${DEPS_DIR}"

# iroha develop
git clone -b develop --depth=1 https://github.com/hyperledger/iroha
mkdir ${DEPS_DIR}
mkdir ${DEPS_DIR}/lib
mkdir ${DEPS_DIR}/include

# boost
curl -OL https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
tar xf ./boost_1_66_0.tar.gz
cp -R ./boost_1_66_0/boost ${DEPS_DIR}/include

# protobuf
git clone https://github.com/google/protobuf
(cd ./protobuf ; git checkout b5fbb742af122b565925987e65c08957739976a7)
cmake -Dprotobuf_BUILD_TESTS=OFF -H./protobuf/cmake -B./protobuf/host_build # build for host to get js_embed
cmake --build ./protobuf/host_build
sed -i.bak "s~COMMAND js_embed~COMMAND $(pwd)/protobuf/host_build/js_embed~" ./protobuf/cmake/libprotoc.cmake
LDFLAGS="-llog -landroid" cmake ${ANDROID_TOOLCHAIN_ARGS} ${INSTALL_ARGS} -Dprotobuf_BUILD_TESTS=OFF -H./protobuf/cmake -B./protobuf/.build
cmake --build ./protobuf/.build --target install

# ed25519
git clone git://github.com/hyperledger/iroha-ed25519
(cd ./iroha-ed25519 ; git checkout e7188b8393dbe5ac54378610d53630bd4a180038)
cmake ${ANDROID_TOOLCHAIN_ARGS} ${INSTALL_ARGS} -DTESTING=OFF -DBUILD=STATIC -H./iroha-ed25519 -B./iroha-ed25519/build
cmake --build ./iroha-ed25519/build --target install
mv ${DEPS_DIR}/lib/static/libed25519.a ${DEPS_DIR}/lib; rmdir ${DEPS_DIR}/lib/static/

# nonstd-optional
git clone https://github.com/martinmoene/optional-lite
(cd ./optional-lite ; git checkout a0ddabb8b52e1eaaf0dd1515bb85698b747528e4)
cp -R ./optional-lite/include/nonstd ${DEPS_DIR}/include

# SWIG fixes
sed -i.bak "s~find_package(JNI REQUIRED)~#find_package(JNI REQUIRED)~" ./iroha/shared_model/bindings/CMakeLists.txt
sed -i.bak "s~include_directories(${JAVA_INCLUDE_PATH})~#include_directories(${JAVA_INCLUDE_PATH})~" ./iroha/shared_model/bindings/CMakeLists.txt
sed -i.bak "s~include_directories(${JAVA_INCLUDE_PATH2})~#include_directories(${JAVA_INCLUDE_PATH2})~" ./iroha/shared_model/bindings/CMakeLists.txt
sed -i.bak "s~# the include path to jni.h~SET(CMAKE_SWIG_FLAGS \${CMAKE_SWIG_FLAGS} -package ${PACKAGE})~" ./iroha/shared_model/bindings/CMakeLists.txt
sed -i.bak "s~swig_link_libraries(irohajava~swig_link_libraries(irohajava $(pwd)/protobuf/.build/libprotobufd.a ${NDK_PATH}/platforms/android-26/arch-arm64/usr/lib/liblog.so~" ./iroha/shared_model/bindings/CMakeLists.txt

# build iroha
sed -i.bak "s~find_program(protoc_EXECUTABLE protoc~set(protoc_EXECUTABLE $(pwd)/protobuf/host_build/protoc~" ./iroha/cmake/Modules/Findprotobuf.cmake # use host protoc
(cd ./iroha ; git apply ../cmake.patch)
cmake -H./iroha -B./iroha/build ${ANDROID_TOOLCHAIN_ARGS} -DTESTING=OFF -DSHARED_MODEL_DISABLE_COMPATIBILITY=ON -DSWIG_JAVA=ON -DCMAKE_PREFIX_PATH=${DEPS_DIR}
cmake --build ./iroha/build --target irohajava

# copy artifacts
mkdir lib
zip ./lib/bindings.zip ./iroha/build/shared_model/bindings/*.java
cp ./iroha/build/shared_model/bindings/libirohajava.so ./lib
