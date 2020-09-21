cmake_minimum_required(VERSION 3.5)
project(HeatEquation VERSION 0.0.1 LANGUAGES CXX)

# Set default options
option(BUILD_TESTS "BUILD_TESTS" ON)
option(BUILD_EXAMPLES "BUILD_EXAMPLES" ON)

IF(APPLE)
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
ENDIF()

# Set to C++14
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++14 -fno-elide-constructors")

# Initialize required packages
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Enable debugging mode
if (DEBUG_MODE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

# Load packages
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenCL REQUIRED)

# Add includes
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
    ${OPENCL_INCLUDE_DIR})

add_subdirectory(numerical/)
add_subdirectory(window/)

add_executable(main main.cpp)
target_link_libraries(
    main
    "-framework OpenCL"
    numerical
    ${OpenCV_LIBS})

add_executable(mainWindow mainWindow.cpp)
target_link_libraries(
    mainWindow
    window)