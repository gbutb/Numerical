# numerical/CMakeLists.txt

add_library(
    numerical
    numerical.cpp
    program/program.cpp
    program/heat/heat.cpp)
target_link_libraries(numerical "-framework OpenCL")
