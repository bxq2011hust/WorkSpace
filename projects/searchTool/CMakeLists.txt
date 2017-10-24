cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(searchTool)

# Compiler-specific C++11 activation.
set (CMAKE_CXX_STANDARD 11)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
#SET (CMAKE_CXX_COMPILER "g++")
SET (CMAKE_BUILD_TYPE "Release")
#SET (CMAKE_BUILD_TYPE "Debug")
#SET (CMAKE_CXX_FLAGS "-DSINGLE_CORE -O3 -fopenmp -std=c++11")

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wall -fopenmp -std=c++11 -Wreorder -Wno-sign-compare -Wno-reorder")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# bin directory
#SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# source code 
SET (HASH_SCAN_SRC lib/hash_scan/MIH.cpp lib/hash_scan/linscan.cpp lib/hash_scan/mihasher.cpp lib/hash_scan/sparse_hashtable.cpp lib/hash_scan/bucket_group.cpp lib/hash_scan/array32.cpp )
SET (HASH_SCAN_DIR lib/hash_scan)
SET (HIREDIS_SRC lib/hiredis/read.c lib/hiredis/sds.c lib/hiredis/net.c lib/hiredis/dict.c lib/hiredis/hiredis.c)
SET (HIREDIS_DIR lib/hiredis)
#AUX_SOURCE_DIRECTORY(${HIREDIS_DIR} HIREDIS_SRC)

INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/lib" ${HASH_SCAN_DIR} ${HIREDIS_DIR})

# library config
ADD_LIBRARY(searchtool SHARED src/searchTool.cpp src/redisManager.cpp ${HASH_SCAN_SRC} ${HIREDIS_SRC})
target_link_libraries(searchtool boost_serialization)
#ADD_LIBRARY(searchtool_static STATIC src/searchTool.cpp src/redisManager.cpp ${HASH_SCAN_SRC} ${HIREDIS_SRC})

# config version soversion
#SET_TARGET_PROPERTIES(searchtool PROPERTIES VERSION 0.2 SOVERSION 2)
#SET_TARGET_PROPERTIES(searchtool_static PROPERTIES OUTPUT_NAME "searchtool")
#SET_TARGET_PROPERTIES(searchtool PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#SET_TARGET_PROPERTIES(searchtool_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# test binary
ADD_EXECUTABLE(testHiRedis ${PROJECT_SOURCE_DIR}/test/test-hiredis.cpp)
TARGET_LINK_LIBRARIES(testHiRedis  ${PROJECT_BINARY_DIR}/libsearchtool.so)  
ADD_EXECUTABLE(testRedisManager ${PROJECT_SOURCE_DIR}/test/test-redisManage.cpp)
TARGET_LINK_LIBRARIES(testRedisManager  ${PROJECT_BINARY_DIR}/libsearchtool.so)  
ADD_EXECUTABLE(testSearchTool ${PROJECT_SOURCE_DIR}/test/searchTool_test.cpp)
TARGET_LINK_LIBRARIES(testSearchTool ${PROJECT_BINARY_DIR}/libsearchtool.so)  

# tools 
ADD_EXECUTABLE(addCodesToRedis ${PROJECT_SOURCE_DIR}/tools/add_bvecs.cpp)
TARGET_LINK_LIBRARIES(addCodesToRedis searchtool)  

