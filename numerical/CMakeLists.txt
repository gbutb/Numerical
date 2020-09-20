add_library(numerical numerical.cpp program/program.cpp)
target_link_libraries(numerical "-framework OpenCL")
