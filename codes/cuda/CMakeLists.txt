# CUDA learn project
cmake_minimum_required(VERSION 2.8)
project(CUDA-Learn)

option(TESTS_ENABLED "Enable tests build" ON)

# check c++11 standard
message(STATUS "CHECK C++11 compatible compiler")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=c++0x")

# General compilation options
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Ofast -Wall -Wextra ")

# GPU/cuda related stuff
find_package(CUDA 7.5 REQUIRED)
if(NOT DEFINED CUDA_TOOLKIT_SAMPLE_DIR)
  set(CUDA_TOOLKIT_SAMPLE_DIR ${CUDA_TOOLKIT_ROOT_DIR}/samples)
endif()


set(CUDA_PROPAGATE_HOST_FLAGS OFF)
message(STATUS "Building with CUDA-GPU support")
message(STATUS "CUDA_INCLUDE_DIRS=${CUDA_INCLUDE_DIRS}")
message(STATUS "CUDA_TOOLKIT_SAMPLE_DIR=${CUDA_TOOLKIT_SAMPLE_DIR}")
add_definitions(-DWITH_GPU)


#set(CUDA_NVCC_FLAGS "$ENV{CUDA_NVCC_FLAGS} -Xcompiler -fPIC -gencode arch=compute_52,code=sm_52")



#if (EXISTS "${PROJECT_SOURCE_DIR}/vendor/gflags/CMakeLists.txt")
#  add_subdirectory(vendor/gflags)
#else ()
#  find_package(gflags REQUIRED)
#endif ()

#if (EXISTS "${PROJECT_SOURCE_DIR}/vendor/googletest/CMakeLists.txt")
#  add_subdirectory(vendor/googletest)
#else ()
#  find_package(googletest REQUIRED)
#endif ()
#enable_testing()

# sources files-gpu specific
AUX_SOURCE_DIRECTORY(. PQT_CUDA_FILES)

# GPU

set(PQT_CUDA_ADDITIONAL_INC_PATH ${CUDA_INCLUDE_DIRS} ${CUDA_TOOLKIT_SAMPLE_DIR}/common/inc/)
cuda_include_directories(${CMAKE_SOURCE_DIR} ${PQT_CUDA_ADDITIONAL_INC_PATH} )
cuda_compile(PQT_LIB_COMPILED ${PQT_CUDA_FILES})

target_link_libraries(pqt ${CUDA_LIBRARIES})
set_target_properties(pqt PROPERTIES LINKER_LANGUAGE C)

add_executable(hello-cuda kernel.cu ${PQT_CUDA_FILES})
#target_link_libraries(tool_createdb pqt gtest gtest_main pthread gflags)


if(TESTS_ENABLED)
  # cuda_add_executable(verify verify.cu)
  # target_link_libraries(verify pqt gtest gtest_main  pthread gflags)

  cuda_add_executable(run run.cu)
  target_link_libraries(run pqt gtest gtest_main  pthread gflags)

endif()