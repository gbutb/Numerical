# numerical/CMakeLists.txt

add_library(
    numerical
    context.cpp
    program/program.cpp
    program/heat/heat.cpp)
target_link_libraries(numerical "-framework OpenCL")
