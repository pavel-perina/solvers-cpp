cmake_minimum_required(VERSION 3.20)
project(optim)

# Include FetchContent functionality to get repos from git and set it to verbose
# This simplifies manual downloading and setting paths to GIT_REPOSITORY

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

##########################################
# Threads are found automagically
find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

################################################################
# Fetch libraries

#FetchContent_Declare(gflags
#    GIT_REPOSITORY https://github.com/gflags/gflags.git
#    GIT_TAG v2.2.2
#    GIT_PROGRESS TRUE
#)
set(WITH_GFLAGS FALSE)
set(WITH_GMOCK FALSE)
set(BUILD_EXAMPLES FALSE)
set(MINIGLOG TRUE)

#FetchContent_Declare(google-glog
#    GIT_REPOSITORY https://github.com/google/glog.git
#    GIT_TAG v0.7.0
##    GIT_PROGRESS TRUE
#)
#FetchContent_GetProperties(glog)
#if (NOT glog_POPULATED)
#  FetchContent_Populate(glog)
#  set(WITH_GFLAGS OFF CACHE BOOL "we don't use gflags")
#  add_subdirectory(${glog_SOURCE_DIR} ${glog_BINARY_DIR})
#endif ()

# Depends on google-glog
FetchContent_Declare(ceres
    GIT_REPOSITORY https://ceres-solver.googlesource.com/ceres-solver
    GIT_TAG 2.2.0
    GIT_PROGRESS TRUE
)

#FetchContent_MakeAvailable(gflags)

#FetchContent_MakeAvailable(google-glog)
#add_subdirectory(${glog_SOURCE_DIR} ${CMAKE_BINARY_DIR}/glog-build)

FetchContent_MakeAvailable(ceres)

################################################################
### SET C++17 STANDARD
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### SET WARNINGS
if (MSVC)
    # Enable all, but warning C4127: conditional expression is constant 
    # (https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4127?view=msvc-160)
    add_compile_options(/W4 /wd4127)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_executable(ceres_example_1 source/example1/main.cpp)
target_link_libraries(ceres_example_1 PRIVATE ceres)

add_executable(ceres_example_2 source/example2/main.cpp)
target_link_libraries(ceres_example_2 PRIVATE ceres)

add_executable(ceres_example_3 source/example3/main.cpp)
target_link_libraries(ceres_example_3 PRIVATE ceres)

add_executable(simplex_example_4 source/example4/main.cpp)
target_include_directories(simplex_example_4 PRIVATE include)

